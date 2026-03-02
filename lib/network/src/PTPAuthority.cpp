#include "PTPAuthority.h"
#include <QNEthernet.h>
#include <SystemUtils.h>

using namespace qindesign::network;

namespace ananas::network
{
    void PTPAuthority::connect()
    {
        EthernetIEEE1588.writeTimer({0, 0});

        socket.beginMulticast(ip, eventPort);
        generalSocket.beginMulticast(ip, generalPort);

        announceTimer.begin(announceInterrupt, 1'000'000);
        syncTimer.begin(syncInterrupt, 1'000'000);

        Serial.println("PTP Started");
    }

    void PTPAuthority::beginImpl()
    {
        Serial.println("Clock Authority");

        txAnnouncePacket.header.messageType = Constants::AnnounceMessage.type;
        txAnnouncePacket.header.messageLength = htons(Constants::AnnounceMessage.size);
        txAnnouncePacket.header.flagField[1] = 8; // PTP_TIMESCALE: True
        txAnnouncePacket.header.sourcePortIdentity[0] = 0xff;
        txAnnouncePacket.header.sourcePortIdentity[1] = 0xff;
        Ethernet.macAddress(&txAnnouncePacket.header.sourcePortIdentity[2]);
        txAnnouncePacket.header.sourcePortIdentity[8] = 0x0;
        txAnnouncePacket.header.sourcePortIdentity[9] = 0x1;
        txAnnouncePacket.header.controlField = Constants::AnnounceMessage.control;
        txAnnouncePacket.gmIdentity[0] = 0xff;
        txAnnouncePacket.gmIdentity[1] = 0xff;
        Ethernet.macAddress(&txAnnouncePacket.gmIdentity[2]);

        txSyncPacket.header.messageType = Constants::SyncMessage.type;
        txSyncPacket.header.messageLength = htons(Constants::SyncMessage.size);
        txSyncPacket.header.flagField[0] = 2; // PTP_TWO_STEP: True
        txSyncPacket.header.sourcePortIdentity[0] = 0xff;
        txSyncPacket.header.sourcePortIdentity[1] = 0xff;
        Ethernet.macAddress(&txSyncPacket.header.sourcePortIdentity[2]);
        txSyncPacket.header.sourcePortIdentity[8] = 0x0;
        txSyncPacket.header.sourcePortIdentity[9] = 0x1;
        txSyncPacket.header.controlField = Constants::SyncMessage.control;
        txSyncPacket.header.logMessageInterval = 0;

        txFollowUpPacket.header.messageType = Constants::FollowUpMessage.type;
        txFollowUpPacket.header.messageLength = htons(Constants::FollowUpMessage.size);
        txFollowUpPacket.header.flagField[0] = 0;
        txFollowUpPacket.header.flagField[1] = 0;
        txFollowUpPacket.header.sourcePortIdentity[0] = 0xff;
        txFollowUpPacket.header.sourcePortIdentity[1] = 0xff;
        Ethernet.macAddress(&txFollowUpPacket.header.sourcePortIdentity[2]);
        txFollowUpPacket.header.sourcePortIdentity[8] = 0x0;
        txFollowUpPacket.header.sourcePortIdentity[9] = 0x1;
        txFollowUpPacket.header.controlField = Constants::FollowUpMessage.control;
        txFollowUpPacket.header.logMessageInterval = 0;

        txDelayRespPacket.header.messageType = Constants::DelayResponseMessage.type;
        txDelayRespPacket.header.messageLength = htons(Constants::DelayResponseMessage.size);
        txDelayRespPacket.header.sourcePortIdentity[0] = 0xff;
        txDelayRespPacket.header.sourcePortIdentity[1] = 0xff;
        Ethernet.macAddress(&txDelayRespPacket.header.sourcePortIdentity[2]);
        txDelayRespPacket.header.sourcePortIdentity[8] = 0x0;
        txDelayRespPacket.header.sourcePortIdentity[9] = 0x1;
        txDelayRespPacket.header.controlField = Constants::DelayResponseMessage.control;
        txDelayRespPacket.header.logMessageInterval = 0;

        PTPClock::beginImpl();
    }

    void PTPAuthority::run()
    {
        if (sendAnnounce) {
            sendAnnounce = false;
            sendAnnounceMessage();
        }

        if (sendSync) {
            sendSync = false;
            auto t1{sendSyncMessage()};
            sendFollowUpMessage(t1);
            ++syncSequenceID;
        }

        // Attempt to read on event socket (port 319)
        while (true) {
            if (const auto size{socket.parsePacket()}; size > 0) {
                timespec t4{};
                if (socket.readWithTimestamp(reinterpret_cast<uint8_t *>(&rxDelayReqPacket), size, &t4) > 0) {
                    if (logging > SystemUtils::LogLevel::None) {
                        Serial.printf("(%5" PRIu16 ") Received delay request\n", ntohs(rxDelayReqPacket.header.sequenceID));
                    }
                    handleDelayReqMessage(t4);
                }
            } else break;
        }
    }

