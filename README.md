# AI Deco — Geometry Dash (Geode) mod scaffold

This repository contains a scaffold for a Geometry Dash mod using Geode named "AI Deco". The mod demonstrates a UI toolbar button in the level editor that opens a panel for generating decorative objects using an "AI-style" prompt, a detail level selector, and deterministic seeds/presets.

The scaffold focuses on modular design (include/, src/, resources/, AI_Deco/Presets/) and contains placeholder hooks where Geode APIs should be implemented. The code is written to be compatible with a Geode dev environment on Windows (C++17, CMake-friendly).

Suggested steps to build in a Geode environment (Windows):

1. Make sure you have Geode dev headers and the Geode build toolchain configured for Visual Studio.
2. Generate a MSVC solution using CMake (configure CMake to point to Geode includes/libs if needed).
3. Build the produced project and drop the built DLL into the Geometry Dash mods folder for testing.

Layout of this scaffold

ai-deco/
├── include/
├── src/
├── resources/
└── AI_Deco/Presets/

See the code comments and placeholders in the source files for additional guidance and where to add Geode-specific API calls.

Quick local smoke-test (Linux)
1. Compile a quick test binary of the scaffold (no Geode integration) to validate syntax:

```bash
g++ -std=c++17 -Iinclude src/*.cpp -O0 -g -o aid_deco_test
./aid_deco_test
```

You should see placeholder initialization output. This confirms the scaffold compiles locally (note: the mod will still need to be built against Geode/Geometry Dash SDK for Windows to run in-game).

---

Detailed Geode porting notes

1) Prepare your Geode dev environment on Windows. See Geode docs for installing the SDK and configuring a Visual Studio toolchain.
2) Point the CMake file's include and link paths towards the Geode SDK (headers and libs). Replace the placeholder include paths in `CMakeLists.txt`.
3) Replace UI placeholders in `src/ui/*` with proper Cocos2d-x / Geode UI code. You can use the provided `src/hooks/GeodeStubs.hpp` for a minimal example of guarded Geode code.
4) The shader/asset pipeline and object spawning must use Geometry Dash / Geode APIs. Look at Geode's EditorUI or EditorLayer hooks to query selection and spawn objects.
5) When compiling for Geode, either enable `-DBUILD_GEODE=ON` and provide `GEODE_INCLUDE_DIR` (the default is `third_party/geode/loader/include` if present) or explicitly pass `-DGEODE` and set your include and libs variables.

Quick Windows helper

I added a convenience PowerShell helper to configure and build using a local SDK or the bundled `third_party/geode` folder if available:

```powershell
.\tools\build_geode_win.ps1 -GeodeInclude "C:\path\to\geode\loader\include" -GeodeLibs "C:\path\to\geode\lib\geode.lib"
```

This will configure the project with `-DBUILD_GEODE=ON` and attempt to build the Release target (Visual Studio generator by default).

Publishing a Windows release (DLL) via GitHub Actions
---------------------------------------------------

You can produce an official Windows release (an ai_deco.dll that will run as a Geode mod) directly from GitHub by creating a tag named `vX.Y.Z` on the `main` branch. When you push the tag, the `Build & Publish Windows Release` workflow will:

- Build the Release DLL on `windows-latest` using the bundled Geode headers and link-time libs.
- Package a ZIP containing `mod.json`, `ai_deco.dll`, and the `resources/` folder as `ai-deco-mod.zip`.
- Create a GitHub Release for the tag and attach both `ai_deco.dll` and `ai-deco-mod.zip` as release assets.

How to release a new version from your machine:

1) Locally tag the commit you want to release:

```bash
git tag v0.1.0
git push origin v0.1.0
```

2) Wait for the GitHub Actions run to complete. The release will appear on the repository's Releases page. Download `ai-deco-mod.zip` and drop its contents into Geometry Dash's Geode mods directory or instruct users to unzip the folder into their mods folder.

Important notes about including SDKs and source in releases
---------------------------------------------------------
- Do NOT include the Geode SDK or the full `geode-main` repository in your distributed mod package or release assets. The Geode SDK is separate software and should not be bundled with your mod — it is installed by the end user via Geode.
- The repo previously contained a `geode-main` upload for development and testing; do not include that folder in any release. The release workflows package only the mod files (DLL, `mod.json`, and `resources/`) — not the Geode SDK or large third-party code.

Local packaging helper
----------------------
You can quickly create a distributable ZIP containing your mod ready to drop into Geometry Dash using the included helper script (uses the locally-built DLL):

```bash
./tools/package_mod.sh [PATH_TO_DLL] [OUT_ZIP]

# Example (auto-find):
./tools/package_mod.sh
# or specify the built dll explicitly
./tools/package_mod.sh build/ai_deco.dll build/ai_deco_manual.zip
```

