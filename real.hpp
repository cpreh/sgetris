#ifndef SGETRIS_REAL_HPP_HPP_INCLUDED
#define SGETRIS_REAL_HPP_HPP_INCLUDED

#include <sge/math/real.hpp>
#include <sge/math/machine_epsilon.hpp>

namespace sgetris
{
typedef
sge::math::real
<
	float,
	sge::math::machine_epsilon<float>
>
real;
}

#endif
