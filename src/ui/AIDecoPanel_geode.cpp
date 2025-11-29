// src/ui/AIDecoPanel_geode.cpp
// GEODE/Cocos2d-x implementation for the AI Deco panel (stub).
// This file demonstrates how to build the panel using Geode APIs. It is
// surrounded by #ifdef GEODE so it won't interfere with local testing builds.

#ifdef GEODE

#include <Geode/Geode.hpp>
#include <cocos2d.h>
#include <Geode/ui/Editors/EditorUI.hpp>

#include "../../include/AIDeco.hpp"

using namespace geode;

// Example stub function to create and present a panel using Cocos2d-x nodes.
void open_ai_deco_panel_geode() {
    using namespace geode;
    using namespace cocos2d;

    // Create a simple Popup using Geode UI utilities.
    try {
        // Show a quick popup that asks the user to confirm generation.
        auto layer = createQuickPopup(
            "AI Deco",
            "Generate decorations around your current selection using the demo prompt 'crystal neon'?",
            "Generate",
            "Cancel",
            [&](FLAlertLayer* l, bool clicked){
                if (!clicked) return; // canceled
                std::string prompt = "crystal neon";
                auto selection = aid::selection_to_stubs();
                if (selection.empty()) return;
                auto generated = aid::generate_decorations_from_selection(selection, prompt, DetailLevel::Medium, 0);
                for (const auto &g : generated) {
                    aid::spawn_stub_to_level(g);
                }
            }
        );
        // add to running scene
        if (layer) CCDirector::get()->getRunningScene()->addChild(layer);
    } catch (...) {
        // If GEODE UI types are not available at compile time, silently fail.
    }
}

#endif // GEODE
