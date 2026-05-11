#include <AudioSystemManager.h>
#include <AnanasClient.h>
#include <passthrough.h>
#include <EthernetManager.h>
#include <PTPSubscriber.h>
#include <ComponentManager.h>

extern "C" uint8_t external_psram_size;

class PassthroughAudioProcessor : public AudioProcessor, public ProgramComponent
{
    public:
    PassthroughAudioProcessor(AudioProcessor &ananasClient, AudioProcessor &faustPassthrough)
        : client(ananasClient), passthrough(faustPassthrough)
    {
    }

    void prepare(const uint sampleRate) override
    {
        client.prepare(sampleRate);
        passthrough.prepare(sampleRate);
    }

    void run() override
    {
    }

    size_t printTo(Print &p) const override
    {
        return p.print("Module:            ") + AudioProcessor::printTo(p);
    }

    [[nodiscard]] size_t getNumInputs() const override { return client.getNumInputs(); }

    [[nodiscard]] size_t getNumOutputs() const override { return passthrough.getNumOutputs(); }

protected:
    void processImpl(int16_t **inputBuffer, int16_t **outputBuffer, size_t const numFrames) override
    {
        client.processAudio(inputBuffer, outputBuffer, numFrames);
        passthrough.processAudio(outputBuffer, outputBuffer, numFrames);
    }

    void beginImpl() override
    {
    }

private:
    AudioProcessor &client;
    AudioProcessor &passthrough;
};

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
passthrough passthrough;
PassthroughAudioProcessor passthroughProcessor{ananasClient, passthrough};
std::vector<NetworkProcessor *> networkProcessors{
    &ptpSubscriber,
    &ananasClient
};
EthernetManager ethernetManager{"t41passthrough", networkProcessors};
std::vector<ProgramComponent *> programComponents{
    &ethernetManager,
    &ptpSubscriber,
    &audioSystemManager,
    &ananasClient,
    &passthrough,
    &passthroughProcessor
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

    ananasClient.setFirmwareType(SystemUtils::FirmwareType::passthrough);

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

    audioSystemManager.setAudioProcessor(&passthroughProcessor);
    componentManager.begin();
}

void loop()
{
    componentManager.run();

    ananasClient.setPercentCPU(passthroughProcessor.getCurrentPercentCPU());
}
