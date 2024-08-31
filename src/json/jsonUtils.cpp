#include "jsonUtils.hpp"

#include <nlohmann/json.hpp>

namespace json
{

std::string map2json( const std::map< std::string, ServiceData >& passwordMap )
{
    nlohmann::json json;

    for ( const auto& [serviceName, serviceData] : passwordMap )
    {
        json[serviceName] = { { "username", serviceData.username }, { "password", serviceData.password } };
    }

    return nlohmann::to_string( json );
}

std::map< std::string, ServiceData > json2map( const std::string& jsonString )
{
    std::map< std::string, ServiceData > vaultMap;

    nlohmann::json json = nlohmann::json::parse( jsonString );

    for ( const auto& [serviceName, serviceData] : json.items() )
    {
        ServiceData data = { serviceData["username"], serviceData["password"] };
        vaultMap[serviceName] = data;
    }

    return vaultMap;
}

} //namespace json
