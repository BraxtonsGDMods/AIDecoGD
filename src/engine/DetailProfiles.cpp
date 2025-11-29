// src/engine/DetailProfiles.cpp
// Definitions of detail level profiles. These control approximate object counts
// and other limits for each DetailLevel.

#include "../../include/AIDeco.hpp"
#include <array>

namespace aid {

static constexpr std::array<int, 4> kObjectBudgetByDetail = {
    8,   // Low
    20,  // Medium
    60,  // High
    150  // Ultra
};

int get_object_budget(DetailLevel detail) {
    int idx = static_cast<int>(detail);
    if (idx < 0) idx = 0;
    if (idx >= static_cast<int>(kObjectBudgetByDetail.size())) idx = static_cast<int>(kObjectBudgetByDetail.size()) - 1;
    return kObjectBudgetByDetail[idx];
}

} // namespace aid
