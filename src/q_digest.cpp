#include "commum_header.h"
#include "global_generator.h"
using namespace std;
using ll = long long int;

class q_digest{
private:
	class node{
	private:
		static int ids;
		friend class q_digest;
		int weight;
		int id;
		node * left;
		node * right;

		void private_print(node* rt,int indent = 0){
			if(rt){
				if(rt->right) private_print(rt->right, indent + 4);
		        if (indent) cout << setw(indent) << ' ';
		        if (rt->right) cout<<" /\n" << setw(indent) << ' ';

		        cout<< rt->weight << "\n ";

		        if(rt->left) {
					cout << setw(indent) << ' ' <<" \\\n";
					private_print(rt->left, indent + 4);
		        }
			}
		}

		void private_sub_tree_weights_print(node* rt,int indent,vector<int>& sub_tree_weights){
			if(rt){
				if(rt->right) private_sub_tree_weights_print(rt->right,indent + 4,sub_tree_weights);
		        if (indent) cout << setw(indent) << ' ';
		        if (rt->right) cout<<" /\n" << setw(indent) << ' ';

		        cout<< sub_tree_weights[rt->id] << "\n ";

		        if(rt->left) {
					cout << setw(indent) << ' ' <<" \\\n";
					private_sub_tree_weights_print(rt->left,indent + 4,sub_tree_weights);
		        }
			}
		}

		void dfs(node * rt,vector<bool>& visit,vector<int>& sub_tree_weights){
			if(rt == nullptr) return;

			visit[rt->id] = true;
			sub_tree_weights[rt->id] += rt->weight;

			if(rt->left and not visit[rt->left->id]){
				dfs(rt->left,visit,sub_tree_weights);
			}

			if(rt->right and not visit[rt->right->id]){
				dfs(rt->right,visit,sub_tree_weights);
			}

			if(rt->left) sub_tree_weights[rt->id] += sub_tree_weights[rt->left->id]; 
			if(rt->right) sub_tree_weights[rt->id] += sub_tree_weights[rt->right->id];

			return;
		}

		void delete_tree(node * rt){
			if(not rt) return;

			delete_tree(rt->left);
			delete_tree(rt->right);

			delete rt;
			return;
		}

		void set_subtree_weight_to_zero(node * rt){
			if(not rt) return;

			set_subtree_weight_to_zero(rt->left);
			rt->weight = 0;
			set_subtree_weight_to_zero(rt->right);

			return;
		}

	public:
		node(int weight){
			this->weight = weight;
			this->id = ids;
			this->left = nullptr;
			this->right = nullptr;
			this->ids++;
		}

		~node(){
		}

		bool is_leaf(){
			if(this->left or this->right) return false;
			else return true;
		}

		void print(int indent = 0){
			private_print(this,indent);
		}

		void print_subtree_weights(){
			auto sub_tree_weights = this->get_subtree_weights();
			private_sub_tree_weights_print(this,0,sub_tree_weights);
		}

		vector<int> get_subtree_weights(){
			vector<bool> visit;
			vector<int> sub_tree_weights;

			visit.assign(this->ids,false);
			sub_tree_weights.assign(this->ids,0);

			dfs(this,visit,sub_tree_weights);

			return sub_tree_weights;
		}

		//DEBUG
		friend q_digest tree_on_paper_ex1();
		friend q_digest tree_on_paper_ex2();
	};

	class merge_error:public exception{
	public:
		const char* what() const throw(){
			return "On Q-Digest::merge(): the error and the domain must match";
		}
	};

	vector<pair<int,int>> small_buffer;
	int total_weight;
	int ceil_weight;
	int capacity;
	int universe;
	double error;
	node* root;

	void check_adress(node* t){
		if(not t) return;

		check_adress(t->left);
		check_adress(t->right);

		cout << t << endl;
		return;
	}

	bool in_range(int x,int l,int r){
		return (x >= l and x <= r);
	}

	bool is_leaf(int x,int l,int r){
		return (x == l and x == r);
	}

	void private_compress(node * rt,int debt,vector<int>& sub_tree_weights){
		if (rt == nullptr) return;
		if (rt->weight == 0) return;

		if (rt->is_leaf()){
			rt->weight -= debt;
		}else{

			if((sub_tree_weights[rt->id] - debt) > capacity){
				debt += (capacity - rt->weight);
				rt->weight = capacity;

				//pass the sub-tree weight for the left and the right child

				int left_sub_weight = (rt->left) ? sub_tree_weights[rt->left->id] : 0;

				private_compress(rt->left,min(debt,left_sub_weight),sub_tree_weights);
				private_compress(rt->right,max(debt - left_sub_weight,0),sub_tree_weights);
			}else{
				rt->weight = sub_tree_weights[rt->id] - debt;

				//see after if i can delete this from memory
				rt->set_subtree_weight_to_zero(rt->left);
				rt->set_subtree_weight_to_zero(rt->right);
			}
		}
	}

