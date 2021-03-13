#ifndef QUANTILE_QUADTREE_H
#define QUANTILE_QUADTREE_H
#include "../quantile_sketch/quantile_sketch.hpp"
#include "../sketch_factory/sketch_factory.hpp"
#include "../aabb/aabb.h"

template<class ObjType>
class quantile_quadtree{
private:
    class node{
    public:
        aabb box;

        quantile_sketch<ObjType> * payload;
        quantile_quadtree<ObjType>::node * childs[4];

        node(const aabb& bound){
            this->box = bound;
            this->payload = nullptr;

            for(int i = 0; i < 4; i++){
                this->childs[i] = nullptr;
            }
        }
    };

    aabb boundarys;
    int max_deep;
    sketch_factory<ObjType> * factory;
    quantile_quadtree<ObjType>::node * root;

    int direction(const quantile_quadtree<ObjType>::node& root, const point<double>& pos){
        aabb box = root.box;
        point<double> center((box.bounds().first.x() + box.bounds().second.x()) / 2.0, (box.bounds().first.y() + box.bounds().second.y()) / 2.0);

        if(pos.x() > center.x() and pos.y() > center.y()) return 0;
        else if(pos.x() < center.x() and pos.y() > center.y()) return 1;
        else if(pos.x() < center.x() and pos.y() < center.y()) return 2;
        else if(pos.x() > center.x() and pos.y() < center.y()) return 3;
        else if(fabs(pos.x() - center.x()) <= 1e-6 and pos.y() > center.y()) return 0;
        else if(fabs(pos.x() - center.x()) <= 1e-6 and pos.y() < center.y()) return 1;
        else if(pos.x() < center.x() and fabs(pos.y() - center.y()) <= 1e-6) return 2;
        else if(pos.x() > center.x() and fabs(pos.y() - center.y()) <= 1e-6) return 3;
        else return -1;
    }

    quantile_sketch<ObjType> * search_region(const quantile_quadtree<ObjType>::node& root, const aabb& region, int deep){
        if(deep == this->max_deep or root.box.is_inside(region)){ // leaf 
           return root.payload;
        }

        quantile_sketch<ObjType> * to_merge[4] = {nullptr, nullptr, nullptr, nullptr};

        for(int i = 0; i < 4; i++){
            if (root.childs[i] != nullptr){
                if (region.intersects(root.childs[i]->box)){
                    to_merge[i] = search_region(*root.childs[i], region, deep + 1);
                }
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

    void delete_tree(quantile_quadtree<ObjType>::node* root){
        for(int i = 0; i < 4; i++){
            if (root->childs[i] != nullptr){
                delete_tree(root->childs[i]);
            }
        }

        if(root->payload != nullptr){
            delete root->payload;
        }

        delete root;
    }

public:
    

    quantile_quadtree(const aabb& region, const int& deep_length, sketch_factory<ObjType>* fact){
        this->boundarys = region;
        this->max_deep = deep_length;
        this->factory = fact;
        this->root = new quantile_quadtree<ObjType>::node(this->boundarys);
        this->root->payload = this->factory->instance();
    }

    quantile_quadtree(const quantile_quadtree& other) = delete;

    quantile_quadtree<ObjType>& operator=(const quantile_quadtree& other) = delete;

    ~quantile_quadtree(){
        delete_tree(this->root);
    }

    bool update(const point<double>& pos, const ObjType& value){
        quantile_quadtree<ObjType>::node * cur = this->root;
        int cur_deep = 0;
        int what_child = direction(*cur, pos);

        if (what_child == -1) return false;

        cur->payload->update(value);

        while(cur_deep <= this->max_deep){ // leaf case
            if(cur->childs[what_child] == nullptr){
                //needs to create a new node

                point<double> center((cur->box.bounds().second.x() + cur->box.bounds().first.x()) / 2.0, (cur->box.bounds().second.y() + cur->box.bounds().first.y()) / 2.0);
                aabb bound_region;
                switch (what_child) {
                    case 0:
                        bound_region.bounds(center.x(), center.y(), cur->box.bounds().second.x(), cur->box.bounds().second.y());
                        break;
                    case 1:
                        bound_region.bounds(cur->box.bounds().first.x(), center.y(), center.x(), cur->box.bounds().second.y());
                        break;
                    case 2:
                        bound_region.bounds(cur->box.bounds().first.x(), cur->box.bounds().first.y(), center.x(), center.y());
                        break;
                    case 3:
                        bound_region.bounds(center.x(), cur->box.bounds().first.y(), cur->box.bounds().second.x(), center.y());
                        break;
                    default:
                        break;
                }

                cur->childs[what_child] = new quantile_quadtree<ObjType>::node(bound_region);
                cur->childs[what_child]->payload = this->factory->instance();
            }

            cur->childs[what_child]->payload->update(value);
            cur = cur->childs[what_child];
            cur_deep++;

            what_child = direction(*cur, pos);
            if (what_child == -1) return false;
        }

        return true;
    }

    bool update(const point<double>& pos, const ObjType& value, int weight){
        quantile_quadtree< ObjType>::node * cur = this->root;
        int cur_deep = 0;
        int what_child = direction(*cur, pos);

        if (what_child == -1) return false;

        cur->payload->update(value, weight);

        while(cur_deep <= this->max_deep){ // leaf case
            if(cur->childs[what_child] == nullptr){
                //needs to create a new node

                point<double> center((cur->box.bounds().second.x() + cur->box.bounds().first.x()) / 2.0, (cur->box.bounds().second.y() + cur->box.bounds().first.y()) / 2.0);
                aabb bound_region;
                switch (what_child) {
                    case 0:
                        bound_region.bounds(center.x(), center.y(), cur->box.bounds().second.x(), cur->box.bounds().second.y());
                        break;
                    case 1:
                        bound_region.bounds(cur->box.bounds().first.x(), center.y(), center.x(), cur->box.bounds().second.y());
                        break;
                    case 2:
                        bound_region.bounds(cur->box.bounds().first.x(), cur->box.bounds().first.y(), center.x(), center.y());
                        break;
                    case 3:
                        bound_region.bounds(center.x(), cur->box.bounds().first.y(), cur->box.bounds().second.x(), center.y());
                        break;
                    default:
                        break;
                }

                cur->childs[what_child] = new quantile_quadtree<ObjType>::node(bound_region);
                cur->childs[what_child]->payload = this->factory->instance();
            }

            cur->childs[what_child]->payload->update(value, weight);
            cur = cur->childs[what_child];
            cur_deep++;

            what_child = direction(*cur, pos);
            if (what_child == -1) return false;
        }

        return true;
    }

    int query(const aabb& region, const ObjType& value){
        quantile_sketch<ObjType> * sketch = search_region(*this->root, region, 0);

        if(sketch == nullptr) return -1;
        else{
            return sketch->query(value);
        }
    }
};

#endif