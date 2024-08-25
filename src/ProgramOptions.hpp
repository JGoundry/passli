#pragma once

#include <optional>
#include <string>

enum MODE {
    ADD,
    GET,
    DEL
};

struct ProgramOptions {
    MODE mode;
    std::string name;
    std::optional<std::string> username;
    std::optional<std::string> password;
};

