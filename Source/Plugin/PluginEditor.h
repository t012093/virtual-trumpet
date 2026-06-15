#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

namespace VirtualTrumpet {

class PluginEditor : public juce::AudioProcessorEditor,
                     private juce::Timer {
public:
    PluginEditor (PluginProcessor&);
    ~PluginEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    PluginProcessor& audioProcessor;

    juce::Label titleLabel;
    juce::Label statusLabel;
    juce::Label midiCounterLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};

} // namespace VirtualTrumpet
