// src/engine/Themes.cpp
// Theme definitions and helper utilities for the decoration generator.
// This file provides placeholder "themes" derived from the prompt and
// utilities to pick object types for a theme. Replace or extend with a
// real style system or prompt parser.

#include "../../include/AIDeco.hpp"
#include <vector>
#include <string>
#include <cctype>
#include <random>

namespace aid {

struct Theme {
    std::string name;
    std::string mainColor; // simple color descriptor (placeholder)
    std::vector<std::string> preferredObjectTypes;
};

static const std::vector<Theme> kBuiltinThemes = {
    {"Crystal", "#7adfff", {"gem", "spike_small", "spark"}},
    {"Overgrowth", "#6fbf6f", {"vine", "leaf", "shrub"}},
    {"Neon", "#ff48c4", {"neon_strip", "tube", "glow"}},
    {"Mechanical", "#8a8a8a", {"gear", "pipe", "bolt"}}
};

// Basic mapping from prompt -> theme. This is a placeholder: in a real
// implementation you would run simple keyword matching or call a remote NLP model
// to pick style. We include an RNG so that theme picking can be deterministic
// with a given seed.
Theme pick_theme_from_prompt(const std::string& prompt, std::mt19937_64 &rng) {
    std::string lower = prompt;
    for (auto &c : lower) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    
    // Quick keyword checks
    if (lower.find("crystal") != std::string::npos || lower.find("gem") != std::string::npos) {
        return kBuiltinThemes[0];
    }
    if (lower.find("vine") != std::string::npos || lower.find("forest") != std::string::npos) {
        return kBuiltinThemes[1];
    }
    if (lower.find("neon") != std::string::npos || lower.find("cyber") != std::string::npos) {
        return kBuiltinThemes[2];
    }
    if (lower.find("tech") != std::string::npos || lower.find("metal") != std::string::npos) {
        return kBuiltinThemes[3];
    }

    // If no keywords matched, pick a pseudo-random theme deterministically
    std::uniform_int_distribution<size_t> pick(0, kBuiltinThemes.size() - 1);
    return kBuiltinThemes[pick(rng)];
}

std::string pick_random_object_type(const Theme &theme, std::mt19937_64 &rng) {
    if (theme.preferredObjectTypes.empty()) return "generic_deco";
    std::uniform_int_distribution<size_t> pick(0, theme.preferredObjectTypes.size() - 1);
    return theme.preferredObjectTypes[pick(rng)];
}

} // namespace aid
