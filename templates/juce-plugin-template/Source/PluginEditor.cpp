#include "PluginEditor.h"

namespace
{
constexpr const char* outputGainId = "outputGain";
}

PluginLabAudioProcessorEditor::PluginLabAudioProcessorEditor (PluginLabAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (420, 260);

    outputGainSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    outputGainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 90, 24);
    outputGainSlider.setTextValueSuffix (" dB");
    addAndMakeVisible (outputGainSlider);

    outputGainLabel.setText ("Output", juce::dontSendNotification);
    outputGainLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (outputGainLabel);

    outputGainAttachment = std::make_unique<SliderAttachment>(
        processor.getValueTreeState(), outputGainId, outputGainSlider);
}

void PluginLabAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff111318));

    g.setColour (juce::Colour (0xfff2f2f2));
    g.setFont (juce::FontOptions (24.0f, juce::Font::bold));
    g.drawFittedText (JucePlugin_Name, getLocalBounds().removeFromTop (60), juce::Justification::centred, 1);

    g.setColour (juce::Colour (0xff8f98a8));
    g.setFont (juce::FontOptions (14.0f));
    g.drawFittedText ("Professional JUCE Plugin Lab Template", 0, 52, getWidth(), 24, juce::Justification::centred, 1);
}

void PluginLabAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced (24);
    bounds.removeFromTop (76);

    outputGainLabel.setBounds (bounds.removeFromTop (24));
    outputGainSlider.setBounds (bounds.withSizeKeepingCentre (150, 150));
}
