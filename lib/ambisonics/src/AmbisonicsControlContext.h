#ifndef AMBISONICSCONTROLCONTEXT_H
#define AMBISONICSCONTROLCONTEXT_H

#include <AnanasUtils.h>

namespace ananas::Ambisonics
{
    struct ControlContext
    {
        ListenableParameter<int> moduleID;
    };
};

#endif //AMBISONICSCONTROLCONTEXT_H
