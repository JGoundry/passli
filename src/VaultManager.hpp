#pragma once

#include <string>
#include <filesystem>

class VaultManager
{
public:
    VaultManager( const std::filesystem::path& vaultPath );
    bool add( const std::string& name, const std::string& username, const std::string& password ) const;
    bool get( const std::string& name ) const;
    bool del( const std::string& name ) const;

private:
    std::filesystem::path vaultPath_;
};
