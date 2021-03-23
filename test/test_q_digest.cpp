#include <gtest/gtest.h>
#include <q_digest/q_digest.h>
#include <q_digest_factory/q_digest_factory.h>
using namespace std;
using namespace qsbd;

/*
	Calculates the rank(x), for a given stream
*/
int rank_from_stream(vector<pair<int,int>>& stream, int x){
	sort(stream.begin(), stream.end());

	int rank = 0;
	for(int i = 0; i < stream.size() and (stream[i].first < x); i++){
		rank += stream[i].second;
	}

	return rank;
}

/*
	Prints the tree and the subtree sum from ex1
*//*
void print_q_digest_tree_ex1(){
	q_digest qdst = tree_on_paper_ex1();

	cout << "Tree: " << endl;

	qdst.print();

	cout << "----------------------------------------" <<endl;
	cout << "Subtree Sum: " << endl;
	qdst.print_subtree_weights();
}*/

/*
	Prints the tree and the subtree sum from ex2
*/
/*
void print_q_digest_tree_ex2(){
	q_digest qdst = tree_on_paper_ex2();

	cout << "Tree: " << endl;

	qdst.print();

	cout << "----------------------------------------" <<endl;
	cout << "Subtree Sum: " << endl;
	qdst.print_subtree_weights();
}
*/

/*
	Prints all the querys from the tree in the ex1
*/
/*
void test_query_from_ex1(){
	q_digest qdst = tree_on_paper_ex1();

	cout << "Universe [8]:" << endl;
	for(int x = 0; x < 8; x++){
		cout << "query(" << x << "): ";
		cout << qdst.query(x) << endl;
	}
}*/

/*
	Test the update method explained in the ex
*/
/*
void test_update_from_ex1(){
	q_digest qdst = tree_on_paper_ex1();

	cout << "Before update: " << endl;
	qdst.print();

	qdst.update(6, 1);

	cout << "After update: " << endl;
	qdst.print();
}*/

/*
	Merge the two trees from the examples and print the result
*/
/*
void test_merge_from_ex1_and_ex2(){
	q_digest qdst1 = tree_on_paper_ex1();
	q_digest qdst2 = tree_on_paper_ex2();

	q_digest* merged_qdst = dynamic_cast<q_digest*>(qdst1.merge(qdst2));

	cout << "Merging tree from ex1 & ex2 :" << endl;
	cout << "Tree1 : " << endl;
	qdst1.print();
	cout << endl;
	cout << "Tree2 :" << endl;
	qdst2.print();
	cout << endl;
	cout << "Tree1 merge Tree2 : " << endl; 
	merged_qdst->print();
	cout << endl;
	cout << "Sub tree from merge :" << endl;
	merged_qdst->print_subtree_weights();
	cout << endl;
}*/

/*
	Test the update method from my whiteboard
*/
void test_update_from_scratch(){
	vector<pair<int, int>> stream = {{0, 5}, {1, 3}, {2, 4}, {1, 4}, {4, 2}, {2, 1}, {3, 5}};
	int total_weight = 0;
	q_digest qdst(0.25, 5);

	for(auto& it : stream){
		qdst.update(it.first, it.second);
		total_weight += it.second;
	}

	cout << "From stream : " << endl;

	for(auto& it : stream){
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
	for(int i = 0; i < 5; i++){
		int query = qdst.query(i);
		cout << "query(" << i << "): " << query << endl;
		int real_rank = rank_from_stream(stream, i);
		cout << "Range to approximated rank -> ("<< query << " [r] <= " << real_rank << " [rank(x)] <= " << (query * 1.0 + 0.25 * qdst.weight_total()) << " [r + e * W] )" << endl; 
	}

	cout << endl;
}

/*
	Makes the stream commands by the args, the stream comes from [vector_size] with number_of_update commands and number_of_query commands.
	The query command goes if a -1 on the first element.
	if the shuffle arg is true, the command to the stream is shuffled.
*/
vector<pair<int, int>> make_stream_commands(int vector_size, int number_of_updates, int number_of_query, bool shuffle = false){
	uniform_int_distribution<int> who_pick(0, vector_size - 1);
	uniform_int_distribution<int> how_many_give(0, 100);
	vector<pair<int, int>> commands_to_stream;


	for(int i = 0; i < number_of_updates; i++){
		int random_element = who_pick(generator);
		
		int weight = how_many_give(generator);

		commands_to_stream.push_back({random_element, weight});
	}

	for(int i = 0; i < number_of_query; i++){
		int random_element = who_pick(generator);

		// -1 -> mask to say that this is a query command
		commands_to_stream.push_back({-1, random_element});
	}

	if(shuffle) random_shuffle(commands_to_stream.begin(), commands_to_stream.end());

	return commands_to_stream;
}


TEST(QDigestTest, TestUpdateAndQuery){
    double epsilon = 0.3;
    int vector_size = 1000;
    int number_of_updates = 100;
    int number_of_query = 100;
    bool shuffle = false;
    bool print_tree_on_query = true;
    vector<pair<int,int>> commands = make_stream_commands(vector_size, number_of_updates, number_of_query, shuffle);
	vector<pair<int,int>> stream;
	q_digest qdst(epsilon, vector_size);

	for(auto& command : commands){
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

TEST(QDigestTest, TestFactory){

}