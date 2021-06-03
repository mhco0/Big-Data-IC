#include "utils.h"
using namespace std;

namespace qsbd{
    // Function that returns true if n  
    // is prime else returns false  
    bool isPrime(int n){  
        // Corner cases  
        if (n <= 1)  return false;  
        if (n <= 3)  return true;  
        
        // This is checked so that we can skip   
        // middle five numbers in below loop  
        if (n%2 == 0 || n%3 == 0) return false;  
        
        for (int i = 5; i*i <= n; i = i + 6)  
            if (n%i == 0 || n%(i+2) == 0)  
            return false;  
        
        return true;  
    }

    // Function to return the smallest 
    // prime number greater than N 
    int nextPrime(int N){ 
    
        // Base case 
        if (N <= 1) 
            return 2; 
    
        int prime = N; 
        bool found = false; 
    
        // Loop continuously until isPrime returns 
        // true for a number greater than n 
        while (!found) { 
            prime++; 
    
            if (isPrime(prime)) 
                found = true; 
        } 
    
        return prime; 
    }

    vector<string> parse_string(const string& sample, const string& delimiter){
        vector<string> values;

        string copy_str(sample);

        size_t pos = 0;

        while(pos = copy_str.find(delimiter), pos != string::npos){
            string pre_str = copy_str.substr(0, pos);

            values.push_back(pre_str);

            copy_str.erase(0, pos + delimiter.length());
        }

        values.push_back(copy_str);

        return values;
    }

    string erase_special_characters(const string& sample, const string& except){
        string copy_str(sample);
        
        copy_str.erase(remove_if(copy_str.begin(), copy_str.end(),
        [&except](char c) {
            bool is_excepiton = false;

            for(auto &it : except){ 
                if(c == it){
                    is_excepiton = true;
                }
            }

            return not (isalpha(c) || isdigit(c) || is_excepiton);
        }), 
        copy_str.end());

        return copy_str;
    }

    deque<string> process_args(int argc, char* argv[]){
        deque<string> args;

        for(int i = 1; i < argc; i++){
            string arg(argv[i]);

            args.push_back(arg);
        }
        
        return args;
    }

    bool ends_with(string const& value, string const& ending){
        if (ending.size() > value.size()) return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    }
    
    int map_coord(double coord, double min_coord, double max_coord, int resolution){
        return ((coord - min_coord) * (1 << resolution)) / (max_coord - min_coord); 
    }

    double map_coord_inv(int coord, double min_coord, double max_coord, int resolution){
        return (((coord * (max_coord - min_coord)) / (1 << resolution)) + min_coord);
    }
}