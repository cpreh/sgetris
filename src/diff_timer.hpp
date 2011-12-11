#ifndef SGETRIS_DIFF_TIMER_HPP_INCLUDED
#define SGETRIS_DIFF_TIMER_HPP_INCLUDED

#include "diff_clock.hpp"
#include <sge/timer/basic.hpp>

namespace sgetris
{

typedef sge::timer::basic<
	sanguis::diff_clock
> diff_timer;

}

#endif
