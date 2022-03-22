/**
 * @file aabb.hpp
 * @brief QSBD aabb class
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */
#ifndef QSBD_AABB_H
#define QSBD_AABB_H
#include "../point/point.hpp"
#include <iostream>

namespace qsbd{

	/** @class aabb\<T\>
	 *  @tparam T The class to work with, sould be int, double or float
	 *  @brief A class to represent a AABB(Axis-Aligned Bounding Box) in a 2D space. 
	*/
	template<class T>
	class aabb{
	private:
		point<T> min_bound;
		point<T> max_bound;
	public:

		/**
		 * @brief Constructs the aabb\<T\> class using @p minx and @p miny as the (minx, miny) coordenates and 
		 * uses @p maxx and @p maxy as the (maxx, maxy) coordenates
		 * @param minx The min x coordenate
		 * @param miny The min y coordenate
		 * @param maxx The max x coordenate
		 * @param maxy The max y coordenate
		*/
		aabb(const T& minx, const T& miny, const T& maxx, const T& maxy);

		/**
		 * @brief Constructs the aabb\<T\> class another aabb\<T\s> class @p other
		 * @param other A aabb\<T\> instance
		*/
		aabb(const aabb<T>& other);

		/**
		 * @brief Constructs a point class with nothing
		 * @note The (minx, miny) and (maxx, maxy) coordenates of the box will be initialized with memory trash.
		*/
		aabb();

		/**
		 * @brief Destructs the aabb\<T\> class
		*/
		~aabb();
		
		/**
		 * @brief Assign the aabb\<T\> instance using another aabb\<T\> @p other
		 * @param other A instance of a aabb\<T\>
		*/
		aabb<T>& operator=(const aabb<T>& other);

		/**
		 * @brief Tests if this aabb instance intersects with another aabb @p rhs
		 * @param rhs A instance of a aabb\<T\>
		 * @return True if the two aabb intersects with each other and false otherwise
		 * 
		 * @note 
		 *  AABB's just touching each other are considered as non-intersecting.
		*/
		bool intersects(const aabb<T>& rhs) const;
		
		/**
		 * @brief Sets the new bounds for the aabb\<T\> class using @p minx and @p miny as the (minx, miny) coordenates and 
		 * uses @p maxx and @p maxy as the (maxx, maxy) coordenates
		 * @param minx The new min x coordenate
		 * @param miny The new min y coordenate
		 * @param maxx The new max x coordenate
		 * @param maxy The new max y coordenate
		*/
		void bounds(const T& minx, const T& miny, const T& maxx, const T& maxy);

		/**
		 * @brief Tests if this aabb instance contains the point\<T\> @p pos
		 * @param pos A instance of a point\<T\>
		 * @return True if this aabb contains the point @p pos and false otherwise
		*/
		bool contains(const point<T>& pos) const;

		/**
		 * @brief Tests if this aabb is inside another aabb @p rhs 
		 * @param rhs The aabb to test
		 * @return True if this aabb is inside @p rhs and false otherwise 
		*/
		bool is_inside(const aabb<T>& rhs) const;

		/**
		 * @brief Gets the bounds for this aabb
		 * @return a std::pair\<point\<T\>, point\<T\>\>, where the `first` value is the (minx, miny) point and  `second` 
		 * is the (maxx, maxy) point
		*/
		std::pair<point<T>, point<T>> bounds() const;
		
		/**
		 * @brief A function to ouput aabb\<U\>
		 * @tparam U The class to work with, sould be int, double or float
		 * @param os The output stream
		 * @param box The aabb to be outputed
		 * @return The same as @p os
		*/
		template<class U>
		friend std::ostream& operator<<(std::ostream& os, const aabb<U>& box);
	};
	
	/**
	 * @brief A function to ouput aabb\<T\>
	 * @tparam T The class to work with, sould be int, double or float
	 * @param os The output stream
	 * @param box The aabb to be outputed
	 * @return The same as @p os
	*/
	template<class T>
	std::ostream& operator<<(std::ostream& os, const aabb<T>& box) { 
		return os << "[Bottom Left : (" << box.min_bound.x() << ", " << box.min_bound.y() << "), Top Right : (" << box.max_bound.x() << ", " << box.max_bound.y() << ")]";
	}
}
#endif