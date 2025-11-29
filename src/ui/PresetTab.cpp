// src/ui/PresetTab.cpp
// Preset tab UI placeholder - allows listing, applying and managing presets.

#include "../../include/AIDeco.hpp"
#include "../../include/EditorIntegration.hpp"
#include "../engine/AIDecoEngine.hpp"
#include <iostream>

namespace aid {

class PresetTab {
public:
    PresetTab() {}

    // Render the list of presets
    void render(const PresetList& presets) {
        std::cout << "[PresetTab] List of presets:" << std::endl;
        for (size_t i = 0; i < presets.size(); ++i) {
            const auto &p = presets[i];
            std::cout << "  [" << i << "] " << p.name << " (detail=" << static_cast<int>(p.detail) << ")" << std::endl;
        }
    }

    // Called when user requests to apply a preset to the current selection
    void apply_preset(const AIDecoPreset& preset) {
        std::cout << "[PresetTab] Applying preset '" << preset.name << "'" << std::endl;

        // 1) Gather selection using integrations helper
        auto selection = selection_to_stubs();
        if (selection.empty()) {
            std::cout << "[PresetTab] No selection found — nothing to apply." << std::endl;
            return;
        }

        // 2) Generate decorations using the engine
        auto generated = generate_decorations_from_selection(selection, preset.prompt, preset.detail, preset.seed);

        // 3) Spawn generated objects
        for (const auto &g : generated) {
            if (!spawn_stub_to_level(g)) {
                std::cout << "[PresetTab] Failed to spawn object id=" << g.id << std::endl;
            }
        }
    }

    // Rename, delete and create preset functions should be implemented using
    // PresetManager APIs to persist them to AI_Deco/Presets/
};

} // namespace aid
