/**
 * @file quantile_quadtree.hpp
 * @brief QSBD quantile_quadtree class
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */

#ifndef QSBD_QUANTILE_QUADTREE_H
#define QSBD_QUANTILE_QUADTREE_H
#include "../quantile_sketch/quantile_sketch.hpp"
#include "../sketch_factory/sketch_factory.hpp"
#include "../aabb/aabb.hpp"
#include "../qsbd_debug/qsbd_debug.h"


namespace qsbd {

	/** @class quantile_quadtree\<ObjType\> 
	 *  @brief A quadtree that uses quantile sketchs in it's nodes 
	 * 
	 * A quadtree is a data structure that is used to subdivide a 2D-space into for region, each region
	 * in the subdivided space can also subdivide it's space in four more regions. That can be use to make
	 * efficient queries and updates if the data has coordenates. A quantile_quadtree is a new data structure
	 * that simulates a quadtree behavier but also uses quantile_sketchs in it's nodes, that way, this structure
	 * is able to also anwer group statistics , such as the rank for a given element, the cdf for the data and 
	 * it's quantile. Remember that the quantile quadtree subdivides the space of this mode:
	 * 
	 * - If we have a space going from (0, 0) to (4, 4), The root and the first level of subdivision will be:
	 * 
	 * ```
	 * 
	 * .Lvl = 0 :
	 * (0, 4)---------------(4, 4)
	 * |						 |
	 * |						 |
	 * |						 |
	 * |						 |
	 * |						 |
	 * (0, 0)---------------(4, 0)
	 * 
	 * - The second level :
	 * 
	 * .Lvl = 1 :
	 * 
	 * (0, 4)-----(2, 4)----(4, 4)
	 * |	 NW  	|	  NE	 |
	 * |			|			 |
	 * (0, 2)-----(2, 2)----(4, 2)
	 * |	 SW		|	  SE	 |
	 * |			|			 |
	 * (0, 0)-----(2, 0)----(4, 0)
	 * 
	 * 	
	 * ```
	 * 
	 * Where NE, NW , SW and SE are nodes of the quantile quadtree and are also directions of where put the point based
	 * on it's position in space. The node in Lvl = 0 now owns all nodes in Lvl = 1, it's points to the first node (NE), 
	 * the other three nodes are stores sequencialy in the tree, we can see that in this form:
	 * 
	 * ```
	 * Lvl = 0:
	 * | root |
	 * 		|
	 *  	\ Lvl = 1
	 * 		 -> [NE, NW, SW, SE]
	 * 			  |   |   |   |
	 * 			  |	  |	  |	  \ Lvl = 2 
	 * 			  |	  |   \		-> [NE, NW, SW, SE]
	 * 			  |   \		-> [NE, NW, SW, SE]
	 * 			  \		-> [NE, NW, SW, SE] 
	 * 				-> [NE, NW, SW, SE]
	 * 				...
	 * 
	 * ```
	 * @tparam ObjType The type of the object that the quantile quadtree works with
	 * 
	 * @warning 
	 * To the quantile_quadtree anwer properly it's queries, all quantile sketchs inside this structure must be
	 * able to merge together.
	 * @warning 
	 * The quantile_quadtree only subdivides the space until it's reach in a node of size 1 x 1, if we hit this cell
	 * we can't no longer subdivide the tree
	*/
	template<class ObjType>
	class quantile_quadtree{
	private:

		/** @class quantile_quadtree\<ObjType\>\::node 
		 *  @brief A class to represent each quantile quadtree node
		 * @tparam ObjType The type of the object that the quantile quadtree works with
		 *
		*/
		class node{
		public:
			int ne_child_pos; //<! A integer to represent the position of the first child of this node.
			quantile_sketch<ObjType> * payload; //<! A quantile_sketch pointer for each node

			/**
			 * @brief A contructor for the node
			 * @param child_pos A position for the first child of this node in the tree. -1 if this node 
			 * doesn't have any child
			*/
			node(int childs_pos){
				this->payload = nullptr;
				this->ne_child_pos = childs_pos;
			}
		};

