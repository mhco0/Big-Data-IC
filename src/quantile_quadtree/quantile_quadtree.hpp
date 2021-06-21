#ifndef QSBD_QUANTILE_QUADTREE_H
#define QSBD_QUANTILE_QUADTREE_H
#include "../quantile_sketch/quantile_sketch.hpp"
#include "../sketch_factory/sketch_factory.hpp"
#include "../aabb/aabb.hpp"
#include "../qsbd_debug/qsbd_debug.h"

namespace qsbd {
	template<class ObjType>
	class quantile_quadtree{
	private:
		class node{
		public:
			int ne_child_pos;
			quantile_sketch<ObjType> * payload;

			node(int childs_pos){
				this->payload = nullptr;
				this->ne_child_pos = childs_pos;
			}
		};

		aabb<int> boundarys;
		int max_deep;
		int inmergs;
		bool only_leaf;
		sketch_factory<ObjType> * factory;
		quantile_quadtree<ObjType>::node * root;
		std::vector<quantile_quadtree<ObjType>::node> tree;

		short direction(const aabb<int>& box, const point<int>& pos){
			int center_x = (box.bounds().first.x() + box.bounds().second.x()) / 2;
			int center_y = (box.bounds().first.y() + box.bounds().second.y()) / 2;

			if(pos.x() >= center_x and pos.y() >= center_y) return 0;
			else if(pos.x() < center_x and pos.y() >= center_y) return 1;
			else if(pos.x() < center_x and pos.y() < center_y) return 2;
			else if(pos.x() >= center_x and pos.y() < center_y) return 3;
			else return 0;
		}

		bool unit_box(const aabb<int>& region){
			int width = region.bounds().second.x() - region.bounds().first.x();
			int height = region.bounds().second.y() - region.bounds().first.y();
			return not (width > 1 || height > 1);
		}

		int alloc_childs(){
			int ne_child_pos = this->tree.size();
			// ne
			this->tree.emplace_back(-1);
			// nw
			this->tree.emplace_back(-1);
			// sw
			this->tree.emplace_back(-1);
			// se
			this->tree.emplace_back(-1);
 
			return ne_child_pos;
		}

		void change_box(aabb<int>& cur_box, short direction){
			int center_x = (cur_box.bounds().second.x() + cur_box.bounds().first.x()) / 2;
			int center_y = (cur_box.bounds().second.y() + cur_box.bounds().first.y()) / 2;

			switch (direction){
				case 0 : {
					// ne
					cur_box.bounds(center_x, center_y, cur_box.bounds().second.x(), cur_box.bounds().second.y());
				}
				break;
				case 1 : {
					// nw
					cur_box.bounds(cur_box.bounds().first.x(), center_y, center_x, cur_box.bounds().second.y());
				}
				break;
				case 2 : {
					// sw
					cur_box.bounds(cur_box.bounds().first.x(), cur_box.bounds().first.y(), center_x, center_y);
				}
				break;
				case 3 : {
					// se
					cur_box.bounds(center_x, cur_box.bounds().first.y(), cur_box.bounds().second.x(), center_y);
				}
				break;
				default:{
					DEBUG_ERR("Invalid direction on quantile_quadtree::change_box");
				}
				break;
			}
		}

		bool private_update(const point<int>& pos, ObjType value){
			aabb<int> cur_box(this->boundarys);
			int cur_deep = 1;
			int what_child = direction(this->boundarys, pos);
			int cur_pos = this->root->ne_child_pos + what_child;
			this->root->payload->update(value);

			while(cur_deep <= this->max_deep and not unit_box(cur_box)){
				if(this->tree[cur_pos].ne_child_pos == -1 and this->tree[cur_pos].payload == nullptr){ // leaf
					//needs to create four new nodes
					int ne_child_pos = (cur_deep == this->max_deep) ? -1 : alloc_childs();

					this->tree[cur_pos].ne_child_pos = ne_child_pos;
					this->tree[cur_pos].payload = this->factory->instance();
				}

				this->tree[cur_pos].payload->update(value);
				
				change_box(cur_box, what_child);
				what_child = direction(cur_box, pos);
				cur_pos = this->tree[cur_pos].ne_child_pos + what_child;
				cur_deep++;
			}

			if(cur_deep == this->max_deep) return true;
			else return false;
		}

