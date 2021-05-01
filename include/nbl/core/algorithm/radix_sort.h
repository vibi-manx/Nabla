// Copyright (C) 2018-2020 - DevSH Graphics Programming Sp. z O.O.
// This file is part of the "Nabla Engine".
// For conditions of distribution and use, see copyright notice in nabla.h

#ifndef __NBL_CORE_RADIX_SORT_H_INCLUDED__
#define __NBL_CORE_RADIX_SORT_H_INCLUDED__

#include <algorithm>
#include <bitset>
#include <cstdint>
#include <numeric>

#include "nbl/macros.h"

namespace nbl
{
namespace core
{

namespace impl
{

template<typename T>
struct KeyAdaptor
{
	static_assert(std::is_integral_v<T>&&std::is_unsigned_v<T>,"Need to use your own key value accessor.");
	_NBL_STATIC_INLINE_CONSTEXPR size_t key_bit_count = sizeof(T)*8u;

	template<auto bit_offset, auto radix_mask>
	inline decltype(radix_mask) operator()(const T& item) const
	{
		return static_cast<decltype(radix_mask)>(item>>static_cast<T>(bit_offset))&radix_mask;
	}
};

template<typename T>
constexpr int8_t find_msb(const T& a_variable)
{
    static_assert(std::is_unsigned<T>::value, "Variable must be unsigned");

    constexpr uint8_t number_of_bits = std::numeric_limits<T>::digits;
    const std::bitset<number_of_bits> variable_bitset{a_variable};

    for (uint8_t msb = number_of_bits - 1; msb >= 0; msb--)
    {
        if (variable_bitset[msb] == 1)
            return msb;
    }        
    return -1;
}

template<size_t key_bit_count, typename histogram_t>
struct RadixSorter
{
		_NBL_STATIC_INLINE_CONSTEXPR uint16_t histogram_bytesize = 8192u;
		_NBL_STATIC_INLINE_CONSTEXPR size_t histogram_size = size_t(histogram_bytesize)/sizeof(histogram_t);
		_NBL_STATIC_INLINE_CONSTEXPR uint8_t radix_bits = find_msb(histogram_size);
		_NBL_STATIC_INLINE_CONSTEXPR size_t last_pass = (key_bit_count-1ull)/size_t(radix_bits);
		_NBL_STATIC_INLINE_CONSTEXPR uint16_t radix_mask = (1u<<radix_bits)-1u;

		enum class PassMod { First, Default, Last };

		template<class RandomIt, class KeyAccessor, size_t pass_ix, PassMod pass_mod>
		struct Pass
		{
			inline RandomIt operator()(RandomIt input, RandomIt output, const histogram_t rangeSize, const KeyAccessor& comp, histogram_t (&histogram_front)[histogram_size], histogram_t(&histogram_back)[histogram_size]);
		};

		template<class RandomIt, class KeyAccessor, size_t pass_ix>
		struct Pass<RandomIt, KeyAccessor, pass_ix, PassMod::Default>
		{
			inline RandomIt operator()(RandomIt input, RandomIt output, const histogram_t rangeSize, const KeyAccessor& comp, histogram_t(&histogram_front)[histogram_size], histogram_t(&histogram_back)[histogram_size]) {
				constexpr histogram_t shift = static_cast<histogram_t>(radix_bits * pass_ix);
				std::fill_n(histogram_front, histogram_size, static_cast<histogram_t>(0u));
				std::inclusive_scan(histogram_back, histogram_back + histogram_size, histogram_back);
				for (histogram_t i = rangeSize; i != 0u;)
				{
					i--;
					++histogram_front[comp.operator() < shift, radix_mask > (input[(rangeSize - 1) - i])];
					output[--histogram_back[comp.operator() < shift, radix_mask > (input[i])]] = input[i];
				}

				if constexpr (pass_ix != last_pass)
					return Pass<RandomIt, KeyAccessor, pass_ix + 1ull, PassMod::Default>{}(output, input, rangeSize, comp, histogram_back, histogram_front);
				else
					return Pass<RandomIt, KeyAccessor, pass_ix, PassMod::Last>{}(input, output, rangeSize, comp, histogram_back, histogram_front);
			}
		};

