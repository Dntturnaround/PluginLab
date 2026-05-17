# Cross-Platform Notes

PluginLab builds on the Debian VPS first, but commercial plugins must account for Windows and macOS from the beginning.

## VPS reality

The VPS can prove:

- JUCE source compiles.
- CMake project is valid.
- Linux VST3 artifacts can be produced.
- Basic static/build checks pass.
- Agents can inspect and modify code.

The VPS cannot fully prove:

- FL Studio compatibility.
- Ableton Live compatibility.
- Windows VST3 loading.
- macOS AU/VST3 loading.
- Code signing/notarization readiness.
- Real audio-device behavior on producer machines.

## Target platform matrix

### Phase 1

- Linux VPS build for development proof.
- Windows VST3 as first real producer target.

### Phase 2

- macOS VST3.
- macOS AU.

### Phase 3

- Installer/package pipeline.
- Code signing/notarization.
- Automated build CI.

## Early rules

- Keep code portable.
- Use JUCE APIs over direct OS APIs.
- Avoid hardcoded absolute paths.
- Keep parameter IDs stable.
- Keep plugin codes unique.
- Document every dependency.
