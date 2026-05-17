#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
constexpr const char* inputGainId  = "inputGain";
constexpr const char* driveId      = "drive";
constexpr const char* ceilingId    = "ceiling";
constexpr const char* mixId        = "mix";
constexpr const char* outputGainId = "outputGain";
constexpr const char* softClipId   = "softClip";
constexpr const char* bypassId     = "bypass";

float hardClip (float sample, float ceiling) noexcept
{
    return juce::jlimit (-ceiling, ceiling, sample);
}

float softClip (float sample, float ceiling) noexcept
{
    if (ceiling <= 0.0f)
        return 0.0f;

    const auto normalised = sample / ceiling;
    return ceiling * std::tanh (normalised);
}
}

ProClipperAudioProcessor::ProClipperAudioProcessor()
    : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout ProClipperAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { inputGainId, 1 }, "Input", juce::NormalisableRange<float> (-24.0f, 24.0f, 0.01f), 0.0f, juce::AudioParameterFloatAttributes().withLabel ("dB")));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { driveId, 1 }, "Drive", juce::NormalisableRange<float> (0.0f, 24.0f, 0.01f), 6.0f, juce::AudioParameterFloatAttributes().withLabel ("dB")));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { ceilingId, 1 }, "Ceiling", juce::NormalisableRange<float> (-24.0f, 0.0f, 0.01f), -1.0f, juce::AudioParameterFloatAttributes().withLabel ("dB")));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { mixId, 1 }, "Mix", juce::NormalisableRange<float> (0.0f, 100.0f, 0.01f), 100.0f, juce::AudioParameterFloatAttributes().withLabel ("%")));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { outputGainId, 1 }, "Output", juce::NormalisableRange<float> (-24.0f, 24.0f, 0.01f), 0.0f, juce::AudioParameterFloatAttributes().withLabel ("dB")));
    params.push_back (std::make_unique<juce::AudioParameterBool>(juce::ParameterID { softClipId, 1 }, "Soft Clip", true));
    params.push_back (std::make_unique<juce::AudioParameterBool>(juce::ParameterID { bypassId, 1 }, "Bypass", false));

    return { params.begin(), params.end() };
}

void ProClipperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (samplesPerBlock);

    inputGain.reset (sampleRate, 0.02);
    driveGain.reset (sampleRate, 0.02);
    ceilingGain.reset (sampleRate, 0.02);
    mixAmount.reset (sampleRate, 0.02);
    outputGain.reset (sampleRate, 0.02);

    inputGain.setCurrentAndTargetValue (juce::Decibels::decibelsToGain (apvts.getRawParameterValue (inputGainId)->load()));
    driveGain.setCurrentAndTargetValue (juce::Decibels::decibelsToGain (apvts.getRawParameterValue (driveId)->load()));
    ceilingGain.setCurrentAndTargetValue (juce::Decibels::decibelsToGain (apvts.getRawParameterValue (ceilingId)->load()));
    mixAmount.setCurrentAndTargetValue (apvts.getRawParameterValue (mixId)->load() / 100.0f);
    outputGain.setCurrentAndTargetValue (juce::Decibels::decibelsToGain (apvts.getRawParameterValue (outputGainId)->load()));
}

void ProClipperAudioProcessor::releaseResources() {}

bool ProClipperAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto& mainOut = layouts.getMainOutputChannelSet();
    const auto& mainIn  = layouts.getMainInputChannelSet();
    if (mainOut != juce::AudioChannelSet::mono() && mainOut != juce::AudioChannelSet::stereo())
        return false;
    return mainIn == mainOut;
}

void ProClipperAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ignoreUnused (midi);
    juce::ScopedNoDenormals noDenormals;

    if (apvts.getRawParameterValue (bypassId)->load() > 0.5f)
        return;

    inputGain.setTargetValue (juce::Decibels::decibelsToGain (apvts.getRawParameterValue (inputGainId)->load()));
    driveGain.setTargetValue (juce::Decibels::decibelsToGain (apvts.getRawParameterValue (driveId)->load()));
    ceilingGain.setTargetValue (juce::Decibels::decibelsToGain (apvts.getRawParameterValue (ceilingId)->load()));
    mixAmount.setTargetValue (apvts.getRawParameterValue (mixId)->load() / 100.0f);
    outputGain.setTargetValue (juce::Decibels::decibelsToGain (apvts.getRawParameterValue (outputGainId)->load()));

    const bool useSoftClip = apvts.getRawParameterValue (softClipId)->load() > 0.5f;

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* samples = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            const auto dry = samples[sample] * inputGain.getNextValue();
            const auto driven = dry * driveGain.getNextValue();
            const auto ceiling = juce::jmax (ceilingGain.getNextValue(), 0.0001f);
            const auto clipped = useSoftClip ? softClip (driven, ceiling) : hardClip (driven, ceiling);
            const auto wet = dry + ((clipped - dry) * mixAmount.getNextValue());

            samples[sample] = wet * outputGain.getNextValue();
        }
    }
}

void ProClipperAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xml = apvts.copyState().createXml())
        copyXmlToBinary (*xml, destData);
}

void ProClipperAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        if (xml->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessorEditor* ProClipperAudioProcessor::createEditor()
{
    return new ProClipperAudioProcessorEditor (*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ProClipperAudioProcessor();
}
