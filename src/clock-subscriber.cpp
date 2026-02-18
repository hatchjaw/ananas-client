#include <AudioSystemManager.h>
#include <AnanasClient.h>
#include <ComponentManager.h>
#include <EthernetManager.h>
#include <PTPSubscriber.h>
#include <program_components/PTPManager.h>

extern "C" uint8_t external_psram_size;

volatile uint32_t syncReceiveCounter{0};
volatile uint32_t followUpReceiveCounter{0};

AudioSystemConfig config{
    ananas::Constants::AudioBlockFrames,
    ananas::Constants::AudioSamplingRate,
    ClockRole::Subscriber
};
AudioSystemManager audioSystemManager{config};
PTPSubscriber ptpSubscriber{
    Constants::PTPEventSocketParams,
    Constants::PTPGeneralSocketParams,
    SystemUtils::None
};
ananas::AudioClient ananasClient{ananas::Constants::AudioSocketParams};
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
ComponentManager componentManager{programComponents};

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
        // TODO: update PLL4 NUM more frequently?
    });

    audioSystemManager.setAudioProcessor(&ananasClient);
    componentManager.begin();
}

void loop()
{
    componentManager.run();
}
