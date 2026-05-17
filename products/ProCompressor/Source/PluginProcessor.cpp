#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
constexpr const char* inputGainId  = "inputGain";
constexpr const char* thresholdId  = "threshold";
constexpr const char* ratioId      = "ratio";
constexpr const char* attackId     = "attack";
constexpr const char* releaseId    = "release";
constexpr const char* outputGainId = "outputGain";
constexpr const char* bypassId     = "bypass";
}

ProCompressorAudioProcessor::ProCompressorAudioProcessor()
    : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout ProCompressorAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { inputGainId, 1 }, "Input", juce::NormalisableRange<float> (-24.0f, 24.0f, 0.01f), 0.0f, juce::AudioParameterFloatAttributes().withLabel ("dB")));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { thresholdId, 1 }, "Threshold", juce::NormalisableRange<float> (-60.0f, 0.0f, 0.01f), -18.0f, juce::AudioParameterFloatAttributes().withLabel ("dB")));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { ratioId, 1 }, "Ratio", juce::NormalisableRange<float> (1.0f, 20.0f, 0.01f), 4.0f, juce::AudioParameterFloatAttributes().withLabel (":1")));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { attackId, 1 }, "Attack", juce::NormalisableRange<float> (0.1f, 100.0f, 0.01f), 10.0f, juce::AudioParameterFloatAttributes().withLabel ("ms")));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { releaseId, 1 }, "Release", juce::NormalisableRange<float> (5.0f, 1000.0f, 0.01f), 100.0f, juce::AudioParameterFloatAttributes().withLabel ("ms")));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { outputGainId, 1 }, "Output", juce::NormalisableRange<float> (-24.0f, 24.0f, 0.01f), 0.0f, juce::AudioParameterFloatAttributes().withLabel ("dB")));
    params.push_back (std::make_unique<juce::AudioParameterBool>(juce::ParameterID { bypassId, 1 }, "Bypass", false));

    return { params.begin(), params.end() };
}

void ProCompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (samplesPerBlock), static_cast<juce::uint32> (getTotalNumOutputChannels()) };
    compressor.prepare (spec);
    compressor.reset();

    inputGain.reset (sampleRate, 0.02);
    outputGain.reset (sampleRate, 0.02);
    inputGain.setCurrentAndTargetValue (juce::Decibels::decibelsToGain (apvts.getRawParameterValue (inputGainId)->load()));
    outputGain.setCurrentAndTargetValue (juce::Decibels::decibelsToGain (apvts.getRawParameterValue (outputGainId)->load()));
}

void ProCompressorAudioProcessor::releaseResources() {}

bool ProCompressorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto& mainOut = layouts.getMainOutputChannelSet();
    const auto& mainIn  = layouts.getMainInputChannelSet();
    if (mainOut != juce::AudioChannelSet::mono() && mainOut != juce::AudioChannelSet::stereo())
        return false;
    return mainIn == mainOut;
}

void ProCompressorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ignoreUnused (midi);
    juce::ScopedNoDenormals noDenormals;

    if (apvts.getRawParameterValue (bypassId)->load() > 0.5f)
        return;

    inputGain.setTargetValue (juce::Decibels::decibelsToGain (apvts.getRawParameterValue (inputGainId)->load()));
    outputGain.setTargetValue (juce::Decibels::decibelsToGain (apvts.getRawParameterValue (outputGainId)->load()));

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* samples = buffer.getWritePointer (channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            samples[sample] *= inputGain.getNextValue();
    }

    compressor.setThreshold (apvts.getRawParameterValue (thresholdId)->load());
    compressor.setRatio (apvts.getRawParameterValue (ratioId)->load());
    compressor.setAttack (apvts.getRawParameterValue (attackId)->load());
    compressor.setRelease (apvts.getRawParameterValue (releaseId)->load());

    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    compressor.process (context);

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* samples = buffer.getWritePointer (channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            samples[sample] *= outputGain.getNextValue();
    }
}

void ProCompressorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xml = apvts.copyState().createXml())
        copyXmlToBinary (*xml, destData);
}

void ProCompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        if (xml->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessorEditor* ProCompressorAudioProcessor::createEditor()
{
    return new ProCompressorAudioProcessorEditor (*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ProCompressorAudioProcessor();
}
