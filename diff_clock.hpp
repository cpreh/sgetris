#ifndef SGETRIS_DIFF_CLOCK_HPP_INCLUDED
#define SGETRIS_DIFF_CLOCK_HPP_INCLUDED

#include "time_delta.hpp"
#include <sge/time/unit.hpp>
#include <sge/time/fun.hpp>
#include <sge/noncopyable.hpp>

namespace sgetris
{

class diff_clock 
{
SGE_NONCOPYABLE(diff_clock)
public:
	diff_clock();

	void 
	update(
		time_delta);

	sge::time::fun const 
	callback() const;
private:
	time_delta
	time() const;

	time_delta diff_time_;
};

}

#endif
