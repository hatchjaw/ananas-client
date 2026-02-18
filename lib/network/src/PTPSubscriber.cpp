#include "PTPSubscriber.h"
#include <QNEthernet.h>
#include <sys/unistd.h>

extern volatile uint32_t syncReceiveCounter;
extern volatile uint32_t followUpReceiveCounter;

using namespace qindesign::network;

namespace ananas::network
{
    void PTPSubscriber::connect()
    {
        reset();

        socket.beginMulticast(ip, eventPort);
        generalSocket.beginMulticast(ip, generalPort);

        // readTimer.begin(readInterrupt, 10);

        Serial.println("PTP Started");
    }

    void PTPSubscriber::beginImpl()
    {
        Serial.println("Clock Subscriber");

        txDelayReqPacket.header.messageType = Constants::DelayRequestMessage.type;
        txDelayReqPacket.header.messageLength = htons(Constants::DelayRequestMessage.size);
        txDelayReqPacket.header.sourcePortIdentity[0] = 0xff;
        txDelayReqPacket.header.sourcePortIdentity[1] = 0xff;
        Ethernet.macAddress(&txDelayReqPacket.header.sourcePortIdentity[2]);
        txDelayReqPacket.header.sourcePortIdentity[8] = 0x0;
        txDelayReqPacket.header.sourcePortIdentity[9] = 0x1;
        txDelayReqPacket.header.controlField = Constants::DelayRequestMessage.control;

        pinMode(LEDPin, OUTPUT);

        PTPClock::beginImpl();
    }

    void PTPSubscriber::run()
    {
        for (auto i{0}; i < 3; ++i) {
            pollEventSocket();
            pollGeneralSocket();
        }

        digitalWrite(LEDPin, isLocked() ? HIGH : LOW);
    }

    void PTPSubscriber::reset()
    {
        Serial.println("Reset PTP state");

        EthernetIEEE1588.writeTimer({0, 0});

        lockCount = 0;
    }

    void PTPSubscriber::onControllerUpdated(const std::function<void(double adjust)> &callback)
    {
        controllerUpdatedCallback = callback;
    }

    void PTPSubscriber::onLockChanged(const std::function<void(bool isLocked, NanoTime now)> &callback)
    {
        lockChangedCallback = callback;
    }

    size_t PTPSubscriber::printTo(Print &p) const
    {
        return 0;
    }

    void PTPSubscriber::pollEventSocket()
    {
        while (true) {
            if (const auto size{socket.parsePacket()}; size > 0) {
                timespec t2{};
                if (socket.readWithTimestamp(reinterpret_cast<uint8_t *>(&rxSyncPacket), size, &t2) > 0) {
                    handleSyncMessage(t2);
                    syncReceiveCounter++;
                    Serial.printf("Sync received (%" PRIu32 ")\n", syncReceiveCounter);
                }
            } else break;
        }
    }

    void PTPSubscriber::pollGeneralSocket()
    {
        while (true) {
            if (const auto size{generalSocket.parsePacket()}; size > 0) {
                if (generalSocket.read(rxBuffer, size) > 0) {
                    switch (rxBuffer[0]) {
                        case Constants::FollowUpMessage.type:
                            handleFollowUpMessage();
                            followUpReceiveCounter++;
                            Serial.printf("Follow-up received (%" PRIu32 ", %" PRId32 ")\n", followUpReceiveCounter,
                                          static_cast<int32_t>(followUpReceiveCounter) -
                                          static_cast<int32_t>(syncReceiveCounter));
                            break;
                        case Constants::DelayResponseMessage.type:
                            handleDelayRespMessage();
                            break;
                        default:
                            // Silently ignore all other message types, i.e. announce messages.
                            break;
                    }
                }
            } else break;
        }
    }

    void PTPSubscriber::sendDelayReqMessage()
    {
        txDelayReqPacket.header.sequenceID = htons(delayReqSequenceId);
        EthernetIEEE1588.timestampNextFrame();
        socket.send(ip, eventPort, reinterpret_cast<uint8_t *>(&txDelayReqPacket), Constants::DelayRequestMessage.size);

        // Read the time at which it was sent.
        timespec t3{};

        if (logging >= SystemUtils::Medium) { Serial.print("Wait for T3 delay send timestamp"); }
        while (!EthernetIEEE1588.readAndClearTxTimestamp(t3)) {
            if (logging >= SystemUtils::Medium) { Serial.print("."); }
        }
        if (logging >= SystemUtils::Medium) { Serial.println(" finished"); }

        pendingExchanges[exchangeId].t3 = Utils::timespecToNanoTime(t3);
        pendingExchanges[exchangeId].delaySequenceId = delayReqSequenceId;

        if (logging > SystemUtils::None) {
            Serial.printf("(%5" PRIu16 ") Sent delay request,      t3 = ", exchangeId);
            Utils::printTime(pendingExchanges[exchangeId].t3);
        }
    }

