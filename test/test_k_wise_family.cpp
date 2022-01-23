#include <gtest/gtest.h>
#include <k_wise_family/k_wise_family.h>
using namespace std;
using namespace qsbd;
using ll = unsigned long long int;

/*
	Test if the Two Wise Family is uniform distribuited
*/
TEST(TwoWiseTest, TestUniformity){
    ll universe = 200;
	ll contra_domain = nextPrime(universe);
	vector<int> constants = {0, 0};
	vector<vector<ll>> counter;

	counter.resize(contra_domain);
	for(int i = 0; i < contra_domain; i++){
		counter[i].assign(contra_domain, 0);
	}

	for(int i = 0; i < contra_domain; i++){
		for(int j = 0; j < contra_domain; j++){
			constants[0] = i;
			constants[1] = j;

			k_wise_family f(2, universe, constants);

			for(int x = 0; x < contra_domain; x++){
				counter[x][f(x)]++;
			}
		}
	}

	ll frequency = counter[0][0];
	for(int i = 0; i < contra_domain; i++){
		for(int j = 0; j < contra_domain; j++){
			if(counter[i][j] != frequency){
				FAIL() << "frequency not match " << counter[i][j];
			}
		}
	}

	EXPECT_NEAR( frequency / (contra_domain * contra_domain * 1.0), (1 / (contra_domain * 1.0)), 1e-6);
}


/*
	Test if the function family provided by the class is two wise
*/
TEST(TwoWiseTest, IsTwoWise){
    ll universe = 10;
	ll contra_domain = nextPrime(universe);
	vector<int> constants = {0, 0};
	vector<vector<vector<vector<ll>>>> counter;

	counter.resize(contra_domain);
	for(int i = 0;i < contra_domain; i++){
		counter[i].resize(contra_domain);
		for(int j = 0; j < contra_domain; j++){
			counter[i][j].resize(contra_domain);
			for(int k = 0; k < contra_domain; k++){
				counter[i][j][k].assign(contra_domain, 0);
			}
		}
	}

	for(int i = 0; i < contra_domain; i++){
		for(int j = 0; j < contra_domain; j++){
			constants[0] = i;
			constants[1] = j;

			k_wise_family f(2, universe, constants);

			for(int x1 = 0; x1 < contra_domain; x1++){
				for(int x2 = 0; x2 < contra_domain; x2++){
					counter[x1][x2][f(x1)][f(x2)]++;
				}
			}
		}
	}

	ll frequency = counter[0][1][0][1];

	for(int x1 = 0; x1 < contra_domain; x1++){
		for(int x2 = 0; x2 < contra_domain; x2++){
			for(int f1 = 0; f1 < contra_domain; f1++){
				for(int f2 = 0; f2 < contra_domain; f2++){
					if(x1 != x2 and frequency != counter[x1][x2][f1][f2]) FAIL() << "Frenquency not match " << counter[x1][x2][f1][f2];
				}
			}
		}
	}

	EXPECT_NEAR(frequency / (contra_domain * contra_domain * 1.0), 1 / (contra_domain * contra_domain * 1.0), 1e-6);
}
