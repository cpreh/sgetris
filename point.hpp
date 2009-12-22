#ifndef SGETRIS_POINT_HPP_HPP_INCLUDED
#define SGETRIS_POINT_HPP_HPP_INCLUDED

#include "real.hpp"
#include <fcppt/math/vector/static.hpp>

namespace sgetris
{
typedef
fcppt::math::vector::static_
<
	real,
	2
>::type
point;
}

#endif
