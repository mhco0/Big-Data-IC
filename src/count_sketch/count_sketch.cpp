#include "count_sketch.h"

namespace qsbd {
    void count_sketch::set_param_double(double err, double delt){
        this->error = err;
        this->delta = delt;

        this->d = (int) ceil(48 * log(1.0 / delta));
        this->t = (int) (3.0 / error);
    }

    void count_sketch::set_param_int(int fixd, int fixt){
        this->d = fixd;
        this->t = fixt;

        this->error = 3.0 / this->t;
        this->delta = exp(-this->d / 48.0);
    }

    int count_sketch::g(int random_bit){
        if(random_bit & 1) return 1;
        else return -1;
    }

    count_sketch::count_sketch(int fixd, int fixt){
        this->set_param_int(fixd, fixt);

        this->estimators.assign(this->d, {});
        for(int i = 0; i < this->d; i++){
            this->estimators[i].assign(this->t, 0);
        }

        for(int i = 0; i < this->d; i++){
            k_wise_family h(2, 2 * this->t);

            this->hash_functions.push_back(h);
        }
    }

    count_sketch::count_sketch(int fixd, int fixt, const std::vector<k_wise_family>& hashs){
        this->set_param_int(fixd, fixt);

        this->estimators.assign(this->d, {});
        for(int i = 0; i < this->d; i++){
            this->estimators[i].assign(this->t, 0);
        }

        for(int i = 0; i < this->d; i++){
            k_wise_family h(hashs[i]);

            this->hash_functions.push_back(h);
        }
    }

    count_sketch::count_sketch(double err, double delt){
        this->set_param_double(err, delt);

        this->estimators.assign(this->d, {});
        for(int i = 0; i < this->d; i++){
            this->estimators[i].assign(this->t, 0);
        }

        for(int i = 0; i < this->d; i++){
            k_wise_family h(2, 2 * this->t);

            this->hash_functions.push_back(h);
        }
    }

    count_sketch::count_sketch(double err, double delt, const std::vector<k_wise_family>& hashs){
        this->set_param_double(err, delt);

        this->estimators.assign(this->d, {});
        for(int i = 0; i < this->d; i++){
            this->estimators[i].assign(this->t, 0);
        }

        // the hash should map to 2 * t

        for(int i = 0; i < this->d; i++){
            k_wise_family h(hashs[i]);

            this->hash_functions.push_back(h);
        }
    }

    count_sketch::count_sketch(const count_sketch& other){
        this->error = other.error;
        this->delta = other.delta;
        this->d = other.d;
        this->t = other.t;

        this->estimators.assign(other.d, {});
        for(int i = 0; i < other.d; i++){
            this->estimators[i].assign(other.t, 0);
            for(int j = 0; j < other.t; j++){
                this->estimators[i][j] = other.estimators[i][j];
            }
        }

        for(int i = 0; i < other.d; i++){
            k_wise_family h(other.hash_functions[i]);

            this->hash_functions.push_back(h);
        }
    }

    count_sketch::~count_sketch(){
    }

    void count_sketch::update(int elem, int weight){
        for(int i = 0; i < this->d; i++){
            int hx = hash_functions[i](elem) % (2 * this->t);
            int hi = hx >> 1;

            estimators[i][hi] += g(hx) * weight;
        }
    }

    int count_sketch::query(int elem){
        std::vector<int> ranks;

        for(int i = 0; i < this->d; i++){
            int hx = hash_functions[i](elem) % (2 * this->t);
            int hi = hx >> 1;

            ranks.push_back(g(hx) * estimators[i][hi]);
        }

        //std::sort(ranks.begin(), ranks.end());
        int size = ranks.size();

        if (size == 0) return -1;

        // picking the median in linear time;
        if ((size & 1)){

            std::nth_element(ranks.begin(), ranks.begin() + size / 2, ranks.end());

            return ranks[size / 2];
        }else{
            nth_element(ranks.begin(), ranks.begin() + size / 2, ranks.end());
  
            nth_element(ranks.begin(), ranks.begin() + (size - 1) / 2, ranks.end());

            return (ranks[size/2 - 1] + ranks[size / 2]) / 2;
        }
    }

    count_sketch* count_sketch::merge(count_sketch& rhs){
        ASSERT(this->t == rhs.t);
        ASSERT(this->d == rhs.d);

        for(int i = 0; i < this->d; i++){
            ASSERT(this->hash_functions[i].get_constants() == rhs.hash_functions[i].get_constants());
        }

        count_sketch* merged_cs = new count_sketch(this->error, this->delta, this->hash_functions);

        for(int i = 0; i < merged_cs->d; i++){
            for(int j = 0; j < merged_cs->t; j++){
                merged_cs->estimators[i][j] = this->estimators[i][j] + rhs.estimators[i][j];
            }
        }

        return merged_cs;
    }

    count_sketch& count_sketch::operator=(const count_sketch& other){
        if(this != &other){
            this->error = other.error;
            this->delta = other.delta;
            this->d = other.d;
            this->t = other.t;

            this->hash_functions.resize(other.hash_functions.size());

            for(int i = 0; i < other.hash_functions.size(); i++){
                this->hash_functions[i] = other.hash_functions[i];
            }

            this->estimators.resize(other.estimators.size());

            for(int i = 0; i < other.estimators.size(); i++){
                this->estimators[i].resize(other.estimators[i].size());

                for(int j = 0; j < other.estimators[i].size(); j++){
                    this->estimators[i][j] = other.estimators[i][j];
                }
            }
        }

        return *this;
    }

    int count_sketch::get_d() const {
        return this->d;
    }

    int count_sketch::get_t() const {
        return this->t;
    }

    double count_sketch::get_error() const {
        return this->error;
    }

    double count_sketch::get_delta() const {
        return this->delta;
    }

    std::vector<std::vector<int>> count_sketch::get_estimators() const {
        return this->estimators;
    }

    std::vector<k_wise_family> count_sketch::get_hash_functions() const {
        return this->hash_functions;
    }
}