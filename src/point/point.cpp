#include "point.hpp"
using namespace std;

template<class T>
point<T>::point(T x, T y) : coord{x, y}{
}

template<class T>
point<T>::point(){
}

template<class T>
point<T>::~point(){
}

template<class T>
void point<T>::x(const T& val){
    this->coord.first = val;
}

template<class T>
void point<T>::y(const T& val){
    this->coord.second = val;
}

template<class T>
void point<T>::xy(const T& xval, const T& yval){
    this->coord.first = xval;
    this->coord.second = yval;
}

template<class T>
pair<T, T> point<T>::xy() const {
    return this->coord;
}

template<class T>
T point<T>::x() const {
    return this->coord.first;
}

template<class T>
T point<T>::y() const {
    return this->coord.second;
}

template class point<int>;
template class point<double>;
template class point<float>;