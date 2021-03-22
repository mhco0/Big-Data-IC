#include "quadtree.h"
using namespace std;


namespace qsbd {
    template<class T>
    int quadtree<T>::direction(const quadtree<T>::node& head, const point<double>& pos){
        point<double> center = head.coord;

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

    template<class T>
    quadtree<T>::node::node(){
        this->childs[0] = nullptr;
        this->childs[1] = nullptr;
        this->childs[2] = nullptr;
        this->childs[3] = nullptr;
    }

    template<class T>
    void quadtree<T>::search_region(const quadtree<T>::node& head, aabb tree_region, const aabb& region_to_search, vector<T>& ret){
        auto reg_bounds = tree_region.bounds();
        double left = reg_bounds.first.x();
        double right = reg_bounds.second.x();
        double bottom = reg_bounds.first.y();
        double top = reg_bounds.second.y();
        double x_center = head.coord.x();
        double y_center = head.coord.y();

        // some error hear, region is used like region search but we divide all tree

        if(region_to_search.contains(head.coord)){
            ret.push_back(head.key);
        }

        if (head.childs[0] != nullptr){
            aabb sub_region(x_center, y_center, right, top);

            if (region_to_search.intersects(sub_region)){
                search_region(*head.childs[0], sub_region, region_to_search, ret);
            }
        }

        if(head.childs[1] != nullptr){
            aabb sub_region(left, y_center, x_center, top);

            if (region_to_search.intersects(sub_region)){
                search_region(*head.childs[1], sub_region, region_to_search, ret);
            }
        }

        if(head.childs[2] != nullptr){
            aabb sub_region(left, bottom, x_center, y_center);

            if (region_to_search.intersects(sub_region)){
                search_region(*head.childs[2], sub_region, region_to_search, ret);
            }
        }

        if (head.childs[3] != nullptr){
            aabb sub_region(x_center, bottom, right, y_center);

            if (region_to_search.intersects(sub_region)){
                search_region(*head.childs[3], sub_region, region_to_search, ret);
            }
        }
    }

    template<class T>
    void quadtree<T>::delete_tree(quadtree<T>::node* head){
        for(int i = 0; i < 4; i++){
            if (head->childs[i] != nullptr){
                delete_tree(head->childs[i]);
            }
        }

        delete head;
    }


    template<class T>
    quadtree<T>::node::node(const point<double>& pos, T value) : quadtree<T>::node(){
        this->key = value;
        this->coord = pos;
    }

    template<class T>
    typename quadtree<T>::node* quadtree<T>::node::create(const point<double>& pos, T value){
        quadtree<T>::node * nnode = new quadtree<T>::node(pos, value);

        return nnode;    
    }

    template<class T>
    quadtree<T>::quadtree(){
        tree_bounds.bounds(numeric_limits<double>::infinity(), numeric_limits<double>::infinity(), -numeric_limits<double>::infinity(), -numeric_limits<double>::infinity());
        this->root = nullptr;
    }

    template<class T>
    quadtree<T>::~quadtree(){ 
        this->delete_tree(this->root);
    } 


    template<class T>
    bool quadtree<T>::insert(const point<double>& pos, T value){
        if (root == nullptr){
            this->root = quadtree<T>::node::create(pos, value);
            
            return true;
        }

        auto last_bounds = tree_bounds.bounds();
        double min_bound_x = min(last_bounds.first.x(), pos.x());
        double min_bound_y = min(last_bounds.first.y(), pos.y());
        double max_bound_x = max(last_bounds.second.x(), pos.x());
        double max_bound_y = max(last_bounds.second.y(), pos.y());

        tree_bounds.bounds(min_bound_x, min_bound_y, max_bound_x, max_bound_y);

        quadtree<T>::node * cur = this->root;

        int what_child = direction(*cur, pos);

        if (what_child == -1) return false; // point already inserted;

        while(cur->childs[what_child] != nullptr){
            cur = cur->childs[what_child];
            what_child = direction(*cur, pos);

            if (what_child == -1) return false;
        }

        cur->childs[what_child] = quadtree<T>::node::create(pos, value);

        return true;
    }

    template<class T>
    vector<T> quadtree<T>::query(const aabb& region){
        vector<T> values;

        search_region(*this->root, this->tree_bounds, region, values);

        return values;
    }

    template<class T>
    aabb quadtree<T>::bounds() const {
        return this->tree_bounds;
    }

    template<class T>
    void quadtree<T>::show_root_tree(){
        if(this->root != nullptr){
            cout << this->root->key << endl;

            for(int i = 0; i < 4; i++){
                if(this->root->childs[i] != nullptr){
                    cout << this->root->childs[i]->key << " ";
                }
            }
            cout << endl;
        }
    }


    template class quadtree<int>;
}