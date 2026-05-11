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
    class AudioClientTAL final : public MulticastListenerSocket,
                                 public AudioStream,
                                 public ProgramComponent
    {
    public:
        explicit AudioClientTAL(const SocketParams &p, SystemUtils::LogLevel logLevel = SystemUtils::LogLevel::None);

        void connect() override;

        void run() override;

        size_t printTo(Print &p) const override;

        void setPercentCPU(float percentCPU);

        void setFirmwareType(SystemUtils::FirmwareType firmwareType);

        void setReportedSamplingRate(double samplingRate);

        void setIsPtpLocked(bool ptpLock);

        void setAudioPtpOffsetNs(int32_t offset);

        void setSecondarySourceCoordinates(float ss0x, float ss0y, float ss1x, float ss1y);

        void adjustBufferReadIndex(int64_t ptpNow);

    protected:
        void beginImpl() override;

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

        AudioPacket rxPacket{};
        PacketBuffer packetBuffer;
        DMAMEM static inline int16_t audioBlockData[Constants::MaxChannels][Constants::AudioBlockFrames]{};
        int16_t *audioBlock[Constants::MaxChannels]{};

        uint32_t nWrite{0}, nRead{0};
        NanoTime prevTime{0}, totalTime{0};
        uint32_t sampleRate{Constants::AudioSamplingRate};

        bool mute{false};
        uint16_t numPacketBufferReadIndexAdjustments{0};

        Announcer<ClientAnnouncePacket> announcer;
        RebootListener rebootListener;
        SystemUtils::LogLevel logging;

        uint16_t currEnetCyc{0}, maxEnetCyc{0};
    };
}


#endif //ANANASCLIENTTAL_H
