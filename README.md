# Ananas Client

A time-sensitive audio client for the Teensy 4.1 microcontroller development
board. Coupled with the [Ananas](https://github.com/hatchjaw/ananas-server) 
audio server and associated DAW plugins, supports distributed audio applications
with sub-microsecond synchronisation via IEEE 1588 Precision Time Protocol over
a wired ethernet network.

Note that, due to the specific aims of media clock and presentation time
recovery in a distributed setting, this repository, specifically its
`AudioSystemManager` class and `AudioProcessor` interface, are not compatible
with the Teensy Audio Library.

## Dependencies

- [Teensy 4.1](https://www.pjrc.com/store/teensy41.html) microcontroller 
  development boards with 
  [ethernet](https://www.pjrc.com/store/ethernet_kit.html) and 
  [audio](http://pjrc.com/store/teensy3_audio.html) add-ons
- [A network switch with PTP support](#a-network-switch-with-ptp-support)

To install software dependencies, execute the script at
[scripts/installs-tools.sh](./scripts/install-tools.sh)

- [PlatformIO CLI](#platformio)
- [tytools](#tytools)
- [Faust](#faust)

- [Other dependencies](#other-dependencies)

### A network switch with PTP support

[This helpful guide](https://github.com/jclark/rpi-cm4-ptp-guide/blob/main/switches.md)
describes a few. The MikroTik
[CRS326-24G-2S series](https://mikrotik.com/product/crs326_24g_2s_in) is
probably the most cost-effective, but it is only capable as acting as a boundary
clock, i.e. not as a transparent clock; if daisy-chaining switches, some timing
discrepancy will be introduced, anecdotally on the order of a few hundred
nanoseconds per jump. 

Additionally, the CRS326's PTP algorithm does not cope well when traffic on the
switch is high, e.g. when a lot of bandwidth is used for transmitting audio 
data; specifically, although it sends sync messages without issue, follow-up 
messages are not transmitted on all PTP-enabled ports 100% of the time --- 
anecdotally, when transmitting eight channels (16-bit @ 48 kHz), follow-up loss
is on the order of 1-2 %; for sixteen channels it's upwards of 10 %, which is 
sporadically catastrophic for synchronisation.

### PlatformIO

[scripts/installs-tools.sh](./scripts/install-tools.sh) installs PlatformIO as 
per the [official documentation](https://docs.platformio.org/en/latest/core/installation/methods/installer-script.html#id1). After running the script you 
should now be able to execute `pio`

```
pio --version
```

yielding a result like `PlatformIO Core, version 6.1.18`.

### tytools

The other scripts in the `scripts` directory are useful for building PlatformIO
environments and uploading to multiple Teensies in a single command. They are
thus dependent upon the availability of the `pio` CLI and Niels Martigène's
very useful `tytools` utilities, specifically `tycmd`. Once again
[scripts/installs-tools.sh](./scripts/install-tools.sh) follows the [build
instructions](https://koromix.dev/tytools#hacking-tytools); after building `tycmd`, it is copied to `~/.local/bin`.

### Faust

A local installation of the Faust compiler and associated ecosystem is required
to compile .dsp files to classes compatible with the audiosync/ananas
`AudioProcessor` API. [scripts/installs-tools.sh](./scripts/install-tools.sh)
essentially follows Faust's basic [build instructions](https://github.com/grame-cncm/faust/wiki/BuildingSimple).

With the Faust compiler installed, navigate to the [faust](faust) directory and run

```shell
./faust2ananas -lib [program_name].dsp
```

This will place `[program_name].h` and `[program_name].cpp` in
`lib/[program_name]/src/`

### Other dependencies

The following are handled automatically by PlatformIO via `lib_deps` in
[platformio.ini](platformio.ini).

- A stripped-down version of the Teensy framework in directory
  [framework-arduinoteensy](framework-arduinoteensy); this includes
  modifications to permit the `clock-authority-usb-audio` firmware to run as a
  48 kHz USB audio device.
- A fork of [QNEthernet](https://github.com/ssilverman/qnethernet), based in
  turn on the fork
  [HedgeHawk/QNEthernet](https://github.com/HedgeHawk/QNEthernet), which
  implements various hardware registers associated with the IEEE 1588 ENET
  timer, and takes advantage of more recent QNEthernet developments, such as
  improved IGMP behaviour.
- For the `wfs-module` environment, a fork of
  [CNMAT/OSC](https://github.com/CNMAT/OSC), implementing class member callback
  functions on message/bundle receipt.

## Building and uploading firmware, rebooting

The utilities in [scripts](scripts) streamline these processes.

### `upload.sh`

#### Usage

```shell
./scripts/upload.sh [pio-environment]
```

Builds a given PlatformIO environment and uploads the result to all Teensies
connected to the host machine over USB, _except any running as a USB Audio
device_. E.g.

```shell
./scripts/upload.sh wfs-module
```

**To upload to a device that's currently programmed as a USB Audio device, first
press the button on the Teensy 4.1 board.** A small red LED should illuminate,
indicating that it's ready to receive new firmware.

### `upload-ptp-authority.sh`

#### Usage

```shell
./scripts/upload-ptp-authority.sh
```

This will build the `clock-authority-usb-audio` environment, and upload the
resulting firmware to the Teensy selected at the subsequent prompt.

### `upload-ptp-select-authority.sh`

#### Usage

```shell
./scripts/upload-env-select-authority.sh [pio-environment]
```

This will build the `clock-authority-usb-audio` and `[pio-environment]`
environments, prompt for a Teensy to which to upload the
`clock-authority-usb-audio` firmware, then upload the `[pio-environment]`
firmware to all other connected Teensies.

### `reboot.sh`

#### Usage

```shell
./scripts/reboot.sh
```

Reboots all connected Teensies **except for those running as a USB Audio
device** (i.e. those running the `clock-authority-usb-audio` firmware).

## Media clock recovery

The approach to media clock recovery, i.e. achieving sampling rate parity with
the time authority, is three-pronged:

1. When the PTP controller is updated, a callback function, with the
   nanosecond-per-second offset is called.
2. Additionally, once per second, `AudioSystemManager` calculates the offset of
   the audio subsystem with respect to PTP time. NB this only works for a
   sampling rate ($f_s$) of 48 kHz and audio buffer sizes ($N$) of 128 or less,
   i.e. those for which $f_s/N$ takes an integer value.
3. These two offsets are combined and used to calculate a new sampling rate,
   or, to be more precise, a new value for the `CCM_ANALOG_PLL_AUDIO_NUM`
   register of the NXP iMXRT1062 SoC, i.e. the numerator to the chip's audio
   phase locked loop (PLL4).

The need for the second prong is not 100% clear, but may be due to the fact that
whereas the ENET timer is an incrementing **numerical register**, the result of
the dividers to the audio PLL and synchronous audio interface (SAI1) is a
**clock frequency**, _and_ updating the value of the PLL4 numerator takes a
small amount of time (on the order of 28 ns). In any case this approach results
in a reliable sampling rate lock amongst clients on the network.

## Presentation time recovery

This too only works for $f_s/N \in \mathbb{N}$. In essence a given client awaits
a reported PTP offset of less than 100 ns (considered a PTP "lock"), then 
starts its audio subsystem. This entails powering up, unbypassing, and enabling
PLL4, enabling various properties of SAI1, setting up audio DMA, and configuring
and powering up the audio add-on.

For a network of Teensies, all synchronised to the same _clock authority_, this
ensures that presentation time, i.e. the moment at which samples are reproduced,
begins on a given PTP second. Additionally, clock subscribers check, on a 
per-second basis, whether they have an appropriately-timestamped audio packet
available and, if not, seek the packet with the smallest positive offset from
their internal time reference, i.e. the IEEE 1588 timer. Under typical (i.e.
ideal) working conditions, clock subscribers (and other firmwares based on the
clock subscriber model, such as `wfs-module`) all, after a second or two, find 
the appropriate packet to reproduce next, and continue to read packets 
consecutively thenceforth.

## Creating an `AudioProcesor` instance

The `AudioSystemManager` expects to receive an `AudioProcessor` instance to use
for audio processing via the `AudioSystemManager::setAudioProcessor` method.

It is possible to chain audio processors; for instance the `wfs-module` 
environment does this by way of an audio processor instance that contains two
audio processor references as private fields:

```c++
class WFSModule final : public AudioProcessor
{
public:
    WFSModule(AudioProcessor &ananasClient, AudioProcessor &faustWFS)
        : client(ananasClient), wfs(faustWFS) {}

    // ...

private:
    AudioProcessor &client;
    AudioProcessor &wfs;
};
```

Audio processors must override the `AudioProcessor::processImpl` method; for
the `wfs-module` this method takes the following form:

```c++
void processImpl(int16_t **inputBuffer, int16_t **outputBuffer, size_t const numFrames) override
{
    client.processAudio(inputBuffer, outputBuffer, numFrames);
    wfs.processAudio(outputBuffer, outputBuffer, numFrames);
}
```

Note that both the input and the output to `wfs.processAudio` is `outputBuffer`
this being a multidimensional array of pointers to the output of the `client`
instance.
