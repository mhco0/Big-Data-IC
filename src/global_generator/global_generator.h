/**
 * @file global_generator.h
 * @brief QSBD generator of random numbers global
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */

#ifndef QSBD_GLOBAL_GEN_H
#define QSBD_GLOBAL_GEN_H
#include <chrono>
#include <random>

namespace qsbd{
	/** @var unsigned seed 
	 * @brief The global seed used by the global generator 
	 * @sa qsbd::generator
	*/
	extern unsigned seed; 

	/** @var std::default_random_engine generator
	 * @brief The global generator responsable for creating the random numbers
	*/
	extern std::default_random_engine generator;
}

#endif