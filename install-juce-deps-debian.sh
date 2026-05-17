#!/usr/bin/env bash
set -euo pipefail

sudo apt update
sudo apt install -y \
  ninja-build \
  pkg-config \
  libasound2-dev \
  libjack-jackd2-dev \
  ladspa-sdk \
  libcurl4-openssl-dev \
  libfreetype6-dev \
  libfontconfig1-dev \
  libx11-dev \
  libxext-dev \
  libxinerama-dev \
  libxrandr-dev \
  libxcursor-dev \
  libxcomposite-dev \
  libxrender-dev \
  libwebkit2gtk-4.1-dev \
  libgtk-3-dev

printf '\nJUCE build dependencies installed.\n'
