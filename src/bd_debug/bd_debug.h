#ifndef BD_DEBUG_H
#define BD_DEBUG_H
#include <iostream>

namespace bddebug{
    #ifndef DISABLE_DEBUG
        const bool debug_enable = true;
    #else
        const bool debug_enable = false;
    #endif
}

#define DEBUG(x) do { if(bddebug::debug_enable) std::cout << x << std::endl;} while(0)
#define VDEBUG(x) do { if(bddebug::debug_enable) std::cout << #x << " := " << x << std::endl;} while(0)
#define DEBUG_ERR(x) do{ if(bddebug::debug_enable) std::cerr << x << std::endl;} while(0)

#endif