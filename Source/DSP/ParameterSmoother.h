#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

namespace VirtualTrumpet {

template <typename Type>
class ParameterSmoother {
public:
    ParameterSmoother() = default;
    ~ParameterSmoother() = default;

    void reset (double sampleRate, double rampTimeInSeconds)
    {
        smoothedValue.reset (sampleRate, rampTimeInSeconds);
    }

    void setCurrentAndTargetValue (Type newValue)
    {
        smoothedValue.setCurrentAndTargetValue (newValue);
    }

    void setTargetValue (Type targetValue)
    {
        smoothedValue.setTargetValue (targetValue);
    }

    Type getNextValue() noexcept
    {
        return smoothedValue.getNextValue();
    }

    Type getCurrentValue() const noexcept
    {
        return smoothedValue.getCurrentValue();
    }

    bool isSmoothing() const noexcept
    {
        return smoothedValue.isSmoothing();
    }

private:
    juce::LinearSmoothedValue<Type> smoothedValue;
};

} // namespace VirtualTrumpet
