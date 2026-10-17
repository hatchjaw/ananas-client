#include "AnanasClientTAL.h"

namespace ananas
{
    AudioClientTALBase::AudioClientTALBase(const SocketParams &p, SystemUtils::LogLevel logLevel)
        : MulticastListenerSocket(p),
          AudioStream(Constants::MaxChannels, new audio_block_t *[Constants::MaxChannels]),
          logging(logLevel),
          rebootListener(Constants::RebootSocketParams)
    {
    }

    void AudioClientTALBase::run()
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

    void AudioClientTALBase::connect()
    {
        MulticastListenerSocket::connect();
        announcerConnect();
        rebootListener.connect();
    }

    //==========================================================================

    AudioClientTALBase::RebootListener::RebootListener(const SocketParams &p)
        : MulticastListenerSocket(p)
    {
    }

    void AudioClientTALBase::RebootListener::beginImpl()
    {
    }

    void AudioClientTALBase::RebootListener::run()
    {
        if (const auto size{socket.parsePacket()}; size == 0) {
            Serial.println("Rebooting.");
            SystemUtils::reboot();
        }
    }

    size_t AudioClientTALBase::RebootListener::printTo(Print &p) const
    {
        return 0;
    }

    void AudioClientTALBase::update()
    {
        audio_block_t *outBlock[Constants::MaxChannels];
        constexpr auto channelFrameSize{Constants::AudioBlockFrames * sizeof(int16_t)};

        if (mute) {
            for (size_t ch{0}; ch < Constants::MaxChannels; ++ch) {
                outBlock[ch] = allocate();
                if (outBlock[ch]) {
                    memset(outBlock[ch]->data, 0, channelFrameSize);

                    // Finish up.
                    transmit(outBlock[ch], ch);
                    release(outBlock[ch]);
                }
            }
            return;
        }

        size_t processFrame{0};
        while (processFrame < Constants::AudioBlockFrames) {
            auto packet{packetBuffer.read()};

            const size_t numChannels{min(packet.header.numChannels, Constants::MaxChannels)};
            const auto audioData{packet.audio()};

            for (size_t packetFrame{0}; packetFrame < packet.header.numFrames; ++packetFrame, ++processFrame) {
                for (size_t channel{0}; channel < numChannels; ++channel) {
                    audioBlock[channel][processFrame] = audioData[packetFrame * numChannels + channel];
                }
            }
        }

        for (size_t ch{0}; ch < Constants::MaxChannels; ++ch) {
            outBlock[ch] = allocate();
            if (outBlock[ch]) {
                // Copy the samples to the output block.
                memcpy(outBlock[ch]->data, audioBlock[ch], channelFrameSize);

                // Finish up.
                transmit(outBlock[ch], ch);
                release(outBlock[ch]);
            }
        }
    }

    //==========================================================================

    template<typename AnnouncePacketType>
    size_t AudioClientTAL<AnnouncePacketType>::printTo(Print &p) const
    {
        return p.print("Ananas Client:")
               + p.printf("  ENET %% CPU: %f (max %f)\n",
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
    void AudioClientTAL<AnnouncePacketType>::adjustBufferReadIndex(int64_t ptpNow)
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
    class AudioClientTAL<ClientAnnouncePacket>;

    template
    class AudioClientTAL<WfsClientAnnouncePacket>;

    template
    class AudioClientTAL<HoaClientAnnouncePacket>;
}
