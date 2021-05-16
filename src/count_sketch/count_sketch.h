#ifndef QSBD_COUNT_SKETCH_H
#define QSBD_COUNT_SKETCH_H
#include "../qsbd_debug/qsbd_debug.h"
#include "../global_generator/global_generator.h"
#include "../commum_header/commum_header.h"
#include "../k_wise_family/k_wise_family.h"

namespace qsbd {
    class dcs;

    class count_sketch {
    private:  
        friend class dcs;

        double error, delta;
        // d is the number of rows and t is the number of collums for the estimator matrix
        int d, t;
        std::function<int(int)> g;
        std::vector<k_wise_family> hash_functions;
        std::vector<std::vector<int>> estimators;

        void set_param_double(double err, double delt);
        void set_param_int(int fixd, int fixt);

        count_sketch(int fixd, int fixt);
        count_sketch(int fixd, int fixt, const std::vector<k_wise_family>& hashs);
    public:
        count_sketch(double err, double delt);
        count_sketch(double err, double delt, const std::vector<k_wise_family>& hashs);
        ~count_sketch();

        void update(int elem, int weight);
        int query(int elem);
        count_sketch* merge(count_sketch& rhs);

        int get_d() const;
        int get_t() const;
        double get_error() const;
        double get_delta() const;
        std::vector<std::vector<int>> get_estimators() const;
        std::vector<k_wise_family> get_hash_functions() const;
    };
}
#endif