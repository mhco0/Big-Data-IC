#include <stream_maker/stream_maker.h>
#include <utils/utils.h>
#include <dcs/dcs.h>
using namespace std;
using namespace qsbd;

int main(int argc, char* argv[]){
    deque<string> args = process_args(argc, argv);

    double error = stod(args[0]);
    int universe = stod(args[1]);
    int stream_size = 1000;
    int attempts = 100;
    vector<pair<int, int>> stream = stream_maker::random_int_stream_with_weight(stream_size, 0, universe - 1, 1, 50);
    int total_weight = stream_maker::weight_from_stream(stream);
    vector<int> real_ranks = stream_maker::real_ranks_from_stream_with_weight(stream);
    vector<int> app_ranks(real_ranks.size(), 0);
    vector<int> fails(real_ranks.size(), 0);

    for(int i = 0; i < attempts; i++){
        dcs * test = new dcs(error, universe);

        for(int j = 0; j < stream.size(); j++){
            test->update(stream[j].first, stream[j].second);
        }

        for(int j = 0; j < real_ranks.size(); j++){
            int approx_r = test->query(j);
            int real_r = real_ranks[j];

            app_ranks[j] = approx_r;

            if(fabs(approx_r - real_r) > (error * total_weight)) fails[j]++;
        }

        delete test;
    }


    if(args.size() == 3){
        if(args[2] == "-diff"){
            cout << error * total_weight << endl;

            for(int i = 0; i < app_ranks.size(); i++){
                cout << i << " " << (double) (app_ranks[i] - error * total_weight) << " " << real_ranks[i] << " " << (double) (app_ranks[i] + error * total_weight) << endl;
            }
        }else if(args[2] == "-fail"){

            for(int i = 0; i < fails.size(); i++){
                cout << i << " " << fails[i] / (double)(stream.size() * attempts) << " " << (double) (error / log(universe)) << endl;
            }
        }
    }

    return 0;
}