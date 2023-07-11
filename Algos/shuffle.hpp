#include <random>
#include <utility>
#include <functional>

#define ADVANCED_RANDOM

#if defined(ADVANCED_RANDOM)
	#include <chrono>
	#define USE_ADVRAND
#endif

namespace algo {

template <class T>
T choose(const T &v1, const T &v2)
{
	#ifndef USE_ADVRAND
	static std::default_random_engine gen;
	
	//static auto prob = std::bind(dist, re);
	#elifdef USE_ADVRAND
	static std::random_device rd;
    std::mt19937::result_type seed = rd() ^ (static_cast<std::mt19937::result_type>
    	(std::chrono::duration_cast<std::chrono::seconds>
    	(std::chrono::system_clock::now().time_since_epoch()).count() +
    	std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()).count() ));

    static std::mt19937 gen(seed); //std::mersenne_twister_engine<>
    #endif
    static std::uniform_int_distribution<uint8_t> dist(0,1);
    static auto prob = std::bind(dist, gen);

	bool choice = prob();
	return choice ? v1 : v2;
}

template <class C>
void merge_subarrays(C &con, C &aux, unsigned lo, unsigned mid, unsigned hi)
{
	for(unsigned k = lo; k <= hi; ++k) aux[k] = con[k];

	unsigned i = lo, j = mid + 1;
	for(unsigned k = lo; k <= hi; ++k)
	{
		if(i > mid) con[k] = aux[j++];
    	else if(j > hi) con[k] = aux[i++];
    	else {
    		auto choice = choose(aux[i], aux[j]);
    		con[k] = choice;
    		choice == aux[i] ? ++i : ++j;
    	}
	}
}

template <typename C>
void shuffle_container(C &con, C &aux, unsigned lo, unsigned hi)
{
	if(hi <= lo) return;
	unsigned mid = lo + (hi - lo) / 2;
	shuffle_container(con, aux, lo, mid);
	shuffle_container(con, aux, mid+1, hi);
	merge_subarrays(con, aux, lo, mid, hi);
}

template <typename C>
C shuffle(const C &con)
{
	C aux(con.size()), cop(con);
	shuffle_container(cop, aux, 0, cop.size() - 1);
	return cop;
}


} // algo