		aabb<int> boundarys; //!< The bounds for the tree
		int max_deep; //!< The maximum depth for the quantile quadtree subdivides
		bool only_leaf; //!< A flag to represents if this quadtree only contains sketchs on it's leafs
		sketch_factory<ObjType> * factory; //!< A factory to create a new sketch when needed.
		quantile_quadtree<ObjType>::node * root; //!< A node to only represents the root 
		std::vector<quantile_quadtree<ObjType>::node> tree; //!< The tree itself

		/**
		 * @brief A private quantile quadtree function, that directs where a point @p pos can go based on a certain aabb @p box
		 * 
		 * Image that we have the following AABB :
		 * 
		 * (0, 4)-----(2, 4)----(4, 4)
		 * |	 NW  	|	  NE	 |
		 * |			|			 |
		 * (0, 2)-----(2, 2)----(4, 2)
		 * |	 SW		|	  SE	 |
		 * |			|			 |
		 * (0, 0)-----(2, 0)----(4, 0)
		 *
		 * The return for any point in region NE should return 0, in NW return 1, 
		 * SW returns 2, and SE returns 3.
		 * 
		 * @param box A AAABB to represent the space that we are
		 * @param pos A point to represent where we are in space
		 * @return A short int to represent what direction the point should go.
		*/
		short direction(const aabb<int>& box, const point<int>& pos){
			int center_x = (box.bounds().first.x() + box.bounds().second.x()) / 2;
			int center_y = (box.bounds().first.y() + box.bounds().second.y()) / 2;

			if(pos.x() >= center_x and pos.y() >= center_y) return 0;
			else if(pos.x() < center_x and pos.y() >= center_y) return 1;
			else if(pos.x() < center_x and pos.y() < center_y) return 2;
			else if(pos.x() >= center_x and pos.y() < center_y) return 3;
			else return 0;
		}

		/**
		 * @brief A private quantile quadtree function, that checks if some aabb @p regions is a box 1 x 1.
		 * @param region A AABB to be tested
		 * @return true if it's is a box 1x1, false otherwise.
		*/
		bool unit_box(const aabb<int>& region){
			int width = region.bounds().second.x() - region.bounds().first.x();
			int height = region.bounds().second.y() - region.bounds().first.y();
			return not (width > 1 || height > 1);
		}

		/**
		 * @brief A private quantile quadtree function, that allocates fours new nodes on the tree.
		 * @return The position for the NE child for the new four nodes in the tree
		*/
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

		/**
		 * @brief A private quantile quadtree function that changes the bounds of the @p cur_box to the bounds of it's childs in the position @p direction
		 * 
		 * The @p direction should be a number in range [0 .. 3], imagine that we are given a @p cur_box as below:
		 * 
		 * (0, 4)-----(2, 4)----(4, 4)
		 * |	 NW  	|	  NE	 |
		 * |			|			 |
		 * (0, 2)-----(2, 2)----(4, 2)
		 * |	 SW		|	  SE	 |
		 * |			|			 |
		 * (0, 0)-----(2, 0)----(4, 0)
		 * 
		 * All four values of the @p direction means that we can go from 0 = NE to 3 = SE in anti-clockwise, so 
		 * if we give a direction equals 2, the box will be changed from [(0, 0), (4, 4)] to [(0, 0), (2, 2)].
		 * 
		 * 
		 * @param cur_box The AABB to have the bounds changed
		 * @param direction A short value in range [0 .. 3] 
		 * 
		 * @warning 
		 * The @p cur_box will be changed.
		*/
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


