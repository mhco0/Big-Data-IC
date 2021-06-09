#ifndef QSBD_K_WISE_FAMILY_H
#define QSBD_K_WISE_FAMILY_H
#include "../commum_header/commum_header.h"
#include "../global_generator/global_generator.h"
#include "../utils/utils.h"
#include "../qsbd_debug/qsbd_debug.h"

namespace qsbd{
	class k_wise_family{
	private:
		int big_prim = 2LL;
		std::vector<int> constants;
		int universe;
		int _k;
	public:
		k_wise_family() = default;
		k_wise_family(int k, int universe);
		k_wise_family(int k, int universe, const std::vector<int>& conf);
		k_wise_family(const k_wise_family& rhs);
		~k_wise_family();
		
		int operator() (int elem);
		k_wise_family& operator=(const k_wise_family& other);

		std::vector<int> get_constants() const;
		int get_universe() const;
		int get_k() const;
	};
}
#endif