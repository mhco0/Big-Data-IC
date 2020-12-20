#include "commum_header.h"
#include "global_generator.h"
using namespace std;

template <class T>
class gk{
private:
    multimap<T,pair<int, int>> tuple_list;
    int N;
    double epsilon;

public:
    gk(double eps){
        N = 0;
        epsilon = eps;

        tuple_list.insert({numeric_limits<T>::max(),{1,0}});
    }

    ~gk(){}

    void update(T value){
        N += 1;

        //find smallest i such x[i] > value
        auto up_it = tuple_list.upper_bound(value);

        int weight = (*up_it).second.first;
        int uncertainty = (*up_it).second.second;
        double capacity = (2.0*epsilon*N);

        if((capacity - (weight*1.0 + uncertainty*1.0 + 1.0)) > 1e-8){
            (*up_it).second.first += 1;
        }else{
        
            tuple_list.insert(up_it,{value,{1,weight + uncertainty - 1}});

            for(auto it = tuple_list.begin(),it2 = next(it); it2 != tuple_list.end();it++,it2++){
                if ((capacity - ((*it).second.first*1.0 + (*it2).second.first*1.0 + (*it2).second.second*1.0)) > 1e-8){
                    (*it2).second.first += (*it).second.first;

                    tuple_list.erase(it);

                    break;
                }
            } 
        }
        
        return;
    }

    gk<T> merge(gk<T>& rhs){
        assert(rhs.epsilon == epsilon);

        auto left_it = tuple_list.begin();
        auto right_it = rhs.tuple_list.begin(); 
        gk<T> merged_summary(epsilon);

        while(left_it != tuple_list.end() and right_it != rhs.tuple_list.end()){
            merged_summary.N += 1;

            if((*left_it).first < (*right_it).first){
                T value = (*left_it).first;
                int weight = (*left_it).second.first;
                int uncertainty = (*left_it).second.second + (*right_it).second.first + (*right_it).second.second - 1; 
                
                merged_summary.tuple_list.insert({value,{weight,uncertainty}});

                left_it = next(left_it);
            }else{
                T value = (*right_it).first;
                int weight = (*right_it).second.first;
                int uncertainty = (*right_it).second.second + (*left_it).second.first + (*left_it).second.second - 1; 

                merged_summary.tuple_list.insert({value,{weight,uncertainty}});

                right_it = next(right_it);
            }
        }

        while(left_it != tuple_list.end()){
            merged_summary.N += 1;
            merged_summary.tuple_list.insert(*left_it);

            left_it = next(left_it);
        }

        while(right_it != rhs.tuple_list.end()){
            merged_summary.N += 1;
            merged_summary.tuple_list.insert(*right_it);

            right_it = next(right_it);
        }

        double capacity = 2.0 * epsilon * merged_summary.get_N();
        for(auto it = merged_summary.tuple_list.begin(),it2 = next(it); it2 != merged_summary.tuple_list.end();it++,it2++){
            if ((capacity - ((*it).second.first*1.0 + (*it2).second.first*1.0 + (*it2).second.second*1.0)) > 1e-8){
                (*it2).second.first += (*it).second.first;

                merged_summary.tuple_list.erase(it);
            }
        } 

        return merged_summary;
    }

    double query(T value){
        double weight_sum = 0.0;
        auto it = tuple_list.begin();

        while(value >= (*it).first){
            weight_sum += (*it).second.first;
            it = next(it);
        }

        return weight_sum - 1.0 + (1.0*((*it).second.first + (*it).second.second)/2.0);
    }

    //DEBUG
    void print(){
        for (auto &it : tuple_list){
            cout << "(" << it.first << "," << it.second.first << "," << it.second.second << ") ";
        }
        cout << endl;
    }

    //DEBUG
    int get_N(){
        return N;
    }

};


/*
    Tests tuple insertion on multimap
*/
void test_tuple_insert(){
    multimap<int,pair<int,int>> tuple_list;
    vector<int> input = {1,4,2,8,5,7,6,7,6,7,2,1};

    for(auto&it:input){
        tuple_list.insert({it,{1,10}});
    }

    for(auto&it:tuple_list){
        cout << "(" << it.first << "," << it.second.first << "," << it.second.second << ") ";
    }
    cout << endl;

    auto up_it = tuple_list.upper_bound(7);

    cout << "(" << (*up_it).first << "," << (*up_it).second.first << "," << (*up_it).second.second << ") ";
    cout << endl;

    tuple_list.insert(up_it,{7,{1,0}});

    for(auto&it:tuple_list){
        cout << "(" << it.first << "," << it.second.first << "," << it.second.second << ") ";
    }
    cout << endl;
}

