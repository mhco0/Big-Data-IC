/**
 * @file stream_maker.h
 * @brief QSBD stream_maker namespace
 * @author Marcos H C Oliveira <mhco@cin.ufpe.br>
 * @version 1.0
 *
 */

#ifndef QSBD_STREAM_MAKER_H
#define QSBD_STREAM_MAKER_H
#include "../commum_header/commum_header.h"
#include "../global_generator/global_generator.h"
#include "../qsbd_debug/qsbd_debug.h"

namespace qsbd{
	namespace stream_maker{
		
		/**
		 * @brief This function generates a random stream with weight for it's integer values.
		 * 
		 * This method returns a random stream with evenly distributed values in the range [ @p min_v .. @p max_v ], 
		 * the weigth for all the values is also evenly distributed in the range [ @p min_w .. @p max_w ].  
		 * 
		 * @param vector_size The size of the elements that will be on the stream
		 * @param min_v The minimum value that the values can appear in the stream 
		 * @param max_v The maximum value that the values can appear in the stream
		 * @param min_w The minimum weight for the values in the stream
		 * @param max_w The maximum weight for the values in the stream
		 * @return A random stream made by this rules.
		 * 
		 * @note 
		 * Isn't true that the sum of all weights on stream is equal to @p vector_size .
		 * 
		 * @warning 
		 * If any minimum value is greater than the maximum value, this function will have a undefined behavior.
		*/
		std::vector<std::pair<int, int>> random_int_stream_with_weight(int vector_size, int min_v, int max_v, int min_w, int max_w);
		
		/**
		 * @brief This function generates a random stream of int values with a normal distribution;
		 * 
		 * This method returns a random stream with normal distributed values with standard deviation @p stddev and mean @p mean of size @p vector_size .
		 * 
		 * @param vector_size The size of the stream
		 * @param mean The mean value for the normal distribution
		 * @param stddev The standar deviation for the normal distribution
		 * @param min_value The minimum value in the stream
		 * @param max_value The maximum value in the stream
		 * @return A random stream made by this rules.
		*/
		std::vector<int> normal_int_stream(int vector_size, double mean, double stddev, int min_value, int max_value);
		
		/**
		 * @brief This function generates a random stream with weight for it's integer values.
		 * 
		 * This method returns a random stream with normal distributed values with standard deviation @p stddev and mean @p mean , 
		 * the weigth for all the values is also evenly distributed in the range [ @p min_w .. @p max_w ].  
		 * 
		 * @param vector_size The size of the elements that will be on the stream
		 * @param mean The mean value for the normal distribution 
		 * @param stddev The standard deviation for the normal distribution
		 * @param min_w The minimum weight for the values in the stream
		 * @param max_w The maximum weight for the values in the stream
		 * @return A random stream made by this rules.
		 * 
		 * @note
		 * Isn't true that the sum of all weights on stream is equal to @p vector_size .
		 * 
		 * @warning 
		 * If any minimum value is greater than the maximum value, this function will have a undefined behavior.
		*/
		std::vector<std::pair<int, int>> normal_int_stream_with_weight(int vector_size, double mean, double stddev, int min_w, int max_w);
		
		/**
		 * @brief Sorts a @p stream stream with weight and merges the weights for the values that are the same.
		 * @param stream A stream with weight
		 * @return The same stream but sorted and with the weights merged.
		 * 
		 * @note 
		 * The original stream will be modified.
		*/
		std::vector<std::pair<int, int>> sort_and_merge_weight(std::vector<std::pair<int, int>>& stream);
		
		/**
		 * @brief This function generates a random int stream without weight.
		 * 
		 * This method returns a random stream with evenly distributed values in the range [ @p min_v .. @p max_v ].  
		 * 
		 * @param vector_size The size of the elements that will be on the stream
		 * @param min_v The minimum value that the values can appear in the stream 
		 * @param max_v The maximum value that the values can appear in the stream
		 * @return A random stream made by this rules.
		 * 
		 * @warning 
		 * If any minimum value is greater than the maximum value, this function will have a undefined behavior.
		*/
		std::vector<int> random_int_stream(int vector_size, int min_v, int max_v);
		
