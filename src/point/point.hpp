/**
 * @file point.hpp
 * @brief QSBD point class
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */

#ifndef QSBD_POINT_H
#define QSBD_POINT_H
#include "../commum_header/commum_header.h"

namespace qsbd{

	/** @class point\<T\>
	 *  @tparam T The class to work with, sould be int, double or float
	 *  @brief A class to represent a point in a 2D space. 
	*/

	template<class T>
	class point{
	private:
		std::pair<T, T> coord;
	public:
		/**
		 * @brief Constructs the point\<T\> class using @p x and @p y as coordenates
		 * @param x The x coordenate
		 * @param y The y coordenate
		*/
		point(const T& x, const T& y);

		/**
		 * @brief Constructs the point\<T\> class using another point @p other
		 * @param other A instance of a point
		*/
		point(const point<T>& other);

		/**
		 * @brief Constructs a point class with nothing
		 * @note The x and y coordenates of the point will be initialized with memory trash.
		*/
		point();

		/**
		 * @brief Destructs the point class
		*/
		~point();

		/**
		 * @brief Assign the point instance using another point @p other
		 * @param other A instance of a point
		*/
		point<T>& operator=(const point<T>& other);

		/**
		 * @brief Sets the point x to the value of @p val
		 * @param val The value to assign x 
		*/
		void x(const T& val);

		/**
		 * @brief Sets the point y to the value of @p val
		 * @param val The value to assign y 
		*/
		void y(const T& val);

		/**
		 * @brief Sets the point x to the value of @p xval and the point y to the value of @p yval
		 * @param xval The value to assign x
		 * @param yval The value to assign y
		*/
		void xy(const T& xval, const T& yval);

		/**
		 * @brief Gets the point x 
		 * @return The value of the x coordenate
		*/
		T x() const;

		/**
		 * @brief Gets the point y 
		 * @return The value of the y coordenate 
		*/
		T y() const;

		/**
		 * @brief Getter for the x and y coordenates
		 * @return A std::pair<T, T> with the `first` being the x coordenate and `second` being the y coordenate
		*/
		std::pair<T, T> xy() const;
		
	};
}

#endif