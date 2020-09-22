#include <bits/stdc++.h>


int h_map(int x,int m){
	int a = 1,b = 1;
	// a and b here aren't 1 , they must be random numbers in [1..2^m] choose properly after

	int y = (a*x+b) % (1<<m);
	return y;
}

int main(int argc,char * argv[]){
	std::cin.tie(0);
	std::ios::sync_with_stdio(false);

	return 0;
}