		/**
		 * @brief This function returns all the ranks for a given int stream
		 * @param stream A int stream without weight
		 * @return A rank for each element on stream, going from [0..max element on stream]
		 * 
		 * @note 
		 * The stream will be sorted.
		*/
		std::vector<int> real_ranks_from_stream(std::vector<int>& stream);
		
		/**
		 * @brief This function returns all the ranks for a given int stream with weight
		 * @param stream A int stream with weight
		 * @return A rank for each element on stream, going from [0..max element on stream]
		 * 
		 * @note 
		 * The stream will be sorted.
		*/
		std::vector<int> real_ranks_from_stream_with_weight(std::vector<std::pair<int, int>>& stream);
		
		/**
		 * @brief This function returns the total weight for a given stream with weight. 
		 * 
		 * This method can also return the total weight as if all weights are absoluted values.
		 * 
		 * @param stream A stream with weight.
		 * @param absolute A bool flag to say if all weights should be sum as absolute values or not.
		 * @return A int representing the total weight for this stream.
		 * 
		 * @code{.cpp}
		 * #include <stream_maker/stream_maker.h>
		 * using namespace qsbd;
		 * using namespace std;
		 * 
		 * int main(){
		 * 		vector<pair<int, int>> stream = {{1, 3}, {2, 3}, {1, -2}};
		 * 
		 * 		cout << weight_from_stream(stream) << endl; // will ouput 8
		 * 		cout << weight_from_stream(stream, false) << endl; // will ouput 4
		 * }
		 * @endcode
		*/
		int weight_from_stream(std::vector<std::pair<int, int>>& stream, bool absolute = true);
		
		/**
		 * @brief This function maps all points inside a given stream and returns its frequency
		 * @param stream A int stream without weight.
		 * @return A map where the key is the value of the element on the stream and the value is how many time this element appears.
		*/
		std::map<int, int> frequency_counter(std::vector<int>& stream);
		
		/**
		 * @brief This function maps all points inside a given stream and returns its frequency
		 * @param stream A int stream with weight.
		 * @return A map where the key is the value of the element on the stream and the value is how many time this element appears.
		*/
		std::map<int, int> frequency_counter(std::vector<std::pair<int, int>>& stream);
		
		/**
		 * @brief This function calculates the element that represents a given quantile @p quant .
		 * @param stream A int stream with weight
		 * @param total_weight The total weight for the given @p stream
		 * @param quant The quantile to be queried
		 * @return The value in the stream that represents this quantile
		 * 
		 * @note
		 * @p quant should be a number in range [0..1]
		*/
		int quantile(std::vector<std::pair<int, int>>& stream, int total_weight, double quant);
		
		/**
		 * @brief This function generates a random stream of integer values inside a given region in space.
		 * 
		 * This method returns a random stream with evenly distributed values in the range [ @p min_v .. @p max_v ], 
		 * where the value is the first part of the pair, and the second part is the (x, y) position of the value
		 * evenly distributed in the following box :
		 * 
		 * --------------------------------- ( @p maxx , @p maxy )
		 * |													 |
		 * |													 |
		 * |					(Randomx, Randomy)				 |
		 * |													 |
		 * |													 |
		 * ( @p minx , @p miny )  --------------------------------
		 * 
		 * @param vector_size The size of the elements that will be on the stream
		 * @param minx The min x value for the position of the element
		 * @param miny The min y value for the position of the element
		 * @param maxx The max x value for the position of the element
		 * @param maxy The max y value for the position of the element
		 * @param min_v The minimum value that the values can appear in the stream 
		 * @param max_v The maximum value that the values can appear in the stream
		 * @return A random int stream made by this rules 
		 *
		 * @warning 
		 * If any minimum value is greater than the maximum value, this function will have a undefined behavior.
		*/
		std::vector<std::pair<int, std::pair<double, double>>> random_stream_in_region(int vector_size, double minx, double miny, double maxx, double maxy, int min_v, int max_v);
		