    void PTPSubscriber::handleSyncMessage(const timespec &t2)
    {
        if (logging >= SystemUtils::Medium) {
            Serial.printf("PTPMessage messageType: %d versionPTP: %d domainNumer: %d\n",
                          rxSyncPacket.header.messageType,
                          rxSyncPacket.header.versionPTP,
                          rxSyncPacket.header.domainNumber);
        }

        const auto twoStep{rxSyncPacket.header.flagField[0] & 0x02};

        if (rxSyncPacket.header.versionPTP == 2 &&
            rxSyncPacket.header.messageType == Constants::SyncMessage.type &&
            twoStep > 0) {
            pendingExchanges[exchangeId].t2 = Utils::timespecToNanoTime(t2);
            pendingExchanges[exchangeId].syncSequenceId = ntohs(rxSyncPacket.header.sequenceID);

            if (logging > SystemUtils::None) {
                Serial.printf("(%5" PRIu16 ") Received sync,           t2 = ", exchangeId);
                Utils::printTime(pendingExchanges[exchangeId].t2);
            }
        }
    }

    void PTPSubscriber::handleFollowUpMessage()
    {
        const auto *p{reinterpret_cast<PTPV2Packet *>(&rxBuffer)};
        if (logging >= SystemUtils::Medium) {
            Serial.printf("PTPMessage messageType: %d versionPTP: %d domainNumer: %d\n",
                          p->header.messageType,
                          p->header.versionPTP,
                          p->header.domainNumber);
        }

        // TODO: check that p->header.sequenceID equals pendingExchanges[exchangeId].syncSequenceId

        const auto s{Utils::readBigEndian<6>(&p->originTimestamp[0])},
                ns{Utils::readBigEndian<4>(&p->originTimestamp[6])};
        pendingExchanges[exchangeId].t1 = s * Constants::NanosecondsPerSecond + ns;

        if (logging > SystemUtils::None) {
            Serial.printf("(%5" PRIu16 ") Received follow-up,      t1 = ", exchangeId);
            Utils::printTime(pendingExchanges[exchangeId].t1);
        }

        pendingExchanges[exchangeId + 1].prevT1 = pendingExchanges[exchangeId].t1;
        pendingExchanges[exchangeId + 1].prevT2 = pendingExchanges[exchangeId].t2;

        sendDelayReqMessage();

        std::vector<uint16_t> toErase{};
        for (const auto &[fst, snd]: pendingExchanges) {
            if (snd.age > 2000) {
                toErase.push_back(fst);
            }
        }
        for (uint16_t id: toErase) {
            if (logging > SystemUtils::None) Serial.printf("Erasing exchange %" PRIu16 "\n", id);
            pendingExchanges.erase(id);
        }
    }

    void PTPSubscriber::handleDelayRespMessage()
    {
        const auto *p{reinterpret_cast<PTPV2DelayResponsePacket *>(&rxBuffer)};

        if (logging >= SystemUtils::Medium) {
            Serial.printf("PTPMessage messageType: %d versionPTP: %d domainNumer: %d\n",
                          p->header.messageType,
                          p->header.versionPTP,
                          p->header.domainNumber);
        }

        // Check whether requesting port matches source port.
        if (memcmp(
                p->requestingPortIdentity,
                txDelayReqPacket.header.sourcePortIdentity,
                sizeof(p->requestingPortIdentity)
            ) == 0) {
            const auto s{Utils::readBigEndian<6>(&p->receiveTimestamp[0])},
                    ns{Utils::readBigEndian<4>(&p->receiveTimestamp[6])};
            pendingExchanges[exchangeId].t4 = s * Constants::NanosecondsPerSecond + ns;

            if (logging > SystemUtils::None) {
                Serial.printf("(%5" PRIu16 ") Received delay response, t4 = ", exchangeId);
                Utils::printTime(pendingExchanges[exchangeId].t4);
            }

            std::vector<uint16_t> toErase{};
            for (const auto &[fst, snd]: pendingExchanges) {
                if (snd.isComplete()) {
                    updateController(fst);
                    toErase.push_back(fst);
                } else if (snd.age > 2000) {
                    toErase.push_back(fst);
                }
            }
            for (uint16_t id: toErase) {
                if (logging > SystemUtils::None) Serial.printf("Erasing exchange %" PRIu16 "\n", id);
                pendingExchanges.erase(id);
            }

            delayReqSequenceId++;
            exchangeId++;
        } else {
            Serial.println("Delay resp message received, but source port identities didn't match");
        }
    }

