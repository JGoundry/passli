#pragma once

#include <string>
#include <filesystem>

namespace crypt
{

bool encryptDataToFile( const std::string& data, const std::filesystem::path& path );

std::optional< std::string > decryptDataFromFile( const std::filesystem::path& path );

}
