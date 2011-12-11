#ifndef SGETRIS_TIMER_HPP_INCLUDED
#define SGETRIS_TIMER_HPP_INCLUDED

#include "clock.hpp"
#include <sge/timer/basic.hpp>

namespace sgetris
{

typedef sge::timer::basic<
	sanguis::clock
> timer;

}

#endif
