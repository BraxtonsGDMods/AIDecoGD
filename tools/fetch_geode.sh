#!/usr/bin/env bash
# tools/fetch_geode.sh
# Try common Geode GitHub locations and download a release into third_party/geode
# This script is best-effort; if no release is found you'll need to provide a path to your SDK.

set -euo pipefail
cd "$(dirname "$0")/.." || exit 1
mkdir -p third_party
cd third_party

CANDIDATES=(
  "https://github.com/GeodeMod/Geode"
  "https://github.com/geode-mod/Geode"
  "https://github.com/GeodeSDK/Geode"
  "https://github.com/geode-sdk/geode"
)

function try_clone() {
  local url=$1
  local dir="geode"
  echo "Trying: $url"
  # Try shallow clone
  if git clone --depth 1 "$url" "$dir"; then
    echo "Cloned $url -> third_party/geode"
    return 0
  fi
  return 1
}

for u in "${CANDIDATES[@]}"; do
  if [ -d geode ]; then
    echo "third_party/geode already present. Skipping additional fetches."
    break
  fi
  try_clone "$u" || true
done

if [ -d geode ]; then
  echo "Geode SDK present in third_party/geode"
  exit 0
fi

# Try to fetch from GitHub releases using curl (best-effort)
for owner in "GeodeMod" "geode-mod" "GeodeSDK" "geode-sdk"; do
  url="https://api.github.com/repos/${owner}/Geode/releases/latest"
  echo "Checking releases: $url"
  if curl -sSf "$url" -o /tmp/geode_release.json; then
    tarball=$(jq -r '.tarball_url // empty' /tmp/geode_release.json || true)
    if [ -n "$tarball" ]; then
      echo "Found release tarball: $tarball"
      curl -sL "$tarball" | tar -xz --strip-components=1 -C geode || true
      if [ -d geode ]; then
        echo "Downloaded release into third_party/geode"
        exit 0
      fi
    fi
  fi
done

cat <<EOF
Couldn't fetch Geode automatically from known locations.
If you have a Geode SDK or release tarball, download it and extract to third_party/geode.
EOF
exit 1
