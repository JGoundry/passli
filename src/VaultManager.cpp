#include "VaultManager.hpp"

#include <cstdlib>
#include <filesystem>

#include <iostream>

VaultManager::VaultManager( const std::filesystem::path& vaultPath ) :
    vaultPath_( vaultPath )
{
}

bool VaultManager::add( const std::string& name, const std::string& username, const std::string& password ) const
{
    const std::string addString = name + "|" + username + "|" + password;
    const std::string command = "echo \"" + addString + "\" | gpg --symmetric -o " + vaultPath_.string() + "/" + name + ".gpg";
    std::system(command.c_str());

    return true;
}

bool VaultManager::get( const std::string& name ) const
{
    const std::string command = "echo \"" + name + "\" | gpg --symmetric -o \"" + vaultPath_.string() + "/" + name + ".gpg\"";
    std::system(command.c_str());

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
    const std::string command = "rm " + path.string();
    std::system(command.c_str());

    return true;
}
