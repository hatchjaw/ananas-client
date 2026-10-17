#include <AudioSystemManager.h>
#include <AnanasClient.h>
#include <AmbisonicsControlDataListener.h>
#include <ambisonics.h>
#include <EthernetManager.h>
#include <PTPSubscriber.h>
#include <ComponentManager.h>
#include "audio_processors/DistributedAmbisonicsProcessor.h"

using namespace ananas::HOA;

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
ananas::AudioClient<ananas::HoaClientAnnouncePacket> ananasClient{
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

    // Set up decoder coefficients
    char path[8];
    for (size_t i{0}; i < ananas::Constants::NumHOAChannels; ++i) {
        sprintf(path, "dm/0/%d", i);
        context.decoderCoefficients.insert(DecoderCoefficientsMap::value_type(
            path,
            ananas::ListenableParameter(0.f)
        ));
        sprintf(path, "dm/1/%d", i);
        context.decoderCoefficients.insert(DecoderCoefficientsMap::value_type(
            path,
            ananas::ListenableParameter(0.f)
        ));
    }

    for (auto &dc: context.decoderCoefficients) {
        dc.second.onChange = [dc](const float value)
        {
            Serial.printf("%s changed: %.9f\n", dc.first.c_str(), value);
            ambisonics.setParamValue(dc.first, value);
        };
    }

    context.decoderRow0.onChange = [](const int value)
    {
        Serial.printf("decoder row 0 changed: %d\n", value);
        ananasClient.setDecoderMatrixRow0(value);
    };

    context.decoderRow1.onChange = [](const int value)
    {
        Serial.printf("decoder row 1 changed: %d\n", value);
        ananasClient.setDecoderMatrixRow1(value);
    };

    audioSystemManager.setAudioProcessor(&ambisonicsModule);
    componentManager.begin();
}

void loop()
{
    componentManager.run();

    ananasClient.setPercentCPU(ambisonicsModule.getCurrentPercentCPU());
}
