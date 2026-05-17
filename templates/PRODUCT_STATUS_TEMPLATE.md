# Product Status: <ProductName>

## Current goal

Describe the current product goal in one or two sentences.

## What changed

- List the latest meaningful source, UI, DSP, build, or docs changes.

## How to build on VPS

Use Linux builds only as sanity checks. Do not treat Linux plugin bundles as Mac deliverables.

```bash
cd /home/edward/PluginLab
cmake -S products/<ProductName> -B products/<ProductName>/build
cmake --build products/<ProductName>/build --config Release
```

## How to build on Mac

```bash
cd ~/Documents/PluginLab
./scripts/build-mac-plugin.sh <ProductName>
```

## Known issues

- List known build, DSP, UI, or DAW testing issues.

## Next recommended step

Describe the next concrete action.
