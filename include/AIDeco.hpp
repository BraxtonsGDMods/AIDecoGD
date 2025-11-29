#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <memory>

// AI Deco header
// Contains data structures, enums and serializable preset structs used by the mod.
// This is intentionally standalone and does not require Geode includes so the
// code can be compiled for basic testing. Replace placeholder types and
// serialization logic with your Geode / rapidjson / nlohmann::json code when
// integrating in a Geode environment.

namespace aid {

// A lightweight stub that represents a Geometry Dash level object suitable
// for generating and returning from the decoration engine.
// In a real Geode integration, substitute with the actual object type or a
// serializer/deserializer to/from the in-game object.
struct GDObjectStub {
    // Unique-ish identifier for debugging / temporary tracking
    uint64_t id = 0;

    // Position in level coordinates
    float x = 0.0f;
    float y = 0.0f;

    // Rotation angle in degrees
    float rotation = 0.0f;

    // Object type name or ID (e.g., "junction", "block", "spikes")
    std::string type;

    // Arbitrary parameters or metadata (color, scale, opacity, etc.)
    std::string meta;

    // Convert to a textual representation for debug / tests
    std::string to_string() const;
};

// Detail level of generated decorations; affects how many objects will be spawned
// and how dense/complex the decorations will be.
enum class DetailLevel : int {
    Low = 0,
    Medium = 1,
    High = 2,
    Ultra = 3
};

// Preset structure. Presets are saved by users for repeatable AI-driven decorating.
// The `seed` field ensures deterministic results when applying the preset.
struct AIDecoPreset {
    std::string name;             // Human-readable name
    std::string prompt;           // Prompt text describing the style
    DetailLevel detail = DetailLevel::Medium;
    uint64_t seed = 0;            // Deterministic seed (0 means random fallback)

    // Additional optional metadata for the preset.
    std::string description;

    // Serialize/deserialize placeholders (when integrating, use JSON lib)
    std::string to_json() const;
    static AIDecoPreset from_json(const std::string& jsonText);
};

using GDObjectList = std::vector<GDObjectStub>;
using PresetList = std::vector<AIDecoPreset>;

// Preset manager interface (implemented in src/presets/PresetManager.cpp)
PresetList load_presets_from_disk();
bool save_preset_to_disk(const AIDecoPreset& preset);
void ensure_default_presets_exist();

} // namespace aid

// Serialization helpers implemented in src/presets/AIDecoPreset_json.cpp
// using nlohmann::json (see CMake FetchContent configuration).
