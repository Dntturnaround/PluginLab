#pragma once

#include "PluginProcessor.h"

class ProClipperAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit ProClipperAudioProcessorEditor (ProClipperAudioProcessor&);
    ~ProClipperAudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    ProClipperAudioProcessor& processor;

    juce::Slider inputSlider, driveSlider, ceilingSlider, mixSlider, outputSlider;
    juce::TextButton softClipButton { "Soft Clip" };
    juce::TextButton bypassButton { "Bypass" };
    juce::OwnedArray<juce::Label> labels;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    std::unique_ptr<SliderAttachment> inputAttachment, driveAttachment, ceilingAttachment, mixAttachment, outputAttachment;
    std::unique_ptr<ButtonAttachment> softClipAttachment, bypassAttachment;

    void setupSlider (juce::Slider& slider, const juce::String& labelText, const juce::String& suffix);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProClipperAudioProcessorEditor)
};
