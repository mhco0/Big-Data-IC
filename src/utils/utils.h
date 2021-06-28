/**
 * @file utils.h
 * @brief QSBD functions that may be utils for the user
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */

#ifndef QSBD_UTILS_H
#define QSBD_UTILS_H
#include <algorithm>
#include <string>
#include <vector>
#include <deque>
#include <stdint.h>
#include <cmath>

namespace qsbd{
	/**
	 * @brief This function calculate if a given number @p n is prime
	 * 
	 * @param n A int number
	 * @return true if the @p n is prime, and false otherwise.
	*/
	bool isPrime(int n);
	
	/**
	 * @brief This function calculates the next prime greater than @p N
	 * 
	 * @param N A int number where we start to look for the next prime number
	 * @return A prime greater than @p N
	 * 
	*/
	int nextPrime(int N);
	
	/**
	 * @brief This function separates the string @p sample by any @p delimiter
	 * 
	 * The function will copy the @p sample string and separates it in new strings.
	 * The cut will be delimited by @p delimiter 
	 * 
	 * @param sample A string to be parsed
	 * @param delimiter A demiliter for the string @p sample
	 * @return A vector of strings 
	 * 
	 * @code{.cpp}
	 * #include <utils/utils.h>
	 * using namespace qsbd;
	 * using namespace std;
	 * 
	 * int main(){
	 * 		string t1 = "hello world!";
	 * 		string t2 = "hello.world.again";
	 * 
	 * 		auto vt1 = parse_string(t1); // {"hello", "world!"}
	 * 		auto vt2 = parse_string(t2, "."); {"hello", "world", "again"}
	 * }
	 * @endcode
	*/
	std::vector<std::string> parse_string(const std::string& sample, const std::string& delimiter = " ");
	
	/**
	 * @brief Erase any character in a given string that isn't a alphabetic character or a digit in the string
	 * 
	 * @param sample A string to get the special characters erase
	 * @param except Some characters that should be also a exception
	 * @return The string after erase special characters
	 * 
	 * @sa isalpha(char);
	 * @sa isdigit(char);
	 * 
	 * @code{.cpp}
	 * #include <utils/utils.h>
	 * #include <iostream>
	 * using namespace qsbd;
	 * using namespace std;
	 * 
	 * int main(){
	 * 		string t1 = "hello world!1";
	 * 
	 * 		cout << erase_special_characters(t1); // will ouput "helloworld1"
	 * 		cout << erase_special_characters(t1, "! ") // will ouput "hello world!1"
	 * }
	 * @endcode
	*/
	std::string erase_special_characters(const std::string& sample, const std::string& except = {});
	
	/**
	 * @brief This function takes the program argc and argv format and transforms it in a deque of strings
	 * 
	 * This a helper function, this justs takes the program name from the argv format and trasforms
	 * all char* in strings.
	 * 
	 * @param argc The number of arguments for the program
	 * @param argv The arguments for the program
	 * @return A deque with all @p argv as strings, except the program name
	 * 
	*/
	std::deque<std::string> process_args(int argc, char* argv[]);
	
	/**
	 * @brief This function verifys with some string @p value ends with other string @p ending
	 * 
	 * @param value A string to be tested
	 * @param ending A string that represents the end that wee want to test.
	 * @return true if @p value ends with @p ending , false otherwise
	 * 
	*/
	bool ends_with(std::string const& value, std::string const& ending);
	
	/**
	 * @brief This function maps some double coordenate into a int coordenate
	 * 
	 * We subdivide the space based on the @p resolution  given, imagine that we are
	 * subdividing a space that goes from @p min_coord = 0.0 to  @p max_coord = 10.0, if
	 * we choose @p resolution equals to 2, we gonna have 2^2 = 4 possible spaces in int coordenates.
	 * 
	 * 0.0--------)2.5--------)5.0--------)7.5--------)10.0 
	 * 
	 * [0.0 .. 2.5) = 0
	 * [2.5 .. 5.0) = 1
	 * [5.0 .. 7.5) = 2
	 * [7.5 .. 10.0) = 3
	 * 
	 * So, if we give a coordenate @p coord  = 3.5 that coordenate will be mapped in a int coordenated
	 * equal to 1.
	 * 
	 * @param coord The double coordenate to be mapped
	 * @param min_coord The minimum coordenate that can appear in the space
	 * @param max_coord The maximum coordenate that can appear in the space
	 * @param resolution The amout of subdivisions in the space
	 * @return The coordenate @p coord mapped in a int space
	 * 
	 * @note
	 * This can be use to map double coordenates into a quantile quadtree. @sa quantile_quadtree
	*/
	int map_coord(double coord, double min_coord, double max_coord, int resolution);
	
	/**
	 * @brief This function maps some int coordenate into a double coordenate
	 * 
	 * We subdivide the space based on the @p resolution  given, imagine that we are
	 * subdividing a space that goes from @p min_coord = 0.0 to  @p max_coord = 10.0, if
	 * we choose @p resolution equals to 2, we gonna have 2^2 = 4 possible spaces in int coordenates.
	 * 
	 * 0.0--------)2.5--------)5.0--------)7.5--------)10.0 
	 * 
	 * [0.0 .. 2.5) = 0
	 * [2.5 .. 5.0) = 1
	 * [5.0 .. 7.5) = 2
	 * [7.5 .. 10.0) = 3
	 * 
	 * So, if we give a coordenate @p coord = 1 that means that the double coordenated should be in range [2.5 .. 5.0),
	 * so we return the lower bound of this range.
	 * 
	 * @param coord The int coordenate to be mapped
	 * @param min_coord The minimum coordenate that can appear in the space
	 * @param max_coord The maximum coordenate that can appear in the space
	 * @param resolution The amout of subdivisions in the space
	 * @return The coordenate @p coord mapped in a double space
	 * 
	*/
	double map_coord_inv(int coord, double min_coord, double max_coord, int resolution);

	/**
	 * @brief This function inserts a element @p data into a vector @p vec and keeps it sorted.
	 * 
	 * @tparam T The type of a vector
	 * @param vec A sorted vector
	 * @param data The element to be inserted on the vector
	 *
	 * @warning
	 * The @p vec should be sorted.
	*/
	template<class T>
	void insert_sorted(std::vector<T>& vec, const T& data){
		// The vector must be sorted

		typename std::vector<T>::iterator insert_position;

		insert_position = std::lower_bound(vec.begin(), vec.end(), data);
		vec.insert(insert_position, data);

		return;
	}

	/**
	 * @brief This function calculate the p-norm of a vector and returns it to the user
	 * 
	 * The p-norm of a vector x = {x1, x2, ... xn} is defined as
	 *  
	 * \f$ ||x||_{p} = (|x_{1}|^p + |x_{2}|^p ... + |x_{n}|^p)^{p^{-1}} \f$ 
	 * 
	 * @tparam T The type of a vector. This can be any type that is used by std::pow
	 * @param vec A vector to calculate the p-norm
	 * @param norm The norm to be calculated. This is the 'p' in the definition
	 * @return The p-norm of @p vec
	 * 
	 * @warning
	 * The @p norm should be greater or equal to zero.
	*/
	template<class T>
	double p_norm(std::vector<T>& vec, const int& norm){
		double sum = 0.0;

		for(auto& it : vec){
			sum += pow(abs(it), norm);
		}

		if(norm > 1) return pow(sum, 1.0 / norm);
		else return sum;
	}
}

#endif