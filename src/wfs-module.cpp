#include <AudioSystemManager.h>
#include <AnanasClient.h>
#include <wfs2.h>
#include <SecondarySourceCoordinateListener.h>
#include <VirtualSourceCoordinateListener.h>
#include <EthernetManager.h>
#include <PTPSubscriber.h>
#include <ComponentManager.h>

#include "audio_processors/DistributedWFSProcessor.h"

using namespace ananas::WFS;

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
ControlContext context;
VirtualSourceCoordinateListener vsListener{context};
SecondarySourceCoordinateListener ssListener{context};
wfs2 wfs;
WFSModule wfsModule{ananasClient, wfs, context};
std::vector<NetworkProcessor *> networkProcessors{
    &ptpSubscriber,
    &ananasClient,
    &vsListener,
    &ssListener
};
EthernetManager ethernetManager{"t41wfsmodule", networkProcessors};
std::vector<ProgramComponent *> programComponents{
    &ethernetManager,
    &ptpSubscriber,
    &audioSystemManager,
    &ananasClient,
    &wfs,
    &wfsModule,
    &vsListener,
    &ssListener
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

    ananasClient.setFirmwareType(SystemUtils::FirmwareType::wfsModule);

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

    // Set up virtual source positions.
    char path[8];
    for (size_t i{0}; i < 2; ++i) {
        sprintf(path, "ss/%d/x", i);
        context.secondarySourceCoordinates.insert(SecondarySourceCoordinatesMap::value_type(
            path,
            ananas::ListenableParameter(0.f)
        ));
        sprintf(path, "ss/%d/y", i);
        context.secondarySourceCoordinates.insert(SecondarySourceCoordinatesMap::value_type(
            path,
            ananas::ListenableParameter(0.f)
        ));
    }

    for (auto &ss: context.secondarySourceCoordinates) {
        ss.second.onChange = [ss](const float value)
        {
            // Serial.printf("%s changed: %.9f\n", ss.first.c_str(), value);
            wfs.setParamValue(ss.first, value);
            ananasClient.setSecondarySourceCoordinates(
                wfs.getParamValue("ss/0/x"),
                wfs.getParamValue("ss/0/y"),
                wfs.getParamValue("ss/1/x"),
                wfs.getParamValue("ss/1/y")
            );
        };
    }

    for (size_t i{0}; i < ananas::Constants::MaxChannels; ++i) {
        sprintf(path, "vs/%d/x", i);
        context.virtualSourceCoordinates.insert(VirtualSourceCoordinatesMap::value_type(
                path,
                ananas::SmoothedValue<float>{0., .975f})
        );
        sprintf(path, "vs/%d/y", i);
        context.virtualSourceCoordinates.insert(VirtualSourceCoordinatesMap::value_type(
                path,
                ananas::SmoothedValue<float>{0., .975f})
        );
    }
    for (auto &vs: context.virtualSourceCoordinates) {
#ifdef USE_SI_SMOO
        // If smoothing in Faust with si.smoo:
        vs.second.onSet = [vs](const float value)
        {
            // Serial.printf("Updating %s: %f\n", vs.first.c_str(), value);
            wfs.setParamValue(vs.first, value);
        };
#else
        // If smoothing outside of Faust:
        vs.second.onChange = [vs](float value)
        {
            // Serial.printf("%s changed: %.9f\n", vs.first.c_str(), value);
            // value = ananas::Utils::clamp(value, -1.f, 1.f);
            wfs.setParamValue(vs.first, value);
        };
    }
#endif

    audioSystemManager.setAudioProcessor(&wfsModule);

    componentManager.begin();
}

void loop()
{
    componentManager.run();

    ananasClient.setPercentCPU(wfsModule.getCurrentPercentCPU());
}
