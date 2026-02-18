#ifndef PTPAUTHORITY_H
#define PTPAUTHORITY_H

#include <NetworkProcessor.h>
#include <NetworkUtils.h>
#include <ProgramComponent.h>
#include <PTPClock.h>
#include <SystemUtils.h>

namespace ananas::network
{
    class PTPAuthority final : public PTPClock,
                               public NetworkProcessor
    {
    public:
        using PTPClock::PTPClock;

        void connect() override;

    protected:
        void beginImpl() override;

    public:
        void run() override;

        size_t printTo(Print &p) const override;

    private:
        static void syncInterrupt();

        static void announceInterrupt();

        void handleSyncInterrupt();

        void handleAnnounceInterrupt();

        void handle1588Interrupt() override;

        void sendAnnounceMessage();

        void sendSyncMessage();

        void sendFollowUpMessage();

        void sendDelayResponseMessage(const uint8_t *sourcePortID, const timespec &t4, uint16_t seqID);

        void handleDelayReqMessage(timespec &t4);

        qindesign::network::EthernetUDP generalSocket{4};
        uint ppsCount{0};
        IntervalTimer syncTimer;
        timespec t1{0, 0};
        elapsedMicros elapsedSinceSync{};
        static constexpr uint32_t followUpThresholdUs{1000};
        IntervalTimer announceTimer;
        PTPV2AnnouncePacket txAnnouncePacket;
        PTPV2Packet txSyncPacket, txFollowUpPacket, rxDelayReqPacket;
        PTPV2DelayResponsePacket txDelayRespPacket;
        uint16_t syncSequenceID{0}, announceSequenceID{0};
    };
}

#endif //PTPAUTHORITY_H
