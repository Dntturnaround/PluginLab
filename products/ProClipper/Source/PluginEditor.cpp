#include "PluginEditor.h"

ProClipperAudioProcessorEditor::ProClipperAudioProcessorEditor (ProClipperAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setupSlider (inputSlider, "Input", " dB");
    setupSlider (driveSlider, "Drive", " dB");
    setupSlider (ceilingSlider, "Ceiling", " dB");
    setupSlider (mixSlider, "Mix", " %");
    setupSlider (outputSlider, "Output", " dB");

    softClipButton.setClickingTogglesState (true);
    softClipButton.setColour (juce::TextButton::buttonColourId, juce::Colour (0xff18222b));
    softClipButton.setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xff1f6f91));
    addAndMakeVisible (softClipButton);

    bypassButton.setClickingTogglesState (true);
    bypassButton.setColour (juce::TextButton::buttonColourId, juce::Colour (0xff18222b));
    bypassButton.setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xff783030));
    addAndMakeVisible (bypassButton);

    auto& state = processor.getValueTreeState();
    inputAttachment = std::make_unique<SliderAttachment> (state, "inputGain", inputSlider);
    driveAttachment = std::make_unique<SliderAttachment> (state, "drive", driveSlider);
    ceilingAttachment = std::make_unique<SliderAttachment> (state, "ceiling", ceilingSlider);
    mixAttachment = std::make_unique<SliderAttachment> (state, "mix", mixSlider);
    outputAttachment = std::make_unique<SliderAttachment> (state, "outputGain", outputSlider);
    softClipAttachment = std::make_unique<ButtonAttachment> (state, "softClip", softClipButton);
    bypassAttachment = std::make_unique<ButtonAttachment> (state, "bypass", bypassButton);

    setSize (720, 320);
}

void ProClipperAudioProcessorEditor::setupSlider (juce::Slider& slider, const juce::String& labelText, const juce::String& suffix)
{
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 82, 24);
    slider.setTextValueSuffix (suffix);
    slider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xff39b5e8));
    slider.setColour (juce::Slider::thumbColourId, juce::Colour (0xff58d38c));
    slider.setColour (juce::Slider::textBoxTextColourId, juce::Colour (0xffe8ecf1));
    slider.setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0xff3b424c));
    addAndMakeVisible (slider);

    auto* label = labels.add (new juce::Label());
    label->setText (labelText, juce::dontSendNotification);
    label->setJustificationType (juce::Justification::centred);
    label->setColour (juce::Label::textColourId, juce::Colour (0xffe8ecf1));
    addAndMakeVisible (label);
}

void ProClipperAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff101216));

    g.setColour (juce::Colour (0xfff2f2f2));
    g.setFont (juce::FontOptions (26.0f, juce::Font::bold));
    g.drawFittedText ("Pro Clipper", 24, 18, getWidth() - 48, 36, juce::Justification::centredLeft, 1);

    g.setColour (juce::Colour (0xff7f8794));
    g.setFont (juce::FontOptions (14.0f));
    g.drawFittedText ("Clean clipping foundation: input, drive, ceiling, mix, output", 24, 52, getWidth() - 48, 24, juce::Justification::centredLeft, 1);
}

void ProClipperAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced (24);
    bounds.removeFromTop (86);

    const int sliderW = 112;
    const int gap = 16;
    juce::Array<juce::Slider*> sliders { &inputSlider, &driveSlider, &ceilingSlider, &mixSlider, &outputSlider };

    int x = bounds.getX();
    for (int i = 0; i < sliders.size(); ++i)
    {
        labels[i]->setBounds (x, bounds.getY(), sliderW, 22);
        sliders[i]->setBounds (x, bounds.getY() + 24, sliderW, 142);
        x += sliderW + gap;
    }

    softClipButton.setBounds (getWidth() - 246, 26, 96, 34);
    bypassButton.setBounds (getWidth() - 126, 26, 96, 34);
}
