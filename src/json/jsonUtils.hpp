#include <map>
#include <string>

struct ServiceData
{
    std::string username;
    std::string password;

    bool operator==( const ServiceData& other ) const
    {
        return username == other.username &&
               password == other.password;
    }
};

namespace json
{

std::string map2json( const std::map< std::string, ServiceData >& passwordMap );

std::map< std::string, ServiceData > json2map( const std::string& jsonString );

} // namespace json
