/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleWavetableSynthAudioProcessorEditor::SimpleWavetableSynthAudioProcessorEditor (SimpleWavetableSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    mGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    mGainSlider.setRange(-72.0f, 0.0f, 0.01f);
    mGainSlider.setValue(audioProcessor.mGain);
    mGainSlider.addListener(this);
    addAndMakeVisible(mGainSlider);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 400);
}

SimpleWavetableSynthAudioProcessorEditor::~SimpleWavetableSynthAudioProcessorEditor()
{
}

//==============================================================================
void SimpleWavetableSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void SimpleWavetableSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    mGainSlider.setBounds(getWidth() / 2 - 100, getHeight() / 2 - 100, 200, 200);
}

void SimpleWavetableSynthAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &mGainSlider)
    {
        audioProcessor.mGain = mGainSlider.getValue();
    }
}