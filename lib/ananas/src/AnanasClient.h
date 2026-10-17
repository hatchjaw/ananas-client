#ifndef ANANASCLIENT_H
#define ANANASCLIENT_H

#include <AnanasPacketBuffer.h>

#include "AnanasPacket.h"
#include <AudioProcessor.h>
#include <ProgramComponent.h>
#include <Announcer.h>

namespace ananas
{
    class AudioClientBase : public MulticastListenerSocket,
                            public AudioProcessor,
                            public ProgramComponent
    {
    public:
        AudioClientBase(const SocketParams &p, SystemUtils::LogLevel logLevel);

        void run() override;

        void connect() override;

        void prepare(uint sampleRate) override;

    protected:
        void processImpl(int16_t **inputBuffer, int16_t **outputBuffer, size_t numFrames) override;

        virtual void announcerConnect() = 0;

        virtual void announcerRun() = 0;

        SystemUtils::LogLevel logging;

        AudioPacket rxPacket{};
        PacketBuffer packetBuffer;

        uint32_t nWrite{0}, nRead{0};
        NanoTime prevTime{0}, totalTime{0};
        uint32_t sampleRate{0};

        bool mute{false};
        uint16_t numPacketBufferReadIndexAdjustments{0};

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

        RebootListener rebootListener;
    };

    //==========================================================================

    template<typename AnnouncePacketType>
    class AudioClient final : public AudioClientBase
    {
    public:
        explicit AudioClient(const SocketParams &p, const SystemUtils::LogLevel logLevel = SystemUtils::LogLevel::None)
            : AudioClientBase(p, logLevel),
              announcer(Constants::ClientAnnounceSocketParams)
        {
        }

        void adjustBufferReadIndex(NanoTime ptpNow);

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

        void setDecoderMatrixRow0(int idx)
        {
            if constexpr (std::is_same_v<AnnouncePacketType, HoaClientAnnouncePacket>) {
                announcer.txPacket.decoderMatrixRow0 = idx;
            }
        }

        void setDecoderMatrixRow1(int idx)
        {
            if constexpr (std::is_same_v<AnnouncePacketType, HoaClientAnnouncePacket>) {
                announcer.txPacket.decoderMatrixRow1 = idx;
            }
        }

        [[nodiscard]] size_t getNumInputs() const override { return 0; }

        [[nodiscard]] size_t getNumOutputs() const override { return Constants::MaxChannels; }

    protected:
        void beginImpl() override
        {
            announcer.txPacket.firmwareVersion = SystemUtils::getFirmwareVersion();
            announcer.begin();
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

#endif //ANANASCLIENT_H
