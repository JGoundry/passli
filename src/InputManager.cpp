#include "InputManager.hpp"

#include <algorithm>
#include <cstring>
#include <iostream>

std::string getUsageString()
{
    return "Usage: passli [add|get|del] [name]";
}

InputManager::InputManager( int argc, char* argv[] ) : argc_( argc ),
                                                       argv_( argv )
{
}

bool InputManager::validateArgs()
{
    if ( argc_ < 2 || strcmp( argv_[1], "help" ) == 0 )
    {
        std::cerr << getUsageString() << std::endl;
        return false;
    }
    else if ( strcmp( argv_[1], "list" ) == 0 )
    {
        opts_.mode = MODE::LIST;
    }
    else if ( argc_ > 2 && strcmp( argv_[1], "add" ) == 0 )
    {
        opts_.mode = MODE::ADD;
        opts_.name = argv_[2];
    }
    else if ( argc_ > 2 && strcmp( argv_[1], "get" ) == 0 )
    {
        opts_.mode = MODE::GET;
        opts_.name = argv_[2];
    }
    else if ( argc_ > 2 && strcmp( argv_[1], "del" ) == 0 )
    {
        opts_.mode = MODE::DEL;
        opts_.name = argv_[2];
    }
    else
    {
        std::cerr << "Invalid mode." << std::endl;
        std::cerr << getUsageString() << std::endl;
        return false;
    }

    return true;
}

ProgramOptions InputManager::getOptions()
{
    if ( opts_.mode == MODE::ADD )
    {
        opts_.username = getUsername();
        opts_.password = getPassword();
    }

    return opts_;
}

std::string InputManager::getUsername()
{
    std::cout << "Enter username: " << std::endl;
    return getInput();
}

std::string InputManager::getPassword()
{
    std::cout << "Enter password: " << std::endl;
    return getInput();
}

std::string InputManager::getInput() const
{
    std::string input;
    std::cout << "> ";
    std::cin >> input;
    return input;
}

void InputManager::displayHelp()
{
    // todo
}

void InputManager::displayPassword( const ServiceData& data )
{
    std::cout << data.username << " > " << data.password << std::endl;
}

void InputManager::displayPasswordNames( const std::vector< std::string >& passwordNames )
{
    unsigned long longestName = 0;
    for ( const std::string& name : passwordNames )
        longestName = std::max( longestName, name.size() );

    std::cout << " " << std::string( longestName + 6, '-' ) << std::endl;
    for ( const std::string& name : passwordNames )
    {
        std::cout << " |--" << std::endl;
        std::cout << " |- " << name << std::endl;
    }
    std::cout << " |--" << std::endl;
    std::cout << " " << std::string( longestName + 6, '-' ) << std::endl;
}

void InputManager::printDebug( const std::string& message )
{
    std::cout << message << std::endl;
}

void InputManager::printError( const std::string& message )
{
    std::cout << message <<std::endl;
}
