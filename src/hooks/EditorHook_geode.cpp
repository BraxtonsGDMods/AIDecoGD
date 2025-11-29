// src/hooks/EditorHook_geode.cpp
// GEODE-specific editor hook stub. This file contains example Geode/Cocos2d-x
// code that demonstrates how to register the toolbar button and open the UI
// panel inside the Geometry Dash Level Editor.

#ifdef GEODE

#include <Geode/Geode.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/ui/Editors/EditorUI.hpp>

#include "../../include/AIDeco.hpp"

using namespace geode;

// Forward-declare geode toolbar helper from UI files
void create_ai_deco_toolbar_button_geode();

// Example: called when mod loads inside Geometry Dash
static void geode_on_load() {
    // Hook EditorUI or LevelEditorLayer to add a button to the toolbar
    // This is pseudo-code using common Geode/Cocos2d-x patterns.

    // Use the geode-specific toolbar helper if available
    create_ai_deco_toolbar_button_geode();
}

GEODE_MODIFY_CALL_BASECLASS(Mod, Hook, bool, onLoad) {
    geode_on_load();
    return Mod::onLoad();
}

#endif // GEODE
