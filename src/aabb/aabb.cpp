#include "aabb.hpp"
using namespace std;

namespace qsbd {

	template<class T>
	aabb<T>::aabb(const T& minx, const T& miny, const T& maxx, const T& maxy){
		this->min_bound.xy(minx, miny);
		this->max_bound.xy(maxx, maxy);
	}

	template<class T>
	aabb<T>::aabb(const aabb<T>& other) : min_bound(other.min_bound), max_bound(other.max_bound){
	}

	template<class T>
	aabb<T>::aabb(){
	}

	template<class T>
	aabb<T>::~aabb(){
	}

	template<class T>
	aabb<T>& aabb<T>::operator=(const aabb<T>& other){
		this->min_bound = other.min_bound;
		this->max_bound = other.max_bound;

		return (*this);
	}

	template<class T>
	bool aabb<T>::contains(const point<T>& pos) const{
		return (pos.x() >= min_bound.x() and pos.x() < max_bound.x() and pos.y() >= min_bound.y() and pos.y() < max_bound.y());
	}

	template<class T>
	bool aabb<T>::intersects(const aabb<T>& rhs) const{
		auto bounds = rhs.bounds();

		T left = bounds.first.x();
		T right = bounds.second.x();
		T bottom = bounds.first.y();
		T top = bounds.second.y();

		return (left < this->max_bound.x()) and (right > this->min_bound.x()) \
			and (bottom < this->max_bound.y()) and (top > this->min_bound.y());
	}

	template<class T>
	bool aabb<T>::is_inside(const aabb<T>& rhs) const{
		return (rhs.min_bound.x() <= this->min_bound.x()) and (rhs.max_bound.x() >= this->max_bound.x()) \
				and (rhs.min_bound.y() <= this->min_bound.y()) and (rhs.max_bound.y() >= this->max_bound.y());
	}

	template<class T>
	void aabb<T>::bounds(const T& minx, const T& miny, const T& maxx, const T& maxy){
		this->min_bound.xy(minx, miny);
		this->max_bound.xy(maxx, maxy);
	}

	template<class T>
	pair<point<T>, point<T>> aabb<T>::bounds() const {
		return {this->min_bound, this->max_bound};
	}

	template class aabb<int>;
	template class aabb<double>;
	template class aabb<float>;

}