This script will create a ZIP containing only `mod.json`, `ai_deco.dll`, and `resources/` so you can publish the package without bundling the Geode SDK.

.geode packages (one-file installers)
----------------------------------
This repository packs the mod contents into a standard ZIP-based package with a `.geode` extension (a zip archive renamed to `.geode`). Geode-compatible installers and some distribution channels accept `.geode` files as single-file mod packages.

How to use a `.geode` file:
- Download the `.geode` asset from the Release (e.g., `ai-deco.geode`).
- If your Geode installs accept `.geode` files directly you can follow your usual process (some launchers allow double-click or drag & drop). Otherwise, treat it like a zip:
	- Rename `.geode` to `.zip` and extract OR open it with any archive manager.
	- Confirm the top-level folder is `ai-deco/` and that inside it are `mod.json` + `ai_deco.dll` + `resources/`.
	- Place the `ai-deco` folder into your Geode mods folder:
		- `%localappdata%\GeometryDash\mods\ai-deco\` on Windows.

Notes:
- `.geode` is only a convenient package format — the internal contents must still be `mod.json` + DLL + resources.
- Always use the CI-built Windows `.geode` or `.dll` artifact that matches your Geometry Dash/Geode bitness and version. If your game is 32-bit you must use a 32-bit mod binary; for 64-bit use a 64-bit binary.

Testing the produced DLL in Geometry Dash
---------------------------------------

1) Download the `ai-deco-mod.zip` from the GitHub Release or the `ai_deco.dll` artifact directly.
2) Unzip or copy the files so the directory structure is:

```
%localappdata%/GeometryDash/mods/ai-deco/
	- ai_deco.dll
	- mod.json
	- resources/...
```

3) Launch Geometry Dash with Geode. Open the Level Editor and verify the AI Deco toolbar button and panel appear and work.

If the workflow fails with link-time errors, it's usually because the Geode / game import libraries in `third_party/geode/loader/include/link/win64` don't match the exact expected layout for a given Geometry Dash version. In that case, provide your own Geode libs by modifying the workflow or running the PowerShell helper `tools/build_release_win.ps1` on a Windows machine and providing the correct `-GeodeLibs` path.

Example of a minimal Geode entry: (adapt to current Geode version)

```cpp
// Example: src/hooks/EditorHook_geode.cpp
#ifdef GEODE
#include <Geode/Geode.hpp>

using namespace geode;

extern "C" __declspec(dllexport) void load() {
	// Called when the DLL loads inside the game. Register hooks here.
	// Hook Editor initialization and add a toolbar button.
}
#endif
```

If you'd like, I can produce a focused porting patch that adds `#ifdef GEODE` stubs in the UI files and demonstrates how to add an Editor toolbar button and open the panel using Geode+ Cocos2d-x types. Would you like me to do that now?

Ready-to-build instructions for Windows + Geode

1) Install Geode SDK and the Geometry Dash dev headers; ensure you know the path to the Geode include directory and any required libraries.
2) Use CMake to configure with Geode support enabled and point to the Geode SDK paths. Example (PowerShell / command prompt):

```powershell
cmake -S . -B build -DBUILD_GEODE=ON -DGEODE_INCLUDE_DIR="C:/path/to/geode/includes" -DGEODE_LIBS="path\to\geode\libs\geode.lib" -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

3) This project contains geode-specific stubs (files named *_geode.cpp). They are compiled when you pass `-DBUILD_GEODE=ON` and provide GEODE headers/libraries. Replace pseudo-code in geode stubs with the latest Geode API calls based on your Geode version.

4) Deploy the produced DLL to your Geometry Dash mods folder (e.g., ai_deco.dll) or follow your usual Geode mod testing workflow.

If you want me to finish the last mile (produce a real ai_deco.dll that spawns real in-game objects):

- Option 1 (easiest for me to complete): upload or give me the Geode SDK folder or the game's header files (like `cocos-ext.h`, `cocos2d.h` and the game's binding headers). I will port the stubs to the exact APIs and build a Release DLL for you.

- Option 2 (you do it locally): run the scripts in `tools/` to configure/build on Windows:

PowerShell example (local Windows machine):

```powershell
# place your Geode SDK / game headers in third_party/geode/loader/include OR provide the path below
.\tools\prepare_game_headers.sh "C:\path\to\game\headers"
.\tools\build_release_win.ps1 -GeodeInclude "C:\path\to\geode\loader\include" -GeodeLibs "C:\path\to\geode\lib\geode.lib"
```

After a successful build the final `ai_deco.dll` will be in the `build/Release` folder. Copy it into your Geometry Dash mod folder and test in-game.

Don't worry — if you upload or point me to the correct Geode/game headers I can finish this last bit for you and produce the DLL. If you'd rather handle the private game headers locally, run the above commands and paste any build errors back here and I'll debug them for you.
