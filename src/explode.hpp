#ifndef SGETRIS_EXPLODE_HPP_HPP_INCLUDED
#define SGETRIS_EXPLODE_HPP_HPP_INCLUDED

#include <boost/next_prior.hpp>
#include <vector>

namespace sgetris
{
template<typename Container>
std::vector<Container> const
explode(
	Container const &c,
	typename Container::value_type const &separator)
{
	std::vector<Container> r;
	typename Container::const_iterator last = 
		c.begin();
	for (typename Container::const_iterator i = c.begin(); i != c.end(); ++i)
	{
		if (*i != separator)
			continue;

		r.push_back(
			Container(
				last,
				i));

		last = 
			boost::next(
				i);
	}

	r.push_back(
		Container(
			last,
			c.end()));

	return r;
}
}

#endif
