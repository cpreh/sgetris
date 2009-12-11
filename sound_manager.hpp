#ifndef SGETRIS_SOUND_MANAGER_HPP_HPP_INCLUDED
#define SGETRIS_SOUND_MANAGER_HPP_HPP_INCLUDED

#include <sge/audio/multi_loader.hpp>
#include <sge/audio/player_fwd.hpp>
#include <sge/audio/sound_fwd.hpp>
#include <sge/filesystem/path.hpp>
#include <sge/string.hpp>
#include <sge/noncopyable.hpp>
#include <map>

namespace sgetris
{
class sound_manager
{
SGE_NONCOPYABLE(sound_manager)
public:
	explicit
	sound_manager(
		sge::audio::multi_loader &,
		sge::audio::player_ptr);
	
	void
	load(
		sge::filesystem::path const &);
	
	sge::audio::sound_ptr const
	sound(
		sge::string const &);
private:
	typedef
	std::map
	<
		sge::string,
		sge::audio::sound_ptr
	>
	sound_map;

	sound_map sound_map_;
	sge::audio::multi_loader &loader_;
	sge::audio::player_ptr const player_;
};
}

#endif
