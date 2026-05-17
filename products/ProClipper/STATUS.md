# Pro Clipper Status

## Current goal

Create a controlled v0.1 JUCE clipping plugin for the PluginLab pipeline.

## Product scope v0.1

- VST3 + Standalone on Linux sanity builds
- AU also enabled when building on macOS
- Input gain
- Drive
- Ceiling
- Wet/dry mix
- Output gain
- Soft Clip toggle
- Bypass toggle
- APVTS state save/load
- Simple professional dark UI

## VPS/Linux validation

VPS builds are sanity checks only. They do not produce the final Mac FL Studio plugin.

```bash
cmake -S products/ProClipper -B products/ProClipper/build
cmake --build products/ProClipper/build --config Release
```

## Mac handoff

Mac should pull from GitHub and build/test the real FL Studio plugin as VST3:

```bash
cd ~/Documents/PluginLab
./scripts/build-mac-plugin.sh ProClipper
```

Then restart/rescan FL Studio and load **Pro Clipper** as VST3.

## Known notes

- Editor creates all sliders/buttons/labels before `setSize()` to avoid JUCE startup crashes from early `resized()` calls.
- Linux `.vst3` artifacts are not Mac deliverables.
- Do not commit build folders or plugin binaries.
