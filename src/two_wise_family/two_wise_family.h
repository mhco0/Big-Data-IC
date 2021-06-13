#ifndef QSBD_TWO_WISE_FAMILY_H
#define QSBD_TWO_WISE_FAMILY_H
#include "../commum_header/commum_header.h"
#include "../global_generator/global_generator.h"
#include "../utils/utils.h"
#include "../qsbd_debug/qsbd_debug.h"

namespace qsbd{
	class two_wise_family{
	private:
		int big_prim = 2;
		int constants[2];
		int universe;
	public:
		two_wise_family() = default;
		two_wise_family(int universe);
		two_wise_family(int universe, const std::vector<int>& conf);
		two_wise_family(const two_wise_family& rhs);
		~two_wise_family();
		
		int operator() (int elem);
		two_wise_family& operator=(const two_wise_family& other);

		std::vector<int> get_constants() const;
		int get_universe() const;
		int get_k() const;
	};
}
#endif