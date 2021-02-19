#ifndef POINT_H
#define POINT_H
#include "../commum_header/commum_header.h"

template<class T>
class point{
private:
    std::pair<T, T> coord;
public:
    point(T x, T y);
    point();
    ~point();

    void x(const T& val);
    void y(const T& val);
    void xy(const T& xval, const T& yval);

    T x() const;
    T y() const;
    std::pair<T, T> xy() const;
    
};

#endif