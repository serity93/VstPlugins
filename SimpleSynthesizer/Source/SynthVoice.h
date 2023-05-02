/*
  ==============================================================================

    SynthVoice.h
    Created: 1 May 2023 10:34:30pm
    Author:  serit

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

    void prepareToPlay(double sampleRate, int samplesPerBlock, int numOutputChannels);

private:
    bool mIsPrepared{ false };

    juce::ADSR mAdsr;
    juce::ADSR::Parameters mAdsrParams;

    // return std::sin(x); // sine wave
    // return x / MathConstants<float>::pi; // saw wave
    // return x < 0.0f ? -1.0f : 1.0f; // square wave
    juce::dsp::Oscillator<float> mOsc{ [](float x) { return std::sin(x); }};
    juce::dsp::Gain<float> mGain;
};