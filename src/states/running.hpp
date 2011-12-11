#ifndef SGETRIS_STATES_RUNNING_HPP_HPP_INCLUDED
#define SGETRIS_STATES_RUNNING_HPP_HPP_INCLUDED

#include "../machine.hpp"
#include "../events/tick.hpp"
#include "../objects/weak_ptr.hpp"
#include "../objects/base_fwd.hpp"
#include "../game_logics/base.hpp"
#include "../sprite/system.hpp"
#include "../sprite/object.hpp"
#include "../sprite/rect.hpp"
#include "../sprite/dim.hpp"
#include "../parser/stone_sequence.hpp"
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/vector.hpp>
#include <fcppt/scoped_ptr.hpp>
#include <fcppt/shared_ptr.hpp>
#include <fcppt/container/field_decl.hpp>
#include <vector>

namespace sgetris
{
namespace states
{
class running
	: public boost::statechart::state<running,machine>
{
public:
	typedef 
	boost::mpl::vector1
	<
		boost::statechart::custom_reaction<events::tick>
	>
	reactions;

	typedef
	fcppt::container::field<
		std::vector<
			objects::weak_ptr
		>
	>
	field;

	typedef
	std::vector<field>
	upcoming_sequence;

	running(
		my_context);

	boost::statechart::result 
	react(
		events::tick const &);
private:
	typedef
	std::vector
	<
		fcppt::shared_ptr<objects::base>
	>
	object_sequence;

	fcppt::scoped_ptr<game_logics::base> logic_;
	parser::stone_sequence possible_stones_;
	upcoming_sequence upcoming_;
	object_sequence objects_;
	field field_;
	sprite::system ss_;

	sprite::dim block_dim_;
	sprite::rect 
		field_rect_,
		upcoming_rect_,
		hud_rect_;

	void
	calculate_rects();

	void
	generate_upcoming_list();
};
}
}

#endif
