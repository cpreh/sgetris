#ifndef SGETRIS_STATES_RUNNING_HPP_HPP_INCLUDED
#define SGETRIS_STATES_RUNNING_HPP_HPP_INCLUDED

#if 0
#include "../events/tick.hpp"
#include "../objects/weak_ptr.hpp"
#include <boost/ptr_container/ptr_list.hpp>

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

	running(
		my_context);

	boost::statechart::result 
	react(
		events::tick const &);
private:
	typedef
	boost::ptr_list<object>
	object_sequence;

	object_sequence objects_;
	field field_;
	sge::sprite::intrusive::system ss_;
};
}
}

#endif

#endif
