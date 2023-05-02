/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleSynthesizerAudioProcessor::SimpleSynthesizerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    mSynth.addSound(new SynthSound());
    mSynth.addVoice(new SynthVoice());
}

SimpleSynthesizerAudioProcessor::~SimpleSynthesizerAudioProcessor()
{
}

//==============================================================================
const juce::String SimpleSynthesizerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleSynthesizerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleSynthesizerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleSynthesizerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleSynthesizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleSynthesizerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleSynthesizerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleSynthesizerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SimpleSynthesizerAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleSynthesizerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SimpleSynthesizerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSynth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < mSynth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(mSynth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumInputChannels());
        }
    }
}

void SimpleSynthesizerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleSynthesizerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SimpleSynthesizerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int i = 0; i < mSynth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<juce::SynthesiserVoice*>(mSynth.getVoice(i)))
        {
            // osc controls
            // ADSR
            // LFO
        }
    }

    mSynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool SimpleSynthesizerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleSynthesizerAudioProcessor::createEditor()
{
    return new SimpleSynthesizerAudioProcessorEditor (*this);
}

//==============================================================================
void SimpleSynthesizerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SimpleSynthesizerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleSynthesizerAudioProcessor();
}
