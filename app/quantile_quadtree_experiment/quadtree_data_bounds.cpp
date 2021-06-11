#include <aabb/aabb.hpp>
#include <utils/utils.h>
#include <qsbd_debug/qsbd_debug.h>
#include <fstream>
#include <iostream>
using namespace std;
using namespace qsbd;

/*
    GLOBALS
*/

double gminx, gminy, gmaxx, gmaxy;


/*
    Copy functions from quantile quadtree search. BEGIN
*/

short direction(const aabb<int>& box, const point<int>& pos){
    int center_x = (box.bounds().first.x() + box.bounds().second.x()) / 2;
    int center_y = (box.bounds().first.y() + box.bounds().second.y()) / 2;

    if(pos.x() >= center_x and pos.y() >= center_y) return 0;
    else if(pos.x() < center_x and pos.y() >= center_y) return 1;
    else if(pos.x() < center_x and pos.y() < center_y) return 2;
    else if(pos.x() >= center_x and pos.y() < center_y) return 3;
    else return 0;
}

bool unit_box(const aabb<int>& region){
    int width = region.bounds().second.x() - region.bounds().first.x();
    int height = region.bounds().second.y() - region.bounds().first.y();
    return not (width > 1 || height > 1);
}

void change_box(aabb<int>& cur_box, short direction){
    int center_x = (cur_box.bounds().second.x() + cur_box.bounds().first.x()) / 2;
    int center_y = (cur_box.bounds().second.y() + cur_box.bounds().first.y()) / 2;

    switch (direction){
        case 0 : {
            // ne
            cur_box.bounds(center_x, center_y, cur_box.bounds().second.x(), cur_box.bounds().second.y());
        }
        break;
        case 1 : {
            // nw
            cur_box.bounds(cur_box.bounds().first.x(), center_y, center_x, cur_box.bounds().second.y());
        }
        break;
        case 2 : {
            // sw
            cur_box.bounds(cur_box.bounds().first.x(), cur_box.bounds().first.y(), center_x, center_y);
        }
        break;
        case 3 : {
            // se
            cur_box.bounds(center_x, cur_box.bounds().first.y(), cur_box.bounds().second.x(), center_y);
        }
        break;
        default:{
            DEBUG_ERR("Invalid direction on quantile_quadtree::change_box");
        }
        break;
    }
}

/*
    Copy functions from quantile quadtree search. END
*/

/*
    Function to write bounds 
*/

void write_bounds(aabb<int>& root_box, int depth, const int& max_depth, ofstream& out){
    int iminx = root_box.bounds().first.x();
    int iminy = root_box.bounds().first.y();
    int imaxx = root_box.bounds().second.x();
    int imaxy = root_box.bounds().second.y();

    double dminx = map_coord_inv(iminx, gminx, gmaxx, max_depth);
    double dminy = map_coord_inv(iminy, gminy, gmaxy, max_depth);
    double dmaxx = map_coord_inv(imaxx, gminx, gmaxx, max_depth);
    double dmaxy = map_coord_inv(imaxy, gminy, gmaxy, max_depth);

    out << dminx << " " << dminy << " " << dmaxx << " " << dmaxy << endl;


    if(depth == max_depth or unit_box(root_box)){
        return;
    }

    for(int i = 0; i < 4; i++){
        aabb<int> child_box(root_box);
        change_box(child_box, i);
        write_bounds(child_box, depth + 1, max_depth, out);    
    }

    return;
}

/*
    Help usage function
*/
void help(){
    cout << "Quadtree Data Bounds usage : " << endl;
    cout << "./quadtree_data_bounds minx miny maxx maxy depth output_file" << endl;
    cout << "minx miny maxx maxy : The quadtree bounds as double values" << endl;
    cout << "depth : The quadtree max depth" << endl;
    cout << "output_file : The path of the file where the bounds of the quadtree subdivision will be saved" << endl;
    cout << "EX: " << endl;
    cout << "./quadtree_data_bounds 0.0 0.0 1280.0 720.0 5 out/quadtree_bounds.dat" << endl;
}

int main(int argc, char* argv[]){
    deque<string> args = process_args(argc, argv);

    if(args.size() != 6){
        DEBUG_ERR("Wrong args size.");
        help();
        return -1;
    }else{
        gminx = stod(args[0]);
        gminy = stod(args[1]);
        gmaxx = stod(args[2]);
        gmaxy = stod(args[3]);
        int depth = stoi(args[4]);
        ofstream out(args[5]);

        if(not out.is_open()){
            DEBUG_ERR("Couldn't open output file");
            return -1;
        }

        int discrete_minx = map_coord(gminx, gminx, gmaxx, depth);
        int discrete_miny = map_coord(gminy, gminy, gmaxy, depth);
        int discrete_maxx = map_coord(gmaxx, gminx, gmaxx, depth); 
        int discrete_maxy = map_coord(gmaxy, gminy, gmaxy, depth);

        aabb<int> root_bound(discrete_minx, discrete_miny, discrete_maxx, discrete_maxy);

        write_bounds(root_bound, 0, depth, out);

        out.close();
    }

    return 0;
}