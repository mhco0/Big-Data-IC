#include "q_digest.h"
using namespace std;

namespace qsbd {
    void q_digest::node::private_print(node* rt, int indent){
        if(rt){
            if(rt->right) private_print(rt->right, indent + 4);
            if (indent) cout << setw(indent) << ' ';
            if (rt->right) cout << " /\n" << setw(indent) << ' ';

            cout<< rt->weight << "\n ";

            if(rt->left) {
                cout << setw(indent) << ' ' <<" \\\n";
                private_print(rt->left, indent + 4);
            }
        }
    }

    void q_digest::node::private_sub_tree_weights_print(q_digest::node* rt, int indent){
        if(rt){
            if(rt->right) private_sub_tree_weights_print(rt->right, indent + 4);
            if (indent) cout << setw(indent) << ' ';
            if (rt->right) cout<<" /\n" << setw(indent) << ' ';

            cout<< sub_tree_weight_from(rt) << "\n ";

            if(rt->left) {
                cout << setw(indent) << ' ' <<" \\\n";
                private_sub_tree_weights_print(rt->left, indent + 4);
            }
        }
    }

    int q_digest::node::sub_tree_weight_from(q_digest::node * rt){
        if(rt == nullptr) return 0;
        else return rt->weight + sub_tree_weight_from(rt->left) + sub_tree_weight_from(rt->right);
    }

    void q_digest::node::delete_tree(q_digest::node * rt){
        if(not rt) return;

        delete_tree(rt->left);
        delete_tree(rt->right);

        delete rt;
        return;
    }

    void q_digest::node::set_subtree_weight_to_zero(q_digest::node * rt){
        if(not rt) return;

        set_subtree_weight_to_zero(rt->left);
        rt->weight = 0;
        set_subtree_weight_to_zero(rt->right);

        return;
    }

    q_digest::node::node(int weight){
        this->weight = weight;
        this->left = nullptr;
        this->right = nullptr;
    }

    q_digest::node::~node(){
    }

    bool q_digest::node::is_leaf(){
        if(this->left or this->right) return false;
        else return true;
    }

    void q_digest::node::print(int indent){
        private_print(this, indent);
    }

    void q_digest::node::print_subtree_weights(){
        private_sub_tree_weights_print(this, 0);
    }

    const char* q_digest::merge_error::what() const throw(){
        return "On Q-Digest::merge(): the error and the domain must match";
    }

    void q_digest::check_adress(q_digest::node* t){
        if(not t) return;

        check_adress(t->left);
        check_adress(t->right);

        cout << t << endl;
        return;
    }

    bool q_digest::in_range(int x, int l, int r){
        return (x >= l and x <= r);
    }

    bool q_digest::is_leaf(int x, int l, int r){
        return (x == l and x == r);
    }

    void q_digest::private_compress(q_digest::node * rt, int debt){
        if (rt == nullptr) return;
        if (rt->weight == 0) return;

        if (rt->is_leaf()){
            rt->weight -= debt;
        }else{

            if((rt->sub_tree_weight_from(rt) - debt) > capacity){
                debt += (capacity - rt->weight);
                rt->weight = capacity;

                //pass the sub-tree weight for the left and the right child

                int left_sub_weight = rt->sub_tree_weight_from(rt->left);

                private_compress(rt->left, min(debt, left_sub_weight));
                private_compress(rt->right, max(debt - left_sub_weight, 0));
            }else{
                rt->weight = rt->sub_tree_weight_from(rt) - debt;

                //see after if i can delete this from memory
                rt->set_subtree_weight_to_zero(rt->left);
                rt->set_subtree_weight_to_zero(rt->right);
            }
        }
    }

    void q_digest::private_merge(q_digest::node*& new_root, q_digest::node * tree_to_merge1, q_digest::node* tree_to_merge2){
        if(tree_to_merge1 == nullptr and tree_to_merge2 == nullptr) return;
        if(tree_to_merge1 != nullptr and tree_to_merge2 != nullptr){
            new_root = new q_digest::node(tree_to_merge1->weight + tree_to_merge2->weight);

            private_merge(new_root->left,tree_to_merge1->left,tree_to_merge2->left);
            private_merge(new_root->right,tree_to_merge1->right,tree_to_merge2->right);
        }else if(tree_to_merge1 != nullptr){
            new_root = new q_digest::node(tree_to_merge1->weight);

            private_merge(new_root->left,tree_to_merge1->left,nullptr);
            private_merge(new_root->right,tree_to_merge1->right,nullptr);
        }else{
            new_root = new q_digest::node(tree_to_merge2->weight);

            private_merge(new_root->left,nullptr,tree_to_merge2->left);
            private_merge(new_root->right,nullptr,tree_to_merge2->right);
        }
        return;
    }

    void q_digest::private_update(int x, int weight){
        total_weight += weight;
        capacity = (error * total_weight)/log2(universe); 
        q_digest::node** cur = &root;
        int left_range = 0;
        int right_range = universe - 1;

        while(not this->is_leaf(x, left_range, right_range)){
            int middle  = (left_range + right_range)/2;

            // Check if the node exists, if not we create a new memory adress for it.
            if((*cur) == nullptr){
                (*cur) = new q_digest::node(0);
            }

            if((*cur)->weight < capacity){
                int debt = min(capacity - (*cur)->weight,weight);

                (*cur)->weight += debt;
                weight -= debt;
            }

            if(weight == 0) break;
            
            if(this->in_range(x, left_range, middle)){
                cur = &(*cur)->left;
                right_range = middle;
            }else{
                cur = &(*cur)->right;
                left_range = middle + 1;
            }
        }

        if((*cur) == nullptr){
            (*cur) = new q_digest::node(0);
        }

        (*cur)->weight += weight;

        assert(root != nullptr);

        if(total_weight > ceil_weight){
            this->compress();
            ceil_weight = 2 * total_weight;
        }
    }

