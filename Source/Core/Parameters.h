#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace VirtualTrumpet {
namespace Parameters {

// Parameter ID definitions
inline constexpr auto breath = "breath";
inline constexpr auto tongue = "tongue";
inline constexpr auto lipTension = "lipTension";
inline constexpr auto lipAperture = "lipAperture";
inline constexpr auto vibratoDepth = "vibratoDepth";
inline constexpr auto vibratoRate = "vibratoRate";
inline constexpr auto expressionAssist = "expressionAssist";
inline constexpr auto legatoMode = "legatoMode";

inline constexpr auto mouthpiece = "mouthpiece";
inline constexpr auto boreProfile = "boreProfile";
inline constexpr auto bell = "bell";
inline constexpr auto valveTuning = "valveTuning";
inline constexpr auto mute = "mute";
inline constexpr auto tuning = "tuning";

inline constexpr auto outputGain = "outputGain";
inline constexpr auto width = "width";
inline constexpr auto roomAmount = "roomAmount";
inline constexpr auto safetyLimiter = "safetyLimiter";

// Helper to create parameter layout
juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

} // namespace Parameters
} // namespace VirtualTrumpet
