// Minimal compatibility shim for Geode Enums
// This file is added to make GEODE builds work in this workspace. In a
// real Geode SDK this header contains many enums and definitions used by
// Geode's headers. Replace it with the official header from the Geode SDK
// when building for production.

#pragma once

namespace geode {

// Editor base sizes (used by some UI helpers)
enum class EditorBaseSize : int {
    Normal = 0,
    Small = 1,
    SmallAlt = 2,
    Large = 3
};

// Generic editor base colors
enum class EditorBaseColor : int {
    Green = 0,
    Red = 1,
    Blue = 2,
    Yellow = 3
};

} // namespace geode