/*
	Makes a random stream with length vector_size using elements from [min_v..max_v]
*/
vector<int> make_random_int_stream(int vector_size, int min_v, int max_v){
	uniform_int_distribution<int> who_pick(min_v,max_v);
	vector<int> random_stream;

	for(int i=0;i<vector_size;i++){
		random_stream.push_back(who_pick(generator));
	}

	return random_stream;
}

/*
	Calculates the rank(x), for a given positive stream
*/
vector<int> ranks_from_stream(vector<int> stream, int min_v,int max_v){
    vector<int> weights(max_v + 1, 0);
    vector<int> ranks(max_v + 1, 0);

    for(int i=0;i<stream.size();i++){
        weights[stream[i]]++;    
	}

    for(int i=0,j=1;j <= max_v;i++,j++){
        ranks[j] = weights[i] + ranks[i];
    }

    for(int i=0;i<ranks.size();i++){
        ranks[i] += weights[i] - 1;
    }

    cout << "Weights : " << endl;
    for(auto&it :weights){
        cout << it << " ";
    }
    cout << endl;

    cout << "Ranks : " << endl;
    for(auto&it:ranks){
        cout << it << " ";
    } 
    cout << endl;

	return ranks;
}

/*
    Prints a simple star histogram using the samples in the arg. The stars are bounded by the scale arg
*/
void histogram_print(const vector<double>& samples, int scale = 1){
    for(int i=0;i<samples.size();i++){
        cout << i << ":\t" << string(floor(samples[i]/scale),'*') << endl;
    }
}

/*
    Test if the ranks_from_stream is working   
*/
void test_real_ranks(){
    vector<int> input = {1,4,2,8,5,7,6,7,6,7,2,1};
    vector<int> querys = {1,2,4,5,6,7,8};
    vector<int> real_rank = {0,2,4,5,6,8,11};
    vector<int> ranks = ranks_from_stream(input,1,8);

    cout << "From function : " << endl;

    for(int i=0;i<ranks.size();i++){
        cout << "rank " << i << ":\t" << ranks[i] << endl;
    }

    cout << "-------------------------" << endl;
    cout << "From paper : " << endl;

    for(int i=0;i<real_rank.size();i++){
        cout <<"rank " << querys[i] << ":\t" << real_rank[i] << endl;
    }
}


/*
    Second test to see if the ranks_from_stream is working   
*/
void test_real_ranks2(int vector_size, int min_v, int max_v){
    vector<int> stream = make_random_int_stream(vector_size,min_v,max_v);  
    vector<int> real_ranks = ranks_from_stream(stream,min_v,max_v);

    sort(stream.begin(),stream.end());

    for(auto& it: stream){
        cout << it << " ";
    }
    cout << endl;
    cout << endl;

    for(int i=0;i<real_ranks.size();i++){
        cout << i << " : " << real_ranks[i] << endl;
    }
    cout << endl;
}

/*
    Tests the make_random_int_stream function
*/
void test_stream_creation(int vector_size, int min_v, int max_v){
    vector<int> stream = make_random_int_stream(vector_size,min_v,max_v);

    for(auto&it : stream){
        cout << it << " ";
    }
    cout << endl;
}

/*
    Tests the merge from two summarys
*/
void test_merge(){
    vector<int> input1 = {1,4,2,8,5,7,6,7,6,7,2,1,3,5,6,9,9,10};
    vector<int> input2 = {10,11,14,12,18,15,17,16,17,16,17,12,11,13,15,16,19,19};
    vector<double> samples(20,0);
    gk<int> summary1(1.0/5.0);
    gk<int> summary2(1.0/5.0);

    for(auto&it:input1){
        summary1.update(it);
    }

    for(int i=0;i<20;i++){
        samples[i] = summary1.query(i);
    }

    cout << "Histogram ranks 1:" << endl;
    histogram_print(samples);

    for(auto&it:input2){
        summary2.update(it);
    }

    for(int i=0;i<20;i++){
        samples[i] = summary2.query(i);
    }

    cout << "Histogram ranks 2:" << endl;
    histogram_print(samples);

    gk<int> merged = summary1.merge(summary2);

    for(int i=0;i<20;i++){
        samples[i] = merged.query(i);
    }

    cout << "Histogram ranks merged:" << endl;
    histogram_print(samples);
}

