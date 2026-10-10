#ifndef AMBISONICSCONTROLCONTEXT_H
#define AMBISONICSCONTROLCONTEXT_H

#include <string>
#include <AnanasUtils.h>

namespace ananas::HOA
{
    using DecoderCoefficientsMap = std::unordered_map<std::string, ListenableParameter<float> >;

    struct ControlContext
    {
        DecoderCoefficientsMap decoderCoefficients;
        ListenableParameter<int> decoderRow0;
        ListenableParameter<int> decoderRow1;
    };
};

#endif //AMBISONICSCONTROLCONTEXT_H
