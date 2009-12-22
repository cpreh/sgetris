#ifndef SGETRIS_REAL_HPP_HPP_INCLUDED
#define SGETRIS_REAL_HPP_HPP_INCLUDED

#include <fcppt/math/real.hpp>
#include <fcppt/math/machine_epsilon.hpp>

namespace sgetris
{
typedef
fcppt::math::real
<
	float,
	fcppt::math::machine_epsilon<float>
>
real;
}

#endif