/*
    Testing updates and querys from the paper
*/
void test_update_and_query(){
    vector<int> input = {1,4,2,8,5,7,6,7,6,7,2,1};
    vector<int> querys = {1,2,4,5,6,7,8};
    vector<int> real_ranks = {0,2,4,5,6,8,11};
    gk<int> summary(1.0/5.0);

    cout << "None" << " " << summary.get_N() << " " << (2 * 0.2 * summary.get_N()) << ": ";
    summary.print();
    cout << endl;

    for(int i=0;i<input.size();i++){
        summary.update(input[i]);
        cout << input[i] << " " << summary.get_N() << " " << (2*0.2*summary.get_N()) << ": ";
        summary.print();
        cout << endl;
    }

    assert(querys.size() == real_ranks.size());

    cout << "x\trank(x)\t~r(x)\terror" << endl;
    for(int i=0;i<querys.size();i++){
        double app_rank = summary.query(querys[i]);
        cout << querys[i] << "\t" << real_ranks[i] << "\t" << app_rank << "\t" << abs(app_rank - real_ranks[i]) << endl; 
    }
}

/*
    Testing the gk summary for int numbers
*/
void test_gk_summary(double epsilon, int vector_size, int min_v, int max_v, bool print){
    vector<int> stream = make_random_int_stream(vector_size,min_v,max_v);
    vector<int> real_ranks = ranks_from_stream(stream,min_v,max_v);
    vector<double> samples(real_ranks.size(),0);
    gk<int> summary(epsilon);

    for(auto &it : stream){
        summary.update(it);
    }

    cout << "gk summary: " << endl;

    summary.print();

    cout << endl;

    cout << "x\trank(x)\t~r(x)\terror\t interval" << endl;
    for(int i=min_v;i<=max_v;i++){
        double app_rank = summary.query(i);
        samples[i] = app_rank;

        cout << i << "\t" << real_ranks[i] << "\t" << app_rank << "\t" << abs(app_rank - real_ranks[i]) << "\t";
        cout << "[" << (app_rank - summary.get_N()*epsilon) << " <= " << real_ranks[i] << " <= " << (app_rank + summary.get_N()*epsilon) << "]" << endl;
        assert((app_rank - summary.get_N()*epsilon) <= real_ranks[i] and real_ranks[i] <= (app_rank + summary.get_N()*epsilon));
    }

    if(print){
        histogram_print(samples,5);
    }
}

/*
	This function process the args for the program and check if they size match.
*/
deque<string> process_args(int argc, char* argv[]){
	deque<string> args;

	for(int i=1;i<argc;i++){
		string arg(argv[i]);

		args.push_back(arg);
	}

	//see how to use test
	bool check_parameters = (args.size() == 1 and (args[0] == "--help" or args[0] == "--test"));
	check_parameters = check_parameters or (args.size() == 4);
	check_parameters = check_parameters or (args.size() == 5 and (args[args.size() - 1] == "--print"));

	assert(check_parameters);

	return args;
}

/*
	Show the help guide.
*/
void help(){
	cout << "gk.exe [--help] [args...]" << endl;
	cout << endl;
	cout << "1o arg (optional): " << endl;
	cout << "=> --help: Show this guide." << endl;
	cout << endl;
	cout << "2o arg (required): " << endl;
	cout << "=> [--help] (optional) : No argument is required." << endl;
	cout << endl;
	cout << "=> epsilon vector_size range_min_value range_max_value [--print]: " << endl;
	cout << "1. epsilon (required) : epsilon error used to the algorithm." << endl;
	cout << "2. vector_size (required) : vector size in the test." << endl;
    cout << "3. range_min_value (required) : the min range for the stream vector (values from [range_min_value..range_max_value])." << endl;
    cout << "4. range_max_value (required) : the max range for the stream vector (values from [range_min_value..range_max_value])." << endl;
	cout << "5. --print (optional) : print the histogram for the gk summary. " << endl;
	cout << endl;
}

int main(int argc, char* argv[]){
    ios::sync_with_stdio(false);
    cin.tie(0);

    auto args = process_args(argc,argv);
    if(args[0] == "--help"){
        help();
    }else if(args[0] == "--test"){
        test_update_and_query();
        //test_tuple_insert();
        //test_stream_creation(100,0,100);
        //test_stream_creation(200,8,8);
        //cout << endl;
        //test_real_ranks();
        //cout << endl;
        //test_real_ranks2(20,8,9);
    }else{
        double epsilon = stod(args[0]);
        int vector_size = stoi(args[1]);
        int min_v = stoi(args[2]);
        int max_v = stoi(args[3]);
        bool print = args.size() == 5;

        test_gk_summary(epsilon,vector_size,min_v,max_v,print);
    }
    return 0;
}