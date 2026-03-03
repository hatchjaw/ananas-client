#include <AudioSystemManager.h>
#include <AnanasClient.h>
#include <ComponentManager.h>
#include <EthernetManager.h>
#include <PTPSubscriber.h>

extern "C" uint8_t external_psram_size;

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
std::vector<NetworkProcessor *> networkProcessors{
    &ptpSubscriber,
    &ananasClient
};
EthernetManager ethernetManager{"t41clocksubscriber", networkProcessors};
std::vector<ProgramComponent *> programComponents{
    &ethernetManager,
    &ptpSubscriber,
    &audioSystemManager,
    &ananasClient
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

    ananasClient.setFirmwareType(SystemUtils::FirmwareType::client);

    ptpSubscriber.onLockChanged([](const bool isLocked, const NanoTime now)
    {
        ananasClient.setIsPtpLocked(isLocked);

        if (isLocked && !audioSystemManager.isClockRunning()) {
            audioSystemManager.startClock();
            if (logLevel > SystemUtils::LogLevel::None)
                Serial.println("Subscriber start audio clock.");
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
        if (logLevel > SystemUtils::LogLevel::None)
            Serial.printf("Resetting PTP and stopping audio.\n");
        ptpSubscriber.reset();
        audioSystemManager.stopClock();
    });

    audioSystemManager.onAudioPtpOffsetChanged([](const int32_t offset)
    {
        ananasClient.setAudioPtpOffsetNs(offset);
    });

    audioSystemManager.setAudioProcessor(&ananasClient);
    componentManager.begin();
}

void loop()
{
    componentManager.run();

    ananasClient.setPercentCPU(ananasClient.getCurrentPercentCPU());
}
