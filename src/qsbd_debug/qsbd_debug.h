/**
 * @file qsbd_debug.h
 * @brief QSBD debug helpers
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */

#ifndef QSBD_DEBUG_H
#define QSBD_DEBUG_H
#include <iostream>

namespace qsbd{
	namespace debug{
		#ifndef DISABLE_DEBUG

			/** @var bool debug_enable
			 * @brief A variable flag thats defines if all debug macros in this file will be printed
			 * @note
			 * 	To turn this flag off just compile with -DDISABLE_DEBUG=1
			*/
			const bool debug_enable = true;
		#else
			const bool debug_enable = false;
		#endif
	}
}

/**
 * @def DEBUG(x)
 * @brief A debug macro that outputs the value of @p x in the std::cout 
*/
#define DEBUG(x) do {if(qsbd::debug::debug_enable) std::cout << x << std::endl;} while(0)

/**
 * @def VDEBUG(x)
 * @brief A debug macro that outputs @p x and the value of @p x in the std::cout
*/
#define VDEBUG(x) do {if(qsbd::debug::debug_enable) std::cout << #x << " := " << x << std::endl;} while(0)

/**
 * @def DEBUG_ERR(x)
 * @brief A debug macro that outputs the value of @p x in the std::cerr
*/
#define DEBUG_ERR(x) do {if(qsbd::debug::debug_enable) std::cerr << x << std::endl;} while(0)

/**
 * @def ASSERT(cond)
 * @brief If the condition @p cond is false this debug macro will output in the error console (std::cerr) the condition, the file and the line when it was called
*/
#define ASSERT(cond) do {if(qsbd::debug::debug_enable){if(not (cond)){\
						DEBUG_ERR("===============================");\
						DEBUG_ERR("Assertion: ");\
						DEBUG_ERR(#cond);\
						DEBUG_ERR("Failed On: ");\
						DEBUG_ERR(__FILE__);\
						DEBUG_ERR("In line: ");\
						DEBUG_ERR(__LINE__);\
						DEBUG_ERR("===============================");\
						}}} while(0)

#endif