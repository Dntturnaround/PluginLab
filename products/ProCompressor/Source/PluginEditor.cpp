#include "PluginEditor.h"

ProCompressorAudioProcessorEditor::ProCompressorAudioProcessorEditor (ProCompressorAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (760, 320);

    setupSlider (inputSlider, "Input", " dB");
    setupSlider (thresholdSlider, "Threshold", " dB");
    setupSlider (ratioSlider, "Ratio", ":1");
    setupSlider (attackSlider, "Attack", " ms");
    setupSlider (releaseSlider, "Release", " ms");
    setupSlider (outputSlider, "Output", " dB");

    bypassButton.setClickingTogglesState (true);
    addAndMakeVisible (bypassButton);

    auto& state = processor.getValueTreeState();
    inputAttachment = std::make_unique<SliderAttachment> (state, "inputGain", inputSlider);
    thresholdAttachment = std::make_unique<SliderAttachment> (state, "threshold", thresholdSlider);
    ratioAttachment = std::make_unique<SliderAttachment> (state, "ratio", ratioSlider);
    attackAttachment = std::make_unique<SliderAttachment> (state, "attack", attackSlider);
    releaseAttachment = std::make_unique<SliderAttachment> (state, "release", releaseSlider);
    outputAttachment = std::make_unique<SliderAttachment> (state, "outputGain", outputSlider);
    bypassAttachment = std::make_unique<ButtonAttachment> (state, "bypass", bypassButton);
}

void ProCompressorAudioProcessorEditor::setupSlider (juce::Slider& slider, const juce::String& labelText, const juce::String& suffix)
{
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 82, 24);
    slider.setTextValueSuffix (suffix);
    addAndMakeVisible (slider);

    auto* label = labels.add (new juce::Label());
    label->setText (labelText, juce::dontSendNotification);
    label->setJustificationType (juce::Justification::centred);
    label->setColour (juce::Label::textColourId, juce::Colour (0xffe8ecf1));
    addAndMakeVisible (label);
}

void ProCompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff101216));
    g.setColour (juce::Colour (0xfff2f2f2));
    g.setFont (juce::FontOptions (26.0f, juce::Font::bold));
    g.drawFittedText ("Pro Compressor", 24, 18, getWidth() - 48, 36, juce::Justification::centredLeft, 1);

    g.setColour (juce::Colour (0xff7f8794));
    g.setFont (juce::FontOptions (14.0f));
    g.drawFittedText ("Clean compression foundation: input, threshold, ratio, timing, output", 24, 52, getWidth() - 48, 24, juce::Justification::centredLeft, 1);
}

void ProCompressorAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced (24);
    bounds.removeFromTop (86);

    const int sliderW = 112;
    const int gap = 10;
    juce::Array<juce::Slider*> sliders { &inputSlider, &thresholdSlider, &ratioSlider, &attackSlider, &releaseSlider, &outputSlider };

    int x = bounds.getX();
    for (int i = 0; i < sliders.size(); ++i)
    {
        labels[i]->setBounds (x, bounds.getY(), sliderW, 22);
        sliders[i]->setBounds (x, bounds.getY() + 24, sliderW, 142);
        x += sliderW + gap;
    }

    bypassButton.setBounds (getWidth() - 126, 26, 96, 34);
}
