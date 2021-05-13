#include "dcs.h"

namespace qsbd {

    dcs::dcs(double err, int universe, const std::vector<count_sketch>& other_est){
        this->universe = universe;
        this->error = err;
        this->total_weight = 0;
        this->w = (int) (sqrt(log2(universe) * log(log2(universe) / err)) / err);
        this->d = (int) log(log2(universe) / err);
        this->s = std::max((int) floor(log2(universe / (double) (this->w * this->d))), 0);
        this->lvls = ceil(log2(universe));

        this->frequency_counters.assign(this->lvls - (this->s + 1), {});

        for(int i = this->s + 1; i < this->lvls; i++){
            int dyadic_interval = 1 << i;

            this->frequency_counters[i - (this->s + 1)].assign(universe / dyadic_interval, 0);
        }

        for(int i = 0; i <= this->s; i++){
            count_sketch cs(this->d, this->w, other_est[i].get_hash_functions());

            this->estimators.push_back(cs);
        }
    }

    std::vector<count_sketch> dcs::get_estimators() const {
        return this->estimators;
    }


    dcs::dcs(double err, int universe){
        this->universe = universe;
        this->error = err;
        this->total_weight = 0;
        this->w = (int) (sqrt(log2(universe) * log(log2(universe) / err)) / err);
        this->d = (int) log(log2(universe) / err);
        this->s = std::max((int) floor(log2(universe / (double) (this->w * this->d))), 0);
        this->lvls = ceil(log2(universe));

        this->frequency_counters.assign(this->lvls - (this->s + 1), {});

        for(int i = this->s + 1; i < this->lvls; i++){
            int dyadic_interval = 1 << i;

            this->frequency_counters[i - (this->s + 1)].assign(universe / dyadic_interval, 0);
        }

        for(int i = 0; i <= this->s; i++){
            count_sketch cs(this->d, this->w);  

            this->estimators.push_back(cs);
        }
    }

    dcs::~dcs(){
    }

    void dcs::update(int x, int weight){
        this->total_weight += weight;

        for(int i = 0; i < this->lvls; i++){
            if(i > this->s){
                this->frequency_counters[i - (this->s + 1)][x] += weight;
            }else{
                this->estimators[i].update(x, weight);
            }

            x = x / 2;
        }
    }

    int dcs::query(int x){
        int rank = 0;

        for(int i = 0; i < this->lvls; i++){
            if(x & 1){
                if(i > this->s){
                    rank += this->frequency_counters[i - (this->s + 1)][x - 1];
                }else{
                    rank += this->estimators[i].query(x - 1);
                }
            }

            x = x / 2;
        }


        return rank;
    }

    int dcs::quantile(double quant){
        // quant must be in [0,1]

        int weight = (int) total_weight * quant;
        int x = 0;
        int parse_quant = 0;

        for(int i = this->lvls - 1; i >= 0; i--){
            int M = 0;
            x = 2 * x;

            if(i > this->s){
                M = this->frequency_counters[i - (this->s + 1)][x];
            }else{
                M = this->estimators[i].query(x);
            }

            if(parse_quant + M < weight){
                x += 1;
                parse_quant += M;
            }
        }

        return x;
    }

    quantile_sketch<int> * dcs::merge(quantile_sketch<int>& rhs){
        dcs& rhs_cv = dynamic_cast<dcs&> (rhs);

        if(&rhs_cv == nullptr){
            std::cerr << "Error in dcs cast" << std::endl;
            return nullptr;
        }

        if(this->error - rhs_cv.error > 1e-6){
            std::cerr << "dcs's error need to match" << std::endl;
            return nullptr;
        }

        if(this->universe != rhs_cv.universe){
            std::cerr << "dcs's universe need to match" << std::endl;
            return nullptr;
        }

        dcs* merged = new dcs(this->error, this->universe);

        for(int i = 0; i < merged->lvls; i++){
            if(i > merged->s){
                for(int j = 0; j < merged->frequency_counters[i - (merged->s + 1)].size(); j++){
                    merged->frequency_counters[i - (merged->s + 1)][j] = this->frequency_counters[i - (this->s + 1)][j] + rhs_cv.frequency_counters[i - (rhs_cv.s + 1)][j];
                }
            }else{
                merged->estimators[i] = *(this->estimators[i].merge(rhs_cv.estimators[i]));
            }
        }

        return merged;
    }

    int dcs::get_tree_lvl() const {
        return this->lvls;
    }

    int dcs::get_w() const {
        return this->w;
    }

    int dcs::get_d() const {
        return this->d;
    }

    int dcs::get_s() const {
        return this->s;
    }

    double dcs::get_error() const {
        return this->error;
    }

    int dcs::get_universe() const {
        return this->universe;
    }

    int dcs::get_total_weight() const {
        return this->total_weight;
    }
}