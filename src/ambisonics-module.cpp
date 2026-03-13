#include <AudioSystemManager.h>
#include <AnanasClient.h>
#include <AmbisonicsControlDataListener.h>
#include <ambisonics.h>
#include <EthernetManager.h>
#include <PTPSubscriber.h>
#include <ComponentManager.h>
#include "audio_processors/DistributedAmbisonicsProcessor.h"

using namespace ananas::Ambisonics;

SystemUtils::LogLevel logLevel{SystemUtils::LogLevel::None};
AudioSystemConfig config{
    ananas::Constants::AudioBlockFrames,
    ananas::Constants::AudioSamplingRate
};
AudioSystemManager audioSystemManager{config};
PTPSubscriber ptpSubscriber{
    Constants::PTPEventSocketParams,
    Constants::PTPGeneralSocketParams,
    SystemUtils::LogLevel::None
};
ananas::AudioClient ananasClient{
    ananas::Constants::AudioSocketParams,
    logLevel
};
ControlContext context;
ControlDataListener controlDataListener{context};
ambisonics ambisonics;
AmbisonicsModule ambisonicsModule{ananasClient, ambisonics, context};
std::vector<NetworkProcessor *> networkProcessors{
    &ptpSubscriber,
    &ananasClient,
    &controlDataListener
};
EthernetManager ethernetManager{"t41ambisonicsmodule", networkProcessors};
std::vector<ProgramComponent *> programComponents{
    &ethernetManager,
    &ptpSubscriber,
    &audioSystemManager,
    &ananasClient,
    &ambisonics,
    &ambisonicsModule,
    &controlDataListener
};
ComponentManager componentManager{
    programComponents,
    logLevel
};

void setup()
{
#ifdef WAIT_FOR_SERIAL
    while (!Serial) {
    }
#endif

    Serial.begin(0);

    ananasClient.setFirmwareType(SystemUtils::FirmwareType::ambisonicsModule);

    ptpSubscriber.onLockChanged([](const bool isLocked, const NanoTime now)
    {
        ananasClient.setIsPtpLocked(isLocked);

        if (isLocked && !audioSystemManager.isClockRunning()) {
            audioSystemManager.startClock();
            if (logLevel > SystemUtils::LogLevel::None) Serial.println("Subscriber start audio clock.");
        } else if (audioSystemManager.isClockRunning()) {
            ananasClient.adjustBufferReadIndex(now);
        }
    });

    ptpSubscriber.onControllerUpdated([](const double adjust)
    {
        audioSystemManager.adjustClock(adjust);
        ananasClient.setReportedSamplingRate(config.getExactSamplingRate());
    });

    audioSystemManager.onInvalidSamplingRate([]
    {
        Serial.printf("Resetting PTP and stopping audio.\n");
        ptpSubscriber.reset();
        audioSystemManager.stopClock();
    });

    audioSystemManager.onAudioPtpOffsetChanged([](const int32_t offset)
    {
        ananasClient.setAudioPtpOffsetNs(offset);
    });

    context.moduleID.onChange = [](const int value)
    {
        ambisonics.setParamValue("moduleID", static_cast<float>(value));
    };

    audioSystemManager.setAudioProcessor(&ambisonicsModule);
    componentManager.begin();
}

void loop()
{
    componentManager.run();

    ananasClient.setPercentCPU(ambisonicsModule.getCurrentPercentCPU());
    ananasClient.setModuleID(static_cast<int16_t>(ambisonics.getParamValue("moduleID")));
}
