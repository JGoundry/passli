#include "VaultManager.hpp"
#include "crypt/cryptUtils.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>

VaultManager::VaultManager( const std::filesystem::path& vaultPath ) :
    vaultPath_( vaultPath )
{
}

bool VaultManager::list() const
{
    for ( const auto& entry : std::filesystem::directory_iterator( vaultPath_ ) )
    {
        if ( entry.path().extension() != ".gpg" )
        {
            continue;
        }
        std::cout << entry.path().filename().string() << std::endl;
    }

    return true;
}

bool VaultManager::add( const std::string& name, const std::string& username, const std::string& password ) const
{
    const std::string plaintext = name + "|" + username + "|" + password;
    const std::filesystem::path path( vaultPath_.string() + "/" + name + ".gpg" );
    crypt::encryptDataToFile( plaintext, path );

    return true;
}

bool VaultManager::get( const std::string& name ) const
{
    const std::filesystem::path path( vaultPath_.string() + "/" + name + ".gpg" );
    if ( !std::filesystem::exists( path ) )
    {
        std::cerr << "Password does not exist." << std::endl;
        return false;
    }

    std::string plaintext = crypt::decryptDataFromFile( path );
    std::cout << plaintext << std::endl;

    return true;
}

bool VaultManager::del( const std::string& name ) const
{
    const std::filesystem::path path( vaultPath_.string() + "/" + name + ".gpg" );
    if ( !std::filesystem::exists( path ) )
    {
        std::cerr << "Password does not exist." << std::endl;
        return false;
    }
    
    std::filesystem::remove( path );

    return true;
}