		bool private_update(const point<int>& pos, ObjType value, int weight){
			aabb<int> cur_box(this->boundarys);
			int cur_deep = 1;
			int what_child = direction(this->boundarys, pos);
			int cur_pos = this->root->ne_child_pos + what_child;
			this->root->payload->update(value, weight);

			while(cur_deep <= this->max_deep and not unit_box(cur_box)){
				if(this->tree[cur_pos].ne_child_pos == -1 and this->tree[cur_pos].payload == nullptr){ // leaf
					//needs to create four new nodes
					int ne_child_pos = (cur_deep == this->max_deep) ? -1 : alloc_childs();

					this->tree[cur_pos].ne_child_pos = ne_child_pos;
					this->tree[cur_pos].payload = this->factory->instance();
				}

				this->tree[cur_pos].payload->update(value, weight);

				change_box(cur_box, what_child);
				what_child = direction(cur_box, pos);
				cur_pos = this->tree[cur_pos].ne_child_pos + what_child;
				cur_deep++;
			}
			
			if(cur_deep == this->max_deep) return true;
			else return false;
		}

		bool private_update_only_leaf(const point<int>& pos, ObjType value){
			aabb<int> cur_box(this->boundarys);
			int cur_deep = 1;
			int what_child = direction(this->boundarys, pos);
			int cur_pos = this->root->ne_child_pos + what_child;

			while(cur_deep <= this->max_deep and not unit_box(cur_box)){
				if(this->tree[cur_pos].ne_child_pos == -1 and this->tree[cur_pos].payload == nullptr){ // leaf
					//needs to create four new nodes
					int ne_child_pos = (cur_deep == this->max_deep) ? -1 : alloc_childs();

					this->tree[cur_pos].ne_child_pos = ne_child_pos;
					if(cur_deep == this->max_deep) this->tree[cur_pos].payload = this->factory->instance();
				}

				if(cur_deep == this->max_deep) this->tree[cur_pos].payload->update(value);
				
				change_box(cur_box, what_child);
				what_child = direction(cur_box, pos);
				cur_pos = this->tree[cur_pos].ne_child_pos + what_child;
				cur_deep++;
			}

			if(cur_deep == this->max_deep) return true;
			else return false;
		}

		bool private_update_only_leaf(const point<int>& pos, ObjType value, int weight){
			aabb<int> cur_box(this->boundarys);
			int cur_deep = 1;
			int what_child = direction(this->boundarys, pos);
			int cur_pos = this->root->ne_child_pos + what_child;

			while(cur_deep <= this->max_deep and not unit_box(cur_box)){
				if(this->tree[cur_pos].ne_child_pos == -1 and this->tree[cur_pos].payload == nullptr){ // leaf
					//needs to create four new nodes
					int ne_child_pos = (cur_deep == this->max_deep) ? -1 : alloc_childs();

					this->tree[cur_pos].ne_child_pos = ne_child_pos;
					if(cur_deep == this->max_deep) this->tree[cur_pos].payload = this->factory->instance();
				}

				if(cur_deep == this->max_deep) this->tree[cur_pos].payload->update(value, weight);

				change_box(cur_box, what_child);
				what_child = direction(cur_box, pos);
				cur_pos = this->tree[cur_pos].ne_child_pos + what_child;
				cur_deep++;
			}
			
			if(cur_deep == this->max_deep) return true;
			else return false;
		}

