#include "kll.hpp"

template<class T>
kll<T>::kll(double err) : coin(0,1){
    error = err;
    capacity_max = sqrt(log(1.0/error))/(error);  
    buffer_diff = 0.75; // change here later
    height = 0;
    buffers_array.push_back({});   
}

template<class T>
void kll<T>::insert_sorted(std::vector<T> &buffer, const T &elem){

    buffer.insert( upper_bound(buffer.begin(), buffer.end(), elem), elem);
}

template<class T>
void kll<T>::compress(){
    for(int l = 0; l <= height; l++){
        if(buffers_array[l].size() >= std::max(2.0, std::pow(buffer_diff,height-l)*capacity_max)){
            if(l == height){
                buffers_array.push_back({});
            }

            for(auto i = buffers_array[l].begin() + coin(generator); i < buffers_array[l].end(); i = next(i,2)){
                insert_sorted(buffers_array[l + 1], (*i));
            }
            
            buffers_array[l].clear();
        }
    }

    if((buffers_array.size() - 1) != height){
        height++;
    }
}

template<class T>
void kll<T>::update(T elem){
    insert_sorted(buffers_array[0], elem);
    this->compress();
}

template<class T>
int kll<T>::query(T elem){
    int rank = 0;

    for(int i = 0; i <= height; i++){
        auto it = upper_bound(buffers_array[i].begin(), buffers_array[i].end(), elem);

        rank += (it - buffers_array[i].begin())*(1 << i);
    }

    return rank;
}

template<class T>
void kll<T>::print(){
    for(int l = 0; l <= height; l++){
        std::cout << "height [" << l << "]: ";
        for(auto i:buffers_array[l]){
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}