		/**
		 * @brief This function generates a random stream of integer values with weight inside a given region in space.
		 * 
		 * This method returns a random stream with evenly distributed values in the range [ @p min_v .. @p max_v ], 
		 * where the value and the weight are the first part of the pair where the weigth for all the values is also evenly distributed in the range [ @p min_w .. @p max_w ],
		 *  and the second part is the (x, y) position evenly distributed in the following box :
		 * 
		 * --------------------------------- ( @p maxx , @p maxy )
		 * |													 |
		 * |													 |
		 * |					(Randomx, Randomy)				 |
		 * |													 |
		 * |													 |
		 * ( @p minx , @p miny )  --------------------------------
		 * 
		 * @param vector_size The size of the elements that will be on the stream
		 * @param minx The min x value for the position of the element
		 * @param miny The min y value for the position of the element
		 * @param maxx The max x value for the position of the element
		 * @param maxy The max y value for the position of the element
		 * @param min_v The minimum value that the values can appear in the stream 
		 * @param max_v The maximum value that the values can appear in the stream
		 * @param min_w The minimum weight for the values in the stream
		 * @param max_w The maximum weight for the values in the stream
		 * @return A random int stream made by this rules
		 * 
		 * @note
		 * Isn't true that the sum of all weights on stream is equal to @p vector_size . 
		 *
		 * @warning 
		 * If any minimum value is greater than the maximum value, this function will have a undefined behavior.
		*/
		std::vector<std::pair<std::pair<int, int>, std::pair<double, double>>> random_stream_in_region_with_weight(int vector_size, double minx, double miny, double maxx, double maxy, int min_v, int max_v, int min_w, int max_w);
		
		/**
		 * @brief This function generates a random stream of integer values inside a given region in space. The method simulates a city distribution.
		 * 
		 * This method returns a random stream with evenly distributed values in the range [ @p min_v .. @p max_v ],
		 * the first part of the pair is the value for the stream and the second part is the (x, y) position in the following box :
		 * 
		 * --------------------------------- ( @p maxx , @p maxy )
		 * |													 |
		 * |													 |
		 * |					(Randomx, Randomy)				 |
		 * |													 |
		 * |													 |
		 * ( @p minx , @p miny )  --------------------------------
		 * 
		 * the points will be distributed as if this streams represents some citys in this region, the number of citys is given
		 * by @p citys . This method tries to subdivides a same amount of @p vector_size points for all the citys where all the points
		 * are bounded by the @p max_radius .
		 * 
		 * @param vector_size The size of the elements that will be on the stream
		 * @param minx The min x value for the position of the element
		 * @param miny The min y value for the position of the element
		 * @param maxx The max x value for the position of the element
		 * @param maxy The max y value for the position of the element
		 * @param min_v The minimum value that the values can appear in the stream 
		 * @param max_v The maximum value that the values can appear in the stream
		 * @param citys The number of citys in the stream
		 * @param max_radius The max radius for points in a given city
		 * @return A random int stream made by this rules
		 * 
		 * @note
		 * Isn't true that the sum of all weights on stream is equal to @p vector_size . 
		 *
		 * @warning 
		 * If any minimum value is greater than the maximum value, this function will have a undefined behavior.
		 * 
		 * @warning 
		 * Given a @p citys equal to zero will cause a runtime error.
		*/
		std::vector<std::pair<int, std::pair<double, double>>> random_stream_city(int vector_size, double minx, double miny, double maxx, double maxy, int min_v, int max_v, int citys, double max_radius);
		
		/**
		 * @brief This function generates a random stream of integer values with weight inside a given region in space. The method simulates a city distribution.
		 * 
		 * This method returns a random stream with evenly distributed values in the range [ @p min_v .. @p max_v ],
		 * where the value and the weight are the first part of the pair where the weigth for all the values is also evenly distributed in the range [ @p min_w .. @p max_w ],
		 *  and the second part is the (x, y) position in the following box :
		 * 
		 * --------------------------------- ( @p maxx , @p maxy )
		 * |													 |
		 * |													 |
		 * |					(Randomx, Randomy)				 |
		 * |													 |
		 * |													 |
		 * ( @p minx , @p miny )  --------------------------------
		 * 
		 * the points will be distributed as if this streams represents some citys in this region, the number of citys is given
		 * by @p citys . This method tries to subdivides a same amount of @p vector_size points for all the citys where all the points
		 * are bounded by the @p max_radius .
		 * 
		 * @param vector_size The size of the elements that will be on the stream
		 * @param minx The min x value for the position of the element
		 * @param miny The min y value for the position of the element
		 * @param maxx The max x value for the position of the element
		 * @param maxy The max y value for the position of the element
		 * @param min_v The minimum value that the values can appear in the stream 
		 * @param max_v The maximum value that the values can appear in the stream
		 * @param min_w The minimum weight for the values in the stream
		 * @param max_w The maximum weight for the values in the stream
		 * @param citys The number of citys in the stream
		 * @param max_radius The max radius for points in a given city
		 * @return A random int stream made by this rules
		 * 
		 * @note
		 * Isn't true that the sum of all weights on stream is equal to @p vector_size . 
		 *
		 * @warning 
		 * If any minimum value is greater than the maximum value, this function will have a undefined behavior.
		 * 
		 * @warning 
		 * Given a @p citys equal to zero will cause a runtime error.
		*/
		std::vector<std::pair<std::pair<int, int>, std::pair<double, double>>> random_stream_city_with_weight(int vector_size, double minx, double miny, double maxx, double maxy, int min_v, int max_v, int min_w, int max_w, int citys, double max_radius);
		
