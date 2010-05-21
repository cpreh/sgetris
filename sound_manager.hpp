#ifndef SGETRIS_SOUND_MANAGER_HPP_HPP_INCLUDED
#define SGETRIS_SOUND_MANAGER_HPP_HPP_INCLUDED

#include <sge/audio/multi_loader.hpp>
#include <sge/audio/player_ptr.hpp>
#include <sge/audio/sound_ptr.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/string.hpp>
#include <fcppt/noncopyable.hpp>
#include <map>

namespace sgetris
{
class sound_manager
{
FCPPT_NONCOPYABLE(sound_manager)
public:
	explicit
	sound_manager(
		sge::audio::multi_loader const &,
		sge::audio::player_ptr);
	
	void
	load(
		fcppt::filesystem::path const &);
	
	sge::audio::sound_ptr const
	sound(
		fcppt::string const &);
private:
	typedef
	std::map
	<
		fcppt::string,
		sge::audio::sound_ptr
	>
	sound_map;

	sound_map sound_map_;
	sge::audio::multi_loader const &loader_;
	sge::audio::player_ptr const player_;
};
}

#endif
