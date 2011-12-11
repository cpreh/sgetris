#ifndef SGETRIS_MACHINE_HPP_HPP_INCLUDED
#define SGETRIS_MACHINE_HPP_HPP_INCLUDED

#include "texture_manager_fwd.hpp"
#include "states/running_fwd.hpp"
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
		sge::systems::instance const &,
		sgetris::texture_manager &);

	boost::program_options::variables_map &
	program_options_map() const;

	sge::systems::instance const &
	systems() const;

	sgetris::texture_manager &
	texture_manager() const;
private:
	boost::program_options::variables_map &program_options_map_;
	sge::systems::instance const &systems_;
	sgetris::texture_manager &texture_manager_;
};
}

#endif