	void private_merge(node*& new_root,node * tree_to_merge1,node* tree_to_merge2){
		if(tree_to_merge1 == nullptr and tree_to_merge2 == nullptr) return;
		if(tree_to_merge1 != nullptr and tree_to_merge2 != nullptr){
			new_root = new node(tree_to_merge1->weight + tree_to_merge2->weight);

			private_merge(new_root->left,tree_to_merge1->left,tree_to_merge2->left);
			private_merge(new_root->right,tree_to_merge1->right,tree_to_merge2->right);
		}else if(tree_to_merge1 != nullptr){
			new_root = new node(tree_to_merge1->weight);

			private_merge(new_root->left,tree_to_merge1->left,nullptr);
			private_merge(new_root->right,tree_to_merge1->right,nullptr);
		}else{
			new_root = new node(tree_to_merge2->weight);

			private_merge(new_root->left,nullptr,tree_to_merge2->left);
			private_merge(new_root->right,nullptr,tree_to_merge2->right);
		}
		return;
	}

	void private_update(int x,int weight){
		total_weight += weight;
		capacity = (error * total_weight)/log2(universe); 
		node** cur = &root;
		int left_range = 0;
		int right_range = universe - 1;

		while(not this->is_leaf(x,left_range,right_range)){
			int middle  = (left_range + right_range)/2;

			// Check if the node exists, if not we create a new memory adress for it.
			if((*cur) == nullptr){
				(*cur) = new node(0);
			}

			if((*cur)->weight < capacity){
				int debt = min(capacity - (*cur)->weight,weight);

				(*cur)->weight += debt;
				weight -= debt;
			}

			if(weight == 0) break;
			
			if(this->in_range(x,left_range,middle)){
				cur = &(*cur)->left;
				right_range = middle;
			}else{
				cur = &(*cur)->right;
				left_range = middle + 1;
			}
		}

		if((*cur) == nullptr){
			(*cur) = new node(0);
		}

		(*cur)->weight += weight;

		assert(root != nullptr);

		if(total_weight > ceil_weight){
			this->compress();
			ceil_weight = 2 * total_weight;
		}
	}

	int private_query(int x){
		vector<int> sub_tree_weights = root->get_subtree_weights();
		node * cur = root;
		int rank = 0;
		int left_range = 0;
		int right_range = universe - 1;

		while(cur and not this->is_leaf(x,left_range,right_range)){

			int middle = (left_range + right_range)/2;

			if(this->in_range(x,left_range,middle)){
				cur = cur->left;
				right_range = middle;
			}else{
				if(cur->left) rank += sub_tree_weights[cur->left->id];
				cur = cur->right;
				left_range = middle + 1;
			}
		}

		return rank;
	}

	void insert_in_buffer(int x,int weight){
		total_weight += weight;
		small_buffer.push_back({x,weight});

		int index = small_buffer.size()-1;

		for(int i=small_buffer.size()-2; i>=0;i--){
			if (small_buffer[i].first > small_buffer[index].first){
				swap(small_buffer[i],small_buffer[index]);
				index = i;
			}
		}

		/*
		DEBUG PRINT
		for(auto &it :small_buffer){
			cout << it.first << "," << it.second << " ";
		}
		cout << endl;
		*/
	}

	void transfer_buffer_to_tree(){
		for(auto&it:small_buffer){
			private_update(it.first,it.second);
		}

		small_buffer.clear();
	}

	int query_from_buffer(int x){
		if(small_buffer.size() == 0) return 0;

		int rank = 0;

		for(int i=0;i < small_buffer.size() && small_buffer[i].first < x;i++){
			rank += small_buffer[i].second;
		}

		return rank;
	}

public:
	q_digest(double error,int universe){
		this->error = error;
		this->universe = universe;
		this->total_weight = 0;
		this->capacity = 0;
		this->ceil_weight = 0;
		this->root = nullptr;
	}

	//DEBUG
	q_digest(q_digest::node* root,double error,int universe):q_digest(error,universe){
		this->root = root;
		vector<int> sub_tree_weights = this->root->get_subtree_weights();
		this->total_weight = (this->root) ? sub_tree_weights[this->root->id] : 0;
		this->ceil_weight = 2*total_weight;
		this->capacity = (error*total_weight)/log2(universe);
	}

