#pragma once
#include <vector>

std::vector<unsigned char> ull_to_bytes(unsigned long long int value){
	std::vector<unsigned char> byte_array(sizeof value);

	for(int i=0;i<sizeof value;i++){
		byte_array[sizeof value - 1 - i] = (value >> (i * 8));
	}

	return byte_array;
}

class bob_jenkins_hash{
	
private:

	static unsigned int a;
	static unsigned int b;
	static unsigned int c;

	static void mix(){
		using bjh = bob_jenkins_hash;
		bjh::a -= bjh::b; bjh::a -= bjh::c; bjh::a ^= (bjh::c>>13); 
	    bjh::b -= bjh::c; bjh::b -= bjh::a; bjh::b ^= (bjh::a<<8); 
	    bjh::c -= bjh::a; bjh::c -= bjh::b; bjh::c ^= (bjh::b>>13); 
	    bjh::a -= bjh::b; bjh::a -= bjh::c; bjh::a ^= (bjh::c>>12);  
	    bjh::b -= bjh::c; bjh::b -= bjh::a; bjh::b ^= (bjh::a<<16); 
	    bjh::c -= bjh::a; bjh::c -= bjh::b; bjh::c ^= (bjh::b>>5); 
	    bjh::a -= bjh::b; bjh::a -= bjh::c; bjh::a ^= (bjh::c>>3);  
	    bjh::b -= bjh::c; bjh::b -= bjh::a; bjh::b ^= (bjh::a<<10); 
	    bjh::c -= bjh::a; bjh::c -= bjh::b; bjh::c ^= (bjh::b>>15); 
	}
public:
	bob_jenkins_hash(){
	}
	~bob_jenkins_hash(){
	}

	static unsigned int hash(std::vector<unsigned char> data){
		using bjh = bob_jenkins_hash;
		int len = data.size();
        bjh::a = bjh::b = 0x9e3779b9;
        bjh::c = 0;
        int i = 0;
        while (i + 12 <= len)
        {
            bjh::a += (unsigned int)data[i++] |
                ((unsigned int)data[i++] << 8) |
                ((unsigned int)data[i++] << 16) |
                ((unsigned int)data[i++] << 24);
            bjh::b += (unsigned int)data[i++] |
                ((unsigned int)data[i++] << 8) |
                ((unsigned int)data[i++] << 16) |
                ((unsigned int)data[i++] << 24);
            bjh::c += (unsigned int)data[i++] |
                ((unsigned int)data[i++] << 8) |
                ((unsigned int)data[i++] << 16) |
                ((unsigned int)data[i++] << 24);
            mix();
        }
        bjh::c += (unsigned int) len;
        if (i < len)
            bjh::a += data[i++];
        if (i < len)
           bjh::a += (unsigned int)data[i++] << 8;
        if (i < len)
           bjh::a += (unsigned int)data[i++] << 16;
        if (i < len)
            bjh::a += (unsigned int)data[i++] << 24;
        if (i < len)
            bjh::b += (unsigned int)data[i++];
        if (i < len)
            bjh::b += (unsigned int)data[i++] << 8;
        if (i < len)
            bjh::b += (unsigned int)data[i++] << 16;
        if (i < len)
            bjh::b += (unsigned int)data[i++] << 24;
        if (i < len)
            bjh::c += (unsigned int)data[i++] << 8;
        if (i < len)
            bjh::c += (unsigned int)data[i++] << 16;
        if (i < len)
            bjh::c += (unsigned int)data[i++] << 24;
        mix();
        return bjh::c;
	}
};

unsigned int bob_jenkins_hash::a = 0;
unsigned int bob_jenkins_hash::b = 0;
unsigned int bob_jenkins_hash::c = 0;