#include <gtest/gtest.h>
#include <q_digest/q_digest.h>
#include <q_digest_factory/q_digest_factory.h>
#include <utils/utils.h>
#include <global_generator/global_generator.h>
#include <stream_maker/stream_maker.h>
using namespace std;
using namespace qsbd;

deque<string> g_args;
// error vector_size #updates #querys

/*
	Calculates the rank(x), for a given stream
*/
int rank_from_stream(vector<pair<int, int>>& stream, int x){
	sort(stream.begin(), stream.end());

	int rank = 0;
	for(int i = 0; i < stream.size() and (stream[i].first < x); i++){
		rank += stream[i].second;
	}

	return rank;
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
TEST(QDigestTest, TestUpdate){
	vector<pair<int, int>> stream = {{0, 5}, {1, 3}, {2, 4}, {1, 4}, {4, 2}, {2, 1}, {3, 5}};
	int total_weight = 0;
	double error = 0.25;
	int universe = 5;
	q_digest qdst(error, universe);

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
	for(int i = 0; i < universe; i++){
		int query = qdst.query(i);
		cout << "query(" << i << "): " << query << endl;
		int real_rank = rank_from_stream(stream, i);
		cout << "Range to approximated rank -> ("<< query << " [r] <= " << real_rank << " [rank(x)] <= " << (query * 1.0 + error * qdst.get_total_weight()) << " [r + e * W] )" << endl; 
		EXPECT_LE(real_rank, (query * 1.0 + error * qdst.get_total_weight()));
		EXPECT_GE(real_rank, query);
	}

	cout << endl;
}

TEST(QDigestTest, TestUpdateAndQuery){
    double epsilon = stod(g_args[0]);
    int vector_size = stoi(g_args[1]);
    int number_of_updates = stoi(g_args[2]);
    int number_of_query = stoi(g_args[3]);
    bool shuffle = false;
    bool print_tree_on_query = false;
    vector<pair<int,int>> commands = make_stream_commands(vector_size, number_of_updates, number_of_query, shuffle);
	vector<pair<int,int>> stream;
	q_digest qdst(epsilon, vector_size);

	DEBUG("OK");

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
			int real_rank = rank_from_stream(stream, command.second);
			cout << "Range to approximated rank -> ("<< query << " [r] <= " << real_rank << " [rank(x)] <= " << (query*1.0 + epsilon * qdst.get_total_weight()) << " [r + e*W] )" << endl; 
			EXPECT_LE(real_rank, (query * 1.0 + epsilon * qdst.get_total_weight()));
			EXPECT_GE(real_rank, query); 
			cout << "----------------------------------------------------------------" << endl;
			cout << endl;
		}else{
			qdst.update(command.first, command.second);
			stream.push_back(command);
		}
	}
}

TEST(QDigestTest, TestFactory){
	double error = stod(g_args[0]);
    int vector_size = stoi(g_args[1]);
    int number_of_updates = stoi(g_args[2]);
    int number_of_query = stoi(g_args[3]);
    bool shuffle = false;
    bool print_tree_on_query = false;
    vector<pair<int,int>> commands = make_stream_commands(vector_size, number_of_updates, number_of_query, shuffle);
	vector<pair<int,int>> stream;

	q_digest_factory factory(error, vector_size);
	q_digest* qdst = dynamic_cast<q_digest*>(factory.instance());

	for(auto& command : commands){
		if(command.first == -1){
			cout << endl;
			cout << "----------------------------------------------------------------" << endl;
			if(print_tree_on_query){
				cout << "Tree Print :" << endl;
				qdst->print();
				cout << endl;

				cout << "Subtree Print : " << endl;
				qdst->print_subtree_weights();
				cout << endl;
			}
			int query = qdst->query(command.second);
			cout << "Query(" << command.second << "): " << query << endl;
			int real_rank = rank_from_stream(stream, command.second);
			cout << "Range to approximated rank -> ("<< query << " [r] <= " << real_rank << " [rank(x)] <= " << (query * 1.0 + error * qdst->get_total_weight()) << " [r + e*W] )" << endl;  
			EXPECT_LE(real_rank, (query * 1.0 + error * qdst->get_total_weight()));
			EXPECT_GE(real_rank, query);
			cout << "----------------------------------------------------------------" << endl;
			cout << endl;
		}else{
			qdst->update(command.first, command.second);
			stream.push_back(command);
		}
	}

	delete qdst;
}


