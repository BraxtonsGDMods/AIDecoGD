// src/tests/preset_test.cpp
// Test preset save and load with nlohmann::json and filesystem

#include "../../include/AIDeco.hpp"
#include <iostream>
#include <cassert>

using namespace aid;

int main() {
    // Ensure directory is present and empty-ish
    AIDecoPreset p;
    p.name = "unit_test_preset";
    p.prompt = "neon test";
    p.detail = DetailLevel::Low;
    p.seed = 999;
    p.description = "Unit test preset file.";

    // Save it
    bool ok = save_preset_to_disk(p);
    if (!ok) {
        std::cerr << "[TEST] Failed to save preset" << std::endl;
        return 2;
    }

    // Load presets
    auto list = load_presets_from_disk();
    bool found = false;
    for (auto &e : list) {
        if (e.name == p.name) {
            found = true;
            break;
        }
    }

    if (!found) {
        std::cerr << "[TEST] Failed to find saved preset in loaded presets" << std::endl;
        return 3;
    }

    std::cout << "[TEST] Preset save/load test passed" << std::endl;
    return 0;
}
