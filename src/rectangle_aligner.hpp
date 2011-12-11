#ifndef SGETRIS_RECTANGLE_ALIGNER_HPP_HPP_INCLUDED
#define SGETRIS_RECTANGLE_ALIGNER_HPP_HPP_INCLUDED

#include <list>

namespace sgetris
{
template<typename Rectangle>
class rectangle_aligner
{
public:
	typedef 
	Rectangle 
	rectangle;

	typedef typename
	Rectangle::vector
	point;

	typedef typename
	Rectangle::dim
	dim;

	typedef typename
	Rectangle::scalar
	unit;

	typedef
	std::list<rectangle>
	container;

	typedef typename
	container::const_iterator
	iterator;

	typedef typename
	dim::size_type
	size_type;

	static size_type const 
		x_axis = 
			static_cast<size_type>(
				0),
		y_axis = 
			static_cast<size_type>(
				1);

	explicit
	rectangle_aligner(
		rectangle const &,
		size_type axis);

	iterator const
	insert(
		dim const &);
	
	void 
	compile();
private:
	container container_;
	rectangle top_;
	size_type axis_;

	size_type 
	axis() const;

	size_type 
	complementary_axis() const;
};
}

#include "rectangle_aligner_impl.hpp"

#endif