TEST(QDigestTest, TestMerge){
	double epsilon = stod(g_args[0]);
    int stream_size = stoi(g_args[1]);
    bool print_tree_on_query = false;
	vector<pair<int, int>> stream = stream_maker::random_int_stream_with_weight(stream_size, 0, stream_size - 1, 0, 100);
	vector<pair<int, int>> merged_stream = stream_maker::merge_stream(stream, stream);
	vector<int> real_ranks = stream_maker::real_ranks_from_stream_with_weight(stream);
	vector<int> real_ranks_merge = stream_maker::real_ranks_from_stream_with_weight(merged_stream);
	q_digest qdstl(epsilon, stream_size);
	q_digest qdstr(epsilon, stream_size);


	for(auto& it : stream){
		qdstl.update(it.first, it.second);
		qdstr.update(it.first, it.second);
	}

	for(int i = 0; i < real_ranks.size(); i++){
		cout << endl;
		cout << "----------------------------------------------------------------" << endl;
		if(print_tree_on_query){
			cout << "Tree Print :" << endl;
			qdstl.print();
			cout << endl;

			cout << "Subtree Print : " << endl;
			qdstl.print_subtree_weights();
			cout << endl;
		}
		int query = qdstl.query(i);
		cout << "LQuery(" << i << "): " << query << endl;
		int real_rank = real_ranks[i];
		cout << "Range to approximated rank -> ("<< query << " [r] <= " << real_rank << " [rank(x)] <= " << (query * 1.0 + epsilon * qdstl.get_total_weight()) << " [r + e*W] )" << endl; 
		EXPECT_LE(real_rank, (query * 1.0 + epsilon * qdstl.get_total_weight()));
		EXPECT_GE(real_rank, query); 
		cout << "----------------------------------------------------------------" << endl;
		cout << endl;
	}

	for(int i = 0; i < real_ranks.size(); i++){
		cout << endl;
		cout << "----------------------------------------------------------------" << endl;
		if(print_tree_on_query){
			cout << "Tree Print :" << endl;
			qdstr.print();
			cout << endl;

			cout << "Subtree Print : " << endl;
			qdstr.print_subtree_weights();
			cout << endl;
		}
		int query = qdstr.query(i);
		cout << "RQuery(" << i << "): " << query << endl;
		int real_rank = real_ranks[i];
		cout << "Range to approximated rank -> ("<< query << " [r] <= " << real_rank << " [rank(x)] <= " << (query * 1.0 + epsilon * qdstr.get_total_weight()) << " [r + e*W] )" << endl; 
		EXPECT_LE(real_rank, (query * 1.0 + epsilon * qdstr.get_total_weight()));
		EXPECT_GE(real_rank, query); 
		cout << "----------------------------------------------------------------" << endl;
		cout << endl;
	}

	q_digest* merged = dynamic_cast<q_digest*>(qdstl.merge(qdstr));

	merged->print();

	for(int i = 0; i < real_ranks_merge.size(); i++){
		cout << endl;
		cout << "----------------------------------------------------------------" << endl;
		if(print_tree_on_query){
			cout << "Tree Print :" << endl;
			merged->print();
			cout << endl;

			cout << "Subtree Print : " << endl;
			merged->print_subtree_weights();
			cout << endl;
		}
		int query = merged->query(i);
		cout << "MQuery(" << i << "): " << query << endl;
		int real_rank = real_ranks_merge[i];
		cout << "Range to approximated rank -> ("<< query << " [r] <= " << real_rank << " [rank(x)] <= " << (query * 1.0 + epsilon * merged->get_total_weight()) << " [r + e*W] )" << endl; 
		EXPECT_LE(real_rank, (query * 1.0 + epsilon * merged->get_total_weight()));
		EXPECT_GE(real_rank, query); 
		cout << "----------------------------------------------------------------" << endl;
		cout << endl;
	}


	delete merged;
}

