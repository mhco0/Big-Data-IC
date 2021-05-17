#ifndef QSBD_K_WISE_FAMILY_H
#define QSBD_K_WISE_FAMILY_H
#include "../commum_header/commum_header.h"
#include "../global_generator/global_generator.h"
#include "../utils/utils.h"
#include "../qsbd_debug/qsbd_debug.h"

namespace qsbd{
	class k_wise_family{
	private:
		using ll = unsigned long long int;

		ll big_prim = 2LL;
		std::vector<ll> constants;
		ll universe;
		int _k;
	public:
		k_wise_family() = default;
		k_wise_family(int k, ll universe);
		k_wise_family(int k, ll universe, const std::vector<ll>& conf);
		k_wise_family(const k_wise_family& rhs);
		~k_wise_family();
		
		ll operator() (ll elem);
		k_wise_family& operator=(const k_wise_family& other);

		std::vector<ll> get_constants() const;
		unsigned long long int get_universe() const;
		int get_k() const;
	};
}
#endif