    bool PTPSubscriber::isLocked() const
    {
        return lockCount > 5;
    }

    void PTPSubscriber::updateController(const uint16_t exchangeId)
    {
        if (pendingExchanges[exchangeId].prevT1 == 0 || pendingExchanges[exchangeId].prevT2 == 0) {
            return;
        }

        const auto currentDrift{pendingExchanges[exchangeId].getDrift()};
        const auto offset{pendingExchanges[exchangeId].getOffset()};
        const auto offsetCorrection{-offset};
        const auto driftError{currentDrift > 100'000 || currentDrift < -100'000};
        const auto freqMode{!driftError && (currentDrift > 1000 || currentDrift < -1000)};
        const auto coarseMode{offset > 1000 || offset < -1000};

        adjust = 0;
        double nspsAdjustC{0}, nspsAdjustP{0}, nspsAdjustI{0};

        if (!driftError) {
            if (freqMode) {
                drift = fmod(drift + currentDrift, Constants::NanosecondsPerSecond);
                nspsAdjustC = drift;
                adjust = nspsAdjustC;
                EthernetIEEE1588.adjustFreq(adjust);
                nspsAccu = 0;
                lockCount = 0;
            } else if (coarseMode) {
                EthernetIEEE1588.offsetTimer(-offset);
                nspsAccu = 0;
                pendingExchanges[exchangeId + 1].prevT2 += offsetCorrection;
                lockCount = 0;
            } else {
                nspsAccu += offsetCorrection;
                nspsAdjustC = drift;
                nspsAdjustP = (static_cast<double>(offsetCorrection) * kP);
                nspsAdjustI = (static_cast<double>(nspsAccu) * kI);
                adjust = nspsAdjustC + nspsAdjustP + nspsAdjustI;
                EthernetIEEE1588.adjustFreq(adjust);
                pendingExchanges[exchangeId + 1].prevT2 += offsetCorrection;
                lockCount++;
            }

            if (controllerUpdatedCallback != nullptr) {
                controllerUpdatedCallback(adjust);
            }
        } else {
            lockCount = 0;
        }

        if (logging > SystemUtils::None) {
            Serial.printf("T2 diff: %" PRId64 " T1 diff: %" PRId64 "\n", pendingExchanges[exchangeId].getT2Diff(), pendingExchanges[exchangeId].getT1Diff());
            Serial.printf("T2-T1: %" PRId64 " T4-T3: %" PRId64 "\n", pendingExchanges[exchangeId].getT2mT1(), pendingExchanges[exchangeId].getT4mT3());
            Serial.printf("Delay: %" PRId64 " ns ", pendingExchanges[exchangeId].getDelay());
            Serial.printf("Offset: %" PRId64 " ns ", pendingExchanges[exchangeId].getOffset());
            Serial.printf("Drift: %f ns \n", pendingExchanges[exchangeId].getDrift());

            if (driftError) {
                Serial.printf("Drift Error --- No controller update.\n");
            } else if (freqMode) {
                Serial.printf("Freq mode adjust f: %f ns/s", adjust);
            } else if (coarseMode) {
                Serial.printf("Coarse mode adjust: %" PRId64 " ns", offsetCorrection);
            } else {
                Serial.printf("Fine filter mode ns/s: %f C: %f P (%f): %f I (%f): %f", adjust, nspsAdjustC, kP, nspsAdjustP, kI, nspsAdjustI);
            }

            Serial.println();
            if (!driftError) {
                Serial.printf("ENET_ATINC %08X\n", ENET_ATINC);
                Serial.printf("ENET_ATPER %d\n", ENET_ATPER);
                Serial.printf("ENET_ATCOR %d (%f)\n", ENET_ATCOR, 25'000'000 / adjust);
            }
            Serial.println();
        }
    }

    void PTPSubscriber::handle1588Interrupt()
    {
        PTPClock::handle1588Interrupt();

        if (lockChangedCallback != nullptr) {
            lockChangedCallback(
                lockCount > 0,
                currentInterruptTime.tv_sec * Constants::NanosecondsPerSecond + currentInterruptTime.tv_nsec
            );
        }
    }
}
