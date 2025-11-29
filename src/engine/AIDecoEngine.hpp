// src/engine/AIDecoEngine.hpp
// Declarations for the decoration engine entry-points.

#pragma once

#include "../../include/AIDeco.hpp"

namespace aid {

GDObjectList generate_decorations_from_selection(
    const GDObjectList& selection,
    const std::string& prompt,
    DetailLevel detail,
    uint64_t seed
);

} // namespace aid
