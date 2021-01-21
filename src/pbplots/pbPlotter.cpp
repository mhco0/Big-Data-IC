#include "pbPlotter.h"

using namespace std;

//PBPLOTTER SERIES:

pbplotter_serie::pbplotter_serie(vector<double>& xs, vector<double>& ys){
    this->series = GetDefaultScatterPlotSeriesSettings();

    this->series->xs = &xs;
    this->series->ys = &ys;
}

pbplotter_serie::pbplotter_serie(){
    this->series = GetDefaultScatterPlotSeriesSettings();
}

pbplotter_serie::~pbplotter_serie(){
}

void pbplotter_serie::use_lines(const wchar_t * type){
    this->series->linearInterpolation = true;
    this->series->lineType = toVector(type);
}  

void pbplotter_serie::use_points(const wchar_t * type){
    this->series->linearInterpolation = false;
    this->series->pointType = toVector(type);
}

void pbplotter_serie::feed(vector<double>& xs, vector<double>& ys){
    this->series->xs = &xs;
    this->series->ys = &ys;
}

void pbplotter_serie::set_line_thickness(int size){
    this->series->lineThickness = size;
}

void pbplotter_serie::set_color(double r, double g, double b){
    this->series->color = CreateRGBColor(r, g, b);
}

void pbplotter_serie::set_color(double r, double g, double b, double a){
    this->series->color = CreateRGBAColor(r, g, b, a);
}

//PBPLOTTER :

pbplotter::pbplotter(int width, int height){
    this->img_ref = CreateRGBABitmapImageReference();
    this->settings = GetDefaultScatterPlotSettings();
    this->settings->width = width;
    this->settings->height = height;
}

pbplotter::~pbplotter(){
    if(this->img_ref->image){
        DeleteImage(this->img_ref->image);
    }
}

void pbplotter::add(pbplotter_serie graph){
    this->settings->scatterPlotSeries->push_back(graph.series);
}

void pbplotter::draw_plot(){
    DrawScatterPlotFromSettings(this->img_ref, this->settings);
}

void pbplotter::save_as(const string& file_name){
    vector<double> * png_data = ConvertToPNG(this->img_ref->image); 
    WriteToFile(png_data, file_name);
}

void pbplotter::show_grid(bool option){
    this->settings->showGrid = option;
}

void pbplotter::set_grid_color(double r, double g, double b){
    this->settings->gridColor = CreateRGBColor(r, g, b);
}

void pbplotter::set_grid_color(double r, double g, double b, double a){
    this->settings->gridColor = CreateRGBAColor(r, g, b, a);
}

void pbplotter::set_title(const wchar_t * title){
    this->settings->title = toVector(title);
}

void pbplotter::set_width(int width){
    this->settings->width = width;
}

void pbplotter::set_height(int height){
    this->settings->height = height;
}

void pbplotter::set_x_label(const wchar_t * title){
    this->settings->xLabel = toVector(title);
}

void pbplotter::set_y_label(const wchar_t * title){
    this->settings->yLabel = toVector(title);
}

void pbplotter::set_boundaries(int x_min, int x_max, int y_min, int y_max){
    this->settings->autoBoundaries = false;
    this->settings->xMin = x_min;
    this->settings->xMax = x_max;
    this->settings->yMin = y_min;
    this->settings->yMax = y_max;
}

void pbplotter::set_padding(int x_padding, int y_padding){
    this->settings->xPadding = x_padding;
    this->settings->yPadding = y_padding;
}

void pbplotter::auto_bounds(){
    this->settings->autoBoundaries = true;
}

void pbplotter::auto_padding(){
    this->settings->autoPadding = true;
}
