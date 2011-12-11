#ifndef SGETRIS_ITERATOR_PICKER_HPP_HPP_INCLUDED
#define SGETRIS_ITERATOR_PICKER_HPP_HPP_INCLUDED

#include <fcppt/random/uniform.hpp>

namespace sgetris
{
template<typename Container>
class iterator_picker
{
public:
	typedef 
	Container 
	container;

	typedef typename
	Container::const_iterator
	iterator;

	typedef typename
	Container::size_type
	size_type;

	explicit
	iterator_picker(
		container const &);
	
	iterator const
	pick();
private:
	typedef 
	fcppt::random::uniform<size_type>
	rng;

	container const &container_;
	rng rng_;
};
}

#include "iterator_picker_impl.hpp"

#endif