TEST(QDigestTest, TestInnerMerge){
	double epsilon = stod(g_args[0]);
    int stream_size = stoi(g_args[1]);
    bool print_tree_on_query = false;
	vector<pair<int, int>> stream = stream_maker::random_int_stream_with_weight(stream_size, 0, stream_size - 1, 0, 100);
	vector<pair<int, int>> merged_stream = stream_maker::merge_stream(stream, stream);
	vector<int> real_ranks = stream_maker::real_ranks_from_stream_with_weight(stream);
	vector<int> real_ranks_merge = stream_maker::real_ranks_from_stream_with_weight(merged_stream);
	q_digest qdstl(epsilon, stream_size);
	q_digest qdstr(epsilon, stream_size);


	for(auto& it : stream){
		qdstl.update(it.first, it.second);
		qdstr.update(it.first, it.second);
	}

	for(int i = 0; i < real_ranks.size(); i++){
		cout << endl;
		cout << "----------------------------------------------------------------" << endl;
		if(print_tree_on_query){
			cout << "Tree Print :" << endl;
			qdstl.print();
			cout << endl;

			cout << "Subtree Print : " << endl;
			qdstl.print_subtree_weights();
			cout << endl;
		}
		int query = qdstl.query(i);
		cout << "LQuery(" << i << "): " << query << endl;
		int real_rank = real_ranks[i];
		cout << "Range to approximated rank -> ("<< query << " [r] <= " << real_rank << " [rank(x)] <= " << (query * 1.0 + epsilon * qdstl.get_total_weight()) << " [r + e*W] )" << endl; 
		EXPECT_LE(real_rank, (query * 1.0 + epsilon * qdstl.get_total_weight()));
		EXPECT_GE(real_rank, query); 
		cout << "----------------------------------------------------------------" << endl;
		cout << endl;
	}

	for(int i = 0; i < real_ranks.size(); i++){
		cout << endl;
		cout << "----------------------------------------------------------------" << endl;
		if(print_tree_on_query){
			cout << "Tree Print :" << endl;
			qdstr.print();
			cout << endl;

			cout << "Subtree Print : " << endl;
			qdstr.print_subtree_weights();
			cout << endl;
		}
		int query = qdstr.query(i);
		cout << "RQuery(" << i << "): " << query << endl;
		int real_rank = real_ranks[i];
		cout << "Range to approximated rank -> ("<< query << " [r] <= " << real_rank << " [rank(x)] <= " << (query * 1.0 + epsilon * qdstr.get_total_weight()) << " [r + e*W] )" << endl; 
		EXPECT_LE(real_rank, (query * 1.0 + epsilon * qdstr.get_total_weight()));
		EXPECT_GE(real_rank, query); 
		cout << "----------------------------------------------------------------" << endl;
		cout << endl;
	}

	qdstl.inner_merge(qdstr);

	for(int i = 0; i < real_ranks_merge.size(); i++){
		cout << endl;
		cout << "----------------------------------------------------------------" << endl;
		if(print_tree_on_query){
			cout << "Tree Print :" << endl;
			qdstl.print();
			cout << endl;

			cout << "Subtree Print : " << endl;
			qdstl.print_subtree_weights();
			cout << endl;
		}
		int query = qdstl.query(i);
		cout << "MQuery(" << i << "): " << query << endl;
		int real_rank = real_ranks_merge[i];
		cout << "Range to approximated rank -> ("<< query << " [r] <= " << real_rank << " [rank(x)] <= " << (query * 1.0 + epsilon * qdstl.get_total_weight()) << " [r + e*W] )" << endl; 
		EXPECT_LE(real_rank, (query * 1.0 + epsilon * qdstl.get_total_weight()));
		EXPECT_GE(real_rank, query); 
		cout << "----------------------------------------------------------------" << endl;
		cout << endl;
	}

}

int main(int argc, char* argv[]){
    testing::InitGoogleTest(&argc, argv);

    g_args = process_args(argc, argv);

    cout << fixed;

    return RUN_ALL_TESTS();
}