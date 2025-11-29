#!/usr/bin/env bash
# tools/prepare_game_headers.sh
# Helper to copy game headers into third_party/geode/loader/include so GEODE builds can use them.

set -euo pipefail

if [ $# -lt 1 ]; then
  echo "Usage: $0 <path-to-game-headers-folder>"
  echo "Example: $0 \"C:/Games/GeometryDash/headers\" (on Windows, run via WSL or adapt the path)"
  exit 1
fi

SRC="$1"
DEST="$(pwd)/third_party/geode/loader/include"

echo "Source: $SRC"
echo "Dest: $DEST"

if [ ! -d "$SRC" ]; then
  echo "Source path doesn't exist. Please ensure the path contains headers such as cocos-ext.h, cocos2d.h, and game headers." >&2
  exit 2
fi

mkdir -p "$DEST"

echo "Copying headers..."
rsync -av --progress "$SRC/" "$DEST/"

echo "Done. Running quick checks for common headers..."
for h in cocos-ext.h cocos2d.h; do
  if [ -f "$DEST/$h" ]; then
    echo "Found $h in $DEST"
  else
    echo "Warning: $h not found in $DEST (you may still be missing game headers)"
  fi
done

echo "If you placed headers in a different folder, re-run this script pointing to that folder."
