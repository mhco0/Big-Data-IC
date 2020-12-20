#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class node{
public:
	int value;

	node* next;

	node(int val){
		next = nullptr;

		value = val;
	}

	~node(){
	}
};

class alloc_memory{
protected:
	node * head;
	int logic;

	bool _delete_to_first(){
		node * cur = head->next;

		if(cur == nullptr) return true;

		node * cur_next = cur->next;

		head->next = cur_next;

		delete cur;

		return false;
	}
public:

	alloc_memory(){
		head = new node(-0x3f3f3f3f);
		logic = 0;
	}

	~alloc_memory(){
		while(not _delete_to_first());

		delete head;
	}

	void print_list(){
		for(node * cur = head->next;cur != nullptr; cur = cur->next){
			cout << cur->value << " ";
		}
		cout << endl;

		return;
	}
};

class alloc_memory_from_p : public alloc_memory{
public:	
	alloc_memory_from_p():alloc_memory(){
	}

	~alloc_memory_from_p(){
	}

	void insert(int v){
		node * cur = head;

		while(cur->next != nullptr){
			cur = cur->next;
		}

		cur->next = new node(v);

		logic++;

		return;
	}
};

class alloc_memory_from_pp : public alloc_memory{
public:

	alloc_memory_from_pp():alloc_memory(){
	}

	~alloc_memory_from_pp(){
	}

	void insert(int v){
		node ** cur = &head->next;

		while((*cur) != nullptr){
			cur = &(*cur)->next; 
		}

		(*cur) = new node(v);

		logic++;

		return;
	}
};

class not_alloc_memory : public alloc_memory{
public:
	not_alloc_memory():alloc_memory(){
	}
	
	~not_alloc_memory(){
	}
	
	void insert(int v){
		node * cur = head->next;

		while(cur != nullptr){
			cur = cur->next; 
		}

		cur = new node(v);

		logic++;

		return;
	}
};

int main(void){
	vector<int> input_v{9,2,3,1,2,5,6,7,8};

	alloc_memory_from_p p;
	alloc_memory_from_pp pp;
	not_alloc_memory np;

	for(auto&it:input_v){
		p.insert(it);
		pp.insert(it);
		np.insert(it);
	}

	cout << "-" << endl;
	p.print_list();
	cout << "-" << endl;
	pp.print_list();
	cout << "-" << endl;
	np.print_list();
	cout << "-" << endl;
}