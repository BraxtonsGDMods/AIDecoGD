// src/ui/AIDecoButton_geode.cpp
// GEODE-specific toolbar button creation.

#ifdef GEODE

#include <Geode/Geode.hpp>
#include <Geode/ui/Editors/EditorUI.hpp>

#include "../../include/AIDeco.hpp"

using namespace geode;

void create_ai_deco_toolbar_button_geode() {
    using namespace geode;
    using namespace cocos2d;

    // Create an editor-style button using Geode UI helpers and add it to the
    // running scene. This is intentionally generic so it works in most scenes.
    try {
        auto scene = CCDirector::get()->getRunningScene();
        if (!scene) return;

        // Create a small editor-style button and position in the top-right.
        auto btn = EditorButtonSprite::createWithSpriteFrameName("Mods/ai-deco/res/ui/ai_deco_panel.png", EditorBaseColor::Green, EditorBaseSize::Small);
        if (!btn) return;

        btn->addClickListener([](cocos2d::CCObject*){
            open_ai_deco_panel_geode();
        });

        // Position and add to scene. Use addChild so the button is visible in-game.
        btn->setPosition(ccp(CCDirector::get()->getWinSize().width - 60.0f, CCDirector::get()->getWinSize().height - 60.0f));
        scene->addChild(btn);
    } catch (...) {
        // If the types are not exactly available in the build environment,
        // silently fail and keep original fallback behavior.
    }
}

#endif // GEODE
