# Pro Gain Utility v0.1 Product Brief

## Purpose

A clean professional utility plugin that proves PluginLab can build stable JUCE products.

## Current v0.1 feature

- Output gain parameter: -24 dB to +12 dB
- Smooth gain changes to prevent zipper noise
- Stereo/mono bus support
- APVTS state save/load
- Simple professional dark UI
- VST3 build
- Standalone build

## Next professional features

- Input gain
- Bypass
- Polarity flip
- Mono switch
- Left/right output meters
- Peak hold / clip indicator
- Better branded UI
- Preset menu
- DAW validation checklist

## Build command

```bash
cd /home/edward/PluginLab/products/ProGainUtility
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Release artifact

```text
/home/edward/PluginLab/products/ProGainUtility/build/ProGainUtility_artefacts/Release/VST3/Pro Gain Utility.vst3
```
