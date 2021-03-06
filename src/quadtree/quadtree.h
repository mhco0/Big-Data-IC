#ifndef QSBD_QUADTREE_H
#define QSBD_QUADTREE_H
#include "../aabb/aabb.hpp"

namespace qsbd {
	template<class T>
	class quadtree{
	private:
		class node{
		public:
			point<double> coord;
			T key;

			quadtree<T>::node* childs[4];

			node();
			node(const point<double>& pos, T value);

			static typename quadtree<T>::node* create(const point<double>& pos, T value);
		};

		aabb<double> tree_bounds;
		

		int direction(const quadtree<T>::node& head, const point<double>& pos);
		void search_region(const quadtree<T>::node& head, aabb<double> tree_region, const aabb<double>& region_to_search, std::vector<T>& ret);
		void delete_tree(quadtree<T>::node* head);
	public:

		quadtree<T>::node* root;
		quadtree();
		~quadtree();

		quadtree(const quadtree<T>& other) = delete;
		quadtree<T>& operator=(const quadtree& other) = delete;
		bool insert(const point<double>& pos, T value);
		std::vector<T> query(const aabb<double>& region);

		aabb<double> bounds() const;

		void show_root_tree();
	};
}
#endif