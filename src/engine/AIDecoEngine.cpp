// src/engine/AIDecoEngine.cpp
// The core decoration engine: given a selection of base objects, a textual
// prompt, a detail level and a deterministic seed, return a list of GDObjectStubs
// representing decorations to spawn near/around the selection.

#include "../../include/AIDeco.hpp"
#include "DetailProfiles.hpp"
#include "Themes.hpp"

#include <random>
#include <sstream>
#include <chrono>

namespace aid {

// Deterministic random engine used for generation. If seed == 0 a random
// seed will be created from clock.
static std::mt19937_64 make_rng(uint64_t seed) {
    if (seed == 0) {
        seed = static_cast<uint64_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    }
    return std::mt19937_64(seed);
}

// Generate decorations from a selection of objects.
// - selection: list of GDObjectStub representing the selected structural objects
// - prompt: user text describing decoration style
// - detail: detail level to control the density/complexity
// - seed: deterministic seed; identical seed + inputs should produce identical outputs
GDObjectList generate_decorations_from_selection(
    const GDObjectList& selection,
    const std::string& prompt,
    DetailLevel detail,
    uint64_t seed
) {
    GDObjectList output;

    // Quick guard: nothing selected -> return empty
    if (selection.empty()) {
        return output;
    }

    // Create deterministic RNG
    auto rng = make_rng(seed);

    // Fetch object budget based on detail level
    int budget = get_object_budget(detail);

    // Choose a theme from prompt (placeholder) — in a real implementation do
    // NLP / prompt parsing or prompt-to-theme mapping.
    Theme theme = pick_theme_from_prompt(prompt, rng);

    // Distribute decorations across the selected objects.
    // Very simple placeholder algorithm: for each selected object, spawn up to
    // budget_per_base decorations near it.
    int budget_per_base = std::max(1, budget / static_cast<int>(selection.size()));

    uint64_t id_counter = 1;

    std::uniform_real_distribution<float> offset_dist(-60.0f, 60.0f);
    std::uniform_real_distribution<float> rotation_dist(-45.0f, 45.0f);

    for (const auto &base : selection) {
        for (int i = 0; i < budget_per_base; ++i) {
            GDObjectStub deco;
            deco.id = id_counter++;
            deco.type = pick_random_object_type(theme, rng);

            // Place decorations in a ring around the base object
            deco.x = base.x + offset_dist(rng);
            deco.y = base.y + offset_dist(rng) * 0.5f; // bias vertically smaller
            deco.rotation = rotation_dist(rng);

            // Meta is a compact description (color/scale) – placeholder
            std::stringstream meta;
            meta << "color=" << theme.mainColor << ";scale=" << (1.0f + ((rng() % 20) / 100.0f));
            deco.meta = meta.str();

            output.push_back(std::move(deco));

            // Respect an upper limit in case budget_per_base * num_selection > budget
            if (static_cast<int>(output.size()) >= budget) {
                break;
            }
        }
        if (static_cast<int>(output.size()) >= budget) {
            break;
        }
    }

    return output;
}

} // namespace aid
