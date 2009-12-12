#include "running.hpp"
#include "../program_options.hpp"
#include "../game_logics/standard.hpp"
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
						20)),
				"Field size")
			(
				"upcoming-count",
				boost::program_options::value<sgetris::states::running::upcoming_sequence::size_type>()->default_value(
					static_cast<sgetris::states::running::upcoming_sequence::size_type>(
						2)),
				"The number of upcoming stones to be displayed");
	}
private:
} options;
}

sgetris::states::running::running(
	my_context _ctx)
:
	my_base(
		_ctx),
	logic_(
		new game_logics::standard()),
	possible_stones_(
		parser::parse_file(
			logic_->stone_file())),
	upcoming_(),
	objects_(),
	field_(
		context<machine>().program_options_map()["field-size"].as<field::dim_type>()),
	ss_(
		context<machine>().systems().renderer())
{
	generate_upcoming_list();
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

void
sgetris::states::running::generate_upcoming_list()
{
	upcoming_.resize(
		context<machine>().program_options_map()["upcoming-count"].as<upcoming_sequence::size_type>());
	
	BOOST_FOREACH(upcoming_sequence::reference r,upcoming_)
	{
		parser::stone_sequence::const_reference stone = 
			pick_one_possible_stone();

		stone_template::size_type const 
			maxdim = 
				std::max(
					stone.dim().w(),
					stone.dim().h());

		r.resize(
			field::dim_type(
				maxdim,
				maxdim));

		for (field::size_type y = sge::math::null<field::size_type>(); y < maxdim; ++y)
		{
			for (field::size_type x = sge::math::null<field::size_type>(); x < maxdim; ++x)
			{
				if (x >= stone.dim().w() || y >= stone.dim().h())
					continue;

				// TODO: create stone here (sprite_block), then make an entry in r
	//			r.pos(field::vector_type(x,y)) = 
			}
		}
	}
}
