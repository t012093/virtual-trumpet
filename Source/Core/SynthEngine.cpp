#include "SynthEngine.h"
#include "Parameters.h"

namespace VirtualTrumpet {

SynthEngine::SynthEngine(juce::AudioProcessorValueTreeState& apvts)
    : state(apvts)
{
}

void SynthEngine::prepare(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    maxBlockSize = samplesPerBlock;
    phase = 0.0f;
    isNoteActive = false;
    midiNoteOnCount = 0;
}

void SynthEngine::process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    handleMidiEvents(midiMessages);

    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    // Read parameter values
    const auto breathVal = state.getRawParameterValue(Parameters::breath)->load();
    const auto outputGainDb = state.getRawParameterValue(Parameters::outputGain)->load();
    const auto outputGainLinear = juce::Decibels::decibelsToGain(outputGainDb);

    // Simple test tone: a sine wave activated by midi notes and shaped by breath.
    // This will be replaced with the actual physical model in Phase 1.
    const float breathGain = breathVal;
    
    phaseIncrement = (float)(juce::MathConstants<double>::twoPi * currentFrequency / currentSampleRate);

    auto* leftChannel = buffer.getWritePointer(0);
    auto* rightChannel = numChannels > 1 ? buffer.getWritePointer(1) : nullptr;

    for (int sample = 0; sample < numSamples; ++sample)
    {
        float outputSample = 0.0f;
        if (isNoteActive)
        {
            outputSample = std::sin(phase) * 0.2f * breathGain * outputGainLinear;
            phase += phaseIncrement;
            if (phase >= juce::MathConstants<float>::twoPi)
                phase -= juce::MathConstants<float>::twoPi;
        }
        else
        {
            phase = 0.0f;
        }

        leftChannel[sample] = outputSample;
        if (rightChannel != nullptr)
        {
            rightChannel[sample] = outputSample;
        }
    }
}

void SynthEngine::release()
{
}

void SynthEngine::handleMidiEvents(juce::MidiBuffer& midiMessages)
{
    for (const auto metadata : midiMessages)
    {
        const auto message = metadata.getMessage();
        if (message.isNoteOn())
        {
            isNoteActive = true;
            currentFrequency = (float)juce::MidiMessage::getMidiNoteInHertz(message.getNoteNumber());
            midiNoteOnCount++;
        }
        else if (message.isNoteOff())
        {
            isNoteActive = false;
        }
        else if (message.isAllNotesOff())
        {
            isNoteActive = false;
        }
    }
}

} // namespace VirtualTrumpet
