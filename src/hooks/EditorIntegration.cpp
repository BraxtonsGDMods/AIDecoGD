// src/hooks/EditorIntegration.cpp
// Portable implementations: GEODE builds will include Geode headers and implement
// the conversions and spawning. Non-GEODE builds expose safe no-op functions
// so the code compiles and can be smoke-tested.

#include "../../include/EditorIntegration.hpp"
#include <iostream>

namespace aid {

GDObjectList selection_to_stubs() {
#ifdef GEODE
    // TODO: Replace this pseudo-code with an actual Geode+GeometryDash API to
    // convert real editor selection into GDObjectStub objects. Geode does not
    // provide the game-specific selection types in this repo so we use a
    // developer-friendly fallback that returns a small demo-selection so the
    // UI and generation flow can be tested in-game.

    std::cout << "[EditorIntegration] GEODE: (demo mode) returning a small fake selection for UI test" << std::endl;
    GDObjectList fake;
    GDObjectStub s1; s1.id = 101; s1.x = 200; s1.y = 120; s1.type = "demo_base";
    GDObjectStub s2; s2.id = 102; s2.x = 360; s2.y = 140; s2.type = "demo_base";
    fake.push_back(s1);
    fake.push_back(s2);
    return fake;
#else
    // Local fallback for smoke tests: return a small fake selection
    GDObjectList fake;
    GDObjectStub s1; s1.id = 1; s1.x = 100; s1.y = 120; s1.type = "base_block";
    GDObjectStub s2; s2.id = 2; s2.x = 280; s2.y = 130; s2.type = "base_block";
    fake.push_back(s1);
    fake.push_back(s2);
    return fake;
#endif
}

bool spawn_stub_to_level(const GDObjectStub& stub) {
#ifdef GEODE
    // Create a small visual indicator in-game representing the generated
    // decoration. This is a demo fallback so users can see generated objects
    // on their screen without using game-specific spawn APIs.
    using namespace geode;
    using namespace cocos2d;
    try {
        auto scene = CCDirector::get()->getRunningScene();
        if (!scene) return false;

        // Use a simple sprite and tint based on stub.type hash
        float size = 20.0f;
        auto s = CCSprite::createWithSpriteFrameName("GJ_circle_01.png");
        if (!s) {
            s = CCSprite::create();
            s->setTextureRect({0,0, (int)size, (int)size});
        }

        // Basic color derivation from meta field
        uint32_t colorHash = 0;
        for (char c : stub.meta) colorHash = colorHash * 31 + static_cast<unsigned char>(c);
        uint8_t r = static_cast<uint8_t>((colorHash >> 0) & 0xff);
        uint8_t g = static_cast<uint8_t>((colorHash >> 8) & 0xff);
        uint8_t b = static_cast<uint8_t>((colorHash >> 16) & 0xff);
        s->setColor({ r, g, b });

        s->setPosition(ccp(stub.x, stub.y));
        scene->addChild(s);

        return true;
    } catch (...) {
        return false;
    }
#else
    // In tests, just print the stub contents to stdout and return success
    std::cout << "[EditorIntegration] spawn_stub_to_level (test): id=" << stub.id << " type=" << stub.type << " x=" << stub.x << " y=" << stub.y << std::endl;
    return true;
#endif
}

} // namespace aid
