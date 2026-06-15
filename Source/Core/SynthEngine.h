#pragma once

#include <atomic>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace VirtualTrumpet {

class SynthEngine {
public:
    SynthEngine(juce::AudioProcessorValueTreeState& apvts);
    ~SynthEngine() = default;

    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void release();

    // Getters for testing and verification
    int getMidiNoteOnCount() const noexcept { return midiNoteOnCount; }
    void resetMidiCounter() noexcept { midiNoteOnCount = 0; }
    bool isCurrentlyPlaying() const noexcept { return isNoteActive; }

private:
    void handleMidiEvents(juce::MidiBuffer& midiMessages);

    juce::AudioProcessorValueTreeState& state;
    double currentSampleRate { 0.0 };
    int maxBlockSize { 0 };

    // Oscillator phase for test tone
    float phase { 0.0f };
    float phaseIncrement { 0.0f };
    float currentFrequency { 440.0f };

    // Playback state
    std::atomic<bool> isNoteActive { false };
    std::atomic<int> midiNoteOnCount { 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthEngine)
};

} // namespace VirtualTrumpet
