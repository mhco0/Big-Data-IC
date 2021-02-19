#ifndef QUANTILE_QUADTREE_H
#define QUANTILE_QUADTREE_H
#include "../quantile_sketch/quantile_sketch.hpp"
#include "../count_sketch/count_sketch.h"
#include "../aabb/aabb.h"
#include <iostream>

template<class SketchType , class ObjType>
class quantile_quadtree{
private:
    class node{
    public:
        aabb box;

        quantile_sketch<ObjType> * payload;
        quantile_quadtree<SketchType, ObjType>::node * childs[4];

        node(const aabb& bound){
            this->box = bound;
            this->payload = nullptr;

            for(int i = 0; i < 4; i++){
                this->childs[i] = nullptr;
            }
        }
    };

    aabb boundarys;
    aabb leaf_sz;
    quantile_quadtree<SketchType, ObjType>::node * root;

    int direction(const quantile_quadtree<SketchType, ObjType>::node& root, const point<double>& pos){
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

    template<class ...Args>
    void construct(quantile_quadtree<SketchType, ObjType>::node& root, const aabb& leaf_size, Args... args){
        double min_x_sz = fabs(leaf_size.bounds().second.x() - leaf_size.bounds().first.x());
        double min_y_sz = fabs(leaf_size.bounds().second.y() - leaf_size.bounds().first.y());
        double node_x = fabs(root.box.bounds().second.x() - root.box.bounds().first.x()); 
        double node_y = fabs(root.box.bounds().second.y() - root.box.bounds().first.y());

        if(node_x <= min_x_sz and node_y <= min_y_sz){
            root.payload = new SketchType((args)...);
            return;
        }

        point<double> center((root.box.bounds().second.x() + root.box.bounds().first.x()) / 2.0, (root.box.bounds().second.y() + root.box.bounds().first.y()) / 2.0);

        aabb northeast(center.x(), center.y(), root.box.bounds().second.x(), root.box.bounds().second.y());
        root.childs[0] = new quantile_quadtree<SketchType, ObjType>::node(northeast);
        construct(*root.childs[0], leaf_size, (args)...);

        aabb northwest(root.box.bounds().first.x(), center.y(), center.x(), root.box.bounds().second.y());
        root.childs[1] = new quantile_quadtree<SketchType, ObjType>::node(northwest);
        construct(*root.childs[1], leaf_size, (args)...);

        aabb southwest(root.box.bounds().first.x(), root.box.bounds().first.y(), center.x(), center.y());
        root.childs[2] = new quantile_quadtree<SketchType, ObjType>::node(southwest);
        construct(*root.childs[2], leaf_size, (args)...);

        aabb southeast(center.x(), root.box.bounds().first.y(), root.box.bounds().second.x(), center.y());
        root.childs[3] = new quantile_quadtree<SketchType, ObjType>::node(southeast);
        construct(*root.childs[3], leaf_size, (args)...);

        return;
    }

    quantile_sketch<ObjType> * search_region(const quantile_quadtree<SketchType, ObjType>::node& root, const aabb& region){
        double min_x_sz = fabs(this->leaf_sz.bounds().second.x() - this->leaf_sz.bounds().first.x());
        double min_y_sz = fabs(this->leaf_sz.bounds().second.y() - this->leaf_sz.bounds().first.y());
        double node_x = fabs(root.box.bounds().second.x() - root.box.bounds().first.x()); 
        double node_y = fabs(root.box.bounds().second.y() - root.box.bounds().first.y());

        if(node_x <= min_x_sz and node_y <= min_y_sz){ // leaf 
           return root.payload;
        }

        quantile_sketch<ObjType> * to_merge[4] = {nullptr, nullptr, nullptr, nullptr};


        for(int i = 0; i < 4; i++){
            if (root.childs[i] != nullptr){
                if (region.intersects(root.childs[i]->box)){
                    to_merge[i] = search_region(*root.childs[i], region);
                }
            }
        }

        quantile_sketch<ObjType> * merge_north = nullptr;
        quantile_sketch<ObjType> * merge_south = nullptr;
        quantile_sketch<ObjType> * merged = nullptr;

        if(to_merge[0] != nullptr or to_merge[1] != nullptr){
            if(to_merge[0] != nullptr and to_merge[1] != nullptr){
                merge_north = to_merge[0]->merge(*to_merge[1]);
            }else if(to_merge[0] != nullptr){
                merge_north = to_merge[0];
            }else{
                merge_north = to_merge[1];
            }
        }

        if(to_merge[2] != nullptr or to_merge[3] != nullptr){
            if(to_merge[2] != nullptr and to_merge[3] != nullptr){
                merge_south = to_merge[2]->merge(*to_merge[3]);
            }else if(to_merge[2] != nullptr){
                merge_south = to_merge[2];
            }else{
                merge_south = to_merge[3];
            }
        }

        if(merge_north != nullptr or merge_south != nullptr){
            if(merge_north != nullptr and merge_south != nullptr){
                merged = merge_north->merge(*merge_south);
            }else if(merge_north != nullptr){
                merged = merge_north;
            }else{
                merged = merge_south;
            }
        }

        return merged;
    }

    void delete_tree(quantile_quadtree<SketchType, ObjType>::node* root){
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
    

    // IDEA : transformar o leaf_size na profundidade maxima
    // IDEA : mudar os parametros variaveis para o padrão da classe abstrata
    template<class ...Args>
    quantile_quadtree(const aabb& region, const aabb& leaf_size, Args... args){
        this->boundarys = region;
        this->leaf_sz = leaf_size;
        this->root = new quantile_quadtree<SketchType, ObjType>::node(this->boundarys);
        this->construct(*this->root, this->leaf_sz, (args)...);
    }

    ~quantile_quadtree(){
        delete_tree(this->root);
    }

    bool update(const point<double>& pos, const ObjType& value){
        quantile_quadtree<SketchType, ObjType>::node * cur = this->root;

        int what_child = direction(*cur, pos);

        if (what_child == -1) return false;

        while(cur->childs[what_child] != nullptr){ // leaf case
            cur = cur->childs[what_child];
            what_child = direction(*cur, pos);

            if (what_child == -1) return false;
        }

        cur->payload->update(value);

        return true;
    }

    bool update(const point<double>& pos, const ObjType& value, int weight){
        quantile_quadtree<SketchType, ObjType>::node * cur = this->root;

        int what_child = direction(*cur, pos);

        if (what_child == -1) return false;

        while(cur->childs[what_child] != nullptr){ // leaf case
            cur = cur->childs[what_child];
            what_child = direction(*cur, pos);

            if (what_child == -1) return false;
        }

        cur->payload->update(value, weight);

        return true;
    }

    int query(const aabb& region, const ObjType& value){
        quantile_sketch<ObjType> * sketch = search_region(*this->root, region);

        if(sketch == nullptr) return -1;
        else{
            return sketch->query(value);
        }
    }
};

#endif