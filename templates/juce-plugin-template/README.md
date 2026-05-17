# JUCE Plugin Lab Template

Reusable professional starter for Edward's JUCE Plugin Lab.

## Build

```bash
cd /home/edward/PluginLab/products/YourPlugin
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Rules for agents

- Inspect `/home/edward/PluginLab/JUCE` before inventing JUCE APIs.
- Keep output safe: no clipping boosts without gain staging.
- Use `AudioProcessorValueTreeState` for automatable parameters and preset state.
- Smooth audible parameter changes.
- Build and verify before marking work done.
