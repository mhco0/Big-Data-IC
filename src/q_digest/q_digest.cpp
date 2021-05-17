#include "q_digest.h"
using namespace std;

// file merged
namespace qsbd {

    q_digest::node::node(int w, int lc){
        this->weight = w;
        this->left_child = lc;
    }

    int q_digest::alloc_childs(){
        int left_child_pos = tree.size();

        tree.push_back(q_digest::node(0, -1));
        tree.push_back(q_digest::node(0, -1));

        return left_child_pos;
    }

    void q_digest::private_print(int cur_node, int indent) const {
        if(cur_node != -1){
            if(tree[cur_node].left_child != -1) this->private_print(tree[cur_node].left_child + 1, indent + 4);
            if (indent) cout << setw(indent) << ' ';
            if(tree[cur_node].left_child != -1) cout << " /\n" << setw(indent) << ' ';

            cout << tree[cur_node].weight << endl;
            
            if(tree[cur_node].left_child != -1) {
                cout << setw(indent) << ' ' <<" \\\n";
                this->private_print(tree[cur_node].left_child, indent + 4);
            }
        }
    }

    void q_digest::private_print_subtree_weights(int cur_node, int indent){
        if(cur_node != -1){
            if(tree[cur_node].left_child != -1) private_print_subtree_weights(tree[cur_node].left_child + 1, indent + 4);
            if (indent) cout << setw(indent) << ' ';
            if(tree[cur_node].left_child != -1) cout <<" /\n" << setw(indent) << ' ';

            cout << subtree_weight_from(cur_node) << endl;

            if(tree[cur_node].left_child != -1) {
                cout << setw(indent) << ' ' <<" \\\n";
                private_print_subtree_weights(tree[cur_node].left_child, indent + 4);
            }
        }
    }

    int q_digest::subtree_weight_from(int cur_node){
        if(cur_node == -1) return 0;
        else{
            int left_weight = 0;
            int right_weight = 0;

            if(tree[cur_node].left_child != -1){
                left_weight = this->subtree_weight_from(tree[cur_node].left_child);
                right_weight = this->subtree_weight_from(tree[cur_node].left_child + 1);
            }

            return tree[cur_node].weight + left_weight + right_weight;
        } 
    }
   
    void q_digest::set_subtree_weight_to_zero(int cur_node){
        if(cur_node == -1) return;
        else{
            tree[cur_node].weight = 0;
            
            if(tree[cur_node].left_child != -1){
                this->set_subtree_weight_to_zero(tree[cur_node].left_child);
                this->set_subtree_weight_to_zero(tree[cur_node].left_child + 1);
            }

            return;
        }
    }

    const char* q_digest::merge_error::what() const throw(){
        return "On Q-Digest::merge(): the error and the domain must match";
    }


    bool q_digest::in_range(const int& x, const int& l, const int& r){
        return (x >= l and x <= r);
    }

    bool q_digest::is_leaf(const int& x, const int& l, const int& r){
        return (x == l and x == r);
    }

    void q_digest::private_compress(int cur_node, int debt, int left_range, int right_range){
        if (cur_node == -1) return;
        if (tree[cur_node].weight == 0) return;

        if(left_range == right_range){ 
            tree[cur_node].weight -= debt;
        } else {

            int all_sub_weight = this->subtree_weight_from(cur_node);

            if((all_sub_weight - debt) > capacity){
                int middle  = (left_range + right_range) / 2;

                debt += (capacity - tree[cur_node].weight);
                tree[cur_node].weight = capacity;

                //pass the sub-tree weight for the left and the right child

                int left_sub_weight = this->subtree_weight_from(tree[cur_node].left_child);

                if(tree[cur_node].left_child != -1){
                    this->private_compress(tree[cur_node].left_child, min(debt, left_sub_weight), left_range, middle);
                    this->private_compress(tree[cur_node].left_child + 1, max(debt - left_sub_weight, 0), middle + 1, right_range);
                }
            }else{
                tree[cur_node].weight = all_sub_weight - debt;

                if(tree[cur_node].left_child != -1){
                    this->set_subtree_weight_to_zero(tree[cur_node].left_child);
                    this->set_subtree_weight_to_zero(tree[cur_node].left_child + 1);
                }
            }
        }

        return;
    }

