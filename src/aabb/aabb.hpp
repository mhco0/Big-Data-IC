#ifndef QSBD_AABB_H
#define QSBD_AABB_H
#include "../point/point.hpp"
#include <iostream>

namespace qsbd{

    template<class T>
    class aabb{
    private:
        point<T> min_bound;
        point<T> max_bound;
    public:
        aabb(const T& minx, const T& miny, const T& maxx, const T& maxy);
        aabb(const aabb<T>& other);
        aabb();
        ~aabb();
        
        aabb<T>& operator=(const aabb<T>& other);

        bool intersects(const aabb<T>& rhs) const;
        
        void bounds(const T& minx, const T& miny, const T& maxx, const T& maxy);
        bool contains(const point<T>& pos) const;
        bool is_inside(const aabb<T>& rhs) const;
        std::pair<point<T>, point<T>> bounds() const;
        
        template<class U>
        friend std::ostream& operator<<(std::ostream& os, const aabb<U>& box);
    };

}
#endif