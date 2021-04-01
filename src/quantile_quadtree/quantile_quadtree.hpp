#ifndef QSBD_QUANTILE_QUADTREE_H
#define QSBD_QUANTILE_QUADTREE_H
#include "../quantile_sketch/quantile_sketch.hpp"
#include "../sketch_factory/sketch_factory.hpp"
#include "../aabb/aabb.h"
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

        aabb boundarys;
        int max_deep;
        sketch_factory<ObjType> * factory;
        quantile_quadtree<ObjType>::node * root;
        std::vector<quantile_quadtree<ObjType>::node> tree;

        short direction(const aabb& box, const point<int>& pos){
            point<int> center((box.bounds().first.x() + box.bounds().second.x()) / 2, (box.bounds().first.y() + box.bounds().second.y()) / 2);

            if(pos.x() > center.x() and pos.y() > center.y()) return 0;
            else if(pos.x() < center.x() and pos.y() > center.y()) return 1;
            else if(pos.x() < center.x() and pos.y() < center.y()) return 2;
            else if(pos.x() > center.x() and pos.y() < center.y()) return 3;
            else if(pos.x() == center.x() and pos.y() > center.y()) return 0;
            else if(pos.x() < center.x() and pos.y() == center.y()) return 1;
            else if(pos.x() == center.x() and pos.y() < center.y()) return 2;
            else if(pos.x() > center.x() and pos.y() == center.y()) return 3;
            else return 0;
        }

        bool unit_box(const aabb& region){
            point<int> dimension(region.bounds().second.x() - region.bounds().first.x(), region.bounds().second.y() - region.bounds().first.y());
            return not (dimension.x() > 1 || dimension.y() > 1);
        }

        int alloc_childs(){
            int ne_child_pos = this->tree.size();

            // ne
            this->tree.push_back(quantile_quadtree<ObjType>::node(-1));
            // nw
            this->tree.push_back(quantile_quadtree<ObjType>::node(-1));
            // sw
            this->tree.push_back(quantile_quadtree<ObjType>::node(-1));
            // se
            this->tree.push_back(quantile_quadtree<ObjType>::node(-1));
 
            return ne_child_pos;
        }

        aabb change_box(const aabb& cur_box, short direction){
            aabb dummy(0, 0, 0, 0);
            point<int> center((cur_box.bounds().second.x() + cur_box.bounds().first.x()) / 2, (cur_box.bounds().second.y() + cur_box.bounds().first.y()) / 2);

            switch (direction){
                case 0 : {
                    aabb bound_ne(center.x(), center.y(), cur_box.bounds().second.x(), cur_box.bounds().second.y());
                    return bound_ne;
                }
                break;
                case 1 : {
                    aabb bound_nw(cur_box.bounds().first.x(), center.y(), center.x(), cur_box.bounds().second.y());
                    return bound_nw;
                }
                break;
                case 2 : {
                    aabb bound_sw(cur_box.bounds().first.x(), cur_box.bounds().first.y(), center.x(), center.y());
                    return bound_sw;
                }
                break;
                case 3 : {
                    aabb bound_se(center.x(), cur_box.bounds().first.y(), cur_box.bounds().second.x(), center.y());
                    return bound_se;
                }
                break;
                default:{
                    DEBUG_ERR("Invalid direction on quantile_quadtree::change_box");
                    return dummy;
                }
                break;
            }

            return dummy;
        }


        quantile_sketch<ObjType> * search_region(int cur_node, const aabb& region, int deep, aabb& box){
            if(cur_node == -1){
                if (this->boundarys.is_inside(region) or deep == this->max_deep or unit_box(box)){
                    return this->root->payload;
                }
            }else{
                if(box.is_inside(region) or deep == this->max_deep or unit_box(box)){
                    return this->tree[cur_node].payload;
                }
            }

            int ne_child_pos = (cur_node == -1) ? this->root->ne_child_pos : this->tree[cur_node].ne_child_pos;
            if(ne_child_pos == -1){
                if(cur_node == -1) return this->root->payload;
                else return this->tree[cur_node].payload;
            }

            quantile_sketch<ObjType> * to_merge[4] = {nullptr, nullptr, nullptr, nullptr};

            for(int i = 0; i < 4; i++){
                int cur_pos = ne_child_pos + i;
                aabb child_box = change_box(box, i);
                if (region.intersects(child_box)){
                    aabb parent_box(box);
                    box = change_box(box, i);
                    to_merge[i] = search_region(cur_pos, region, deep + 1, box);
                    box = parent_box;
                }
            }

            quantile_sketch<ObjType> * merged = to_merge[0];

            for(int i = 1; i < 4; i++){
                if(merged == nullptr and to_merge[i] != nullptr){
                    merged = to_merge[i];
                }else if(merged != nullptr and to_merge[i] != nullptr){
                    merged = merged->merge(*to_merge[i]);
                }
            }

            return merged;
        }

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
        quantile_quadtree(const aabb& region, const int& deep_length, sketch_factory<ObjType>* fact){
            assert(fact != nullptr);
            
            this->boundarys = region;
            this->max_deep = deep_length;
            this->factory = fact;
            this->root = new quantile_quadtree<ObjType>::node(0);
            this->root->payload = this->factory->instance();
            this->alloc_childs();
        }

        quantile_quadtree(const quantile_quadtree& other) = delete;

        quantile_quadtree<ObjType>& operator=(const quantile_quadtree& other) = delete;

        ~quantile_quadtree(){
            this->delete_tree();
        }

        bool update(const point<int>& pos, const ObjType& value){
            aabb cur_box(this->boundarys);
            int cur_deep = 0;
            int what_child = direction(this->boundarys, pos);
            int cur_pos = this->root->ne_child_pos + what_child;
            this->root->payload->update(value);

            while(cur_deep <= this->max_deep and not unit_box(cur_box)){
                if(this->tree[cur_pos].ne_child_pos == -1){ // leaf
                    //needs to create four new nodes

                    int ne_child_pos = alloc_childs();

                    this->tree[cur_pos].ne_child_pos = ne_child_pos;
                    this->tree[cur_pos].payload = this->factory->instance();
                }

                this->tree[cur_pos].payload->update(value);
                
                cur_box = change_box(cur_box, what_child);
                what_child = direction(cur_box, pos);
                cur_pos = this->tree[cur_pos].ne_child_pos + what_child;
                cur_deep++;
            }

            if(cur_deep == this->max_deep) return true;
            else return false;
        }

        bool update(const point<int>& pos, const ObjType& value, int weight){
            aabb cur_box(this->boundarys);
            int cur_deep = 0;
            int what_child = direction(this->boundarys, pos);
            int cur_pos = this->root->ne_child_pos + what_child;
            this->root->payload->update(value);

            while(cur_deep <= this->max_deep and not unit_box(cur_box)){
                if(this->tree[cur_pos].ne_child_pos == -1){ // leaf
                    //needs to create four new nodes

                    int ne_child_pos = alloc_childs();

                    this->tree[cur_pos].ne_child_pos = ne_child_pos;
                    this->tree[cur_pos].payload = this->factory->instance();
                }

                this->tree[cur_pos].payload->update(value, weight);
                
                cur_box = change_box(cur_box, what_child);
                what_child = direction(cur_box, pos);
                cur_pos = this->tree[cur_pos].ne_child_pos + what_child;
                cur_deep++;
            }

            if(cur_deep == this->max_deep) return true;
            else return false;
        }

        int query(const aabb& region, const ObjType& value){
            aabb cur_box(this->boundarys);
            quantile_sketch<ObjType> * sketch = search_region(-1, region, 0, cur_box);

            if(sketch == nullptr) return -1;
            else{
                return sketch->query(value);
            }
        }
    };
}
#endif