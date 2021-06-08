#include <stream_maker/stream_maker.h>
#include <utils/utils.h>
#include <count_sketch/count_sketch.h>
using namespace std;
using namespace qsbd;

int main(int argc, char* argv[]){
    deque<string> args = process_args(argc, argv);

    double error = stod(args[0]);
    double delta = stod(args[1]);
    int stream_size = 10000;
    int attempts = 100;
    int p_norm = 2;
    vector<pair<int, int>> stream = stream_maker::random_int_stream_with_weight(stream_size, 0, 1000, 1, 50);
    map<int, int> frequency = stream_maker::frequency_counter(stream);
    map<int, double> diffs;
    map<int, int> fails;
    vector<int> norm_vector;
    map<int, int> max_error_prob;

    for(auto& it : frequency){
        norm_vector.push_back(it.second);
    }

    double lnorm = lp_norm(norm_vector, p_norm);

    for(int i = 0; i < attempts; i++){
        vector<pair<int, int>> stream_aux;
        map<int, int> fails_on_loop;
        count_sketch * cs = new count_sketch(error, delta);

        for(int j = 0, k = 1; j < stream.size(); j++){
            stream_aux.push_back(stream[j]);
            cs->update(stream[j].first, stream[j].second);

            if(j == k) {
                map<int, int> frequency_aux = stream_maker::frequency_counter(stream_aux);
                vector<int> norm_vector_aux;

                for(auto& it : frequency_aux){
                    norm_vector_aux.push_back(it.second);
                }

                double lnorm_aux = lp_norm(norm_vector_aux, p_norm);

                for(auto& it : frequency_aux){
                    int approx_f = cs->query(it.first);
                    int real_f = it.second;
                    

                    if(fabs(approx_f - real_f) > (error * lnorm_aux)) fails_on_loop[it.first]++;
                }

                for(auto& it : fails_on_loop){
                    max_error_prob[k] = max_error_prob[k] ? max(max_error_prob[k], fails_on_loop[it.first]) : fails_on_loop[it.first];
                }
                
                k *= 10;
            }
        }

        for(auto& it : frequency){
            int approx_f = cs->query(it.first);
            int real_f = it.second;
            
            diffs[it.first] = diffs[it.first] ? max(diffs[it.first], fabs(approx_f - real_f)) : fabs(approx_f - real_f);

            if(fabs(approx_f - real_f) > (error * lnorm)) fails[it.first]++;
        }

        delete cs;
    }
    
    if(args.size() == 3){
        if(args[2] == "-diff"){
            cout << error * lnorm << endl;

            for(auto& it : diffs){
                cout << it.first << " " << it.second << endl;
            }
        }else if(args[2] == "-fail"){
            for(auto& it : fails){
                cout << it.first << " " << (it.second / (double) attempts) << endl;
            }
        }else if(args[2] == "-fail-max"){
            for(auto& it : max_error_prob){
                cout << it.first << " " << (it.second / (double) attempts) << endl;
            }
        }
    }


    return 0;
}