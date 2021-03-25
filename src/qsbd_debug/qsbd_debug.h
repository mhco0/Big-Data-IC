#ifndef QSBD_DEBUG_H
#define QSBD_DEBUG_H
#include <iostream>

namespace qsbd{
    namespace debug{
        #ifndef DISABLE_DEBUG
            const bool debug_enable = true;
        #else
            const bool debug_enable = false;
        #endif
    }

#define DEBUG(x) do { if(qsbd::debug::debug_enable) std::cout << x << std::endl;} while(0)
#define VDEBUG(x) do { if(qsbd::debug::debug_enable) std::cout << #x << " := " << x << std::endl;} while(0)
#define DEBUG_ERR(x) do{ if(qsbd::debug::debug_enable) std::cerr << x << std::endl;} while(0)
#define ASSERT(cond) do {if(qsbd::debug::debug_enable){if(not (cond)){\
                        DEBUG_ERR("Assertion Fail On :");\
                        DEBUG_ERR(__FILE__);\
                        DEBUG_ERR("In line :");\
                        DEBUG_ERR(__LINE__);\
                        }}} while(0)

}


#endif