    void q_digest::private_merge(const std::vector<q_digest::node>& left_tree, const std::vector<q_digest::node>& right_tree, int cur_node, int left_node, int right_node){    
        if(cur_node != -1 and left_node != -1 and right_node != -1){
            if(tree[cur_node].left_child == -1 and left_tree[left_node].left_child != -1 and right_tree[right_node].left_child != -1){
                int left_child_pos = this->alloc_childs();
                tree[cur_node].left_child = left_child_pos;
            }

            tree[cur_node].weight = left_tree[left_node].weight + right_tree[right_node].weight;

            if(left_tree[left_node].left_child != -1 and right_tree[right_node].left_child != -1){
                this->private_merge(left_tree, right_tree, tree[cur_node].left_child, left_tree[left_node].left_child, right_tree[right_node].left_child);
                this->private_merge(left_tree, right_tree, tree[cur_node].left_child + 1, left_tree[left_node].left_child + 1, right_tree[right_node].left_child + 1);
            }else if(left_tree[left_node].left_child != -1){
                this->private_merge(left_tree, right_tree, tree[cur_node].left_child, left_tree[left_node].left_child, -1);
                this->private_merge(left_tree, right_tree, tree[cur_node].left_child + 1, left_tree[left_node].left_child + 1, -1);
            }else if(right_tree[right_node].left_child != -1){
                this->private_merge(left_tree, right_tree, tree[cur_node].left_child, -1, right_tree[right_node].left_child);
                this->private_merge(left_tree, right_tree, tree[cur_node].left_child + 1, -1, right_tree[right_node].left_child + 1);
            }
        }else if(cur_node != -1 and left_node != -1){
            if(tree[cur_node].left_child == -1 and left_tree[left_node].left_child != -1){
                int left_child_pos = this->alloc_childs();
                tree[cur_node].left_child = left_child_pos;
            }

            tree[cur_node].weight = left_tree[left_node].weight;

            if(left_tree[left_node].left_child != -1){
                this->private_merge(left_tree, right_tree, tree[cur_node].left_child, left_tree[left_node].left_child, -1);
                this->private_merge(left_tree, right_tree, tree[cur_node].left_child + 1, left_tree[left_node].left_child + 1, -1);
            }
        }else if(cur_node != -1 and right_node != -1){
            if(tree[cur_node].left_child == -1 and right_tree[right_node].left_child != -1){
                int left_child_pos = this->alloc_childs();
                tree[cur_node].left_child = left_child_pos;
            }

            tree[cur_node].weight = right_tree[right_node].weight;
            
            if(right_tree[right_node].left_child != -1){
                this->private_merge(left_tree, right_tree, tree[cur_node].left_child, -1, right_tree[right_node].left_child);
                this->private_merge(left_tree, right_tree, tree[cur_node].left_child + 1, -1, right_tree[right_node].left_child + 1);
            }
        }
        
        return;
    }

