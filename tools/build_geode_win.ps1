# tools/build_geode_win.ps1
# Helper to configure & build the project for Windows + Geode in PowerShell.

param(
    [string] $GeodeInclude = "third_party/geode/loader/include",
    [string] $GeodeLibs = "",
    [string] $BuildDir = "build",
    [string] $Generator = "Visual Studio 17 2022",
    [string] $Arch = "x64"
)

Write-Host "Configuring build with Geode headers: $GeodeInclude"
cmake -S . -B $BuildDir -DBUILD_GEODE=ON -DGEODE_INCLUDE_DIR="$GeodeInclude" -DGEODE_LIBS="$GeodeLibs" -G "$Generator" -A $Arch
if ($LASTEXITCODE -ne 0) { throw "CMake configure failed" }

Write-Host "Building (Release)"
cmake --build $BuildDir --config Release
if ($LASTEXITCODE -ne 0) { throw "Build failed" }

Write-Host "Build completed. Drop produced DLL from $BuildDir/Release into Geometry Dash mods folder."