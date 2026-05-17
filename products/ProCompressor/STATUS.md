# Product Status: ProCompressor

## Current goal

Use ProCompressor as the test product for the PluginLab VPS-to-GitHub-to-Mac workflow.

## What changed

- Product name: ProCompressor
- ProCompressor has source code in `products/ProCompressor/Source`.
- It currently includes Input, Threshold, Ratio, Attack, Release, Output, and Bypass controls.
- The VPS has produced a Linux VST3 build before.
- Linux builds are sanity checks only, not Mac deliverables.
- The Mac clone currently needs a Mac build before the plugin can be opened in a Mac DAW.
- GitHub is the handoff point between VPS agents and the Mac build/test loop.

## How to build on VPS

Use this only as a Linux sanity check:

```bash
cd /home/edward/PluginLab
cmake -S products/ProCompressor -B products/ProCompressor/build
cmake --build products/ProCompressor/build --config Release
```

## How to build on Mac

```bash
cd ~/Documents/PluginLab
./scripts/build-mac-plugin.sh ProCompressor
```

The Mac build is the one that creates plugin bundles intended for Mac DAWs.

## Known issues

- Mac build workflow still needs to be tested on the Mac.
- DAW scan/load behavior has not been verified yet.

## Next recommended step

Pull the latest GitHub changes on the Mac and run `./scripts/build-mac-plugin.sh ProCompressor` to test the Mac build/install workflow.
