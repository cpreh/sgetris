#ifndef SGETRIS_GAME_LOGICS_BASE_HPP_HPP_INCLUDED
#define SGETRIS_GAME_LOGICS_BASE_HPP_HPP_INCLUDED

#include "../level_up_callback.hpp"
#include "../score.hpp"
#include "../level.hpp"
#include <sge/noncopyable.hpp>
#include <sge/filesystem/path.hpp>
#include <sge/signal/auto_connection.hpp>

namespace sgetris
{
namespace game_logics
{
class base
{
SGE_NONCOPYABLE(base)
protected:
	explicit
	base();

public:
	virtual bool
	fall_down_expired() const = 0;

	virtual sgetris::score 
	score() const = 0;

	virtual sgetris::level 
	level() const = 0;

	virtual sge::signal::auto_connection
	register_level_up(
		level_up_callback const &) = 0;

	virtual void
	row_clearing_begin() = 0;

	virtual void
	row_clearing_end() = 0;

	virtual void
	row_cleared() = 0;

	virtual 
	sge::filesystem::path const
	stone_file() const = 0;
public:
	virtual ~base();
};
}
}

#endif
