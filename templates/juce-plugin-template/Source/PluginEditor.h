#pragma once

#include "PluginProcessor.h"

class PluginLabAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit PluginLabAudioProcessorEditor (PluginLabAudioProcessor&);
    ~PluginLabAudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    PluginLabAudioProcessor& processor;

    juce::Slider outputGainSlider;
    juce::Label outputGainLabel;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> outputGainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginLabAudioProcessorEditor)
};
