#ifndef PTPSUBSCRIBER_H
#define PTPSUBSCRIBER_H

#include <lwip_t41.h>
#include <map>
#include <NetworkProcessor.h>
#include <ProgramComponent.h>
#include <PTPClock.h>

namespace ananas::network
{
    class PTPSubscriber final : public PTPClock,
                                public NetworkProcessor
    {
    public:
        using PTPClock::PTPClock;

        void connect() override;

    protected:
        void beginImpl() override;

    public:
        void run() override;

        void reset();

        void onControllerUpdated(const std::function<void(double adjust)> &callback);

        void onLockChanged(const std::function<void(bool isLocked, NanoTime now)> &callback);

        size_t printTo(Print &p) const override;

    private:
        struct TimeExchange
        {
            NanoTime t1{-1};
            NanoTime prevT1{0};
            NanoTime t2{-1};
            NanoTime prevT2{0};
            NanoTime t3{-1};
            NanoTime t4{-1};
            elapsedMillis age;
            uint16_t syncSequenceId{0};
            uint16_t delaySequenceId{0};

            [[nodiscard]] bool isFollowedUp() const
            {
                return t1 > -1 && t2 > -1;
            }

            [[nodiscard]] bool isComplete() const
            {
                return isFollowedUp() && t3 > -1 && t4 > -1;
            }

            [[nodiscard]] NanoTime getT2Diff() const
            {
                return t2 - prevT2;
            }

            [[nodiscard]] NanoTime getT1Diff() const
            {
                return t1 - prevT1;
            }

            [[nodiscard]] NanoTime getT2mT1() const
            {
                return t2 - t1;
            }

            [[nodiscard]] NanoTime getT4mT3() const
            {
                return t4 - t3;
            }

            [[nodiscard]] NanoTime getDelay() const
            {
                return (t4 - t1 - (t3 - t2)) / 2;
            }

            [[nodiscard]] NanoTime getOffset() const
            {
                return t2 - t1 - getDelay() + Constants::HardwareOffsetNs;
            }

            [[nodiscard]] double getDrift() const
            {
                return (1. - static_cast<double>(getT2Diff()) / static_cast<double>(getT1Diff())) * Constants::NanosecondsPerSecond;
            }
        };

        void pollEventSocket();

        void pollGeneralSocket();

        void sendDelayReqMessage();

        void handleSyncMessage(const timespec &t2);

        void handleFollowUpMessage();

        void handleDelayRespMessage();

        [[nodiscard]] bool isLocked() const;

        void updateController(uint16_t exchangeId);

        void handle1588Interrupt() override;

        constexpr static uint8_t LEDPin{13};
        qindesign::network::EthernetUDP generalSocket{16};
        IntervalTimer readTimer;
        uint8_t rxBuffer[MTU];
        PTPV2Packet rxSyncPacket, txDelayReqPacket;
        std::map<uint16_t, TimeExchange> pendingExchanges;
        uint16_t exchangeId{0}, syncSequenceId{0}, delayReqSequenceId{0};
        double kP{1}, kI{.5};
        double drift{0}, adjust{0};
        int nspsAccu{0};
        uint lockCount{0};
        std::function<void(double)> controllerUpdatedCallback{nullptr};
        std::function<void(bool, NanoTime)> lockChangedCallback{nullptr};
    };
}

#endif //PTPSUBSCRIBER_H
