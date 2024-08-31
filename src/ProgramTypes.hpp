#pragma once

#include <map>
#include <optional>
#include <string>

enum MODE
{
    LIST,
    ADD,
    GET,
    DEL
};

struct ProgramOptions
{
    MODE mode;
    std::optional< std::string > name;
    std::optional< std::string > username;
    std::optional< std::string > password;
};

struct ProgramSettings
{
    bool googleDrive = false;
    /* EncryptionType encryptionType = EncryptionType::AES; */
    /* std::string drivePath; */
};

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

using VaultMap = std::map< std::string, ServiceData >;
