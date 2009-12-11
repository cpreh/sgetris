#include "running.hpp"
#include "../program_options.hpp"
#include <boost/program_options.hpp>

namespace
{
struct program_options
{
public:
	program_options()
	{
		sgetris::program_options().add_options()
			(
				"field-size",
				boost::program_options::value<sgetris::states::running::field::dim_type>()->default_value(
					sgetris::states::running::field::dim_type(
						10,
						20)));
	}
private:
} options;
}

sgetris::states::running::running(
	my_context _ctx)
:
	my_base(
		_ctx),
	objects_(),
	field_(
		context<machine>().program_options_map()["field-size"]),
	ss_(
		context<machine>().systems().renderer())
{
}

boost::statechart::result 
sgetris::states::running::react(
	events::tick const &t)
{
	for (object_sequence::iterator i = objects_.begin(); i != objects_.end();)
	{
		if (i->can_be_removed())
		{
			i = 
				objects_.erase(
					i);
			continue;
		}

		i->update(
			t.delta());
	
		++i;
	}

	return discard_event();
}
