#pragma once

#include <JuceHeader.h>

class WavetableSynth
{
public:
	void prepareToPlay(double sampleRate);
	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&);

private:
	double sampleRate;
};