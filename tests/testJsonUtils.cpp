#include "../src/json/jsonUtils.hpp"

#include "gtest/gtest.h"

namespace test
{

// Order is not preserved with nlohman::json as it uses a map.
// This string is ordered accordingly.
std::string jsonString =
    "{"
    "\"facebook\":{"
    "\"password\":\"pass\","
    "\"username\":\"user\""
    "},"
    "\"gmail\":{"
    "\"password\":\"pass\","
    "\"username\":\"user\""
    "}"
    "}";

VaultMap vaultMap {
    { "facebook", { "user", "pass" } },
    { "gmail", { "user", "pass" } }
};

TEST( JsonUtils, Json2Map )
{
    EXPECT_EQ( vaultMap, json::json2map( jsonString ) );
}

TEST( JsonUtils, Map2Json )
{
    EXPECT_EQ( jsonString, json::map2json( vaultMap ) );
}

}
