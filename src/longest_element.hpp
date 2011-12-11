#ifndef SGETRIS_LONGEST_ELEMENT_HPP_HPP_INCLUDED
#define SGETRIS_LONGEST_ELEMENT_HPP_HPP_INCLUDED

#include <fcppt/math/null.hpp>
#include <boost/foreach.hpp>
#include <algorithm>

namespace sgetris
{
template<typename T>
typename T::value_type::size_type
longest_element(
	T const &t)
{
	typename T::value_type::size_type longest = 
		fcppt::math::null<typename T::value_type::size_type>();
	BOOST_FOREACH(typename T::const_reference s,t)
		longest = 
			std::max(
				longest,
				s.size());
	return 
		longest;
}
}

#endif
