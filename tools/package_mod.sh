#!/usr/bin/env bash
set -euo pipefail

# packages the mod folder for manual release
# Usage: tools/package_mod.sh [dll-path] [out-zip]
# If dll-path is omitted it will look in build/Release/ai_deco.dll then build/ai_deco.dll

DLL_PATH=${1:-}
OUT=${2:-build/ai-deco-manual.zip}

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

echo "Using DLL: $DLL_PATH"

TMPDIR=$(mktemp -d)
OUT_ABS=$(realpath -m "$OUT")
mkdir -p "$TMPDIR/ai-deco"
cp mod.json "$TMPDIR/ai-deco/"
cp "$DLL_PATH" "$TMPDIR/ai-deco/ai_deco.dll"
if [ -d resources ]; then
  cp -r resources "$TMPDIR/ai-deco/resources"
fi

pushd "$TMPDIR" >/dev/null
zip -r "$OUT_ABS" ai-deco
popd >/dev/null

echo "Created $OUT"

rm -rf "$TMPDIR"

# If user asked for a .geode file (common installer format for Geode) and
# provided a .zip, or if the OUT filename ends with .geode, create a .geode
# copy (zip archive with .geode extension). This keeps the script backwards
# compatible while supporting one-file installers.
if [[ "$OUT" == *.geode ]]; then
  # already a .geode filename -> nothing to do
  exit 0
fi

if [[ "$OUT" == *.zip ]]; then
  GEODE_OUT=$(dirname "$OUT")/ai-deco.geode
  cp "$OUT" "$GEODE_OUT"
  echo "Also created $GEODE_OUT" 
fi
