#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
constexpr const char* outputGainId = "outputGain";
}

PluginLabAudioProcessor::PluginLabAudioProcessor()
    : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout PluginLabAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { outputGainId, 1 },
        "Output Gain",
        juce::NormalisableRange<float> (-24.0f, 12.0f, 0.01f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel ("dB")));

    return { params.begin(), params.end() };
}

void PluginLabAudioProcessor::prepareToPlay (double sampleRate, int)
{
    outputGain.reset (sampleRate, 0.02); // 20 ms smoothing prevents zipper noise.
    outputGain.setCurrentAndTargetValue (juce::Decibels::decibelsToGain (apvts.getRawParameterValue (outputGainId)->load()));
}

void PluginLabAudioProcessor::releaseResources() {}

bool PluginLabAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto& mainOut = layouts.getMainOutputChannelSet();
    const auto& mainIn  = layouts.getMainInputChannelSet();

    if (mainOut != juce::AudioChannelSet::mono() && mainOut != juce::AudioChannelSet::stereo())
        return false;

    return mainIn == mainOut;
}

void PluginLabAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ignoreUnused (midi);
    juce::ScopedNoDenormals noDenormals;

    const auto targetGain = juce::Decibels::decibelsToGain (apvts.getRawParameterValue (outputGainId)->load());
    outputGain.setTargetValue (targetGain);

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* samples = buffer.getWritePointer (channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            samples[sample] *= outputGain.getNextValue();
    }
}

void PluginLabAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xml = apvts.copyState().createXml())
        copyXmlToBinary (*xml, destData);
}

void PluginLabAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        if (xml->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessorEditor* PluginLabAudioProcessor::createEditor()
{
    return new PluginLabAudioProcessorEditor (*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginLabAudioProcessor();
}
