#pragma once

#include <cmath>
#include <atomic>
#include <juce_audio_basics/juce_audio_basics.h>

namespace VirtualTrumpet {

class SafetyLimiter {
public:
    SafetyLimiter() = default;
    ~SafetyLimiter() = default;

    void prepare (double sampleRate)
    {
        // Cutoff frequency for DC blocking filter (5.0 Hz)
        const double cutoffHz = 5.0;
        r = static_cast<float> (std::exp (-juce::MathConstants<double>::twoPi * cutoffHz / sampleRate));
        reset();
    }

    void reset()
    {
        x1 = 0.0f;
        y1 = 0.0f;
        hasBlownUp.store (false);
    }

    float processSample (float input)
    {
        // 1. NaN/Inf check
        if (std::isnan (input) || std::isinf (input))
        {
            hasBlownUp.store (true);
            reset();
            return 0.0f;
        }

        // 2. DC Blocker (1st-order High-Pass Filter)
        float hp = input - x1 + r * y1;
        x1 = input;
        y1 = hp;

        // Double check after HP filter to protect output
        if (std::isnan (hp) || std::isinf (hp))
        {
            hasBlownUp.store (true);
            reset();
            return 0.0f;
        }

        // 3. Soft Clipping (Saturation)
        // Smoothly compress values exceeding 0.9f
        const float threshold = 0.9f;
        if (std::abs (hp) > threshold)
        {
            const float sign = hp > 0.0f ? 1.0f : -1.0f;
            const float absVal = std::abs (hp);
            const float maxVal = 0.98f;
            hp = sign * (threshold + (maxVal - threshold) * std::tanh ((absVal - threshold) / (maxVal - threshold)));
        }

        // 4. Hard Limiter
        if (hp > 0.99f)  return 0.99f;
        if (hp < -0.99f) return -0.99f;

        return hp;
    }

    bool checkAndResetBlowUpFlag()
    {
        return hasBlownUp.exchange (false);
    }

private:
    float r { 0.995f };
    float x1 { 0.0f };
    float y1 { 0.0f };
    std::atomic<bool> hasBlownUp { false };
};

} // namespace VirtualTrumpet
