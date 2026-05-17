#!/usr/bin/env bash
set -euo pipefail

if [[ $# -lt 1 ]]; then
    echo "Usage: $0 <ProductName>"
    echo "Example: $0 ProCompressor"
    exit 1
fi

product="$1"
repo_root="$(git rev-parse --show-toplevel 2>/dev/null || true)"

if [[ -z "$repo_root" ]]; then
    echo "Error: run this from inside the PluginLab git repo."
    exit 1
fi

cd "$repo_root"

if [[ "$(uname -s)" != "Darwin" ]]; then
    echo "Error: this script installs Mac plugins and must be run on macOS."
    exit 1
fi

product_dir="products/$product"
if [[ ! -f "$product_dir/CMakeLists.txt" ]]; then
    echo "Error: product not found: $product_dir"
    exit 1
fi

echo "Updating source and submodules..."
git pull --recurse-submodules
git submodule update --init --recursive

build_dir="$product_dir/build/mac"
configure_args=(-S "$product_dir" -B "$build_dir")

if [[ "$(uname -m)" == "arm64" ]]; then
    configure_args+=("-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64")
fi

if command -v ninja >/dev/null 2>&1; then
    generator="Ninja"
elif command -v xcodebuild >/dev/null 2>&1 && xcodebuild -version 2>/dev/null | grep -q '^Xcode '; then
    generator="Xcode"
else
    generator="Unix Makefiles"
fi

echo
echo "Configuring $product with $generator..."
cmake -G "$generator" "${configure_args[@]}"

echo
echo "Building Release..."
cmake --build "$build_dir" --config Release

vst3_dest="$HOME/Library/Audio/Plug-Ins/VST3"
component_dest="$HOME/Library/Audio/Plug-Ins/Components"
mkdir -p "$vst3_dest" "$component_dest"

copy_bundle() {
    local src="$1"
    local dest_dir="$2"
    local dest="$dest_dir/$(basename "$src")"

    echo "Installing $(basename "$src") -> $dest_dir"
    if command -v ditto >/dev/null 2>&1; then
        ditto "$src" "$dest"
    else
        cp -R "$src" "$dest_dir/"
    fi

    xattr -cr "$dest" 2>/dev/null || true
    codesign --force --deep --sign - "$dest" >/dev/null
}

vst3_count=0
while IFS= read -r -d '' bundle; do
    copy_bundle "$bundle" "$vst3_dest"
    vst3_count=$((vst3_count + 1))
done < <(find "$build_dir" -type d -name "*.vst3" -print0)

if [[ $vst3_count -eq 0 ]]; then
    echo "Error: no .vst3 bundle found under $build_dir"
    exit 1
fi

component_count=0
if [[ "${INSTALL_AU:-0}" == "1" ]]; then
    while IFS= read -r -d '' bundle; do
        copy_bundle "$bundle" "$component_dest"
        component_count=$((component_count + 1))
    done < <(find "$build_dir" -type d -name "*.component" -print0)

    if [[ $component_count -eq 0 ]]; then
        echo "Warning: no .component bundle found under $build_dir"
    fi
else
    echo "Skipping AU install by default. Set INSTALL_AU=1 to install .component bundles."
fi

echo
echo "Done. Restart or rescan your DAW plugins if the plugin does not appear immediately."
