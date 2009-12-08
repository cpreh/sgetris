#include "tick.hpp"

sgetris::events::tick::tick(
	time_delta const _delta)
:
	delta_(
		_delta)
{
}

sgetris::time_delta
sgetris::events::tick::delta() const
{
	return 
		delta_;
}
