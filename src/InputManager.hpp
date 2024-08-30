#pragma once

#include "ProgramTypes.hpp"

#include <string>
#include <vector>

// add - adds a new password to the vault
// get - gets a password from the vault
// del - deletes a password from the vault

class InputManager
{
public:
    InputManager( int argc, char* argv[] );
    bool validateArgs();
    ProgramOptions getOptions();
    static void displayHelp();
    static void displayPassword( const std::string& password );
    static void displayPasswordNames( const std::vector< std::string >& passwordNames );
    static void printDebug( const std::string& message );
    static void printError( const std::string& message );

private:
    std::string getUsername();
    std::string getPassword();
    std::string getInput() const;

    int argc_;
    char** argv_;
    ProgramOptions opts_;
};
