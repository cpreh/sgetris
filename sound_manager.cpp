#include "sound_manager.hpp"
#include "exception.hpp"
#include <fcppt/filesystem/remove_filename.hpp>
#include <fcppt/text.hpp>
#include <sge/audio/player.hpp>
#include <sge/audio/loader.hpp>
#include <sge/parse/ini/parse_file.hpp>
#include <sge/parse/ini/section_vector.hpp>
#include <sge/parse/ini/section.hpp>
#include <sge/parse/ini/entry_vector.hpp>
#include <sge/parse/ini/entry.hpp>
#include <boost/foreach.hpp>

sgetris::sound_manager::sound_manager(
	sge::audio::multi_loader const &_loader,
	sge::audio::player_ptr const _player)
:
	loader_(
		_loader),
	player_(
		_player)
{
}

void
sgetris::sound_manager::load(
	fcppt::filesystem::path const &_fn)
{
	sge::parse::ini::section_vector s;
	if (!parse_file(_fn,s))
		throw exception(
			FCPPT_TEXT("Invalid sound file \"")+_fn.string()+FCPPT_TEXT("\""));
	
	fcppt::filesystem::path const dir = 
		fcppt::filesystem::remove_filename(
			_fn);
	
	BOOST_FOREACH(sge::parse::ini::section_vector::const_reference r,s)
	{
		BOOST_FOREACH(sge::parse::ini::entry_vector::const_reference e,r.entries)
		{
			if (sound_map_.find(e.name) != sound_map_.end())
				throw exception(
					FCPPT_TEXT("Got two sounds named \"")+e.name+FCPPT_TEXT("\""));

			sound_map_.insert(
				sound_map::value_type(
					e.name,
					player_->create_stream_sound(
						loader_.load(
							dir/e.value))));
		}
	}
}

sge::audio::sound_ptr const
sgetris::sound_manager::sound(
	fcppt::string const &_s)
{
	sound_map::const_iterator const i = 
		sound_map_.find(
			_s);
	if (i == sound_map_.end())
		throw exception(
			FCPPT_TEXT("Couldn't find sound \"")+_s+FCPPT_TEXT("\""));
	
	return 
		i->second;
}
