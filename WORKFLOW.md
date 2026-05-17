# PluginLab Workflow

PluginLab uses GitHub as the handoff point between VPS agents and the Mac that builds usable Mac audio plugins.

## Core idea

- VPS agents edit source code and run Linux builds only as sanity checks.
- GitHub stores source code, docs, templates, and scripts.
- The Mac pulls source from GitHub, builds the real Mac plugin, installs it to the local Audio Plug-Ins folders, and tests it in a DAW.
- Linux plugin builds are not final Mac deliverables.

A plugin binary must be built for the platform that will run it. A Linux `.vst3` proves code sanity only; it will not load as a Mac plugin. Mac builds are required for Mac plugin testing. On macOS, `.vst3` and `.component` files are bundle directories, so scripts must copy them as directories rather than plain files.

## VPS agent workflow

From `/home/edward/PluginLab`, agents should:

1. Edit source code, docs, templates, or scripts.
2. Run Linux builds only to catch obvious compile or logic problems.
3. Commit and push source changes to GitHub.
4. Never commit build folders or plugin binaries.

Use:

```bash
./scripts/push-work.sh "Describe the agent update"
```

Do not commit:

- `build/`
- `.vst3`
- `.component`
- `.app`
- `.aaxplugin`
- `.clap`
- `.lv2`
- `.so`
- `.dylib`
- `.dll`
- `.a`

## GitHub workflow

GitHub stores the project source and the workflow around it. It does not need build outputs. The clean handoff is source code plus scripts, not copied plugin binaries.

## Mac workflow

On the Mac, pull latest source and build the product that should be tested in a DAW:

```bash
cd ~/Documents/PluginLab
./scripts/build-mac-plugin.sh ProCompressor
```

The script pulls latest code, updates submodules, configures CMake, builds Release, installs found bundles into the local Audio Plug-Ins folders, and leaves DAW testing to the user:

- `~/Library/Audio/Plug-Ins/VST3/`
- `~/Library/Audio/Plug-Ins/Components/`

After building, open the DAW and rescan plugins if the plugin does not appear immediately.

## JUCE path

Product CMake files default to the repo-local JUCE submodule:

```bash
products/<ProductName>/../../JUCE
```

If a machine uses a different JUCE checkout, supply it explicitly:

```bash
cmake -S products/<ProductName> -B products/<ProductName>/build -DJUCE_PATH=/path/to/JUCE
```

## Product status files

Each product should have a `STATUS.md` file with the current goal, build notes, known issues, and next step. This lets agents, Codex, and the Mac build/test loop pick up the work without guessing.
