#ifndef DISTRIBUTEDAMBISONICSPROCESSOR_H
#define DISTRIBUTEDAMBISONICSPROCESSOR_H

class AmbisonicsModule final : public AudioProcessor, public ProgramComponent
{
public:
    AmbisonicsModule(AudioProcessor &ananasClient, AudioProcessor &faustAmbi, ananas::Ambisonics::ControlContext &controlContext)
        : client(ananasClient), ambi(faustAmbi), context(controlContext)
    {
    }

    void prepare(const uint sampleRate) override
    {
        client.prepare(sampleRate);
        ambi.prepare(sampleRate);
    }

    void run() override
    {
    }

    size_t printTo(Print &p) const override
    {
        return p.print("Module:            ") + AudioProcessor::printTo(p);
    }

    [[nodiscard]] size_t getNumInputs() const override { return client.getNumInputs(); }

    [[nodiscard]] size_t getNumOutputs() const override { return ambi.getNumOutputs(); }

protected:
    void processImpl(int16_t **inputBuffer, int16_t **outputBuffer, size_t const numFrames) override
    {
        client.processAudio(inputBuffer, outputBuffer, numFrames);
        ambi.processAudio(outputBuffer, outputBuffer, numFrames);
    }

    void beginImpl() override
    {
    }

private:
    AudioProcessor &client;
    AudioProcessor &ambi;
    ananas::Ambisonics::ControlContext &context;
};

#endif //DISTRIBUTEDAMBISONICSPROCESSOR_H
