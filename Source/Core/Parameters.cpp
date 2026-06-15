#include "Parameters.h"

namespace VirtualTrumpet {
namespace Parameters {

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Performance Parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { breath, 1 }, "Breath (CC2)", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { tongue, 1 }, "Tongue", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { lipTension, 1 }, "Lip Tension", juce::NormalisableRange<float>(-1.0f, 1.0f, 0.01f), 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { lipAperture, 1 }, "Lip Aperture", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));

    params.push_back(std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { vibratoDepth, 1 }, "Vibrato Depth", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { vibratoRate, 1 }, "Vibrato Rate", juce::NormalisableRange<float>(1.0f, 15.0f, 0.1f), 6.0f));

    params.push_back(std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { expressionAssist, 1 }, "Expression Assist", true));

    params.push_back(std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { legatoMode, 1 }, "Legato Mode", true));

    // Instrument Parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { mouthpiece, 1 }, "Mouthpiece", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));

    params.push_back(std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { boreProfile, 1 }, "Bore Profile", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));

    params.push_back(std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { bell, 1 }, "Bell", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));

    params.push_back(std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { valveTuning, 1 }, "Valve Tuning", juce::NormalisableRange<float>(-100.0f, 100.0f, 1.0f), 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { mute, 1 }, "Mute", false));

    params.push_back(std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { tuning, 1 }, "Tuning (cents)", juce::NormalisableRange<float>(-50.0f, 50.0f, 1.0f), 0.0f));

    // Output Parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { outputGain, 1 }, "Output Gain", juce::NormalisableRange<float>(-96.0f, 12.0f, 0.1f), 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { width, 1 }, "Width", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f)); // Defaults to 0 (Mono)

    params.push_back(std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { roomAmount, 1 }, "Room Amount", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.2f));

    params.push_back(std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { safetyLimiter, 1 }, "Safety Limiter", true));

    return { params.begin(), params.end() };
}

} // namespace Parameters
} // namespace VirtualTrumpet
