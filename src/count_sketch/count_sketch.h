#ifndef QSBD_COUNT_SKETCH_H
#define QSBD_COUNT_SKETCH_H
#include "../qsbd_debug/qsbd_debug.h"
#include "../global_generator/global_generator.h"
#include "../commum_header/commum_header.h"
#include "../k_wise_family/k_wise_family.h"

namespace qsbd {

    class count_sketch {
    private:  

        double error, delta;
        // d is the number of rows and t is the number of columns for the estimator matrix
        int d, t;
        std::vector<k_wise_family> hash_functions;
        std::vector<std::vector<int>> estimators;
        //L1 bound
        void set_param_double(double err, double delt);
        void set_param_int(int fixd, int fixt);
        // L2 bound
        void set_param_double2(double err, double delt);
        void set_param_int2(int fixd, int fixt);

        int g(int random_bit);
    public:
        count_sketch(int fixd, int fixt);
        count_sketch(int fixd, int fixt, const std::vector<std::vector<unsigned long long int>>& hashs_consts);
        count_sketch(double err, double delt);
        count_sketch(double err, double delt, const std::vector<std::vector<unsigned long long int>>& hashs_consts);
        count_sketch(const count_sketch& other);
        ~count_sketch();

        void update(int elem, int weight);
        int query(int elem);
        count_sketch* merge(count_sketch& rhs);
        void inner_merge(count_sketch& rhs);

        count_sketch& operator=(const count_sketch& other);

        int get_d() const;
        int get_t() const;
        double get_error() const;
        double get_delta() const;
        std::vector<std::vector<int>> get_estimators() const;
        std::vector<k_wise_family> get_hash_functions() const;
        std::vector<std::vector<unsigned long long int>> get_hash_functions_consts() const;
    };
}
#endif