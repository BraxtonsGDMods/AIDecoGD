// include/EditorIntegration.hpp
// Helper functions to convert between in-game editor selection and our
// GDObjectStub, and to spawn generated objects into the level. Implemented
// for GEODE builds and safe no-op fallbacks for local tests.

#pragma once

#include "AIDeco.hpp"
#include <vector>

namespace aid {

// Convert actual in-editor selected objects into GDObjectStub list.
// GEODE builds should return the real data; non-GEODE builds return an empty list.
GDObjectList selection_to_stubs();

// Spawn a single GDObjectStub into the current level/editor.
// GEODE builds should create the real object (e.g., spawn using LevelEditor APIs).
bool spawn_stub_to_level(const GDObjectStub& stub);

} // namespace aid
