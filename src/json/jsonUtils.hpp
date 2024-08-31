#include "../ProgramTypes.hpp"

#include <map>
#include <string>

namespace json
{

std::string map2json( const VaultMap& vaultMap );

VaultMap json2map( const std::string& jsonString );

} // namespace json
