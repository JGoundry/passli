#include "PassliManager.hpp"
#include "InputManager.hpp"

#include <fstream>

PassliManager::PassliManager( int argc, char* argv[] ) : vaultPath_( std::filesystem::path( std::getenv( "HOME" ) ) / ".passli" ),
                                                         settingsPath_( std::filesystem::path( std::getenv( "HOME" ) ) / ".passli" / "settings.conf" ),
                                                         inputManager_( argc, argv ),
                                                         vaultManager_( vaultPath_ )
{
}

bool PassliManager::setup()
{
    if ( !inputManager_.validateArgs() )
    {
        return false;
    }

    opts_ = inputManager_.getOptions();

    if ( !initVault() )
    {
        InputManager::printError( "Failed to initialize vault." );
        return false;
    }

    if ( !initSettings() )
    {
        InputManager::printError( "Failed to initialize settings." );
        return false;
    }

    if ( settings_.googleDrive )
    {
        if ( !initDrive() )
        {
            InputManager::printError( "Failed to initialize google drive." );
        }
    }

    return true;
}

bool PassliManager::initVault()
{
    if ( !std::filesystem::exists( vaultPath_ ) )
    {
        std::filesystem::create_directory( vaultPath_ );
    }
    else if ( !std::filesystem::is_directory( vaultPath_ ) )
    {
        InputManager::printError( "Vault path is not a directory." );
        return false;
    }

    return true;
}

bool PassliManager::initSettings()
{
    if ( !std::filesystem::exists( settingsPath_ ) )
    {
        std::ofstream settingsFile( settingsPath_ );
    }

    std::ifstream settingsFile( settingsPath_ );
    std::string line;
    while ( std::getline( settingsFile, line ) )
    {
        if ( line.find( "googleDrive" ) != std::string::npos )
        {
            settings_.googleDrive = line.substr( line.find( "=" ) + 1 ) == "true";
        }
    }

    return true;
}

bool PassliManager::initDrive()
{
    // Connect to google drive

    return true;
}

bool PassliManager::syncDrive()
{
    // Sync to google drive

    return true;
}

bool PassliManager::run()
{
    bool result = false;

    switch ( opts_.mode )
    {
    case MODE::LIST:
    {
        const std::vector< std::string > passwordNames = vaultManager_.list();
        inputManager_.displayPasswordNames( passwordNames );
        result = true;
        break;
    }
    case MODE::ADD:
    {
        result = vaultManager_.add( opts_.name.value(), opts_.username.value(), opts_.password.value() );
        break;
    }
    case MODE::GET:
    {
        const std::optional< std::string > password = vaultManager_.get( opts_.name.value() );
        if ( password )
        {

            inputManager_.displayPassword( password.value() );
            result = true;
        }
        break;
    }
    case MODE::DEL:
    {
        result = vaultManager_.del( opts_.name.value() );
        break;
    }
    }

    if ( result && settings_.googleDrive )
    {
        if ( !syncDrive() )
        {
            InputManager::printError( "Failed to sync to google drive." );
        }
    }

    return result;
}
