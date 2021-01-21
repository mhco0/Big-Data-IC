#include <gtest/gtest.h>
#include <pbplots/pbPlotter.h>
#include <commum_header/commum_header.h>

using namespace std;

TEST(PbPlotterSeriesTest, TestConstructAndMethods){
    random_device generator;
    uniform_real_distribution<double> dist(0.0, 1.0);
    vector<double> xs = {-1, -0.5, 0, 0.5, 1};
    vector<double> ys = {2, 1.5, 1, 1.5, 2};
    vector<int> line_thickness = {1, 2, 3, 4, 5};
    vector<wstring> line_types = {L"solid", L"dotted", L"dashed", L"dotdash", L"longdash"};
    vector<string> name_paths = {"solid", "dotted", "dashed", "dotdash", "longdash", "crosses", "circles", "dots", "triangles", "filled triangles", "pixels"};
    vector<wstring> point_types = {L"crosses", L"circles", L"dots", L"triangles", L"filled triangles", L"pixels"};
    string path = "imgs/pbplots/series_test/";
    int file_index = 0;

    for(int i = 0; i < line_types.size(); i++){
        pbplotter_serie sample;
        pbplotter canvas(500, 500);

        sample.feed(xs, ys);
        
        sample.use_lines(line_types[i].c_str());
        sample.set_line_thickness(line_thickness[i]);
        sample.set_color(dist(generator), dist(generator), dist(generator));

        canvas.add(sample);
        canvas.draw_plot();

        string file_name = path + name_paths[file_index] + ".png";
        canvas.save_as(file_name);
        file_index++;
    }

    for(int i = 0; i < point_types.size(); i++){
        pbplotter_serie sample(xs, ys);
        pbplotter canvas(500, 500);

        sample.use_points(point_types[i].c_str());
        sample.set_color(dist(generator), dist(generator), dist(generator));

        canvas.add(sample);
        canvas.draw_plot();

        string file_name = path + name_paths[file_index] + ".png";
        canvas.save_as(file_name);
        file_index++;
    }
}

TEST(PbPlotterTest, TestDraw){
    vector<double> xs;
    vector<double> ys;

    for(int i = 0; i < 100; i++){
        xs.push_back((double) (i + 1));
        ys.push_back((double) (i + 1));
    }

    pbplotter_serie sample(xs, ys);
    pbplotter canvas(600, 500);

    sample.set_color(1, 0, 0, 0.75);
    sample.use_lines(L"solid");

    canvas.set_title(L"Title test");
    canvas.set_x_label(L"X Test Label");
    canvas.set_y_label(L"Y Test Label");

    canvas.add(sample);

    canvas.draw_plot();

    canvas.save_as("imgs/pbplots/test.png");
}