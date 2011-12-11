#ifndef SGETRIS_DIFF_CLOCK_HPP_INCLUDED
#define SGETRIS_DIFF_CLOCK_HPP_INCLUDED

#include "clock.hpp"
#include <sge/timer/clocks/delta_fwd.hpp>

namespace sgetris
{

typedef sge::timer::clocks::delta<
	sgetris::clock
> diff_clock;

}

#endif
