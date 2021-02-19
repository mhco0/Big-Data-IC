#include "pmq.h"

using namespace std;

long int z_index(int x, int y){
    long int z = 0L;

    for(int i = 0; i < 64; i++){
        if (i & 1){
            z = z | (y & (1 << (i >> 1)));
        }else{
            z = z | (x & (1 << (i >> 1)));
        }
    }

    return z;
}

pair<int, int> epsg_projection(double lon, double lat, int z){
    pair<int, int> coordenates;

    coordenates.first = (lon/360.0 + 0.5) * (1 << z);
    coordenates.second = (1 - ( log(tan(lat * (PI / 180.0)) + 1.0 / cos(lat * (PI / 180.0))) / PI)) * (1 << (z - 1));

    return coordenates;
}

pmq::pmq():N(1 << 16), height(25){
    this->container.resize(this->N);
    this->segment_size = log2(this->N);
    this->num_segments = this->N / this->segment_size;
}

pmq::~pmq(){
}

int pmq::left(int p){
    return (p << 1) + 1;
}

int pmq::right(int p){
    return (p << 1) + 2;
}

double pmq::linear_interpolate_p(int level){
    return ph - (p0 - ph) * ((height - level) / height);
}

double pmq::linear_interpolate_t(int level){
    return th + (t0 - th) * ((height - level) / height);
}