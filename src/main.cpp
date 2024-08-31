#include "InputManager.hpp"
#include "PassliManager.hpp"

int main( int argc, char* argv[] )
{
    try
    {
        PassliManager passliManager( argc, argv );

        if ( !passliManager.setup() )
        {
            InputManager::printDebug( "Failed during setup." );
            return 1;
        }

        if ( !passliManager.run() )
        {
            InputManager::printDebug( "Failed during execution." );
            return 1;
        }
    }
    catch ( const std::exception& e )
    {
        InputManager::printDebug( std::string( "Exception caught: " ).append( e.what() ) );
        return 1;
    }
    catch ( ... )
    {
        InputManager::printDebug( "Unknown exception caught." );
        return 1;
    }

    return 0;
}
