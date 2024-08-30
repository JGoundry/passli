#pragma once

#include "InputManager.hpp"
#include "ProgramTypes.hpp"
#include "VaultManager.hpp"

#include <filesystem>

class PassliManager
{
public:
    PassliManager( int argc, char* argv[] );
    bool setup();
    bool run();

private:
    bool initVault();
    bool initSettings();
    bool initDrive();
    bool syncDrive();

    std::filesystem::path vaultPath_;
    std::filesystem::path settingsPath_;
    ProgramOptions opts_;
    ProgramSettings settings_;
    InputManager inputManager_;
    VaultManager vaultManager_;
};
