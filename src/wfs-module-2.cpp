#include <Audio.h>
#include <AnanasClientTAL.h>
#include <SecondarySourceCoordinateListener.h>
#include <VirtualSourceCoordinateListener.h>
#include <EthernetManager.h>
#include <PTPSubscriber.h>
#include <ComponentManager.h>
#include <wfs2.h>

using namespace ananas::WFS;

SystemUtils::LogLevel logLevel{SystemUtils::LogLevel::Medium};
AudioSystemConfig config{
    ananas::Constants::AudioBlockFrames,
    ananas::Constants::AudioSamplingRate
};
PTPSubscriber ptpSubscriber{
    Constants::PTPEventSocketParams,
    Constants::PTPGeneralSocketParams,
    SystemUtils::LogLevel::None
};
ControlContext context;
VirtualSourceCoordinateListener vsListener{context};
SecondarySourceCoordinateListener ssListener{context};
ananas::AudioClientTAL ananasClient{
    ananas::Constants::AudioSocketParams,
    logLevel
};
wfs2 wfs;
AudioOutputI2SAnanas i2s{config};
std::vector<std::unique_ptr<AudioConnection> > patchCords;
std::vector<NetworkProcessor *> networkProcessors{
    &ptpSubscriber,
    &ananasClient,
    &vsListener,
    &ssListener
};
EthernetManager ethernetManager{"t41wfsmodule2", networkProcessors};
std::vector<ProgramComponent *> programComponents{
    &ethernetManager,
    &ptpSubscriber,
    &i2s,
    &ananasClient,
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

    for (size_t i{0}; i < ananas::Constants::MaxChannels; ++i) {
        // Network inputs to WFS inputs.
        patchCords.push_back(std::make_unique<AudioConnection>(ananasClient, i, wfs, i));
    }

    // WFS outputs to I2S outputs.
    patchCords.push_back(std::make_unique<AudioConnection>(wfs, 0, i2s, 0));
    patchCords.push_back(std::make_unique<AudioConnection>(wfs, 1, i2s, 1));

    ananasClient.setFirmwareType(SystemUtils::FirmwareType::wfsModule);

    ptpSubscriber.onLockChanged([](const bool isLocked, const NanoTime now)
    {
        ananasClient.setIsPtpLocked(isLocked);

        if (isLocked && !i2s.isClockRunning()) {
            i2s.startClock();
            if (logLevel > SystemUtils::LogLevel::None) Serial.println("Subscriber start audio clock.");
        } else if (i2s.isClockRunning()) {
            ananasClient.adjustBufferReadIndex(now);
        }
    });

    ptpSubscriber.onControllerUpdated([](const double adjust)
    {
        i2s.adjustClock(adjust);
        ananasClient.setReportedSamplingRate(config.getExactSamplingRate());
    });

    i2s.onInvalidSamplingRate([]
    {
        Serial.printf("Resetting PTP and stopping audio.\n");
        ptpSubscriber.reset();
        i2s.stopClock();
    });

    i2s.onAudioPtpOffsetChanged([](const int32_t offset)
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

    componentManager.begin();
}

void loop()
{
    componentManager.run();

    ananasClient.setPercentCPU(AudioProcessorUsage());
}
