#include "PTPClock.h"
#include <arm_math.h>
#include <AnanasUtils.h>
#include <QNEthernet.h>
#include <SystemUtils.h>

using namespace qindesign::network;

namespace ananas::network
{
    PTPClock::PTPClock(const SocketParams &eventSocketParams,
                       const SocketParams &generalSocketParams,
                       const SystemUtils::LogLevel logLevel)
        : ip(eventSocketParams.ip),
          eventPort(eventSocketParams.port),
          generalPort(generalSocketParams.port),
          logging(logLevel)
    {
        sInstance = this;
    }

    void PTPClock::beginImpl()
    {
        pin24SwMuxControlRegister.begin();
        // Set up ENET PPS out on pin 24
        if (!pin24SwMuxControlRegister.setMuxMode(Pin24SwMuxControlRegister::MuxMode::ENET_1588_EVENT1_OUT) ||
            !EthernetIEEE1588.setChannelCompareValue(1, ananas::Constants::NanosecondsPerSecond) ||
            !EthernetIEEE1588.setChannelMode(1, EthernetIEEE1588Class::TimerChannelModes::kPulseHighOnCompare) ||
            !EthernetIEEE1588.setChannelOutputPulseWidth(1, 25) ||
            !EthernetIEEE1588.setChannelInterruptEnable(1, true)) {
            Serial.printf("Failed to set up Ethernet IEEE 1588 timer. Restarting.");
            SystemUtils::reboot();
        }

        attachInterruptVector(IRQ_ENET_TIMER, interrupt1588Timer);
        NVIC_ENABLE_IRQ(IRQ_ENET_TIMER);
        NVIC_SET_PRIORITY(IRQ_ENET_TIMER, SystemUtils::IrqPriority::Priority64);
    }

    void PTPClock::handle1588Interrupt()
    {
        if (!EthernetIEEE1588.getAndClearChannelStatus(1)) {
            __DSB();
            return;
        }

        uint32_t t;
        EthernetIEEE1588.getChannelCompareValue(1, t);
        t = (static_cast<NanoTime>(t) + ananas::Constants::NanosecondsPerSecond - 60) % ananas::Constants::NanosecondsPerSecond;

        EthernetIEEE1588.readTimer(currentInterruptTime);

        __DSB();
    }

    void PTPClock::interrupt1588Timer()
    {
        if (sInstance) {
            sInstance->handle1588Interrupt();
        }
    }
}
