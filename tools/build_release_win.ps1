param(
    [string] $GeodeInclude = "third_party/geode/loader/include",
    [string] $GeodeLibs = "",
    [string] $BuildDir = "build",
    [string] $Generator = "Visual Studio 17 2022",
    [string] $Arch = "x64"
)

Write-Host "Preparing a Release build for Windows with Geode headers at: $GeodeInclude"

cmake -S . -B $BuildDir -DBUILD_GEODE=ON -DGEODE_INCLUDE_DIR="$GeodeInclude" -DGEODE_LIBS="$GeodeLibs" -G "$Generator" -A $Arch -DCMAKE_BUILD_TYPE=Release
if ($LASTEXITCODE -ne 0) { throw "CMake configure failed" }

cmake --build $BuildDir --config Release --parallel 2
if ($LASTEXITCODE -ne 0) { throw "Build failed" }

Write-Host "If the build succeeded, the produced DLL (ai_deco.dll) will be in the output directory (e.g., $BuildDir/Release or similar). Copy it into Geometry Dash's mods folder to test in the game." 
