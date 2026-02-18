#include <AudioSystemManager.h>
#include <AnanasClient.h>
#include <ControlDataListener.h>
#include <wfs.h>
#include <EthernetManager.h>
#include <PTPSubscriber.h>
#include <ComponentManager.h>
#include "audio_processors/DistributedWFSProcessor.h"

using namespace ananas::WFS;

extern "C" uint8_t external_psram_size;

volatile uint32_t syncReceiveCounter{0};
volatile uint32_t followUpReceiveCounter{0};

volatile bool ptpLock{false};
AudioSystemConfig config{
    ananas::Constants::AudioBlockFrames,
    ananas::Constants::AudioSamplingRate,
    ClockRole::Subscriber
};
AudioSystemManager audioSystemManager{config};
PTPSubscriber ptpSubscriber{
    Constants::PTPEventSocketParams,
    Constants::PTPGeneralSocketParams,
    SystemUtils::Low
};
ananas::AudioClient ananasClient{ananas::Constants::AudioSocketParams};
ControlContext context;
ControlDataListener controlDataListener{context};
wfs wfs;
WFSModule wfsModule{ananasClient, wfs, context};
std::vector<NetworkProcessor *> networkProcessors{
    &ptpSubscriber,
    &ananasClient,
    &controlDataListener
};
EthernetManager ethernetManager{"t41wfsmodule", networkProcessors};
std::vector<ProgramComponent *> programComponents{
    &ethernetManager,
    &ptpSubscriber,
    &audioSystemManager,
    &ananasClient,
    &wfs,
    &wfsModule,
    &controlDataListener
};
ComponentManager componentManager{programComponents, SystemUtils::LogLevel::None};

void setup()
{
#ifdef WAIT_FOR_SERIAL
    while (!Serial) {
    }
#endif

    Serial.begin(2000000);

    ptpSubscriber.onLockChanged([](const bool isLocked, const NanoTime now)
    {
        ananasClient.setIsPtpLocked(isLocked);

        if (isLocked && !audioSystemManager.isClockRunning()) {
            audioSystemManager.startClock();
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
        Serial.printf("Resetting PTP and stopping audio.\n");
        ptpSubscriber.reset();
        audioSystemManager.stopClock();
    });

    audioSystemManager.onAudioPtpOffsetChanged([](const long offset)
    {
        ananasClient.setAudioPtpOffsetNs(offset);
    });

    context.moduleID.onChange = [](const int value)
    {
        wfs.setParamValue("moduleID", static_cast<float>(value));
    };
    context.speakerSpacing.onChange = [](const float value)
    {
        wfs.setParamValue("spacing", value);
    };
    // Set up source positions.
    char path[5];
    for (size_t i{0}; i < ananas::Constants::MaxChannels; ++i) {
        sprintf(path, "%d/x", i);
        context.sourcePositions.insert(ananas::WFS::SourcePositionsMap::value_type(
                path,
                ananas::SmoothedValue<float>{0., .975f})
        );
        sprintf(path, "%d/y", i);
        context.sourcePositions.insert(ananas::WFS::SourcePositionsMap::value_type(
                path,
                ananas::SmoothedValue<float>{0., .975f})
        );
    }
    for (auto &sp: context.sourcePositions) {
        // // If smoothing in Faust with si.smoo:
        // sp.second.onSet = [sp](const float value)
        // {
        //     // Serial.printf("Updating %s: %f\n", sp.first.c_str(), value);
        //     wfs.setParamValue(sp.first, value);
        // };

        // If smoothing outside of Faust:
        sp.second.onChange = [sp](float value)
        {
            // Serial.printf("%s changed: %.9f\n", sp.first.c_str(), value);
            value = ananas::Utils::clamp(value, -1.f, 1.f);
            wfs.setParamValue(sp.first, value);
        };
    }

    audioSystemManager.setAudioProcessor(&wfsModule);
    componentManager.begin();
}

void loop()
{
    componentManager.run();

    ananasClient.setPercentCPU(wfsModule.getCurrentPercentCPU());
    ananasClient.setModuleID(static_cast<uint16_t>(wfs.getParamValue("moduleID")));
}
