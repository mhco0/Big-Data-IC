#ifndef PMQ_H
#define PMQ_H
#include "../commum_header/commum_header.h"
#ifndef PI 
#define PI 3.14159265358979323846
#endif


long int z_index(int x, int y);
std::pair<int, int> epsg_projection(double lon, double lat);

class pmq{
private:
    struct window{
        // density bounds;
        double pl, tl;
        int counter;
    };

    struct cell_content{
        double lon, lat;
        unsigned int timestamp;
    };

    int left(int p);
    int right(int p);
    double linear_interpolate_p(int level);
    double linear_interpolate_t(int level);

    // NOTE(): N/log2 must be a power of 2
    const int N;
    const int height;
    std::vector<window> accounting_array;
    std::vector<cell_content> container;
    int segment_size, num_segments;
    double p0, ph, t0, th;
public: 
    pmq();
    ~pmq();

    void update(double lon, double lat);
};

#endif