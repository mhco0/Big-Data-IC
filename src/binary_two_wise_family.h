#pragma once
#include "commum_header.h"
#include "global_generator.h"

std::vector<unsigned short> binaryExpansion(unsigned long long int y, unsigned long long int expansionSize){
	std::vector<unsigned short> result(expansionSize,0);

	unsigned long long int index = 0ULL;

	while(index < expansionSize){
		if((y & (1ULL << index)))
			result[index] = 1;
		index++;
	}

	return result;
}

unsigned long long int binaryToDecimal(std::vector<unsigned short>& expansion){
	unsigned long long int expansionSize = expansion.size();
	unsigned long long int index = 0ULL;
	unsigned long long int result = 0ULL;
	
	while(index < expansionSize){
		if(expansion[index])
			result += ((1ULL << index));
		index++;
	}

	return result;
}

class binary_two_wise_family{
/*
	The class for two-wise family functions.

	//f:[n] -> [n] 
*/
private:
	unsigned long long int domainBits;
	unsigned long long int rangeBits;
	//
	std::vector<unsigned short> binaryMatrix;
	std::vector<unsigned short> binaryCteVector;
public:

	binary_two_wise_family(){
	}

	binary_two_wise_family(unsigned long long int db, unsigned long long int rb):
	domainBits(db),
	rangeBits(rb) {
		std::uniform_int_distribution<int> distribution(0,1);

		//
		for(unsigned long long int i = 0 ; i < domainBits ; ++i){
			for(unsigned long long int j = 0 ; j < rangeBits ; ++j){
				binaryMatrix.push_back(distribution(generator));
			}
		}
		//
		for(unsigned long long int j = 0 ; j < rangeBits ; ++j){
			binaryCteVector.push_back(distribution(generator));
		}
	}

	unsigned long long int hash(unsigned long long int x){
		using namespace std;
		//write binary expantion of x with domainBits size
		vector<unsigned short> xb = binaryExpansion(x,domainBits);

		//compute result
		vector<unsigned short> result;
		for(unsigned long long int k = 0 ; k < rangeBits ; ++k){
			short aux = 0;
			for(unsigned long long int n = 0 ; n < domainBits ; ++n){
				aux = (aux + binaryMatrix[k*domainBits + n]*xb[n]) % 2;
			}
			result.push_back(aux);
		}
		
		//get back to result
		return binaryToDecimal(result) + 1;
	}
};