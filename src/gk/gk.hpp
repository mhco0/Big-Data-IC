#ifndef GK_H
#define GK_H
#include "../commum_header/commum_header.h"
#include "../global_generator/global_generator.h"
#include "../quantile_sketch/quantile_sketch.hpp"

template <class T>
class gk : public quantile_sketch<T> {
private:
    std::multimap<T, std::pair<int, int>> tuple_list;
    int N;
    double epsilon;

public:
    gk(double eps){
        N = 0;
        epsilon = eps;

        tuple_list.insert({std::numeric_limits<T>::max(), {1, 0}});
    }

    ~gk(){}

    void update(T value) override {
        N += 1;

        //find smallest i such x[i] > value
        auto up_it = tuple_list.upper_bound(value);

        int weight = (*up_it).second.first;
        int uncertainty = (*up_it).second.second;
        double capacity = (2.0 * epsilon * N);

        if((capacity - (weight * 1.0 + uncertainty * 1.0 + 1.0)) > 1e-8){
            (*up_it).second.first += 1;
        }else{
        
            tuple_list.insert(up_it, {value, {1, weight + uncertainty - 1}});

            for(auto it = tuple_list.begin(), it2 = next(it); it2 != tuple_list.end(); it++, it2++){
                if ((capacity - ((*it).second.first * 1.0 + (*it2).second.first * 1.0 + (*it2).second.second * 1.0)) > 1e-8){
                    (*it2).second.first += (*it).second.first;

                    
                    tuple_list.erase(it);

                    break;
                }
            } 
        }
        
        return;
    }

    quantile_sketch<T>* merge(const quantile_sketch<T>& rhs) override {
        std::cout << "init merge" << std::endl;
        const gk<T>& rhs_cv = dynamic_cast<const gk<T>&>(rhs); 
        std::cout << 1 << std::endl;
        if(&rhs_cv == nullptr){
            std::cerr << "Error in gk cast" << std::endl;
            return nullptr;
        }

        assert((rhs_cv.epsilon - this->epsilon) < 1e-6);


        std::cout << 2 << std::endl;
        auto left_it = tuple_list.begin();
        auto right_it = rhs_cv.tuple_list.begin(); 
        gk<T>* merged_summary = new gk<T>(epsilon);


        while(left_it != tuple_list.end() and right_it != rhs_cv.tuple_list.end()){
            merged_summary->N += 1;

            if((*left_it).first < (*right_it).first){
                T value = (*left_it).first;
                int weight = (*left_it).second.first;
                int uncertainty = (*left_it).second.second + (*right_it).second.first + (*right_it).second.second - 1; 
                
                merged_summary->tuple_list.insert({value, {weight, uncertainty}});

                left_it = next(left_it);
            }else{
                T value = (*right_it).first;
                int weight = (*right_it).second.first;
                int uncertainty = (*right_it).second.second + (*left_it).second.first + (*left_it).second.second - 1; 

                merged_summary->tuple_list.insert({value, {weight, uncertainty}});

                right_it = next(right_it);
            }
        }

        std::cout << 3 << std::endl;
        while(left_it != tuple_list.end()){
            merged_summary->N += 1;
            merged_summary->tuple_list.insert(*left_it);

            left_it = next(left_it);
        }

        std::cout << 4 << std::endl;
        while(right_it != rhs_cv.tuple_list.end()){
            merged_summary->N += 1;
            merged_summary->tuple_list.insert(*right_it);

            right_it = next(right_it);
        }

        std::cout << 5 << std::endl;
        double capacity = 2.0 * epsilon * merged_summary->get_N();
        for(auto it = merged_summary->tuple_list.begin(), it2 = next(it); it2 != merged_summary->tuple_list.end(); it++, it2++){
            if ((capacity - ((*it).second.first * 1.0 + (*it2).second.first * 1.0 + (*it2).second.second * 1.0)) > 1e-8){
                (*it2).second.first += (*it).second.first;

                // seg fault going here
                merged_summary->tuple_list.erase(it);
            }
        } 
        std::cout << "finish merge" << std::endl;

        return merged_summary;
    }

    int query(T value) override {
        double weight_sum = 0.0;
        auto it = tuple_list.begin();

        while(value >= (*it).first){
            weight_sum += (*it).second.first;
            it = next(it);
        }

        return (int) (weight_sum - 1.0 + (1.0 * ((*it).second.first + (*it).second.second) / 2.0));
    }

    //DEBUG
    void print(){
        for (auto &it : tuple_list){
            std::cout << "(" << it.first << "," << it.second.first << "," << it.second.second << ") ";
        }
        std::cout << std::endl;
    }

    //DEBUG
    int get_N(){
        return N;
    }

};

#endif