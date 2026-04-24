#ifndef WFSCONTROLCONTEXT_H
#define WFSCONTROLCONTEXT_H

#include <string>
#include <unordered_map>
#include <AnanasUtils.h>

namespace ananas::WFS
{
    using SecondarySourceCoordinatesMap = std::unordered_map<std::string, ListenableParameter<float> >;
    using VirtualSourceCoordinatesMap = std::unordered_map<std::string, SmoothedValue<float> >;

    struct ControlContext
    {
        SecondarySourceCoordinatesMap secondarySourceCoordinates;
        VirtualSourceCoordinatesMap virtualSourceCoordinates;
    };
};

#endif //WFSCONTROLCONTEXT_H