		quantile_sketch<ObjType> * search_region(int cur_node, const aabb<int>& region, int deep, aabb<int>& box, quantile_sketch<ObjType> *& final_sketch){
			if(box.is_inside(region) or deep == this->max_deep or unit_box(box)){
				if(cur_node == -1){
					this->inmergs++;
					final_sketch->inner_merge(*this->root->payload);
					return this->root->payload;
				}else return this->tree[cur_node].payload;
			}

			int ne_child_pos = (cur_node == -1) ? this->root->ne_child_pos : this->tree[cur_node].ne_child_pos;
			if(ne_child_pos == -1){
				if(cur_node == -1){
					this->inmergs++;
					final_sketch->inner_merge(*this->root->payload);
					return this->root->payload;
				}else return this->tree[cur_node].payload;
			}

			quantile_sketch<ObjType> * to_merge[4] = {nullptr, nullptr, nullptr, nullptr};

			for(int i = 0; i < 4; i++){
				int cur_pos = ne_child_pos + i;
				aabb<int> child_box(box);
				change_box(child_box, i);
				if (region.intersects(child_box)){
					to_merge[i] = search_region(cur_pos, region, deep + 1, child_box, final_sketch);
				}
			}

			for(int i = 0; i < 4; i++){
				if(to_merge[i] != nullptr){
					this->inmergs++;
					final_sketch->inner_merge(*to_merge[i]);
				}
			}

			return nullptr;
		}

		quantile_sketch<ObjType> * search_region_only_leaf(int cur_node, const aabb<int>& region, int deep, aabb<int>& box, quantile_sketch<ObjType> *& final_sketch){
			if(deep == this->max_deep or unit_box(box)){
				if(cur_node == -1){
					this->inmergs++;
					final_sketch->inner_merge(*this->root->payload);
					return this->root->payload;
				}else return this->tree[cur_node].payload;
			}

			int ne_child_pos = (cur_node == -1) ? this->root->ne_child_pos : this->tree[cur_node].ne_child_pos;
			if(ne_child_pos == -1){
				if(cur_node == -1){
					this->inmergs++;
					final_sketch->inner_merge(*this->root->payload);
					return this->root->payload;
				}else return this->tree[cur_node].payload;
			}

			quantile_sketch<ObjType> * to_merge[4] = {nullptr, nullptr, nullptr, nullptr};

			for(int i = 0; i < 4; i++){
				int cur_pos = ne_child_pos + i;
				aabb<int> child_box(box);
				change_box(child_box, i);
				if (region.intersects(child_box)){
					to_merge[i] = search_region_only_leaf(cur_pos, region, deep + 1, child_box, final_sketch);
				}
			}

			for(int i = 0; i < 4; i++){
				if(to_merge[i] != nullptr){
					this->inmergs++;
					final_sketch->inner_merge(*to_merge[i]);
				}
			}

			return nullptr;
		}

		void delete_tree(){
			if(this->root->payload != nullptr){
				delete this->root->payload;
			}

			delete this->root;
			
			int count = 0;

			for(int i = 0; i < this->tree.size(); i++){
				if(this->tree[i].payload != nullptr){
					count++;
					delete this->tree[i].payload;
				}
			}

			VDEBUG(count);
		}

	public:
		quantile_quadtree(const aabb<int>& region, const int& deep_length, sketch_factory<ObjType>* fact, bool only_leafs = false){
			ASSERT(fact != nullptr);
			
			this->boundarys = region;
			this->max_deep = deep_length;
			this->only_leaf = only_leafs;
			this->factory = fact;
			this->root = new quantile_quadtree<ObjType>::node(0);
			if(not this->only_leaf) this->root->payload = this->factory->instance();
			this->alloc_childs();
		}

		quantile_quadtree(const quantile_quadtree& other) = delete;

		quantile_quadtree<ObjType>& operator=(const quantile_quadtree& other) = delete;

		~quantile_quadtree(){
			this->delete_tree();
		}

		bool update(const point<int>& pos, ObjType value){
			if(this->only_leaf) private_update_only_leaf(pos, value);
			else private_update(pos, value);
		}

		bool update(const point<int>& pos, ObjType value, int weight){
			if(this->only_leaf) private_update_only_leaf(pos, value, weight);
			else private_update(pos, value, weight);
		}

		int query(const aabb<int>& region, ObjType value){
			aabb<int> cur_box(this->boundarys);
			quantile_sketch<ObjType> * sketch = this->factory->instance();
			
			this->inmergs = 0;
			//This 0 because here we start from the root
			if(this->only_leaf) search_region_only_leaf(-1, region, 0, cur_box, sketch);
			else search_region(-1, region, 0, cur_box, sketch);

			int ret = sketch->query(value);

			VDEBUG(this->inmergs);
			delete sketch;

			return ret;
		}
	};
}
#endif
