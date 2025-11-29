// src/presets/AIDecoPreset_json.cpp
// Implements AIDecoPreset serialization using nlohmann::json

#include "../../include/AIDeco.hpp"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

namespace aid {

std::string AIDecoPreset::to_json() const {
    json j;
    j["name"] = name;
    j["prompt"] = prompt;
    j["detail"] = static_cast<int>(detail);
    j["seed"] = seed;
    j["description"] = description;
    return j.dump(2);
}

AIDecoPreset AIDecoPreset::from_json(const std::string& jsonText) {
    AIDecoPreset p;
    try {
        auto j = json::parse(jsonText);
        if (j.contains("name")) p.name = j["name"].get<std::string>();
        if (j.contains("prompt")) p.prompt = j["prompt"].get<std::string>();
        if (j.contains("detail")) p.detail = static_cast<DetailLevel>(j["detail"].get<int>());
        if (j.contains("seed")) p.seed = j["seed"].get<uint64_t>();
        if (j.contains("description")) p.description = j["description"].get<std::string>();
    } catch (const std::exception &e) {
        std::cerr << "[AIDecoPreset::from_json] parse error: " << e.what() << std::endl;
    }
    return p;
}

} // namespace aid
