// src/tests/engine_test.cpp
// Simple deterministic test to ensure the generation engine is deterministic
// given the same seed + inputs.

#include "../../include/AIDeco.hpp"
#include "../engine/AIDecoEngine.hpp"
#include <iostream>
#include <cassert>

using namespace aid;

static bool equal_deco_lists(const GDObjectList &a, const GDObjectList &b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i].type != b[i].type) return false;
        if (a[i].x != b[i].x) return false;
        if (a[i].y != b[i].y) return false;
        if (a[i].rotation != b[i].rotation) return false;
        if (a[i].meta != b[i].meta) return false;
    }
    return true;
}

int main() {
    // Build a simple selection: a few base objects
    GDObjectList selection;
    for (int i = 0; i < 3; ++i) {
        GDObjectStub s;
        s.id = i + 1;
        s.x = i * 120.0f;
        s.y = 100.0f + i * 10.0f;
        s.type = "base_block";
        selection.push_back(s);
    }

    const std::string prompt = "crystal neon";
    const uint64_t seed = 555123;

    // Generate twice with the same seed
    GDObjectList out1 = generate_decorations_from_selection(selection, prompt, DetailLevel::High, seed);
    GDObjectList out2 = generate_decorations_from_selection(selection, prompt, DetailLevel::High, seed);

    // They should be equal (deterministic)
    if (!equal_deco_lists(out1, out2)) {
        std::cerr << "[TEST] Failure: identical seeds produced different outputs" << std::endl;
        return 2;
    }

    // Same selection but different seed -> should ideally differ
    GDObjectList out3 = generate_decorations_from_selection(selection, prompt, DetailLevel::High, seed + 1);
    if (equal_deco_lists(out1, out3)) {
        std::cerr << "[TEST] Warning: outputs are identical despite different seed (not necessarily fatal)" << std::endl;
    }

    std::cout << "[TEST] Determinism test passed (same seed -> identical results)" << std::endl;
    return 0;
}
