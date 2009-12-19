#include "machine.hpp"

sgetris::machine::machine(
	boost::program_options::variables_map &_program_options_map,
	sge::systems::instance const &_systems,
	sgetris::texture_manager &_texture_manager)
:
	program_options_map_(
		_program_options_map),
	systems_(
		_systems),
	texture_manager_(
		_texture_manager)
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

sgetris::texture_manager &
sgetris::machine::texture_manager() const
{
	return 
		texture_manager_;
}