		/**
		 * @brief This function givens a random rectangle inside a given bound.
		 * 
		 * All points given by this method are evenly distributed over the x and y range. That means that
		 * we choose four points (x0, y0) .. (x3, y3) so that xi is choose for a uniform range [ @p minxx .. @p maxx ] and yi in a uniform range [ @p miny .. @p maxy ]
		 * 
		 * We can have any return inside this box :
		 * 
		 * --------------------------------- ( @p maxx , @p maxy )
		 * |		|-------------------(possiblex2, possibley2) |
		 * |		|										   | |
		 * |		|							 			   | |
		 * |		(possiblex1, possibley1)-------------------| |
		 * |													 |
		 * ( @p minx , @p miny )  --------------------------------
		 * 
		 * @param minx The min x bound
		 * @param miny The min y bound
		 * @param maxx The max x bound
		 * @param maxy The max y bound
		 * @return A random rectangle in this area where (possiblex1, possibley1) are the first part 
		 * of the return and (possiblex2, possibley2) are the second part 
		*/
		std::pair<std::pair<double, double>, std::pair<double, double>> random_rectangle_in_region(double minx, double miny, double maxx, double maxy);


		/**
		 * @brief A method to merge two separeted streams @p stream1 and @p stream2. Both of the same type
		 * 
		 * 	This method goes by the begin of the two vectors and add the element point by it than increments
		 * 	their pointers until one of the streams ends. If some stream has more elements than the other, this 
		 * 	elements will appear at the end of the merged stream.
		 * 
		 * @tparam T The type for both streams
		 * @param stream1 The first stream
		 * @param stream2 The second stream
		 * @return A new stream as the result of merging @p stream1 and @p stream2
		 * 
		 * @code{.cpp}
		 * #include <stream_maker/stream_maker.h>
		 * using namespace qsbd;
		 * using namespace std;
		 * 
		 * int main(){
		 * 		vector<int> s1 = {1, 2, 3};
		 * 		vector<int> s2 = {4, 5, 6, 7};
		 * 
		 * 		auto ms = merge_stream<int>(s1, s2); // or merge_stream(s1, s2);
		 * 
		 * 		for(auto& it : ms){
		 * 			cout << it << " ";
		 * 		}
		 * 		cout << endl; // will output : 1 4 2 5 3 6 7 
		 * }
		 * @endcode
		*/
		template<class T>
		std::vector<T> merge_stream(std::vector<T>& stream1, std::vector<T>& stream2){
			std::vector<T> merged_stream;
			int i = 0;
			int j = 0;

			while(i < stream1.size() && j < stream2.size()){
				merged_stream.push_back(stream1[i]);
				merged_stream.push_back(stream2[j]);
				i++;
				j++;
			}

			while(i < stream1.size()){
				merged_stream.push_back(stream1[i]);
				i++;
			}

			while(j < stream2.size()){
				merged_stream.push_back(stream2[j]);
				j++;
			}

			return merged_stream;
		}

		/**
		 * @brief outputs @p stream in the std::cout 
		 * @param stream a int stream without weight
		*/
		void show_stream(std::vector<int>& stream);

		/**
		 * @brief outputs @p stream in the std::cout
		 * @param stream a int stream with weight 
		*/
		void show_stream(std::vector<std::pair<int, int>>& stream);
	}
}

#endif
