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
            aabb box;
            int ne_child_pos;

            quantile_sketch<ObjType> * payload;

            node(const aabb& bound, int childs_pos){
                this->box = bound;
                this->payload = nullptr;
                this->ne_child_pos = childs_pos;
            }
        };

        aabb boundarys;
        int max_deep;
        sketch_factory<ObjType> * factory;
        quantile_quadtree<ObjType>::node * root;
        std::vector<quantile_quadtree<ObjType>::node> tree;

        short direction(const quantile_quadtree<ObjType>::node& root, const point<int>& pos){
            aabb box = root.box;
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

        int alloc_childs(const aabb& region){
            point<int> center((region.bounds().second.x() + region.bounds().first.x()) / 2, (region.bounds().second.y() + region.bounds().first.y()) / 2);

            int ne_child_pos = this->tree.size();

            // ne
            aabb bound_region_ne(center.x(), center.y(), region.bounds().second.x(), region.bounds().second.y());
            this->tree.push_back(quantile_quadtree<ObjType>::node(bound_region_ne, -1));

            // nw
            aabb bound_region_nw(region.bounds().first.x(), center.y(), center.x(), region.bounds().second.y());

            this->tree.push_back(quantile_quadtree<ObjType>::node(bound_region_nw, -1));

            // sw
            aabb bound_region_sw(region.bounds().first.x(), region.bounds().first.y(), center.x(), center.y());
            this->tree.push_back(quantile_quadtree<ObjType>::node(bound_region_sw, -1));

            // se
            aabb bound_region_se(center.x(), region.bounds().first.y(), region.bounds().second.x(), center.y());
            this->tree.push_back(quantile_quadtree<ObjType>::node(bound_region_se, -1));
 
            return ne_child_pos;
        }

        quantile_sketch<ObjType> * search_region(int parent, const aabb& region, int deep){
            if(parent == -1){
                if (this->root->box.is_inside(region) or deep == this->max_deep or unit_box(this->tree[parent].box)){
                    return this->root->payload;
                }
            }else{
                if(this->tree[parent].box.is_inside(region) or deep == this->max_deep or unit_box(this->tree[parent].box)){
                    return this->tree[parent].payload;
                }
            }

            int ne_child_pos = (parent == -1) ? this->root->ne_child_pos : this->tree[parent].ne_child_pos;

            quantile_sketch<ObjType> * to_merge[4] = {nullptr, nullptr, nullptr, nullptr};

            for(int i = 0; i < 4; i++){
                int cur_pos = ne_child_pos + i;
                if (region.intersects(this->tree[cur_pos].box)){
                    to_merge[i] = search_region(cur_pos, region, deep + 1);
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
            ASSERT((fact != nullptr));
            
            this->boundarys = region;
            this->max_deep = deep_length;
            this->factory = fact;
            this->root = new quantile_quadtree<ObjType>::node(region, 0);
            this->root->payload = this->factory->instance();
            this->alloc_childs(region);
        }

        quantile_quadtree(const quantile_quadtree& other) = delete;

        quantile_quadtree<ObjType>& operator=(const quantile_quadtree& other) = delete;

        ~quantile_quadtree(){
            this->delete_tree();
        }

        bool update(const point<int>& pos, const ObjType& value){
            int what_child = direction(*this->root, pos);

            this->root->payload->update(value);
            int cur_deep = 0;
            int cur_pos = this->root->ne_child_pos + what_child;

            while(cur_deep <= this->max_deep and not unit_box(this->tree[cur_pos].box)){
                if(this->tree[cur_pos].ne_child_pos == -1){ // leaf
                    //needs to create four new nodes

                    int ne_child_pos = alloc_childs(this->tree[cur_pos].box);

                    this->tree[cur_pos].ne_child_pos = ne_child_pos;
                    this->tree[cur_pos].payload = this->factory->instance();
                }

                this->tree[cur_pos].payload->update(value);
                
                what_child = direction(this->tree[cur_pos], pos);
                cur_pos = this->tree[cur_pos].ne_child_pos + what_child;
                cur_deep++;
            }

            return true;
        }

        bool update(const point<int>& pos, const ObjType& value, int weight){
            int what_child = direction(*this->root, pos);

            this->root->payload->update(value, weight);
            int cur_deep = 0;
            int cur_pos = this->root->ne_child_pos + what_child;

            while(cur_deep <= this->max_deep and not unit_box(this->tree[cur_pos].box)){ 
                if(this->tree[cur_pos].ne_child_pos == -1){ // leaf
                    //needs to create four new nodes

                    int ne_child_pos = alloc_childs(this->tree[cur_pos].box);

                    this->tree[cur_pos].ne_child_pos = ne_child_pos;
                    this->tree[cur_pos].payload = this->factory->instance();
                }

                this->tree[cur_pos].payload->update(value, weight);
                
                what_child = direction(this->tree[cur_pos], pos);
                cur_pos = this->tree[cur_pos].ne_child_pos + what_child;
                cur_deep++;
            }

            return true;
        }

        int query(const aabb& region, const ObjType& value){
            quantile_sketch<ObjType> * sketch = search_region(-1, region, 0);

            if(sketch == nullptr) return -1;
            else{
                return sketch->query(value);
            }
        }
    };
}
#endif