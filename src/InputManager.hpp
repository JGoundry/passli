#pragma once

#include "ProgramTypes.hpp"

#include <string>

// add - adds a new password to the vault
// get - gets a password from the vault
// del - deletes a password from the vault

class InputManager
{
public:
    InputManager( int argc, char *argv[] );
    bool validateArgs();
    ProgramOptions getOptions();

private:
    std::string getUsername();
    std::string getPassword();
    std::string getInput() const;

    int argc_;
    char **argv_;
    ProgramOptions opts_;
};