		template<class RandomIt, class KeyAccessor, size_t pass_ix>
		struct Pass<RandomIt, KeyAccessor, pass_ix, PassMod::First>
		{
			inline RandomIt operator()(RandomIt input, RandomIt output, const histogram_t rangeSize, const KeyAccessor& comp, histogram_t(&histogram_front)[histogram_size], histogram_t(&histogram_back)[histogram_size]) {
				constexpr histogram_t shift = static_cast<histogram_t>(radix_bits * pass_ix);
				std::fill_n(histogram_front, histogram_size, static_cast<histogram_t>(0u));
				for (histogram_t i = 0u; i < rangeSize; i++)
					++histogram_front[comp.operator() < shift, radix_mask > (input[i])];
				if constexpr (pass_ix != last_pass)
					return Pass<RandomIt, KeyAccessor, pass_ix + 1ull, PassMod::Default>{}(output, input, rangeSize, comp, histogram_back, histogram_front);
				else 
					return Pass<RandomIt, KeyAccessor, pass_ix, PassMod::Last>{}(input, output, rangeSize, comp, histogram_front, histogram_back);
			}
		};

		template<class RandomIt, class KeyAccessor, size_t pass_ix>
		struct Pass<RandomIt, KeyAccessor, pass_ix, PassMod::Last>
		{
			inline RandomIt operator()(RandomIt input, RandomIt output, const histogram_t rangeSize, const KeyAccessor& comp, histogram_t(&histogram_front)[histogram_size], histogram_t(&histogram_back)[histogram_size]) {
				constexpr histogram_t shift = static_cast<histogram_t>(radix_bits * pass_ix);
				std::inclusive_scan(histogram_front, histogram_front + histogram_size, histogram_front);
				for (histogram_t i = rangeSize; i != 0u;)
				{
					i--;
					output[--histogram_front[comp.operator() < shift, radix_mask > (input[i])]] = input[i];
				}
				return output;
			}
		};

		template<class RandomIt, class KeyAccessor>
		inline RandomIt operator()(RandomIt input, RandomIt output, const histogram_t rangeSize, const KeyAccessor& comp)
		{
			return Pass<RandomIt,KeyAccessor,0ull,PassMod::First>{}(input, output, rangeSize, comp, histogram[0], histogram[1]);
		}
	private:
		/*template<class RandomIt, class KeyAccessor, size_t pass_ix>
		inline RandomIt pass(RandomIt input, RandomIt output, const histogram_t rangeSize, const KeyAccessor& comp)
		{
			// clear
			std::fill_n(histogram,histogram_size,static_cast<histogram_t>(0u));
			// count
			constexpr histogram_t shift = static_cast<histogram_t>(radix_bits*pass_ix);
			for (histogram_t i=0u; i<rangeSize; i++)
				++histogram[comp.operator()<shift,radix_mask>(input[i])];
			// prefix sum
			std::inclusive_scan(histogram,histogram+histogram_size,histogram);
			// scatter
			for (histogram_t i=rangeSize; i!=0u;)
			{
				i--;
				output[--histogram[comp.operator()<shift,radix_mask>(input[i])]] = input[i];
			}

			if constexpr (pass_ix != last_pass)
				return pass<RandomIt,KeyAccessor,pass_ix+1ull>(output,input,rangeSize,comp);
			else
				return output;
		}*/
	
		alignas(sizeof(histogram_t)) histogram_t histogram[2][histogram_size];
};

}

template<class RandomIt, class KeyAccessor>
inline RandomIt radix_sort(RandomIt input, RandomIt scratch, const size_t rangeSize, const KeyAccessor& comp)
{
	assert(std::abs(std::distance(input,scratch))>=rangeSize);

	if (rangeSize<static_cast<decltype(rangeSize)>(0x1ull<<16ull))
		return impl::RadixSorter<KeyAccessor::key_bit_count,uint16_t>()(input,scratch,static_cast<uint16_t>(rangeSize),comp);
	if (rangeSize<static_cast<decltype(rangeSize)>(0x1ull<<32ull))
		return impl::RadixSorter<KeyAccessor::key_bit_count,uint32_t>()(input,scratch,static_cast<uint32_t>(rangeSize),comp);
	else
		return impl::RadixSorter<KeyAccessor::key_bit_count,size_t>()(input,scratch,rangeSize,comp);
}

//! Because Radix Sort needs O(2n) space and a number of passes dependant on the key length, the final sorted range can be either in `input` or `scratch`
template<class RandomIt>
inline RandomIt radix_sort(RandomIt input, RandomIt scratch, const size_t rangeSize)
{
	return radix_sort<RandomIt>(input,scratch,rangeSize,impl::KeyAdaptor<decltype(*input)>());
}

}
}

#endif