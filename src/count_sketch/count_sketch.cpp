#include "count_sketch.h"


namespace qsbd {
    count_sketch::count_sketch(double err, double delt){
        this->error = err;
        this->delta = delt;

        this->d = (int) ceil(48 * log(1.0 / delta));
        this->t = (int) (3.0 / error);
        this->g = [](int random_bit){
            if(random_bit & 1) return 1;
            else return -1;
        };

        this->estimators.assign(this->d, {});
        for(int i = 0; i < this->d; i++){
            this->estimators[i].assign(this->t, 0);
        }

        for(int i = 0; i < this->d; i++){
            k_wise_family h(2, 2 * this->t);

            this->hash_functions.push_back(h);
        }
    }

    count_sketch::count_sketch(double err, double delt, std::vector<k_wise_family> hashs){
        this->error = err;
        this->delta = delt;

        this->d = ceil(48 * log(1.0 / delta));
        this->t = 3.0 / error;
        this->g = [](int random_bit){
            if(random_bit & 1) return 1;
            else return -1;
        };

        this->estimators.assign(this->d, {});
        for(int i = 0; i < this->d; i++){
            this->estimators[i].assign(this->t, 0);
        }

        // the hash should map to 2 * t
        this->hash_functions = hashs;
    }

    count_sketch::count_sketch(double err, double delt, std::vector<k_wise_family> hashs, std::vector<std::vector<int>> est){
        this->error = err;
        this->delta = delt;

        this->d = ceil(48 * log(1.0 / delta));
        this->t = 3.0 / error;
        this->g = [](int random_bit){
            if(random_bit & 1) return 1;
            else return -1;
        };

        this->estimators = est;
        // The hash should map to 2 * t
        this->hash_functions = hashs;
    }

    count_sketch::count_sketch(int fixd, int fixt){
        this->d = fixd;
        this->t = fixt;

        this->error = 3.0 / this->t;
        this->delta = exp(-this->d / 48);

        this->g = [](int random_bit){
            if(random_bit & 1) return 1;
            else return -1;
        };

        this->estimators.assign(this->d, {});
        for(int i = 0; i < this->d; i++){
            this->estimators[i].assign(this->t, 0);
        }

        for(int i = 0; i < this->d; i++){
            k_wise_family h(2, 2 * this->t);

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

        std::sort(ranks.begin(), ranks.end());
        int size = ranks.size();
        if ((size & 1)) return ranks[size / 2];
        else return (ranks[size/2 - 1] + ranks[size / 2]) / 2;
    }

    count_sketch* count_sketch::merge(const count_sketch& rhs){
        assert(this->t == rhs.t);
        assert(this->d == rhs.d);

        for(int i = 0; i < this->d; i++){
            assert(this->hash_functions[i].get_constants() ==  rhs.hash_functions[i].get_constants());
        }

        std::vector<std::vector<int>> merged_v;

        merged_v.assign(this->d, {});

        for(int i = 0; i < this->d; i++){
            merged_v[i].assign(this->t, 0);

            for(int j = 0; j< this->t; j++){
                merged_v[i][j] =  this->estimators[i][j] + rhs.estimators[i][j];
            }
        }

        count_sketch* merged_cs = new count_sketch(this->error, this->delta, this->hash_functions, merged_v);

        return merged_cs;
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