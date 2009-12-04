#ifndef SGETRIS_POINT_HPP_HPP_INCLUDED
#define SGETRIS_POINT_HPP_HPP_INCLUDED

#include "real.hpp"
#include <sge/math/vector/static.hpp>

namespace sgetris
{
typedef
sge::math::vector::static_
<
	real,
	2
>::type
point;
}

#endif