    void q_digest::private_update(int x, int weight){
        total_weight += weight;
        capacity = (error * total_weight) / log2(universe); 
        int cur_node = 0;
        int left_range = 0;
        int right_range = universe - 1;

        while(not this->is_leaf(x, left_range, right_range)){
            int middle  = (left_range + right_range) / 2;

            // Check if the node exists, if not we create a new memory adress for it.
            if(tree[cur_node].left_child == -1){
                int left_child_pos = this->alloc_childs();
                tree[cur_node].left_child = left_child_pos;
            }

            
            if(tree[cur_node].weight < capacity){
                int debt = min(capacity - tree[cur_node].weight, weight);

                tree[cur_node].weight += debt;
                weight -= debt;
            }

            if(weight == 0) break;
            
            if(this->in_range(x, left_range, middle)){
                cur_node = tree[cur_node].left_child;
                right_range = middle;
            }else{
                cur_node = tree[cur_node].left_child + 1;
                left_range = middle + 1;
            }
        }

        if(tree[cur_node].left_child == -1 and not this->is_leaf(x, left_range, right_range)){
            int left_child_pos = this->alloc_childs();
            tree[cur_node].left_child = left_child_pos;
        }

        tree[cur_node].weight += weight;

        if(total_weight > ceil_weight){
            this->compress();
            ceil_weight = 2 * total_weight;
        }
    }

    int q_digest::private_query(int x){
        int cur_node = 0;
        int rank = 0;
        int left_range = 0;
        int right_range = universe - 1;

        while(tree[cur_node].left_child != -1 and not this->is_leaf(x, left_range, right_range)){

            int middle = (left_range + right_range) / 2;

            if(this->in_range(x, left_range, middle)){
                cur_node = tree[cur_node].left_child;
                right_range = middle;
            }else{
                rank += this->subtree_weight_from(tree[cur_node].left_child);
                cur_node = tree[cur_node].left_child + 1;
                left_range = middle + 1;
            }
        }

        return rank;
    }

    void q_digest::insert_in_buffer(const int& x, const int& weight){
        total_weight += weight;

        insert_sorted(small_buffer, {x, weight});
    }

    void q_digest::transfer_buffer_to_tree(){
        total_weight = 0;

        for(auto& it : small_buffer){
            private_update(it.first, it.second);
        }

        small_buffer.clear();

        transfered_buffer = true;
    }

    int q_digest::query_from_buffer(const int& x){
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
        this->transfered_buffer = false;
        this->tree.push_back(q_digest::node(0, -1));
    }
    
    q_digest::~q_digest(){
    }

    void q_digest::print(int indent) const {
        this->private_print(0, indent);
    }

    void q_digest::print_subtree_weights(){
        this->private_print_subtree_weights(0);
    }

    void q_digest::update(int x, int weight){
        if (x >= universe){
            DEBUG_ERR("On q_digest::update value first parameter 'x' is greater then the universe range");

            return;
        }

        if((total_weight < (log2(universe) / error)) and not transfered_buffer){
            insert_in_buffer(x, weight);

            if(total_weight >= (log2(universe) / error)) transfer_buffer_to_tree();
        }else{
            private_update(x, weight);
        }
    }	

    int q_digest::query(int x){
        if (x >= universe){
            DEBUG_ERR("On q_digest::update value first parameter 'x' is greater then the universe range");

            return -1;
        }

        if((total_weight < (log2(universe) / error)) and not transfered_buffer) return query_from_buffer(x);
        else return private_query(x);
    }

    void q_digest::compress(){
        capacity = (error * total_weight) / log2(universe);
        private_compress(0, 0, 0, universe - 1);
    }

   quantile_sketch<int>* q_digest::merge(quantile_sketch<int>& rhs){
        q_digest& rhs_cv = dynamic_cast<q_digest&> (rhs);

        if (this->universe != rhs_cv.universe or (this->error - rhs_cv.error > 1e-6)){
            throw merge_error();
        }

        q_digest* merged_qdst = new q_digest(this->error, this->universe);

        merged_qdst->total_weight = this->total_weight + rhs_cv.total_weight;
        merged_qdst->capacity = (merged_qdst->error * merged_qdst->total_weight) / log2(merged_qdst->universe);

        this->transfer_buffer_to_tree();
        rhs_cv.transfer_buffer_to_tree();

        merged_qdst->private_merge(this->tree, rhs_cv.tree, 0, 0, 0);

        merged_qdst->compress();

        return merged_qdst;
    }

    int q_digest::get_total_weight(){
        return total_weight;
    }
}