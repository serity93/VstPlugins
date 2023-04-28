#include "WavetableSynth.h"

void WavetableSynth::prepareToPlay(double sampleRate)
{
	this->sampleRate = sampleRate;

	initializeOscillators();
}

void WavetableSynth::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages, float gain)
{
	auto currentSample = 0;

	for (const auto midiMetadata : midiMessages)
	{
		const auto midiMessage = midiMetadata.getMessage();
		const auto midiMessagePosition = static_cast<int>(midiMessage.getTimeStamp());

		render(buffer, currentSample, midiMessagePosition, gain);
		currentSample = midiMessagePosition;
		handleMidiEvent(midiMessage);
	}

	render(buffer, currentSample, buffer.getNumSamples(), gain);
}

void WavetableSynth::initializeOscillators()
{
	constexpr auto OSCILLATORS_COUNT = 128;
	const auto sineWaveTable = generateSineWaveTable();

	oscillators.clear();
	for (auto i = 0; i < OSCILLATORS_COUNT; ++i)
	{
		oscillators.emplace_back(sineWaveTable, sampleRate);
	}
}

std::vector<float> WavetableSynth::generateSineWaveTable()
{
	constexpr auto WAVETABLE_LENGTH = 64;
	std::vector<float> sineWaveTable(WAVETABLE_LENGTH);
	const auto PI = std::atanf(1.0f) * 4;

	for (auto i = 0; i < WAVETABLE_LENGTH; ++i)
	{
		sineWaveTable[i] = std::sinf(2 * PI * static_cast<float>(i) / WAVETABLE_LENGTH);
	}

	return sineWaveTable;
}

void WavetableSynth::handleMidiEvent(const juce::MidiMessage& midiEvent)
{
	if (midiEvent.isNoteOn())
	{
		const auto oscillatorId = midiEvent.getNoteNumber();
		const auto frequency = midiNoteNumberToFrequency(oscillatorId);
		oscillators[oscillatorId].setFrequency(frequency);
	}
	else if (midiEvent.isNoteOff())
	{
		const auto oscillatorId = midiEvent.getNoteNumber();
		oscillators[oscillatorId].stop();
	}
	else if (midiEvent.isAllNotesOff())
	{
		for (auto& oscillator : oscillators)
		{
			oscillator.stop();
		}
	}
}

float WavetableSynth::midiNoteNumberToFrequency(int midiNoteNumber)
{
	constexpr auto A4_FREQUENCY = 440.0f;
	constexpr auto A4_NOTE_NUMBER = 69.0f;
	constexpr auto NOTES_IN_AN_OCTAVE = 12.0f;

	return A4_FREQUENCY * std::powf(2, (static_cast<float>(midiNoteNumber) - A4_NOTE_NUMBER) / NOTES_IN_AN_OCTAVE);
}

void WavetableSynth::render(juce::AudioBuffer<float>& buffer, int startSample, int endSample, float gain)
{
	auto* firstChannel = buffer.getWritePointer(0);

	for (auto& oscillator : oscillators)
	{
		if (oscillator.isPlaying())
		{
			for (auto sample = startSample; sample < endSample; ++sample)
			{
				firstChannel[sample] += oscillator.getSample() * juce::Decibels::decibelsToGain(gain);
			}
		}
	}

	for (auto channel = 1; channel < buffer.getNumChannels(); ++channel)
	{
		auto* channelData = buffer.getWritePointer(channel);
		std::copy(firstChannel + startSample, firstChannel + endSample, channelData + startSample);
	}
}