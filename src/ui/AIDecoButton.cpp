// src/ui/AIDecoButton.cpp
// Placeholder code to create a toolbar button in the editor that opens the
// AI Deco panel. Replace UI creation code with Geode / Cocos2d-x UI calls.

#include "../../include/AIDeco.hpp"
#include <iostream>

namespace aid {

void create_ai_deco_toolbar_button() {
    // PSEUDO-CODE (Geode/Cocos2d-x):
    // auto toolbar = EditorUI::getToolbar();
    // auto button = ToolbarButton::create("AI Deco", iconTexture);
    // button->onClick = []() { open_ai_deco_panel(); };
    // toolbar->addButton(button);

    std::cout << "[UI] AI Deco toolbar button would be created here (placeholder)." << std::endl;

#ifdef GEODE
    // GEODE / Cocos2d-x example (pseudo-code):
    // auto toolbar = EditorLayer::get()->getToolbar();
    // auto texture = LoadTextureFromMod("res/ui/ai_deco_panel.png");
    // auto button = ToolbarButton::create("AI Deco", texture);
    // button->onClick = [](){ open_ai_deco_panel(); };
    // toolbar->addButton(button);
#endif
}

} // namespace aid
