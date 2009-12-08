#ifndef SGETRIS_MACHINE_HPP_HPP_INCLUDED
#define SGETRIS_MACHINE_HPP_HPP_INCLUDED

#if 0
#include "states/running.hpp"
#include <boost/statechart/state_machine.hpp>

namespace sgetris
{
class machine
	: public boost::statechart::state_machine<machine, states::running>
{
public:
	machine(
		int,
		char*argv[]);
private:
};
}
#endif

#endif
