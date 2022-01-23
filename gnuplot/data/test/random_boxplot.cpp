#include<bits/stdc++.h>
using namespace std;

int main(){
	ios::sync_with_stdio(false);
	cin.tie(0);
	size_t quant_sizes = 4;
	random_device dev;
	uniform_real_distribution<double> dist(0.0, 1.0);

	
	for(size_t i = 0; i < 100; i++){
		for (size_t j = 0; j < quant_sizes; j++){
			cout << dist(dev) << " ";
		}
		cout << endl;
	}

	return 0;
}
