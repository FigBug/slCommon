#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "slUtil.h"

//==============================================================================
void applyGain (AudioSampleBuffer& buffer, LinearSmoothedValue<float>& gain);

inline double getMidiNoteInHertz (const double noteNumber, const double frequencyOfA = 440.0)
{
    return frequencyOfA * pow (2.0, (noteNumber - 69) / 12.0);
}
