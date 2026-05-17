# Pro Compressor v0.1 Product Brief

## Purpose

A clean starter compressor plugin for PluginLab. This is the first real dynamics plugin target.

## v0.1 scope

- Input gain
- Threshold
- Ratio
- Attack
- Release
- Output gain
- Bypass
- VST3 build
- Standalone build

## Deferred

- Gain reduction meter
- Sidechain
- Knee control
- Auto makeup
- Mix/parallel blend
- Character modes
- Oversampling/saturation
- Preset browser

## Build

```bash
cd /home/edward/PluginLab/products/ProCompressor
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
