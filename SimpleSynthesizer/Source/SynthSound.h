/*
  ==============================================================================

    SynthSound.h
    Created: 1 May 2023 10:35:21pm
    Author:  serit

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
};