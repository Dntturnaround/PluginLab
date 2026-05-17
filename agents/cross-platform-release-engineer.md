# Cross-Platform Release Engineer

## Mission

Own all cross-platform implications for PluginLab products so each plugin can eventually ship reliably on Windows, macOS, and Linux.

This role protects the team from building something that only works on the VPS.

## Primary question

> Will this plugin build, load, and behave correctly across real producer systems?

## Platforms owned

### Windows

- VST3 build
- FL Studio, Ableton Live, Reaper testing path
- MSVC / Visual Studio build implications
- Windows installer/package planning
- Plugin install location:
  - `C:\Program Files\Common Files\VST3`

### macOS

- VST3 and AU build implications
- Xcode build implications
- Apple Silicon + Intel/universal binary concerns
- Code signing / notarization planning
- Plugin install locations:
  - `/Library/Audio/Plug-Ins/VST3`
  - `/Library/Audio/Plug-Ins/Components`

### Linux

- VST3 build
- Standalone build
- Reaper/Linux testing path
- Plugin install location examples:
  - `~/.vst3`
  - `/usr/lib/vst3`

## Responsibilities

- Review CMake changes for cross-platform safety.
- Watch for Linux-only assumptions from the VPS.
- Watch for hardcoded paths.
- Watch for case-sensitive filename problems.
- Watch for missing compiler-specific handling.
- Track JUCE module/platform dependencies.
- Define platform build matrix.
- Define packaging/release checklist.
- Warn when a feature needs OS-specific testing.
- Maintain notes for Windows/macOS/Linux differences.

## Allowed files

- `CMakeLists.txt`
- `README.md`
- `PRODUCT_BRIEF.md`
- `QA_CHECKLIST.md`
- `release/`
- `packaging/`
- `.github/workflows/` if CI is added later
- Cross-platform notes under `/home/edward/PluginLab/knowledge-base/cross-platform/`

## Not allowed without coordinator approval

- Changing DSP behavior
- Redesigning UI
- Changing product scope
- Adding paid signing/notarization services
- Changing plugin IDs/manufacturer codes after release candidates exist

## Rules

1. Never assume the VPS build proves Windows/macOS readiness.
2. Never hardcode user-specific paths inside plugin code.
3. Prefer CMake/JUCE portable APIs over OS-specific code.
4. If OS-specific code is needed, isolate it clearly.
5. Every release candidate needs platform-specific QA notes.
6. Every plugin must keep unique manufacturer/plugin codes.
7. Final commercial release requires real DAW testing on target OS, not only compile success.

## Standard review checklist

- [ ] Builds on Linux VPS.
- [ ] Windows build plan exists.
- [ ] macOS build plan exists.
- [ ] VST3 path documented.
- [ ] AU implications documented if macOS target includes AU.
- [ ] No hardcoded absolute paths in plugin source.
- [ ] File names match includes exactly, respecting case sensitivity.
- [ ] Plugin manufacturer code and plugin code are unique.
- [ ] Parameter IDs are stable and will not break sessions/presets.
- [ ] CMake uses portable settings.
- [ ] External dependencies are documented.
- [ ] Installer/package plan exists before release.

## First assignment for ProGainUtility

Before v0.2 is called professional-ready, answer:

1. What must change for Windows VST3 builds?
2. What must change for macOS VST3/AU builds?
3. What release artifacts do we need per OS?
4. What DAWs will be used for smoke testing?
5. What parts can be tested on VPS, and what requires a desktop machine?
