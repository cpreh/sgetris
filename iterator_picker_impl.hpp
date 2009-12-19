#ifndef SGETRIS_ITERATOR_PICKER_IMPL_HPP_HPP_INCLUDED
#define SGETRIS_ITERATOR_PICKER_IMPL_HPP_HPP_INCLUDED

#include <sge/random/last_exclusive_range.hpp>
#include <sge/math/null.hpp>
#include <boost/next_prior.hpp>

template<typename Container>
sgetris::iterator_picker<Container>::iterator_picker(
	container const &_container)
:
	container_(
		_container),
	rng_(
		sge::random::make_last_exclusive_range(
			sge::math::null<size_type>(),
			container_.size()))
{
}

template<typename Container>
typename
sgetris::iterator_picker<Container>::iterator const
sgetris::iterator_picker<Container>::pick()
{
	return
		boost::next(
			container_.begin(),
			rng_());
}

#endif
