#include "VaultManager.hpp"
#include "InputManager.hpp"
#include "crypt/cryptUtils.hpp"

#include <cstdlib>
#include <filesystem>

VaultManager::VaultManager( const std::filesystem::path& vaultPath ) : vaultPath_( vaultPath )
{
}

std::vector< std::string > VaultManager::list() const
{
    std::vector< std::string > passwordNames;
    for ( const auto& entry : std::filesystem::directory_iterator( vaultPath_ ) )
    {
        if ( entry.path().extension() != ".gpg" )
        {
            continue;
        }
        const std::string passwordName = entry.path().filename().string().substr( 0, entry.path().filename().string().find( ".gpg" ) );
        passwordNames.emplace_back( entry.path().filename().string().substr( 0, entry.path().filename().string().find( ".gpg" ) ) );
    }

    return passwordNames;
}

bool VaultManager::add( const std::string& name, const std::string& username, const std::string& password ) const
{
    const std::string plaintext = name + "|" + username + "|" + password;
    const std::filesystem::path path( vaultPath_.string() + "/" + name + ".gpg" );

    return crypt::encryptDataToFile( plaintext, path );
}

std::optional< std::string > VaultManager::get( const std::string& name ) const
{
    const std::filesystem::path path( vaultPath_.string() + "/" + name + ".gpg" );
    if ( !std::filesystem::exists( path ) )
    {
        InputManager::printError( "Password does not exist." );
        return std::nullopt;
    }

    return crypt::decryptDataFromFile( path );
}

bool VaultManager::del( const std::string& name ) const
{
    const std::filesystem::path path( vaultPath_.string() + "/" + name + ".gpg" );
    if ( !std::filesystem::exists( path ) )
    {
        InputManager::printError( "Password does not exist." );
        return false;
    }

    std::filesystem::remove( path );

    return true;
}