	~q_digest(){
		root->delete_tree(root);
	}

	void print(int indent = 0){
		root->print(indent);
	}

	void print_subtree_weights(){
		root->print_subtree_weights();
	}

	void update(int x,int weight){
		if(total_weight < (log2(universe)/error)){
			insert_in_buffer(x,weight);

			if(total_weight >= (log2(universe)/error)) transfer_buffer_to_tree();
		}else{
			private_update(x,weight);
		}
	}	

	int query(int x){
		if(total_weight < (log2(universe)/error)) return query_from_buffer(x);
		else return private_query(x);
	}

	void compress(){
		vector<int> sub_tree_weights = this->root->get_subtree_weights();
		this->capacity = (error * total_weight)/log2(universe);
		private_compress(this->root,0,sub_tree_weights);
	}

	q_digest merge(q_digest& rhs){
		if (this->universe != rhs.universe or this->error != rhs.error){
			throw merge_error();
		}

		q_digest merged_qdst(this->error,this->universe);

		merged_qdst.total_weight = this->total_weight + rhs.total_weight;
		merged_qdst.capacity = (merged_qdst.error * merged_qdst.total_weight)/log2(merged_qdst.universe);
		
		this->transfer_buffer_to_tree();
		merged_qdst.transfer_buffer_to_tree();

		private_merge(merged_qdst.root,this->root,rhs.root);

		merged_qdst.compress();

		return merged_qdst;
	}

	int weight_total(){
		return total_weight;
	}

	//DEBUG
	friend q_digest tree_on_paper_ex1();
	friend q_digest tree_on_paper_ex2();
	friend void compress_tree_from_ex1();
};

int q_digest::node::ids = 0;

/*
	Calculates the rank(x), for a given stream
*/
int rank_from_stream(vector<pair<int,int>>& stream,int x){
	sort(stream.begin(),stream.end());

	int rank = 0;
	for(int i=0;i<stream.size() and (stream[i].first < x);i++){
		rank += stream[i].second;
	}

	return rank;
}

/*
	Returns the first tree on the SSB Q-Digest ex
*/
q_digest tree_on_paper_ex1(){
	q_digest::node* root = new q_digest::node(2);
	root->left = new q_digest::node(2);
	root->right = new q_digest::node(2);
	root->left->right = new q_digest::node(2);
	root->left->right->left = new q_digest::node(9);
	root->right->left = new q_digest::node(2);
	root->right->right = new q_digest::node(1);
	root->right->left->left = new q_digest::node(1);
	root->right->left->right = new q_digest::node(3);

	q_digest qdst(root,0.25,8);

	return qdst;
}

/*
	Returns the second tree on the SSB Q-Digest ex
*/
q_digest tree_on_paper_ex2(){
	q_digest::node* root = new q_digest::node(3);
	root->left = new q_digest::node(3);
	root->right = new q_digest::node(3);
	root->left->right = new q_digest::node(3);
	root->left->right->left = new q_digest::node(6);
	root->right->left = new q_digest::node(3);
	root->right->right = new q_digest::node(1);
	root->right->left->right = new q_digest::node(2);

	q_digest qdst(root,0.25,8);

	return qdst;
}

/*
	Prints the tree and the subtree sum from ex1
*/
void print_q_digest_tree_ex1(){
	q_digest qdst = tree_on_paper_ex1();

	cout << "Tree: " << endl;

	qdst.print();

	cout << "----------------------------------------" <<endl;
	cout << "Subtree Sum: " << endl;
	qdst.print_subtree_weights();
}

/*
	Prints the tree and the subtree sum from ex2
*/
void print_q_digest_tree_ex2(){
	q_digest qdst = tree_on_paper_ex2();

	cout << "Tree: " << endl;

	qdst.print();

	cout << "----------------------------------------" <<endl;
	cout << "Subtree Sum: " << endl;
	qdst.print_subtree_weights();
}

/*
	Prints all the querys from the tree in the ex1
*/
void test_query_from_ex1(){
	q_digest qdst = tree_on_paper_ex1();

	cout << "Universe [8]:" << endl;
	for(int x=0;x<8;x++){
		cout << "query(" << x << "): ";
		cout << qdst.query(x) << endl;
	}
}

/*
	Test the update method explained in the ex
*/
void test_update_from_ex1(){
	q_digest qdst = tree_on_paper_ex1();

	cout << "Before update: " << endl;
	qdst.print();

	qdst.update(6,1);

	cout << "After update: " << endl;
	qdst.print();
}

