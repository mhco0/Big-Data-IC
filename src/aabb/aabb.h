#ifndef QSBD_AABB_H
#define QSBD_AABB_H
#include "../point/point.hpp"
#include <iostream>

namespace qsbd{

    class aabb{
    private:
        point<double> min_bound;
        point<double> max_bound;
    public:
        aabb(const double& minx, const double& miny, const double& maxx, const double& maxy);
        aabb(const aabb& other);
        aabb();
        ~aabb();
        
        aabb& operator=(const aabb& other);

        bool intersects(const aabb& rhs) const;
        
        void bounds(const double& minx, const double& miny, const double& maxx, const double& maxy);
        bool contains(const point<double>& pos) const;
        bool is_inside(const aabb& rhs) const;
        std::pair<point<double>, point<double>> bounds() const;
        friend std::ostream& operator<<(std::ostream &os, const aabb& box);
    };

}
#endif