#ifndef ANANASCLIENT_H
#define ANANASCLIENT_H

#include "AnanasPacket.h"
#include "AnanasPacketBuffer.h"
#include <AudioProcessor.h>
#include <ProgramComponent.h>
#include <Announcer.h>
#include <Listener.h>

namespace ananas
{
    class AudioClient final : public MulticastListenerSocket,
                              public AudioProcessor,
                              public ProgramComponent
    {
    protected:
        void beginImpl() override;

    public:
        explicit AudioClient(const SocketParams &p, SystemUtils::LogLevel logLevel = SystemUtils::LogLevel::None);

        void run() override;

        void connect() override;

        void adjustBufferReadIndex(NanoTime ptpNow);

        void prepare(uint sampleRate) override;

        size_t printTo(Print &p) const override;

        void setFirmwareType(SystemUtils::FirmwareType firmwareType);

        void setReportedSamplingRate(double samplingRate);

        void setIsPtpLocked(bool ptpLock);

        void setAudioPtpOffsetNs(int32_t offset);

        void setPercentCPU(float percentage);

        void setSecondarySourceCoordinates(float ss0x, float ss0y, float ss1x, float ss1y);

        [[nodiscard]] size_t getNumInputs() const override { return 0; }

        [[nodiscard]] size_t getNumOutputs() const override { return Constants::MaxChannels; }

    protected:
        void processImpl(int16_t **inputBuffer, int16_t **outputBuffer, size_t numFrames) override;

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

        AudioPacket rxPacket{};
        PacketBuffer packetBuffer;

        uint32_t nWrite{0}, nRead{0};
        NanoTime prevTime{0}, totalTime{0};
        uint32_t sampleRate{0};

        bool mute{false};
        uint16_t numPacketBufferReadIndexAdjustments{0};

        Announcer<ClientAnnouncePacket> announcer;
        RebootListener rebootListener;
        SystemUtils::LogLevel logging;

        uint16_t currEnetCyc{0}, maxEnetCyc{0};
    };
}

#endif //ANANASCLIENT_H