/*
	Merge the two trees from the examples and print the result
*/
void test_merge_from_ex1_and_ex2(){
	q_digest qdst1 = tree_on_paper_ex1();
	q_digest qdst2 = tree_on_paper_ex2();

	q_digest merged_qdst = qdst1.merge(qdst2);


	cout << "Merging tree from ex1 & ex2 :" << endl;
	cout << "Tree1 : " << endl;
	qdst1.print();
	cout << endl;
	cout << "Tree2 :" << endl;
	qdst2.print();
	cout << endl;
	cout << "Tree1 merge Tree2 : " << endl; 
	merged_qdst.print();
	cout << endl;
	cout << "Sub tree from merge :" << endl;
	merged_qdst.print_subtree_weights();
	cout << endl;
}

/*
	Test the compress method explained in the ex
*/
void compress_tree_from_ex1(){
	q_digest qdst = tree_on_paper_ex1();

	qdst.capacity = 3;

	cout << "Before : " << endl;
	qdst.print();
	auto subtree_weights = qdst.root->get_subtree_weights();
	qdst.private_compress(qdst.root,0,subtree_weights);
	cout << "After : " << endl;
	qdst.print(); 
}

/*
	Test the update method from my whiteboard
*/
void test_update_from_scratch(){
	vector<pair<int,int>> stream = {{0,5},{1,3},{2,4},{1,4},{4,2},{2,1},{3,5}};
	int total_weight = 0;
	q_digest qdst(0.25,5);

	for(auto&it:stream){
		qdst.update(it.first,it.second);
		total_weight += it.second;
	}

	cout << "From stream : " << endl;

	for(auto&it:stream){
		cout << "(" << it.first << "," << it.second << ")" << endl;
	}
	cout << endl;

	cout << "Tree : " << endl;
	qdst.print();

	cout << "------------" << endl;
	cout << "Subtree sum :" << endl;

	qdst.print_subtree_weights();
	cout << endl;

	cout << "Querys[5] : " << endl;
	for(int i=0;i<5;i++){
		int query = qdst.query(i);
		cout << "query(" << i << "): " << query << endl;
		int real_rank = rank_from_stream(stream,i);
		cout << "Range to approximated rank -> ("<< query << " [r] <= " << real_rank << " [rank(x)] <= " << (query*1.0 + 0.25*qdst.weight_total()) << " [r + e*W] )" << endl; 
	}

	cout << endl;
}

/*
	Makes the stream commands by the args, the stream comes from [vector_size] with number_of_update commands and number_of_query commands.
	The query command goes if a -1 on the first element.
	if the shuffle arg is true, the command to the stream is shuffled.
*/
vector<pair<int,int>> make_stream_commands(int vector_size,int number_of_updates, int number_of_query,bool shuffle = false){
	uniform_int_distribution<int> who_pick(0,vector_size - 1);
	uniform_int_distribution<int> how_many_give(0, 100);
	vector<pair<int,int>> commands_to_stream;


	for(int i=0;i<number_of_updates;i++){
		int random_element = who_pick(generator);
		
		int weight = how_many_give(generator);

		commands_to_stream.push_back({random_element,weight});
	}

	for(int i=0;i<number_of_query;i++){
		int random_element = who_pick(generator);

		// -1 -> mask to say that this is a query command
		commands_to_stream.push_back({-1,random_element});
	}

	if(shuffle) random_shuffle(commands_to_stream.begin(),commands_to_stream.end());

	return commands_to_stream;
}

/*
	Show the help guide.
*/
void help(void){
	cout << "q_digest.exe [--help] [args...]" << endl;
	cout << endl;
	cout << "1o arg (optional): " << endl;
	cout << "=> --help: Show this guide." << endl;
	cout << endl;
	cout << "2o arg (required): " << endl;
	cout << "=> [--help] (optional) : No argument is required." << endl;
	cout << endl;
	cout << "=> epsilon vector_size updates querys [--print-on-query]: " << endl;
	cout << "1. epsilon (required) : epsilon error used to the algorithm." << endl;
	cout << "2. vector_size (required) : vector size used to the algorithm and the vector in the test." << endl;
	cout << "3. updates (required) : number of updates operations used in the vector." << endl;
	cout << "4. querys (required) : number of querys operations used in the vector." << endl;
	cout << "5. --print-on-query (optional) : print the tree from the q_digest algorithm for the current query." << endl;
	cout << endl;
}

