#include <juce_core/juce_core.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "../../Source/Core/SynthEngine.h"
#include "../../Source/Core/Parameters.h"

class SynthEngineTest : public juce::UnitTest {
public:
    SynthEngineTest() : juce::UnitTest ("SynthEngine Basic Verification") {}

    void runTest() override
    {
        beginTest ("Parameters Setup & SynthEngine Initialization");
        
        struct DummyProcessor : public juce::AudioProcessor {
            DummyProcessor() : AudioProcessor (BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true)) {}
            ~DummyProcessor() override = default;
            
            void prepareToPlay (double, int) override {}
            void releaseResources() override {}
            void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override {}
            juce::AudioProcessorEditor* createEditor() override { return nullptr; }
            bool hasEditor() const override { return false; }
            const juce::String getName() const override { return "Dummy"; }
            bool acceptsMidi() const override { return true; }
            bool producesMidi() const override { return false; }
            bool isMidiEffect() const override { return false; }
            double getTailLengthSeconds() const override { return 0.0; }
            int getNumPrograms() override { return 1; }
            int getCurrentProgram() override { return 0; }
            void setCurrentProgram (int) override {}
            const juce::String getProgramName (int) override { return {}; }
            void changeProgramName (int, const juce::String&) override {}
            void getStateInformation (juce::MemoryBlock&) override {}
            void setStateInformation (const void*, int) override {}
        };

        DummyProcessor processor;
        juce::AudioProcessorValueTreeState apvts (processor, nullptr, "Params", VirtualTrumpet::Parameters::createParameterLayout());
        
        expect (processor.getParameters().size() > 0);

        VirtualTrumpet::SynthEngine engine (apvts);
        engine.prepare (44100.0, 512);

        expectEquals (engine.getMidiNoteOnCount(), 0);
        expect (!engine.isCurrentlyPlaying());

        beginTest ("SynthEngine Process MIDI NoteOn");
        juce::AudioBuffer<float> buffer (2, 512);
        buffer.clear();
        juce::MidiBuffer midiMessages;
        
        midiMessages.addEvent (juce::MidiMessage::noteOn (1, 60, 0.8f), 0);
        engine.process (buffer, midiMessages);

        expectEquals (engine.getMidiNoteOnCount(), 1);
        expect (engine.isCurrentlyPlaying());

        beginTest ("SynthEngine Process MIDI NoteOff");
        midiMessages.clear();
        midiMessages.addEvent (juce::MidiMessage::noteOff (1, 60, 0.0f), 0);
        engine.process (buffer, midiMessages);

        expect (!engine.isCurrentlyPlaying());
    }
};

static SynthEngineTest synthEngineTest;

int main (int argc, char* argv[])
{
    juce::UnitTestRunner runner;
    runner.runAllTests();
    
    for (int i = 0; i < runner.getNumResults(); ++i)
    {
        if (auto* result = runner.getResult (i))
        {
            if (result->failures > 0)
                return 1;
        }
    }
    
    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}
