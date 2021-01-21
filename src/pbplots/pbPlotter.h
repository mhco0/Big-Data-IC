#ifndef PBPLOTTER_H
#define PBPLOTTER_H
#include "pbPlots.hpp"
#include "supportLib.hpp"

class pbplotter_serie{
private:
    ScatterPlotSeries * series;

    friend class pbplotter;
public:
    pbplotter_serie(std::vector<double>& xs, std::vector<double>& ys);
    pbplotter_serie();
    ~pbplotter_serie();

    // "solid", "dotted", "dashed", "dotdash", "longdash"
    void use_lines(const wchar_t * type);
    // "crosses", "circles", "dots", "triangles", "filled triangles", "pixels"
    void use_points(const wchar_t * type);
    void feed(std::vector<double>& xs, std::vector<double>& ys);
    void set_line_thickness(int size);
    void set_color(double r, double g, double b);
    void set_color(double r, double g, double b, double a);
};


class pbplotter{
private:
    ScatterPlotSettings * settings;
    RGBABitmapImageReference * img_ref;

public:
    pbplotter(int width, int height);
    ~pbplotter();

    void add(pbplotter_serie serie);
    void draw_plot();
    void save_as(const std::string& file_name);
    void show_grid(bool option);
    void set_grid_color(double r, double g, double b);
    void set_grid_color(double r, double g, double b, double a);
    void set_title(const wchar_t * title);
    void set_width(int width);
    void set_height(int height);
    void set_x_label(const wchar_t * title);
    void set_y_label(const wchar_t * title);
    void set_boundaries(int x_min, int x_max, int y_min, int y_max);
    void set_padding(int x_padding, int y_padding);
    void auto_bounds();
    void auto_padding();
};

#endif