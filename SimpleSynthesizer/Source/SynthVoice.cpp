/*
  ==============================================================================

    SynthVoice.cpp
    Created: 1 May 2023 10:34:30pm
    Author:  serit

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    mOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    mAdsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    mAdsr.noteOff();
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{

}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    jassert(mIsPrepared);

    juce::dsp::AudioBlock<float> audioBlock{ outputBuffer };
    mOsc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    mGain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    mAdsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int numOutputChannels)
{
    mAdsr.setSampleRate(sampleRate);

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = numOutputChannels;

    mOsc.prepare(spec);
    mGain.prepare(spec);
    mGain.setGainLinear(0.5f);

    mIsPrepared = true;
}