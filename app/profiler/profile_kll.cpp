#include <stream_maker/stream_maker.h>
#include <utils/utils.h>
#include <kll/kll.hpp>
using namespace std;
using namespace qsbd;

int main(int argc, char* argv[]){
    deque<string> args = process_args(argc, argv);

    double error = stod(args[0]);
    int stream_size = 100000;
    int attempts = 100;
    vector<int> stream = stream_maker::random_int_stream(stream_size, 0, stream_size - 1);
    vector<int> real_ranks = stream_maker::real_ranks_from_stream(stream);
    vector<int> app_ranks(real_ranks.size(), 0);
    vector<int> fails(real_ranks.size(), 0);

    for(int i = 0; i < attempts; i++){
        kll<int> * test = new kll<int>(error);

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