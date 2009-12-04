#include "diff_clock.hpp"
#include <sge/time/time.hpp>
#include <sge/math/null.hpp>
#include <tr1/functional>

sgetris::diff_clock::diff_clock()
:
	diff_time_(
		sge::math::null<time_delta>())
{}

void 
sgetris::diff_clock::update(
	time_delta const _tm)
{
	diff_time_ += _tm;
}

sge::time::fun const
sgetris::diff_clock::callback() const
{
	return std::tr1::bind(
		&diff_clock::time,
		this);
}

sgetris::time_delta
sgetris::diff_clock::time() const
{
	return diff_time_;
}
