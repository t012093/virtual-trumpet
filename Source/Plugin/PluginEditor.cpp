#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace VirtualTrumpet {

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 300);

    titleLabel.setText ("Virtual Trumpet", juce::dontSendNotification);
    titleLabel.setFont (juce::FontOptions (24.0f, juce::Font::bold));
    titleLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (titleLabel);

    statusLabel.setText ("Phase 0: Skeleton Engine Active", juce::dontSendNotification);
    statusLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (statusLabel);

    midiCounterLabel.setText ("MIDI Notes Received: 0", juce::dontSendNotification);
    midiCounterLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (midiCounterLabel);

    startTimerHz (30);
}

void PluginEditor::paint (juce::Graphics& g)
{
    // Sleek dark-mode aesthetic background
    g.fillAll (juce::Colour (0xff121214));
    
    // Draw a subtle accent border
    g.setColour (juce::Colour (0xff3f3f46));
    g.drawRect (getLocalBounds(), 2);
}

void PluginEditor::resized()
{
    auto area = getLocalBounds();
    titleLabel.setBounds (area.removeFromTop (80));
    statusLabel.setBounds (area.removeFromTop (60));
    midiCounterLabel.setBounds (area.removeFromTop (60));
}

void PluginEditor::timerCallback()
{
    const int count = audioProcessor.getEngine().getMidiNoteOnCount();
    const bool isPlaying = audioProcessor.getEngine().isCurrentlyPlaying();
    
    juce::String text = "MIDI Notes Received: " + juce::String (count);
    if (isPlaying)
        text += " (Playing)";
        
    midiCounterLabel.setText (text, juce::dontSendNotification);
}

} // namespace VirtualTrumpet
