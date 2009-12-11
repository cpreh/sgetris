#ifndef SGETRIS_MACHINE_HPP_HPP_INCLUDED
#define SGETRIS_MACHINE_HPP_HPP_INCLUDED

#if 0
#include "states/running.hpp"
#include <sge/systems/instance_fwd.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/program_options/variables_map.hpp>

namespace sgetris
{
class machine
	: public boost::statechart::state_machine<machine, states::running>
{
public:
	machine(
		boost::program_options::variables_map &,
		sge::systems::instance const &);

	boost::program_options::variables_map &
	program_options_map() const;

	sge::systems::instance const &
	systems() const;
private:
	boost::program_options::variables_map &program_options_map_;
	sge::systems::instance const &systems_;
};
}
#endif

#endif
