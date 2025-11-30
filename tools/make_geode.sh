#!/usr/bin/env bash
set -euo pipefail

# Create a packaged .geode file using the DLL produced in build/
# Usage: tools/make_geode.sh [dll-path]

DLL_PATH=${1:-}
OUT=${2:-build/ai-deco.geode}

if [ -z "$DLL_PATH" ]; then
  if [ -f build/Release/ai_deco.dll ]; then
    DLL_PATH=build/Release/ai_deco.dll
  elif [ -f build/ai_deco.dll ]; then
    DLL_PATH=build/ai_deco.dll
  else
    echo "ERROR: Could not find ai_deco.dll; provide path as first argument" >&2
    exit 1
  fi
fi

echo "Packaging $DLL_PATH -> $OUT"

# Ensure output directory exists
mkdir -p "$(dirname "$OUT")"

tools/package_mod.sh "$DLL_PATH" "$OUT"

echo "Done: $OUT"