    size_t PTPAuthority::printTo(Print &p) const
    {
        return p.println("Clock Authority");
    }

    void PTPAuthority::syncInterrupt()
    {
        if (sInstance) {
            auto *i{static_cast<PTPAuthority *>(sInstance)};
            i->handleSyncInterrupt();
        }
    }

    void PTPAuthority::announceInterrupt()
    {
        if (sInstance) {
            auto *i{static_cast<PTPAuthority *>(sInstance)};
            i->handleAnnounceInterrupt();
        }
    }

    void PTPAuthority::handleSyncInterrupt()
    {
        if (ppsCount > 5) {
            sendSync = true;
        } else {
            ++ppsCount;
        }
    }

    void PTPAuthority::handleAnnounceInterrupt()
    {
        sendAnnounce = true;
    }

    void PTPAuthority::handle1588Interrupt()
    {
        PTPClock::handle1588Interrupt();
    }

    void PTPAuthority::sendAnnounceMessage()
    {
        txAnnouncePacket.header.sequenceID = htons(announceSequenceID++);

        generalSocket.send(ip, generalPort, reinterpret_cast<uint8_t *>(&txAnnouncePacket), Constants::AnnounceMessage.size);
    }

    timespec PTPAuthority::sendSyncMessage()
    {
        // Set up the message.
        txSyncPacket.header.sequenceID = htons(syncSequenceID);
        // Make sure it gets timestamped.
        EthernetIEEE1588.timestampNextFrame();
        // Send it.
        socket.send(ip, eventPort, reinterpret_cast<uint8_t *>(&txSyncPacket), Constants::SyncMessage.size);

        if (logging > SystemUtils::LogLevel::None) {
            Serial.printf("(%5" PRIu16 ") Sent sync\n", syncSequenceID);
        }

        timespec t1{};
        if (logging >= SystemUtils::LogLevel::Medium) { Serial.print("Wait for T1 delay send timestamp"); }
        while (!EthernetIEEE1588.readAndClearTxTimestamp(t1)) {
            if (logging >= SystemUtils::LogLevel::Medium) { Serial.print("."); }
        }
        if (logging >= SystemUtils::LogLevel::Medium) { Serial.println(" finished"); }

        // Add the hardware offset.
        const auto ns{Utils::timespecToNanoTime(t1) + Constants::HardwareOffsetNs};
        Utils::nanoTimeToTimespec(ns, t1);

        return t1;
    }

    void PTPAuthority::sendFollowUpMessage(const timespec &t1)
    {
        // Set up the message.
        txFollowUpPacket.header.sequenceID = htons(syncSequenceID);

        // Write the timestamp.
        Utils::writeBigEndian<6>(&txFollowUpPacket.originTimestamp[0], t1.tv_sec);
        Utils::writeBigEndian<4>(&txFollowUpPacket.originTimestamp[6], t1.tv_nsec);

        // Send it.
        generalSocket.send(ip, generalPort, reinterpret_cast<uint8_t *>(&txFollowUpPacket), Constants::FollowUpMessage.size);

        if (logging > SystemUtils::LogLevel::None) {
            Serial.printf("(%5" PRIu16 ") Sent follow-up,      t1 = ", syncSequenceID);
            Utils::printTime(t1);
        }
    }

    void PTPAuthority::sendDelayResponseMessage(const uint8_t *sourcePortID, const timespec &t4, const uint16_t seqID)
    {
        txDelayRespPacket.header.sequenceID = htons(seqID);

        Utils::writeBigEndian<6>(&txDelayRespPacket.receiveTimestamp[0], t4.tv_sec);
        Utils::writeBigEndian<4>(&txDelayRespPacket.receiveTimestamp[6], t4.tv_nsec);

        memcpy(txDelayRespPacket.requestingPortIdentity, sourcePortID, 10);

        generalSocket.send(ip, generalPort, reinterpret_cast<uint8_t *>(&txDelayRespPacket), Constants::DelayResponseMessage.size);

        if (logging > SystemUtils::LogLevel::None) {
            Serial.printf("(%5" PRIu16 ") Sent delay response, t4 = ", seqID);
            Utils::printTime(t4);
        }
    }

    void PTPAuthority::handleDelayReqMessage(timespec &t4)
    {
        if (logging >= SystemUtils::LogLevel::Medium) {
            Serial.printf("PTPMessage messageType: %d versionPTP: %d domainNumer: %d\n",
                          rxDelayReqPacket.header.messageType,
                          rxDelayReqPacket.header.versionPTP,
                          rxDelayReqPacket.header.domainNumber);
        }

        if (rxDelayReqPacket.header.versionPTP == 2 &&
            rxDelayReqPacket.header.messageType == Constants::DelayRequestMessage.type) {
            const NanoTime ns{Utils::timespecToNanoTime(t4) + Constants::HardwareOffsetNs};
            Utils::nanoTimeToTimespec(ns, t4);
            sendDelayResponseMessage(rxDelayReqPacket.header.sourcePortIdentity, t4, ntohs(rxDelayReqPacket.header.sequenceID));
        }
    }
}
