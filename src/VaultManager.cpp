#include "VaultManager.hpp"
#include "crypt/cryptUtils.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <vector>
#include <algorithm>

VaultManager::VaultManager( const std::filesystem::path& vaultPath ) :
    vaultPath_( vaultPath )
{
}

bool VaultManager::list() const
{
    std::vector<std::string> passwordNames;
    unsigned long longestName = 0;
    for ( const auto& entry : std::filesystem::directory_iterator( vaultPath_ ) )
    {
        if ( entry.path().extension() != ".gpg" )
        {
            continue;
        }
        const std::string passwordName = entry.path().filename().string().substr( 0, entry.path().filename().string().find( ".gpg" ) );
        longestName = std::max( longestName, passwordName.size() );
        passwordNames.emplace_back( entry.path().filename().string().substr( 0, entry.path().filename().string().find( ".gpg" ) ) );
    }

    if ( passwordNames.empty() )
    {
        return false;
    }

    std::cout << " " << std::string( longestName + 6, '-' ) << std::endl;
    for ( const std::string& password : passwordNames )
    {
        std::cout << " |--" << std::endl;
        std::cout << " |- " << password << std::endl;
    }
    std::cout << " |--" << std::endl;
    std::cout << " " << std::string( longestName + 6, '-' ) << std::endl;


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
