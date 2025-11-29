// src/ui/AIDecoPanel.cpp
// Placeholder UI panel code for the AI Deco editor panel. This stub provides
// the public interface and comments describing how it should behave when
// implemented with the real Geode/Cocos2d-x UI APIs.

#include "../../include/AIDeco.hpp"
#include "../../include/EditorIntegration.hpp"
#include "../engine/AIDecoEngine.hpp"
#include <iostream>

namespace aid {

class AIDecoPanel {
public:
    AIDecoPanel() {
        // Build UI elements: prompt textbox, detail selector buttons,
        // "Generate" button, Presets tab and load/save controls.
    }

    // Show the panel in the editor
    void show() {
        // Example pseudocode:
        // this->rootNode->setVisible(true);
        std::cout << "[Panel] Showing AI Deco panel (placeholder)." << std::endl;

    #ifdef GEODE
        // GEODE + Cocos2d-x example (pseudo-code):
        // auto editor = EditorLayer::get();
        // auto panel = createPanelNode("AI Deco");
        // panel->addChild(createTextField("Prompt..."));
        // panel->addChild(createButtonsForDetails());
        // panel->addChild(createButton("Generate", [this](){ /* call engine & spawn */ }));
        // editor->addChild(panel);
    #endif
    }

    // Hide the panel
    void hide() {
        // this->rootNode->setVisible(false);
        std::cout << "[Panel] Hiding AI Deco panel (placeholder)." << std::endl;
    }

    // Called when the user presses the Generate button; collects prompt +
    // selected detail level + seed and calls the engine.
    void on_generate(const std::string& prompt, DetailLevel detail, uint64_t seed) {
        std::cout << "[Panel] Generating decorations with prompt: '" << prompt << "'" << std::endl;

        // 1) Gather current selection from the editor using a platform-agnostic helper
        auto selection = selection_to_stubs();
        if (selection.empty()) {
            std::cout << "[Panel] No selection found — nothing to generate." << std::endl;
            return;
        }

        // 2) Call the engine to generate decorations
        auto generated = generate_decorations_from_selection(selection, prompt, detail, seed);

        // 3) Spawn returned objects into the level using spawn_stub_to_level
        for (const auto &g : generated) {
            bool spawned = spawn_stub_to_level(g);
            if (!spawned) {
                std::cout << "[Panel] Failed to spawn generated object id=" << g.id << std::endl;
            }
        }
    }
};

void open_ai_deco_panel() {
    static AIDecoPanel instance; // single instance for editor session
    instance.show();
}

} // namespace aid
