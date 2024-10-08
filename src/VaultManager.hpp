#pragma once

#include "json/jsonUtils.hpp"

#include <filesystem>
#include <string>
#include <vector>

class VaultManager
{
public:
    VaultManager( const std::filesystem::path& vaultPath );
    bool add( const std::string& name, const std::string& username, const std::string& password );
    bool del( const std::string& name );
    std::optional< ServiceData > get( const std::string& name ) const;
    std::vector< std::string > list() const;

private:
    std::filesystem::path vaultPath_;
    VaultMap vaultMap_;
};
