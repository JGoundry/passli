#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace crypt
{

bool encryptDataToFile( const std::string& data, const std::filesystem::path& path );

std::optional< std::string > decryptDataFromFile( const std::filesystem::path& path );

} // namespace crypt
