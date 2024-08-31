#include "VaultManager.hpp"
#include "InputManager.hpp"
#include "crypt/cryptUtils.hpp"

#include <cstdlib>
#include <filesystem>

VaultManager::VaultManager( const std::filesystem::path& vaultPath ) : vaultPath_( vaultPath )
{
    // open vault
    std::optional< std::string > jsonString = crypt::decryptDataFromFile( vaultPath_ );

    if ( !jsonString )
        return;

    vaultMap_ = json::json2map( jsonString.value() );
}

std::vector< std::string > VaultManager::list() const
{
    std::vector< std::string > passwordNames;

    for ( const auto& [name, data] : vaultMap_ )
    {
        passwordNames.push_back( name );
    }

    return passwordNames;
}

bool VaultManager::add( const std::string& name, const std::string& username, const std::string& password )
{
    vaultMap_[name] = { username, password };
    return crypt::encryptDataToFile( json::map2json( vaultMap_ ), vaultPath_ );
}

std::optional< ServiceData > VaultManager::get( const std::string& name ) const
{
    if ( !vaultMap_.contains( name ) )
    {
        InputManager::printError( "Password does not exist." );
        return std::nullopt;
    }

    return vaultMap_.at( name );
}

bool VaultManager::del( const std::string& name )
{
    if ( !vaultMap_.contains( name ) )
    {
        InputManager::printError( "Password does not exist." );
        return false;
    }

    vaultMap_.erase( name );

    return true;
}
