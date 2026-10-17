#include "AnanasClient.h"
#include "AnanasPacketBuffer.h"
#include "AnanasUtils.h"
#include <QNEthernet.h>
#include <SystemUtils.h>

namespace ananas
{
    AudioClientBase::AudioClientBase(const SocketParams &p, const SystemUtils::LogLevel logLevel)
        : MulticastListenerSocket(p),
          logging(logLevel),
          rebootListener(Constants::RebootSocketParams)
    {
    }

    void AudioClientBase::run()
    {
        uint32_t cycles = ARM_DWT_CYCCNT;

        while (true) {
            if (const auto size{socket.parsePacket()}; size > 0) {
                nWrite++;

                socket.read(rxPacket.rawData(), size);
                packetBuffer.write(rxPacket);

                if (nWrite > Constants::ClientReportThresholdPackets) {
                    timespec now{};
                    qindesign::network::EthernetIEEE1588.readTimer(now);
                    const auto ns{now.tv_sec * Constants::NanosecondsPerSecond + now.tv_nsec};

                    const int64_t diff{ns - prevTime};
                    if (prevTime != 0) {
                        totalTime += diff;
                    }
                    prevTime = ns;
                }
            } else break;
        }

        announcerRun();
        rebootListener.run();

        cycles = (ARM_DWT_CYCCNT - cycles) >> 6;
        currEnetCyc = cycles;
        if (currEnetCyc > maxEnetCyc) {
            maxEnetCyc = currEnetCyc;
        }
    }

    void AudioClientBase::connect()
    {
        MulticastListenerSocket::connect();
        announcerConnect();
        rebootListener.connect();
    }

    void AudioClientBase::prepare(uint sampleRate)
    {
        this->sampleRate = sampleRate;
        packetBuffer.clear();
    }

    void AudioClientBase::processImpl(int16_t **inputBuffer, int16_t **outputBuffer, size_t numFrames)
    {
        if (mute) {
            for (size_t channel{0}; channel < getNumOutputs(); ++channel) {
                memset(outputBuffer[channel], 0, sizeof(int16_t) * numFrames);
            }
            return;
        }

        size_t processFrame{0};
        while (processFrame < numFrames) {
            auto packet{packetBuffer.read()};

            const size_t numChannels{min(packet.header.numChannels, getNumOutputs())};
            const auto audioData{packet.audio()};

            for (size_t packetFrame{0}; packetFrame < packet.header.numFrames; ++packetFrame, ++processFrame) {
                for (size_t channel{0}; channel < numChannels; ++channel) {
                    outputBuffer[channel][processFrame] = audioData[packetFrame * numChannels + channel];
                }
            }
        }
    }

    //==========================================================================

    AudioClientBase::RebootListener::RebootListener(const SocketParams &p)
        : MulticastListenerSocket(p)
    {
    }

    void AudioClientBase::RebootListener::beginImpl()
    {
    }

    void AudioClientBase::RebootListener::run()
    {
        if (const auto size{socket.parsePacket()}; size == 0) {
            Serial.println("Rebooting.");
            SystemUtils::reboot();
        }
    }

    size_t AudioClientBase::RebootListener::printTo(Print &p) const
    {
        return 0;
    }

    //==========================================================================

    template<typename AnnouncePacketType>
    size_t AudioClient<AnnouncePacketType>::printTo(Print &p) const
    {
        return p.print("Ananas Client:     ") + AudioProcessor::printTo(p) +
               p.printf("  ENET %% CPU: %f (max %f)\n",
                        CYCLES_TO_APPROX_PERCENT(currEnetCyc),
                        CYCLES_TO_APPROX_PERCENT(maxEnetCyc)
               )
               + (nWrite < Constants::ClientReportThresholdPackets
                      ? p.printf("  Packets received: %" PRIu32 "\n", nWrite)
                      : p.printf("  Packets received: %" PRIu32 " Average reception interval: %e ns\n",
                                 nWrite,
                                 static_cast<double>(totalTime) / (nWrite - Constants::ClientReportThresholdPackets)))
               + p.print(packetBuffer)
               + p.printf("  Packet offset: %" PRId64 " ns (%" PRId32
                          " frames), Times adjusted: %" PRIu16 "\n",
                          announcer.txPacket.presentationOffsetNs,
                          announcer.txPacket.presentationOffsetFrame,
                          numPacketBufferReadIndexAdjustments);
    }

    template<typename AnnouncePacketType>
    void AudioClient<AnnouncePacketType>::adjustBufferReadIndex(const NanoTime ptpNow)
    {
        auto didAdjust{false};

        auto diff{ptpNow - packetBuffer.peek().header.time};
        const auto kMaxDiff{(Constants::NanosecondsPerSecond * Constants::FramesPerPacketExpected / sampleRate)};

        if (diff < 0 || diff > kMaxDiff) {
            if (logging > SystemUtils::LogLevel::None)
                Serial.printf("\nPresentation time diff %" PRId64 " ns outside of range 0-%" PRId64 " ns\n"
                              "Seeking closest packet... ", diff, kMaxDiff);
            auto minDiff{INT64_MAX};
            size_t readIndex{0};
            packetBuffer.setReadIndex(readIndex);

            // Seek the packet with the smallest positive presentation time
            // offset with respect to PTP time.
            for (size_t frame{0}; frame < Constants::PacketBufferCapacity; ++frame, packetBuffer.incrementReadIndex()) {
                diff = ptpNow - packetBuffer.peek().header.time;
                if (diff >= 0 && diff <= minDiff) {
                    minDiff = diff;
                    readIndex = packetBuffer.getReadIndex();
                }
            }

            if (minDiff < kMaxDiff) {
                if (logging > SystemUtils::LogLevel::None)
                    Serial.printf("Found packet with presentation time diff %" PRId64 " ns (read index %d)\n",
                                  minDiff, readIndex);
                packetBuffer.setReadIndex(readIndex);
            } else {
                if (logging > SystemUtils::LogLevel::None)
                    Serial.printf("Unable to find a packet with a valid presentation time (min %" PRId64 " ns)\n",
                                  minDiff);
                packetBuffer.setReadIndex(0);
            }

            didAdjust = true;
        }

        if (didAdjust) {
            ++numPacketBufferReadIndexAdjustments;
        }
        mute = didAdjust;

        announcer.txPacket.presentationOffsetNs = diff;
    }

    template
    class AudioClient<ClientAnnouncePacket>;

    template
    class AudioClient<WfsClientAnnouncePacket>;

    template
    class AudioClient<HoaClientAnnouncePacket>;
}
