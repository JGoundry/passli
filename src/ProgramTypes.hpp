#pragma once

#include <optional>
#include <string>

enum MODE {
    LIST,
    ADD,
    GET,
    DEL
};

struct ProgramOptions {
    MODE mode;
    std::optional<std::string> name;
    std::optional<std::string> username;
    std::optional<std::string> password;
};

struct ProgramSettings
{
    bool googleDrive = false;
    /* EncryptionType encryptionType = EncryptionType::AES; */
    /* std::string drivePath; */
};
