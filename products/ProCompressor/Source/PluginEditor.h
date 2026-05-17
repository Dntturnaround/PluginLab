#pragma once

#include "PluginProcessor.h"

class ProCompressorAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit ProCompressorAudioProcessorEditor (ProCompressorAudioProcessor&);
    ~ProCompressorAudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    ProCompressorAudioProcessor& processor;

    juce::Slider inputSlider, thresholdSlider, ratioSlider, attackSlider, releaseSlider, outputSlider;
    juce::TextButton bypassButton { "Bypass" };
    juce::OwnedArray<juce::Label> labels;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    std::unique_ptr<SliderAttachment> inputAttachment, thresholdAttachment, ratioAttachment, attackAttachment, releaseAttachment, outputAttachment;
    std::unique_ptr<ButtonAttachment> bypassAttachment;

    void setupSlider (juce::Slider& slider, const juce::String& labelText, const juce::String& suffix);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProCompressorAudioProcessorEditor)
};