		/**
		 * @brief A private quantile quadtree function, that updates the quantile quadtree.
		 * @param pos The int coordenates for the object @p value
		 * @param value A object to update the sketchs on the tree
		 * @return True if we reach the maximum depth on the tree, false otherwise
		 * 
		 * @note 
		 * 	This function only is called if the sketch only supports unweighted updates and if this 
		 * quantile quadtree uses sketchs in all nodes.
		*/
		bool private_update(const point<int>& pos, ObjType value){
			aabb<int> cur_box(this->boundarys);
			int cur_deep = 1;
			int what_child = direction(this->boundarys, pos);
			int cur_pos = this->root->ne_child_pos + what_child;
			this->root->payload->update(value); // the payload exists in this case

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

		/**
		 * @brief A private quantile quadtree function, that updates the quantile quadtree.
		 * @param pos The int coordenates for the object @p value
		 * @param value A object to update the sketchs on the tree
		 * @param weight The weight for the @p value
		 * @return True if we reach the maximum depth on the tree, false otherwise
		 * 
		 * @note 
		 * 	This function only is called if the sketch only supports weighted updates and if this 
		 * quantile quadtree uses sketchs in all nodes.
		*/
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

		/**
		 * @brief A private quantile quadtree function, that updates the quantile quadtree.
		 * @param pos The int coordenates for the object @p value
		 * @param value A object to update the sketchs on the tree
		 * @return True if we reach the maximum depth on the tree, false otherwise
		 * 
		 * @note 
		 * 	This function only is called if the sketch onlys supports unweighted updates and if this 
		 * quantile quadtree uses sketchs only in the leaf nodes.
		*/
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

		/**
		 * @brief A private quantile quadtree function, that updates the quantile quadtree.
		 * @param pos The int coordenates for the object @p value
		 * @param value A object to update the sketchs on the tree
		 * @param weight The weight for the @p value
		 * @return True if we reach the maximum depth on the tree, false otherwise
		 * 
		 * @note 
		 * 	This function only is called if the sketch only supports weighted updates and if this 
		 * quantile quadtree uses sketchs only in the leaf nodes.
		*/
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

		/**
		 * @brief A private quantile quadtree function, that searchs for some quantile_sketch\<ObjType\> inside a given area
		 * @param cur_node The current node in the tree that we are, -1 means that we are in the root
		 * @param region The region that we are querying 
		 * @param deep The current depth on the recursion tree
		 * @param box The current AABB box that represents the area that we are in the recursion tree
		 * @param final_sketch A sketch that will be merged with all sketchs 
		 * @return A quantile_sketch\<ObjType\> inside @p region
		 * 
		 * @note 
		 * This method is for the quantile quadtree with sketchs in all nodes in the tree.
		*/
		quantile_sketch<ObjType> * search_region(int cur_node, const aabb<int>& region, int deep, aabb<int>& box, quantile_sketch<ObjType> *& final_sketch){
			if(box.is_inside(region) or deep == this->max_deep or unit_box(box)){
				if(cur_node == -1){
					final_sketch->inner_merge(*this->root->payload);
					return this->root->payload;
				}else return this->tree[cur_node].payload;
			}

			int ne_child_pos = (cur_node == -1) ? this->root->ne_child_pos : this->tree[cur_node].ne_child_pos;
			if(ne_child_pos == -1){
				if(cur_node == -1){
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
					final_sketch->inner_merge(*to_merge[i]);
				}
			}

			return nullptr;
		}

		/**
		 * @brief A private quantile quadtree function, that searchs for some quantile_sketch\<ObjType\> inside a given area
		 * @param cur_node The current node in the tree that we are, -1 means that we are in the root
		 * @param region The region that we are querying 
		 * @param deep The current depth on the recursion tree
		 * @param box The current AABB box that represents the area that we are in the recursion tree
		 * @param final_sketch A sketch that will be merged with all sketchs 
		 * @return A quantile_sketch\<ObjType\> inside @p region
		 * 
		 * @note 
		 * This method is for the quantile quadtree with only sketchs on the leafs.
		*/
		quantile_sketch<ObjType> * search_region_only_leaf(int cur_node, const aabb<int>& region, int deep, aabb<int>& box, quantile_sketch<ObjType> *& final_sketch){
			if(deep == this->max_deep or unit_box(box)){
				if(cur_node == -1){
					return nullptr;
				}else return this->tree[cur_node].payload;
			}

			int ne_child_pos = (cur_node == -1) ? this->root->ne_child_pos : this->tree[cur_node].ne_child_pos;
			if(ne_child_pos == -1){
				if(cur_node == -1){
					return nullptr;
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
					final_sketch->inner_merge(*to_merge[i]);
				}
			}

			return nullptr;
		}

		/**
		 * @brief A private quantile quadtree function, that delete all sketchs used by the quantile quadtree.
		*/
		void delete_tree(){
			if(this->root->payload != nullptr){
				delete this->root->payload;
			}

			delete this->root;
			
			for(int i = 0; i < this->tree.size(); i++){
				if(this->tree[i].payload != nullptr){
					delete this->tree[i].payload;
				}
			}

		}

	public:

		/**
		 * @brief A constructor for the quantile_quadtree\<ObjType\>
		 * @param region A AABB to represent the bounds for this quantile quadtree
		 * @param deep_length The maximum depth for the quantile quadtree
		 * @param fact A factory to create new sketchs of a certain class
		 * @param only_leafs A boolean value to indicate that the quantile_quadtree only use sketchs on leafs or no
		*/
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

		/**
		 * @brief It's not possible to copy-construct a quantile_quadtree by other
		*/
		quantile_quadtree(const quantile_quadtree& other) = delete;

		/**
		 * @brief It's not possible to assign a quantile_quadtree to other
		*/
		quantile_quadtree<ObjType>& operator=(const quantile_quadtree& other) = delete;
		
		/**
		 * @brief Destructs the quantile_quadtree instance
		*/
		~quantile_quadtree(){
			this->delete_tree();
		}
		
		/**
		 * @brief Updates the quantile quadtree with the object @p value with position @p pos
		 * @param pos The position for the @p value in int coordenates
		 * @param value The object to update the tree.
		 * @return true if the maximum depth was reached, false otherwise
		 * 
		 * @note 
		 * This function will only works properly if the sketch used works with unweighted objects
		*/
		bool update(const point<int>& pos, ObjType value){
			if(this->only_leaf) private_update_only_leaf(pos, value);
			else private_update(pos, value);
		}

		/**
		 * @brief Updates the quantile quadtree with the object @p value with position @p pos and weight @p weight
		 * @param pos The position for the @p value in int coordenates
		 * @param value The object to update the tree.
		 * @param weight The weight associated with @p value
		 * @return true if the maximum depth was reached, false otherwise
		 * 
		 * @note 
		 * This function will only works properly if the sketch used works with weighted objects
		*/
		bool update(const point<int>& pos, ObjType value, int weight){
			if(this->only_leaf) private_update_only_leaf(pos, value, weight);
			else private_update(pos, value, weight);
		}

		/**
		 * @brief Queries the rank of some object @p value with certain @p region 
		 * @param region A AABB region to be queried.
		 * @param value The object to be queried.
		 * @return The estimated rank for this object @p value in the @p region
		*/
		int query(const aabb<int>& region, ObjType value){
			// is we are using sketchs on all nodes and the bounds are all inside the query region 
			// we can ask this faster in the root sketch
			if(not this->only_leaf and this->boundarys.is_inside(region)){
				int ret = this->root->payload->query(value);

				return ret;
			}

			aabb<int> cur_box(this->boundarys);
			quantile_sketch<ObjType> * sketch = this->factory->instance();

			//This 0 because here we start from the root
			if(this->only_leaf) search_region_only_leaf(-1, region, 0, cur_box, sketch);
			else search_region(-1, region, 0, cur_box, sketch);

			int ret = sketch->query(value);

			delete sketch;

			return ret;
		}

		/**
		 * @brief Queries the cdf of some object @p value with certain @p region 
		 * @param region A AABB region to be queried.
		 * @param value The object to be queried.
		 * @return The estimated probability for some value be less than this object @p value in the @p region
		*/
		double cdf(const aabb<int>& region, ObjType value){
			// is we are using sketchs on all nodes and the bounds are all inside the query region 
			// we can ask this faster in the root sketch
			if(not this->only_leaf and this->boundarys.is_inside(region)){
				double ret = this->root->payload->cdf(value);

				return ret;
			}

			aabb<int> cur_box(this->boundarys);
			quantile_sketch<ObjType> * sketch = this->factory->instance();

			//This 0 because here we start from the root
			if(this->only_leaf) search_region_only_leaf(-1, region, 0, cur_box, sketch);
			else search_region(-1, region, 0, cur_box, sketch);

			double ret = sketch->cdf(value);

			delete sketch;

			return ret;
		}

		/**
		 * @brief Queries the cdfs of some objects @p value with certain @p region 
		 * @param region A AABB region to be queried.
		 * @param value The object to be queried.
		 * @return The estimated probability for some value be less than this object @p value in the @p region
		*/
		std::vector<double> cdfs(const aabb<int>& region, const std::vector<ObjType>& values){
			std::vector<double> rets;
			rets.reserve(values.size());

			// is we are using sketchs on all nodes and the bounds are all inside the query region 
			// we can ask this faster in the root sketch
			if(not this->only_leaf and this->boundarys.is_inside(region)){
				for(auto& value : values){
					rets.emplace_back(this->root->payload->cdf(value));
				}

				return rets;
			}

			aabb<int> cur_box(this->boundarys);
			quantile_sketch<ObjType> * sketch = this->factory->instance();

			//This 0 because here we start from the root
			if(this->only_leaf) search_region_only_leaf(-1, region, 0, cur_box, sketch);
			else search_region(-1, region, 0, cur_box, sketch);

			for(auto& value : values){
				rets.emplace_back(sketch->cdf(value));
			}

			delete sketch;

			return rets;
		}

		/**
		 * @brief Queries the quantiles of some @p region 
		 * @param region A AABB region to be queried.
		 * @param quants A vector of each quantile to be queried.
		 * @return The estimated quantiles @p quants in the @p region
		*/
		std::vector<ObjType> quantiles(const aabb<int>& region, const std::vector<double>& quants){
			std::vector<ObjType> rets;
			rets.reserve(quants.size());
			
			// is we are using sketchs on all nodes and the bounds are all inside the query region 
			// we can ask this faster in the root sketch
			if(not this->only_leaf and this->boundarys.is_inside(region)){
				for(auto& it : quants){
					rets.emplace_back(this->root->payload->quantile(it));
				}

				return rets;
			}

			aabb<int> cur_box(this->boundarys);
			quantile_sketch<ObjType> * sketch = this->factory->instance();

			//This 0 because here we start from the root
			if(this->only_leaf) search_region_only_leaf(-1, region, 0, cur_box, sketch);
			else search_region(-1, region, 0, cur_box, sketch);

			for(auto& it : quants){
				rets.emplace_back(sketch->quantile(it));
			}

			delete sketch;

			return rets;
		}

		/**
		 * @brief Getter for the estimated amount of heap memory allocated for this object for a pointer.
		 * @return The estimated memory in bytes.
		*/
		uint64_t get_heap_size() {
			uint64_t qq_hs = sizeof(quantile_quadtree<ObjType>);
			uint64_t root_hs = sizeof(quantile_quadtree<ObjType>::node);
			uint64_t tree_hs = sizeof(quantile_quadtree<ObjType>::node) * this->tree.capacity();

			if(this->root->payload != nullptr){
				root_hs += this->root->payload->get_heap_size();
			}
			
			for(int i = 0; i < this->tree.size(); i++){
				if(this->tree[i].payload != nullptr){
					tree_hs += this->tree[i].payload->get_heap_size();
				}
			}

			return qq_hs + root_hs + tree_hs;
		}
	};
}
#endif
