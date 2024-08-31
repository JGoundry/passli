#include "jsonUtils.hpp"

#include <nlohmann/json.hpp>

namespace json
{

std::string map2json( const VaultMap& vaultMap )
{
    nlohmann::json json;

    for ( const auto& [serviceName, serviceData] : vaultMap )
    {
        json[serviceName] = { { "username", serviceData.username }, { "password", serviceData.password } };
    }

    return nlohmann::to_string( json );
}

VaultMap json2map( const std::string& jsonString )
{
    VaultMap vaultMap;

    nlohmann::json json = nlohmann::json::parse( jsonString );

    for ( const auto& [serviceName, serviceData] : json.items() )
    {
        ServiceData data = { serviceData["username"], serviceData["password"] };
        vaultMap[serviceName] = data;
    }

    return vaultMap;
}

} //namespace json
