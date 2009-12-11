#include "machine.hpp"

#if 0
sgetris::machine::machine(
	boost::program_options::variables_map &_program_options_map
	sge::systems::instance const &_systems)
:
	program_options_map_(
		_program_options_map),
	systems_(
		_systems)
{
}

boost::program_options::variables_map &
sgetris::machine::program_options_map() const
{
	return 
		program_options_map_;
}

sge::systems::instance const &
sgetris::machine::systems() const
{
	return 
		systems_;
}
#endif
