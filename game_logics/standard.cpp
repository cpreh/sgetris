#include "standard.hpp"
#include "../media_path.hpp"

bool
sgetris::game_logics::fall_down_expired() const
{

}

sgetris::score 
sgetris::game_logics::score() const
{
}

sgetris::level 
sgetris::game_logics::level() const
{
}

sge::signal::auto_connection
sgetris::game_logics::register_level_up(
	level_up_callback const &_f)
{
	return 
		level_up_callback_.connect(
			_f);
}

void
sgetris::game_logics::row_clearing_begin()
{
}

void
sgetris::game_logics::row_clearing_end()
{
}

void
sgetris::game_logics::row_cleared()
{
}

sge::filesystem::path const
sgetris::game_logics::stone_file() const
{
	return 
		media_path()/SGE_TEXT("stones")/SGE_TEXT("standard.txt");
}
