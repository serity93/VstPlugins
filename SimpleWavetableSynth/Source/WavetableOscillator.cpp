#include <cmath>
#include "WavetableOscillator.h"

WavetableOscillator::WavetableOscillator(std::vector<float> waveTable, double sampleRate)
	: waveTable{ std::move(waveTable) }, sampleRate{ sampleRate }
{

}

//==============================================================================
void WavetableOscillator::stop()
{
	index = 0.0f;
	indexIncrement = 0.0f;
}

bool WavetableOscillator::isPlaying()
{
	return indexIncrement != 0.0f;
}

//==============================================================================
void WavetableOscillator::setFrequency(float frequency)
{
	indexIncrement = frequency * static_cast<float>(waveTable.size()) / static_cast<float>(sampleRate);
}

float WavetableOscillator::getSample()
{
	const auto sample = interpolateLinearly();
	index += indexIncrement;
	index = std::fmod(index, static_cast<float>(waveTable.size()));
	return sample;
}

float WavetableOscillator::interpolateLinearly() const
{
	const auto truncatedIndex = static_cast<typename decltype(waveTable)::size_type>(index);
	const auto nextIndex = static_cast<typename decltype(waveTable)::size_type>(std::ceil(index)) % waveTable.size();

	const auto nextIndexWeight = index - static_cast<float>(truncatedIndex);
	return waveTable[nextIndex] * nextIndexWeight + (1.f - nextIndexWeight) * waveTable[truncatedIndex];
}