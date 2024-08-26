#include "PassliManager.hpp"

#include <iostream>

int main( int argc, char *argv[] )
{
    try
    {

        PassliManager passliManager( argc, argv );
        if ( !passliManager.setup() )
        {
            std::cerr << "Failed during setup." << std::endl;
            return 1;
        }

        if ( !passliManager.run() )
        {
            std::cerr << "Failed during execution." << std::endl;
            return 1;
        }
    }
    catch ( const std::exception& e )
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 1;
    }
    catch ( ... )
    {
        std::cerr << "Unknown exception caught." << std::endl;
        return 1;
    }

    return 0;
}
