#ifndef SYSTEMUTILS_H
#define SYSTEMUTILS_H

#include <Arduino.h>

#ifndef LIB_VERSION
  #define LIB_VERSION "0.0.0"
#endif

class SystemUtils
{
public:
    // 0 = highest priority
    // Cortex-M7: 0,16,32,48,64,80,96,112,128,144,160,176,192,208,224,240
    enum IrqPriority : uint8_t
    {
        Priority0 = 0,
        Priority16 = 16,
        Priority32 = 32,
        Priority48 = 48,
        Priority64 = 64,
        Priority80 = 80,
        Priority96 = 96,
        Priority112 = 112,
        Priority128 = 128,
        Priority144 = 144,
        Priority160 = 160,
        Priority176 = 176,
        Priority192 = 192,
        Priority208 = 208,
        Priority224 = 224,
        Priority240 = 240,
    };

    enum class LogLevel {
        None = 0,
        Low = 1,
        Medium,
        High
    };

    struct VersionNumber
    {
        uint8_t major;
        uint8_t minor;
        uint8_t revision;
    };

    enum class FirmwareType : uint8_t
    {
        client = 0,
        wfsModule = 1,
        ambisonicsModule
    };

    static uint32_t computeSerialNumber()
    {
        uint32_t num{HW_OCOTP_MAC0 & 0xFFFFFF};
        if (num < 10000000) num *= 10;
        return num;
    }

    static void reboot()
    {
        SRC_GPR5 = 0x0BAD00F1;
        SCB_AIRCR = 0x05FA0004;
        while (true) {
        }
    }

    static constexpr uint8_t parseNum(const char* s, const int start, const int end) {
        uint8_t result{0};
        for (int i{start}; i < end; i++)
            result = result * 10 + (s[i] - '0');
        return result;
    }

    static constexpr int findDot(const char* s, const int from) {
        int i{from};
        while (s[i] != '.' && s[i] != '\0') i++;
        return i;
    }

    static constexpr VersionNumber getFirmwareVersion()
    {
        constexpr auto d1{findDot(LIB_VERSION, 0)};
        constexpr auto d2{findDot(LIB_VERSION, d1 + 1)};
        constexpr auto end{findDot(LIB_VERSION, d2 + 1)};
        return {
            parseNum(LIB_VERSION, 0, d1),
            parseNum(LIB_VERSION, d1 + 1, d2),
            parseNum(LIB_VERSION, d2 + 1, end)
        };
    }
};

#endif //SYSTEMUTILS_H
