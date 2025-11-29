// src/main.cpp
// Entry point for the AI Deco Geode mod. This file contains registration
// and initialization logic. Replace placeholder Geode API calls with actual
// Geode code when integrating into your Geode dev environment.

#include "../include/AIDeco.hpp"
#include <iostream>
#include <vector>

// Forward declarations for subsystem initializers
void init_ui();
void init_presets();

// Placeholder definitions to allow standalone compilation for quick testing.
void init_ui() {
    // In Geode this would register UI callbacks and create toolbar buttons.
    std::cout << "[main] init_ui() (placeholder)" << std::endl;
}

void init_presets() {
    // In Geode this would initialize the presets manager and prepare default presets.
    std::cout << "[main] init_presets() (placeholder)" << std::endl;
}

int main() {
    // NOTE: Geode loads mods as DLLs. This main() is a convenience for
    // local testing builds or CLI-based testing. In Geode, replace this
    // with the appropriate exported entry points / plugin registration.

    std::cout << "AI Deco: Starting (placeholder main)" << std::endl;

    // Initialize preset manager and UI (placeholders)
    init_presets();
    init_ui();

    std::cout << "AI Deco: Initialization complete (placeholder)." << std::endl;

    // Keep process alive for manual testing if run as a standalone app.
    // When built as a Geode DLL this main() won't be used.

    return 0;
}
