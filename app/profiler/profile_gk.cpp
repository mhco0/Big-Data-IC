#include <stream_maker/stream_maker.h>
#include <utils/utils.h>
#include <gk/gk.hpp>
using namespace std;
using namespace qsbd;

vector<int> ranks_from_stream(vector<int>& stream, int min_v, int max_v){
    vector<int> weights(max_v + 1, 0);
    vector<int> ranks(max_v + 1, 0);

    for(int i = 0; i < stream.size(); i++){
        weights[stream[i]]++;    
	}

    for(int i = 0, j = 1; j <= max_v; i++, j++){
        ranks[j] = weights[i] + ranks[i];
    }

    for(int i = 0; i < ranks.size(); i++){
        ranks[i] += weights[i] - 1;
    }

	return ranks;
}


int main(int argc, char* argv[]){
    deque<string> args = process_args(argc, argv);

    double error = stod(args[0]);
    int stream_size = 100000;
    int attempts = 100;
    vector<int> stream = stream_maker::random_int_stream(stream_size, 0, stream_size - 1);
    vector<int> real_ranks = ranks_from_stream(stream, 0, stream_size - 1);
    vector<int> app_ranks(real_ranks.size(), 0);
    vector<int> fails(real_ranks.size(), 0);

    for(int i = 0; i < attempts; i++){
        gk<int> * test = new gk<int>(error);

        for(int j = 0; j < stream.size(); j++){
            test->update(stream[j]);
        }

        for(int j = 0; j < real_ranks.size(); j++){
            int approx_r = test->query(j);
            int real_r = real_ranks[j];

            app_ranks[j] = approx_r;

            if(fabs(approx_r - real_r) > error * stream_size) fails[j]++;
        }

        delete test;
    }

    if(args.size() == 2){
        if(args[1] == "-diff"){

            for(int i = 0; i < app_ranks.size(); i++){
                cout << i << " " << (double) (app_ranks[i] - error * stream_size) << " " << real_ranks[i] << " " << (double) (app_ranks[i] + error * stream_size) << endl;
            }
        }else if(args[1] == "-fail"){
            for(auto& it : fails){
                cout << it / 100.0 << " " << error << endl;
            }
        }
    }

    return 0;
}