/**
 * This sketch describes a Teensy running as a USB audio device *and* as a PTP
 * clock authority. If this works as intended, a Teensy running this sketch
 * should serve as an authoritative source of time for:
 *
 * - audio interrupts on a general purpose machine acting as a multicast
 * networked audio server.
 * - a network of Teensies running as PTP clock subscribers *and* as recipients
 * for data transmitted by the multicast audio server.
 */

#include <AnanasPacket.h>
#include <Audio.h>
#include <QNEthernet.h>
#include <TimeLib.h>
#include <Announcer.h>
#include <ComponentManager.h>
#include <EthernetManager.h>
#include <PTPAuthority.h>

class PulsePerSecond : public AudioStream
{
public:
    PulsePerSecond() : AudioStream(0, nullptr)
    {
    }

private:
    void update() override
    {
        if (++numBuffers == kBuffersPerSec) {
            numBuffers = 0;
            if (auto *block{allocate()}) {
                memset(block->data, 0, sizeof(block->data));
                block->data[0] = INT16_MAX - 1; // Trigger logic analyser at value 0x7FFE
                transmit(block, 0);
                release(block);
            }
        }
    }

    static constexpr uint kBuffersPerSec{AUDIO_SAMPLE_RATE_EXACT / AUDIO_BLOCK_SAMPLES};
    uint numBuffers{0};
};

AudioControlSGTL5000 audioShield;
#ifdef USB_AUDIO
AudioInputUSB usbIn;
AudioOutputI2S out;
PulsePerSecond pps;
AudioConnection c0(pps, 0, out, 0);
AudioConnection c2(usbIn, 1, out, 1);
#endif

PTPAuthority ptpAuthority{
    Constants::PTPEventSocketParams,
    Constants::PTPGeneralSocketParams,
    SystemUtils::LogLevel::Low
};
Announcer<ananas::AuthorityAnnouncePacket> authorityAnnouncer{
    ananas::Constants::AuthorityAnnounceSocketParams
};
std::vector<NetworkProcessor *> networkProcessors{
    &ptpAuthority,
    &authorityAnnouncer
};
EthernetManager ethernetManager{"t41clockauthority", networkProcessors};
std::vector<ProgramComponent *> programComponents{
    &ethernetManager,
    &ptpAuthority,
    &authorityAnnouncer
};
ComponentManager componentManager{programComponents, SystemUtils::LogLevel::Medium};

void setup()
{
#ifdef WAIT_FOR_SERIAL
    while (!Serial) {}
#endif

    Serial.begin(0);

    AudioMemory(10);
    audioShield.enable();
    audioShield.volume(.75f);

    componentManager.begin();
}

void loop()
{
    componentManager.run();

#ifdef USB_AUDIO
    authorityAnnouncer.txPacket.numUnderruns = AudioInputUSB::getNumUnderruns();
    authorityAnnouncer.txPacket.numOverflows = AudioInputUSB::getNumOverflows();
    authorityAnnouncer.txPacket.usbFeedbackAccumulator = AudioInputUSB::getFeedbackAccumulator();
#endif
}
