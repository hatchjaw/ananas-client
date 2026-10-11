#ifndef ANANASCLIENTTAL_H
#define ANANASCLIENTTAL_H

#include <Audio.h>
#include "AnanasPacket.h"
#include "AnanasPacketBuffer.h"
#include <ProgramComponent.h>
#include <NetworkProcessor.h>
#include <Announcer.h>
#include <Listener.h>

namespace ananas
{
    class AudioClientTALBase : public MulticastListenerSocket,
                               public AudioStream,
                               public ProgramComponent
    {
    public:
        AudioClientTALBase(const SocketParams &p, SystemUtils::LogLevel logLevel);

        void run() override;

        void connect() override;

    protected:
        virtual void announcerConnect() = 0;

        virtual void announcerRun() = 0;

        AudioPacket rxPacket{};
        PacketBuffer packetBuffer;
        DMAMEM static inline int16_t audioBlockData[Constants::MaxChannels][Constants::AudioBlockFrames]{};
        int16_t *audioBlock[Constants::MaxChannels]{};

        uint32_t nWrite{0}, nRead{0};
        NanoTime prevTime{0}, totalTime{0};
        uint32_t sampleRate{Constants::AudioSamplingRate};

        bool mute{false};
        uint16_t numPacketBufferReadIndexAdjustments{0};

        SystemUtils::LogLevel logging;

        uint16_t currEnetCyc{0}, maxEnetCyc{0};

    private:
        class RebootListener final : public MulticastListenerSocket,
                                     public ProgramComponent
        {
        protected:
            void beginImpl() override;

        public:
            explicit RebootListener(const SocketParams &p);

            void run() override;

            size_t printTo(Print &p) const override;
        };

        void update() override;

        RebootListener rebootListener;
    };

    //==========================================================================

    template<typename AnnouncePacketType>
    class AudioClientTAL final : public AudioClientTALBase
    {
    public:
        explicit AudioClientTAL(const SocketParams &p, SystemUtils::LogLevel logLevel = SystemUtils::LogLevel::None)
            : AudioClientTALBase(p, logLevel),
              announcer(Constants::ClientAnnounceSocketParams)
        {
        }

        size_t printTo(Print &p) const override;

        void setFirmwareType(SystemUtils::FirmwareType firmwareType)
        {
            announcer.txPacket.firmwareType = firmwareType;
        }

        void setReportedSamplingRate(double samplingRate)
        {
            announcer.txPacket.samplingRate = samplingRate;
        }

        void setIsPtpLocked(bool ptpLock)
        {
            announcer.txPacket.ptpLock = ptpLock;
        }

        void setAudioPtpOffsetNs(int32_t offset)
        {
            announcer.txPacket.audioPtpOffsetNs = offset;
            announcer.txPacket.presentationOffsetFrame = (announcer.txPacket.presentationOffsetNs + offset) / static_cast<int64_t>(1e9 / sampleRate);
        }

        void setPercentCPU(float percentage)
        {
            announcer.txPacket.percentCPU = percentage + CYCLES_TO_APPROX_PERCENT(currEnetCyc);;
        }

        void setSecondarySourceCoordinates(float ss0x, float ss0y, float ss1x, float ss1y)
        {
            if constexpr (std::is_same_v<AnnouncePacketType, WfsClientAnnouncePacket>) {
                announcer.txPacket.secondarySource0x = ss0x;
                announcer.txPacket.secondarySource0y = ss0y;
                announcer.txPacket.secondarySource1x = ss1x;
                announcer.txPacket.secondarySource1y = ss1y;
            }
        }

        void setDecoderMatrixRows(int idx0, int idx1)
        {
            if constexpr (std::is_same_v<AnnouncePacketType, HoaClientAnnouncePacket>) {
                announcer.txPacket.decoderMatrixRow0 = idx0;
                announcer.txPacket.decoderMatrixRow1 = idx1;
            }
        }

        void adjustBufferReadIndex(int64_t ptpNow);

    protected:
        void beginImpl() override
        {
            announcer.txPacket.firmwareVersion = SystemUtils::getFirmwareVersion();
            announcer.begin();

            // Set up audioBlock pointer.
            for (size_t i{0}; i < Constants::MaxChannels; ++i) {
                audioBlock[i] = audioBlockData[i];
            }
            // Clear buffer.
            memset(audioBlockData, 0, sizeof(audioBlockData));
        }

        void announcerConnect() override
        {
            announcer.connect();
        }

        void announcerRun() override
        {
            announcer.txPacket.bufferFillPercent = packetBuffer.getFillPercent();
            announcer.run();
        }

    private:
        Announcer<AnnouncePacketType> announcer;
    };
}


#endif //ANANASCLIENTTAL_H
