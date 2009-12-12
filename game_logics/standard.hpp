#ifndef SGETRIS_GAME_LOGICS_STANDARD_HPP_HPP_INCLUDED
#define SGETRIS_GAME_LOGICS_STANDARD_HPP_HPP_INCLUDED

#include "base.hpp"

namespace sgetris
{
namespace game_logics
{
class standard
	: public base
{
public:
	bool
	fall_down_expired() const;

	sgetris::score 
	score() const;

	sgetris::level 
	level() const;

	sge::signal::auto_connection
	register_level_up(
		level_up_callback const &);

	void
	row_clearing_begin();

	void
	row_clearing_end();

	void
	row_cleared();

	sge::filesystem::path const
	stone_file() const;
private:
	typedef 
	sge::signal::object<level_up_callback_fn> 
	level_up_callback_signal;

	level_up_callback_signal level_up_callback_;
};
}
}

#endif
