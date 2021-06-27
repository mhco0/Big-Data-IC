/**
 * @file timer.h
 * @brief QSBD timer class
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */

#ifndef QSBD_TIMER_H
#define QSBD_TIMER_H
#include <chrono>

namespace qsbd{

	/** @class timer
	 *  @brief A class to represente a simple timer
	 * 
	*/
	class timer{
		std::chrono::high_resolution_clock::time_point start_point, end_point;
	public:

		/**
		 * @brief The constructor for the timer.
		 *
		*/
		timer();

		/**
		 * @brief The destructor for the timer.
		*/
		~timer();

		/**
		 * @brief Sets the start point for the timer.
		*/
		void start();

		/**
		 * @brief Sets the end point for the timer.
		*/
		void end();

		/**
		 * @brief Counts the time passed between the start and the end point of the timer.
		 * @return The time interval passed between timer::end() and  timer::start() in seconds. 
		*/
		double count();
	};
}

#endif