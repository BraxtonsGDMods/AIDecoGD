// src/engine/Themes.hpp
// Declarations and types for themes and helper utilities.

#pragma once

#include "../../include/AIDeco.hpp"
#include <random>

namespace aid {

struct Theme {
	std::string name;
	std::string mainColor;
	std::vector<std::string> preferredObjectTypes;
};

Theme pick_theme_from_prompt(const std::string& prompt, std::mt19937_64 &rng);
std::string pick_random_object_type(const Theme &theme, std::mt19937_64 &rng);

} // namespace aid
