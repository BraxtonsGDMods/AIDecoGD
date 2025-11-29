// src/presets/PresetManager.cpp
// Handles loading/saving user presets as JSON files and creating default presets.
// NOTE: This file uses plain file IO and lightweight text parsing placeholders.
// For integration you should use a proper JSON library (e.g., nlohmann::json)
// or your engine's JSON utilities.

#include "../../include/AIDeco.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace aid {

// Directory where presets are stored (relative to the mod directory)
static const char* kPresetDir = "AI_Deco/Presets/";

// Load all JSON preset files from the preset directory and return a list of presets.
PresetList load_presets_from_disk() {
    PresetList out;

    // NOTE: For cross-platform directory listing you may want to use <filesystem>
    // or platform-specific APIs. Here we provide a placeholder for integration.

    // Use filesystem to list *.json presets in the preset directory
    fs::path presetDir(kPresetDir);
    if (!fs::exists(presetDir)) {
        std::cout << "[PresetManager] Preset directory does not exist, creating: " << kPresetDir << std::endl;
        try { fs::create_directories(presetDir); } catch (...) { /* ignore */ }
    }

    for (auto &entry : fs::directory_iterator(presetDir)) {
        if (!entry.is_regular_file()) continue;
        if (entry.path().extension() != ".json") continue;

        std::ifstream fin(entry.path());
        if (!fin.is_open()) continue;
        std::stringstream ss;
        ss << fin.rdbuf();
        std::string text = ss.str();
        try {
            auto p = AIDecoPreset::from_json(text);
            if (!p.name.empty()) {
                out.push_back(p);
                std::cout << "[PresetManager] Loaded preset: " << p.name << " (" << entry.path().filename().string() << ")" << std::endl;
            }
        } catch (const std::exception &e) {
            std::cout << "[PresetManager] Failed to parse " << entry.path() << ": " << e.what() << std::endl;
        }
    }
    return out;
}

// Save a single preset to disk (placeholder). Use JSON in real implementation.
bool save_preset_to_disk(const AIDecoPreset& preset) {
    fs::path d(kPresetDir);
    if (!fs::exists(d)) {
        try { fs::create_directories(d); } catch (...) { }
    }

    std::string out_path = (d / (preset.name + ".json")).string();
    std::ofstream fout(out_path);
    if (!fout.is_open()) {
        std::cerr << "[PresetManager] Failed to save preset to " << out_path << std::endl;
        return false;
    }

    // Use AIDecoPreset -> to_json() helper
    fout << preset.to_json();

    fout.close();
    std::cout << "[PresetManager] Saved preset to " << out_path << " (placeholder format)." << std::endl;
    return true;
}

// Create default preset file if missing
void ensure_default_presets_exist() {
    // For robust implementation use std::filesystem to check and create directories
    // Here we will simply mention where the file should be placed.
    std::cout << "[PresetManager] Ensure default presets exist in: " << kPresetDir << std::endl;
}

} // namespace aid
