#include "cryptUtils.hpp"
#include "../InputManager.hpp"

#include <gpgme.h>

#include <fcntl.h>
#include <fstream>

namespace crypt
{

namespace
{

bool initGpgme()
{
    // Initialise gpgme with default version
    gpgme_check_version( NULL );

    // Check opengpg protocol is avaliable
    gpgme_error_t err = gpgme_engine_check_version( GPGME_PROTOCOL_OPENPGP );

    if ( err )
    {
        InputManager::printDebug( std::string( "Failed to initialise GPGME: " ).append( gpgme_strerror( err ) ) );

        return false;
    }

    return true;
}

gpgme_ctx_t getContext()
{
    gpgme_ctx_t ctx;
    gpgme_error_t err = gpgme_new( &ctx );

    if ( err )
    {
        InputManager::printDebug( std::string( "Failed to create GPGME context: " ).append( gpgme_strerror( err ) ) );
        return nullptr;
    }

    return ctx;
}

bool setSymmetricEncrypt( gpgme_ctx_t ctx )
{
    gpgme_error_t err = gpgme_ctx_set_engine_info( ctx, GPGME_PROTOCOL_OPENPGP, NULL, NULL );

    if ( err )
    {
        InputManager::printDebug( std::string( "Failed to set symmetric encryption: " ).append( gpgme_strerror( err ) ) );
        return false;
    }

    gpgme_set_armor( ctx, 1 );
    return true;
}

gpgme_data_t createDataObject( const std::string& data )
{
    gpgme_data_t gpgmeData;
    gpgme_error_t err = gpgme_data_new_from_mem( &gpgmeData, data.c_str(), data.size(), 0 );

    if ( err )
    {
        InputManager::printDebug( std::string( "Failed to create GPGME data object: " ).append( gpgme_strerror( err ) ) );
        return nullptr;
    }

    return gpgmeData;
}

gpgme_data_t createDataObject( const std::filesystem::path& path )
{
    gpgme_data_t gpgmeData;
    FILE* file = fopen( path.string().c_str(), "rb" );

    if ( file == nullptr )
    {
        InputManager::printDebug( std::string( "Failed to open file: " ).append( path.string() ) );
        return nullptr;
    }

    gpgme_error_t err = gpgme_data_new_from_stream( &gpgmeData, file );
    if ( err )
    {
        InputManager::printDebug( std::string( "Failed to create GPGME data object: " ).append( gpgme_strerror( err ) ) );
        return nullptr;
    }

    return gpgmeData;
}

gpgme_data_t createEmptyDataObject()
{
    gpgme_data_t gpgmeData;
    gpgme_error_t err = gpgme_data_new( &gpgmeData );

    if ( err )
    {
        InputManager::printDebug( std::string( "Failed to create GPGME data object: " ).append( gpgme_strerror( err ) ) );
        return nullptr;
    }

    return gpgmeData;
}

bool encrypt( gpgme_ctx_t ctx, gpgme_data_t plaintext, gpgme_data_t ciphertext )
{
    gpgme_error_t err = gpgme_op_encrypt( ctx, NULL, GPGME_ENCRYPT_ALWAYS_TRUST, plaintext, ciphertext );

    if ( err )
    {
        InputManager::printDebug( std::string( "Failed to encrypt data: " ).append( gpgme_strerror( err ) ) );
        return false;
    }

    return true;
}

bool decrypt( gpgme_ctx_t ctx, gpgme_data_t ciphertext, gpgme_data_t plaintext )
{
    gpgme_error_t err = gpgme_op_decrypt( ctx, ciphertext, plaintext );

    if ( err )
    {
        InputManager::printDebug( std::string( "Failed to decrypt data: " ).append( gpgme_strerror( err ) ) );
        return false;
    }

    return true;
}

bool writeCiphertextToFile( gpgme_data_t ciphertext, const std::filesystem::path& path )
{
    gpgme_error_t err = gpgme_data_seek( ciphertext, 0, SEEK_SET );

    if ( err )
    {
        InputManager::printDebug( std::string( "Failed to seek to beginning of ciphertext: " ).append( gpgme_strerror( err ) ) );
        return false;
    }

    std::ofstream file( path );
    if ( !file.is_open() )
    {
        InputManager::printDebug( std::string( "Failed to open file for writing: " ).append( path.string() ) );
        return false;
    }

    char buffer[1024];
    size_t bytesRead = 0;
    while ( ( bytesRead = gpgme_data_read( ciphertext, buffer, sizeof( buffer ) ) ) > 0 )
    {
        file.write( buffer, bytesRead );
    }

    if ( file.bad() )
    {
        InputManager::printDebug( std::string( "Failed to write ciphertext to file: " ).append( path.string() ) );
        return false;
    }

    return true;
}

void cleanup( gpgme_ctx_t ctx, gpgme_data_t plaintext, gpgme_data_t ciphertext )
{
    gpgme_data_release( plaintext );
    gpgme_data_release( ciphertext );
    gpgme_release( ctx );
}

} // anonymous namespace

bool encryptDataToFile( const std::string& data, const std::filesystem::path& path )
{
    if ( !initGpgme() )
    {
        return false;
    }

    gpgme_ctx_t ctx = getContext();
    if ( ctx == nullptr )
    {
        return false;
    }

    if ( !setSymmetricEncrypt( ctx ) )
    {
        return false;
    }

    gpgme_data_t plaintext = createDataObject( data );
    if ( plaintext == nullptr )
    {
        return false;
    }

    gpgme_data_t ciphertext = createEmptyDataObject();
    if ( ciphertext == nullptr )
    {
        return false;
    }

    if ( !encrypt( ctx, plaintext, ciphertext ) )
    {
        return false;
    }

    if ( !writeCiphertextToFile( ciphertext, path ) )
    {
        return false;
    }

    cleanup( ctx, plaintext, ciphertext );

    return true;
}

std::optional< std::string > decryptDataFromFile( const std::filesystem::path& path )
{
    if ( !initGpgme() )
    {
        return std::nullopt;
    }

    gpgme_ctx_t ctx = getContext();
    if ( ctx == nullptr )
    {
        return std::nullopt;
    }

    if ( !setSymmetricEncrypt( ctx ) )
    {
        return std::nullopt;
    }

    gpgme_data_t ciphertext = createDataObject( path );
    if ( ciphertext == nullptr )
    {
        return std::nullopt;
    }

    gpgme_data_t plaintext = createEmptyDataObject();
    if ( plaintext == nullptr )
    {
        return std::nullopt;
    }

    if ( !decrypt( ctx, ciphertext, plaintext ) )
    {
        return std::nullopt;
    }

    gpgme_error_t err = gpgme_data_seek( plaintext, 0, SEEK_SET );

    if ( err )
    {
        InputManager::printDebug( std::string( "Failed to seek to beginning of plaintext: " ).append( gpgme_strerror( err ) ) );
        return std::nullopt;
    }

    char bufferDecrypted[1024];
    std::string decryptedData;
    ssize_t bytesRead;

    while ( ( bytesRead = gpgme_data_read( plaintext, bufferDecrypted, sizeof( bufferDecrypted ) ) ) > 0 )
    {
        decryptedData.append( bufferDecrypted, bytesRead );
    }

    cleanup( ctx, plaintext, ciphertext );

    return decryptedData;
}

} //namespace crypt
