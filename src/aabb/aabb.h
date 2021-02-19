#ifndef AABB_H
#define AABB_H
#include "../point/point.hpp"
#include <iostream>

class aabb{
private:
    point<double> min_bound;
    point<double> max_bound;
public:
    aabb(double minx, double miny, double maxx, double maxy);
    aabb();
    ~aabb();
    
    bool intersects(aabb rhs) const;
    
    void bounds(const double& minx, const double& miny, const double& maxx, const double& maxy);
    bool contains(const point<double>& pos) const;
    std::pair<point<double>, point<double>> bounds() const;
    friend std::ostream& operator<<(std::ostream &os, const aabb& box);
};

#endif