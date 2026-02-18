#ifndef PTPCLOCK_H
#define PTPCLOCK_H

#include <IPAddress.h>
#include <NetworkUtils.h>
#include <ProgramComponent.h>
#include <SystemUtils.h>
#include <registers/SwMuxControlRegister.h>

namespace ananas::network
{
    class PTPClock : public ProgramComponent
    {
    public:
        PTPClock(const SocketParams &eventSocketParams,
            const SocketParams &generalSocketParams,
            SystemUtils::LogLevel logLevel = SystemUtils::LogLevel::None);

    protected:
        void beginImpl() override;

        virtual void handle1588Interrupt();

        static inline PTPClock *sInstance{nullptr};

        timespec currentInterruptTime{};
        IPAddress ip;
        uint16_t eventPort{0}, generalPort{0};
        SystemUtils::LogLevel logging;

    private:
        static void interrupt1588Timer();

        Pin24SwMuxControlRegister pin24SwMuxControlRegister;
    };
}

#endif //PTPCLOCK_H
