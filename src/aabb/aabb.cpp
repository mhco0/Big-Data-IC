#include "aabb.h"
using namespace std;

aabb::aabb(double minx, double miny, double maxx, double maxy){
    this->min_bound.xy(minx, miny);
    this->max_bound.xy(maxx, maxy);
}

aabb::aabb(){
}

aabb::~aabb(){
}

bool aabb::contains(const point<double>& pos) const{
    return (pos.x() >= min_bound.x() and pos.x() <= max_bound.x() and pos.y() >= min_bound.y() and pos.y() <= max_bound.y());
}

bool aabb::intersects(aabb rhs) const{
    auto bounds = rhs.bounds();

    double left = bounds.first.x();
    double right = bounds.second.x();
    double bottom = bounds.first.y();
    double top = bounds.second.y();

    //cout << "left x my_right" << " [ "<< left << " <= " << this->max_bound.x() <<" ] " << ((left <= this->max_bound.x())? "true" : "false") << endl;
    //cout << "right x my_left" << " [ "<< right << " >= " << this->min_bound.x() <<" ] " << ((right >= this->min_bound.x())? "true" : "false") << endl;
    //cout << "bottom x my_top" << " [ "<< bottom << " <= " << this->max_bound.y() <<" ] " << ((bottom <= this->max_bound.y())? "true" : "false") << endl;
    //cout << "top x my_bottom" << " [ "<< top << " >= " << this->min_bound.y() <<" ] " << ((top >= this->min_bound.y())? "true" : "false") << endl;

    return (left <= this->max_bound.x()) and (right >= this->min_bound.x()) \
        and (bottom <= this->max_bound.y()) and (top >= this->min_bound.y());
}

bool aabb::is_inside(const aabb& rhs) const{
    return (rhs.min_bound.x() <= this->min_bound.x()) and (rhs.max_bound.x() >= this->max_bound.x()) \
            and (rhs.min_bound.y() <= this->min_bound.y()) and (rhs.max_bound.y() >= this->max_bound.y());
}

void aabb::bounds(const double& minx, const double& miny, const double& maxx, const double& maxy){
    this->min_bound.xy(minx, miny);
    this->max_bound.xy(maxx, maxy);
}

pair<point<double>, point<double>> aabb::bounds() const {
    return {this->min_bound, this->max_bound};
}

ostream& operator<<(ostream &os, const aabb& box) { 
    return os << "[Bottom Left : (" << box.min_bound.x() << ", " << box.min_bound.y() << "), Top Right : (" << box.max_bound.x() << ", " << box.max_bound.y() << ")]";
}