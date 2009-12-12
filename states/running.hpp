#ifndef SGETRIS_STATES_RUNNING_HPP_HPP_INCLUDED
#define SGETRIS_STATES_RUNNING_HPP_HPP_INCLUDED

#include "../field.hpp"
#include "../events/tick.hpp"
#include "../objects/weak_ptr.hpp"
#include "../game_logics/base.hpp"
#include <boost/ptr_container/ptr_list.hpp>
#include <sge/scoped_ptr.hpp>

namespace sgetris
{
namespace states
{
class running
	: public boost::statechart::state<running,machine,unpaused>
{
public:
	typedef
	sge::container::field<objects::weak_ptr>
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
	boost::ptr_list<object>
	object_sequence;

	sge::scoped_ptr<game_logics::base> logic_;
	parser::stone_sequence possible_stones_;
	upcoming_sequence upcoming_;
	object_sequence objects_;
	field field_;
	sge::sprite::intrusive::system ss_;

	void
	generate_upcoming_list();
};
}
}

#endif
