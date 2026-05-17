# PluginLab

Edward's professional JUCE plugin production lab.

## Important paths

- JUCE framework: `/home/edward/PluginLab/JUCE`
- Reusable template: `/home/edward/PluginLab/templates/juce-plugin-template`
- First product: `/home/edward/PluginLab/products/ProGainUtility`
- Built VST3: `/home/edward/PluginLab/products/ProGainUtility/build/ProGainUtility_artefacts/Release/VST3/Pro Gain Utility.vst3`
- Standalone app: `/home/edward/PluginLab/products/ProGainUtility/build/ProGainUtility_artefacts/Release/Standalone/Pro Gain Utility`

## Build ProGainUtility

```bash
cd /home/edward/PluginLab/products/ProGainUtility
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Agent rules

- Use `/home/edward/PluginLab/JUCE` as the local JUCE source of truth.
- Do not invent JUCE APIs; inspect JUCE examples/source first.
- Build after edits before claiming success.
- Bishop QA must pass before release.
- Keep plugins professional: safe output, parameter smoothing, automatable parameters, save/load state, stable builds.

## Current status

- JUCE 8.0.12 installed.
- Debian build dependencies installed.
- Professional starter template created.
- ProGainUtility v0.1 scaffold builds successfully as VST3 and Standalone.