    int q_digest::private_query(int x){
        q_digest::node * cur = root;
        int rank = 0;
        int left_range = 0;
        int right_range = universe - 1;

        while(cur and not this->is_leaf(x, left_range, right_range)){

            int middle = (left_range + right_range)/2;

            if(this->in_range(x, left_range, middle)){
                cur = cur->left;
                right_range = middle;
            }else{
                rank += cur->sub_tree_weight_from(cur->left);
                cur = cur->right;
                left_range = middle + 1;
            }
        }

        return rank;
    }

    void q_digest::insert_in_buffer(int x, int weight){
        total_weight += weight;
        small_buffer.push_back({x, weight});

        // do one binary_search

        int index = small_buffer.size() - 1;

        for(int i = small_buffer.size() - 2; i >= 0; i--){
            if (small_buffer[i].first > small_buffer[index].first){
                swap(small_buffer[i], small_buffer[index]);
                index = i;
            }
        }
    }

    void q_digest::transfer_buffer_to_tree(){
        for(auto& it : small_buffer){
            private_update(it.first, it.second);
        }

        small_buffer.clear();
    }

    int q_digest::query_from_buffer(int x){
        if(small_buffer.size() == 0) return 0;

        int rank = 0;

        for(int i = 0; i < small_buffer.size() && small_buffer[i].first < x; i++){
            rank += small_buffer[i].second;
        }

        return rank;
    }


    q_digest::q_digest(double error, int universe){
        this->error = error;
        this->universe = universe;
        this->total_weight = 0;
        this->capacity = 0;
        this->ceil_weight = 0;
        this->root = nullptr;
    }

    //DEBUG
    q_digest::q_digest(q_digest::node* root, double error, int universe) : q_digest(error, universe){
        this->root = root;
        this->total_weight = root->sub_tree_weight_from(root);
        this->ceil_weight = 2 * total_weight;
        this->capacity = (error * total_weight)/log2(universe);
    }

    q_digest::~q_digest(){
        root->delete_tree(root);
    }

    void q_digest::print(int indent){
        root->print(indent);
    }

    void q_digest::print_subtree_weights(){
        root->print_subtree_weights();
    }

    void q_digest::update(int x, int weight){
        if(total_weight < (log2(universe) / error)){
            insert_in_buffer(x, weight);

            if(total_weight >= (log2(universe) / error)) transfer_buffer_to_tree();
        }else{
            private_update(x, weight);
        }
    }	

    int q_digest::query(int x){
        if(total_weight < (log2(universe) / error)) return query_from_buffer(x);
        else return private_query(x);
    }

    void q_digest::compress(){
        this->capacity = (error * total_weight) / log2(universe);
        private_compress(this->root, 0);
    }

    quantile_sketch<int>* q_digest::merge(const quantile_sketch<int>& rhs){
        const q_digest& rhs_cv = dynamic_cast<const q_digest&> (rhs);

        if(&rhs_cv == nullptr){
            std::cerr << "Error in q_digest cast" << std::endl;
            return nullptr;
        }

        if (this->universe != rhs_cv.universe or (this->error - rhs_cv.error > 1e-6)){
            throw merge_error();
        }

        q_digest* merged_qdst = new q_digest(this->error, this->universe);

        merged_qdst->total_weight = this->total_weight + rhs_cv.total_weight;
        merged_qdst->capacity = (merged_qdst->error * merged_qdst->total_weight) / log2(merged_qdst->universe);
        
        this->transfer_buffer_to_tree();
        merged_qdst->transfer_buffer_to_tree();

        private_merge(merged_qdst->root, this->root, rhs_cv.root);

        merged_qdst->compress();

        return merged_qdst;
    }

    int q_digest::weight_total(){
        return total_weight;
    }

    /*
    q_digest tree_on_paper_ex1(){
        q_digest::node* root = new q_digest::node(2);
        root->left = new q_digest::node(2);
        root->right = new q_digest::node(2);
        root->left->right = new q_digest::node(2);
        root->left->right->left = new q_digest::node(9);
        root->right->left = new q_digest::node(2);
        root->right->right = new q_digest::node(1);
        root->right->left->left = new q_digest::node(1);
        root->right->left->right = new q_digest::node(3);

        q_digest qdst(root, 0.25, 8);

        return qdst;
    }

    q_digest tree_on_paper_ex2(){
        q_digest::node* root = new q_digest::node(3);
        root->left = new q_digest::node(3);
        root->right = new q_digest::node(3);
        root->left->right = new q_digest::node(3);
        root->left->right->left = new q_digest::node(6);
        root->right->left = new q_digest::node(3);
        root->right->right = new q_digest::node(1);
        root->right->left->right = new q_digest::node(2);

        q_digest qdst(root, 0.25, 8);

        return qdst;
    }


    void compress_tree_from_ex1(){
        q_digest& qdst = tree_on_paper_ex1();

        qdst.capacity = 3;

        cout << "Before : " << endl;
        qdst.print();
        qdst.private_compress(qdst.root, 0);
        cout << "After : " << endl;
        qdst.print(); 
    }
    */
}