#include "standard.hpp"
#include "../media_path.hpp"
#include <fcppt/text.hpp>

bool
sgetris::game_logics::standard::fall_down_expired() const
{
	return false;
}

sgetris::score 
sgetris::game_logics::standard::score() const
{
	return 0;
}

sgetris::level 
sgetris::game_logics::standard::level() const
{
	return 0;
}

fcppt::signal::auto_connection
sgetris::game_logics::standard::register_level_up(
	level_up_callback const &_f)
{
	return 
		level_up_callback_.connect(
			_f);
}

void
sgetris::game_logics::standard::row_clearing_begin()
{
}

void
sgetris::game_logics::standard::row_clearing_end()
{
}

void
sgetris::game_logics::standard::row_cleared()
{
}

fcppt::filesystem::path const
sgetris::game_logics::standard::stone_file() const
{
	return 
		media_path()/FCPPT_TEXT("stones")/FCPPT_TEXT("standard.txt");
}
