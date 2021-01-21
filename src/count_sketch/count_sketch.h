#ifndef COUNT_SKETCH_H
#define COUNT_SKETCH_H
#include "../global_generator/global_generator.h"
#include "../commum_header/commum_header.h"
#include "../k_wise_family/k_wise_family.h"

class count_sketch{
private:    
    double error, delta;
    // d is the number of rows and t is the number of collums for the estimator matrix
    int d, t;
    std::function<int(int)> g;
    std::vector<k_wise_family> hash_functions;
    std::vector<std::vector<int>> estimators;
public:
    count_sketch(double err, double delt);
    count_sketch(double err, double delt, std::vector<k_wise_family> hashs);
    count_sketch(double err, double delt, std::vector<k_wise_family> hashs, std::vector<std::vector<int>> est);
    count_sketch(int fixd, int fixt);
    ~count_sketch();

    void update(int elem, int weight);
    int query(int elem);
    count_sketch merge(const count_sketch& rhs);

    int get_d() const;
    int get_t() const;
    double get_error() const;
    double get_delta() const;
    std::vector<std::vector<int>> get_estimators() const;
    std::vector<k_wise_family> get_hash_functions() const;
};
#endif