/*
	This function process the args for the program and check if they size match.
*/
deque<string> process_args(int argc,char* argv[]){
	deque<string> args;

	for(int i=1;i<argc;i++){
		string arg(argv[i]);

		args.push_back(arg);
	}


	//see how to use test
	bool check_parameters = (args.size() == 1 and (args[0] == "--help" or args[0] == "--test"));
	check_parameters = check_parameters or (args.size() == 4);
	check_parameters = check_parameters or (args.size() == 5 and (args[args.size() - 1] == "--print-on-query"));

	assert(check_parameters);

	return args;
}

/*
	Test the stream creation
*/
void test_stream_creation(int vector_size,int number_of_updates,int number_of_query,bool shuffle){
	vector<pair<int,int>> commands = make_stream_commands(vector_size,number_of_updates,number_of_query,shuffle);
	vector<pair<int,int>> stream;

	cout << "Commands: " << endl;

	for(auto &it:commands){
		if(it.first == -1){
			cout << "Query (" << it.second << ")" << endl;
		}else{
			cout << "Update (" << it.first << "," << it.second << ")" << endl;
			stream.push_back(it);
		}
	}
	cout << endl;

	sort(stream.begin(),stream.end());

	cout << "Stream: " << endl;

	for(auto &it:stream){
		cout << "(" << it.first << "," << it.second << ")" << endl;
	} 

	cout << endl;
}
	
/*
	Makes the test for the Q-Digest algorithm
*/
void test_q_digest(double epsilon,int vector_size,int number_of_updates,int number_of_query,bool shuffle,bool print_tree_on_query){
	vector<pair<int,int>> commands = make_stream_commands(vector_size,number_of_updates,number_of_query,shuffle);
	vector<pair<int,int>> stream;
	q_digest qdst(epsilon,vector_size);

	for(auto &command:commands){
		if(command.first == -1){
			cout << endl;
			cout << "----------------------------------------------------------------" << endl;
			if(print_tree_on_query){
				cout << "Tree Print :" << endl;
				qdst.print();
				cout << endl;

				cout << "Subtree Print : " << endl;
				qdst.print_subtree_weights();
				cout << endl;
			}
			int query = qdst.query(command.second);
			cout << "Query(" << command.second << "): " << query << endl;
			int real_rank = rank_from_stream(stream,command.second);
			cout << "Range to approximated rank -> ("<< query << " [r] <= " << real_rank << " [rank(x)] <= " << (query*1.0 + epsilon*qdst.weight_total()) << " [r + e*W] )" << endl;  
			cout << "----------------------------------------------------------------" << endl;
			cout << endl;
		}else{
			qdst.update(command.first,command.second);
			stream.push_back(command);
		}
	}
}

int main(int argc,char* argv[]){
	ios::sync_with_stdio(false);
	cin.tie(0);

	auto args = process_args(argc,argv);

	if(args[0] == "--help"){
		help();
	}else if(args[0] == "--test"){
		/*cout << "Print tree :" << endl;
		*/
		cout << "----------------------------------------------------------------" << endl;
		print_q_digest_tree_ex1();
		cout << endl;
		cout << "----------------------------------------------------------------" << endl;
		/*cout << endl;
		cout << "Querys :" << endl;
		*/
		cout << "----------------------------------------------------------------" << endl;
		test_query_from_ex1();
		cout << endl;
		cout << "----------------------------------------------------------------" << endl;
		/*cout << endl;
		cout << "Show compression :" << endl; 
		*/
		cout << "----------------------------------------------------------------" << endl;
		compress_tree_from_ex1();
		cout << endl;
		cout << "----------------------------------------------------------------" << endl;
		/*cout << "Update ex :" <<endl;*/
		cout << "----------------------------------------------------------------" << endl;
		test_update_from_ex1();
		cout << endl;
		cout << "----------------------------------------------------------------" << endl;
		/*cout << endl;
		*/
		cout << "----------------------------------------------------------------" << endl;
		test_update_from_scratch();
		cout << endl;
		cout << "----------------------------------------------------------------" << endl;
		/*
		print_q_digest_tree_ex1();
		print_q_digest_tree_ex2();
		test_merge_from_ex1_and_ex2();
		*/
		//test_stream_creation(100,10,5,false);
	}else{
		bool print_tree = (args[args.size()-1] == "--print-on-query");
		bool shuffle = false;

		test_q_digest(stod(args[0]),stoi(args[1]),stoi(args[2]),stoi(args[3]),shuffle,print_tree);
	}

	return 0;
}