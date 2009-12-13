#ifndef SGETRIS_RECTANGLE_ALIGNER_IMPL_HPP_HPP_INCLUDED
#define SGETRIS_RECTANGLE_ALIGNER_IMPL_HPP_HPP_INCLUDED

#include "exception.hpp"
#include <sge/math/null.hpp>
#include <sge/text.hpp>
#include <boost/foreach.hpp>

template<typename Rectangle>
sgetris::rectangle_aligner<Rectangle>::rectangle_aligner(
	rectangle const &_top,
	size_type const _axis)
:
	container_(),
	top_(
		_top),
	axis_(
		_axis)
{
}

template<typename Rectangle>
typename
sgetris::rectangle_aligner<Rectangle>::iterator const
sgetris::rectangle_aligner<Rectangle>::insert(
	dim const &_d)
{
	container_.push_back(
		rectangle(
			point::null(),
			_d));
	return 
		--container_.end();
}

template<typename Rectangle>
void 
sgetris::rectangle_aligner<Rectangle>::compile()
{
	if (container_.empty())
		return;

	// calculate how much overhead we have
	unit sum = 
		sge::math::null<unit>();
	BOOST_FOREACH(rectangle const &r,container_)
		sum += r.dim()[axis()];
	
	unit const overhead = 
		top_.dim()[axis()] - sum;
	
	if (overhead < sge::math::null<unit>())
		throw exception(
			SGE_TEXT("There is no space left"));

	unit const increment = 
		overhead/static_cast<unit>(container_.size()+1);
	
	unit i = top_.pos()[axis()] + increment;
	BOOST_FOREACH(rectangle &r,container_)
	{
		point newpos;
		newpos[axis()] = 
			i;
		newpos[complementary_axis()] = 
			top_.pos()[complementary_axis()] + 
			top_.dim()[complementary_axis()]/static_cast<unit>(2) - 
			r.dim()[complementary_axis()]/static_cast<unit>(2);
		r.pos(
			newpos);
		i += r.dim()[axis()] + increment;
	}
}

template<typename Rectangle>
typename sgetris::rectangle_aligner<Rectangle>::size_type
sgetris::rectangle_aligner<Rectangle>::axis() const
{
	return 
		axis_;
}

template<typename Rectangle>
typename sgetris::rectangle_aligner<Rectangle>::size_type
sgetris::rectangle_aligner<Rectangle>::complementary_axis() const
{
	return 
		axis_ == static_cast<size_type>(0)
		?
			static_cast<size_type>(1)
		:
			static_cast<size_type>(0);
}

#endif
