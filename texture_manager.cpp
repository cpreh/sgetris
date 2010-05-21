#include "texture_manager.hpp"
#include "exception.hpp"
#include "log/object.hpp"
#include <sge/texture/part.hpp>
#include <sge/texture/add_image.hpp>
#include <sge/texture/default_creator_impl.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/parse/ini/parse_file.hpp>
#include <sge/parse/ini/section_vector.hpp>
#include <sge/parse/ini/section.hpp>
#include <sge/parse/ini/entry_vector.hpp>
#include <sge/parse/ini/entry.hpp>
#include <fcppt/text.hpp>
#include <fcppt/filesystem/remove_filename.hpp>
#include <boost/foreach.hpp>

namespace
{
fcppt::log::object 
mylogger(
	fcppt::log::parameters::inherited(
		sgetris::log::object(),
		FCPPT_TEXT("texture_manager")));
}

sgetris::texture_manager::texture_manager(
	sge::image::multi_loader &_loader,
	sge::renderer::device_ptr const _renderer)
:
	loader_(
		_loader),
	renderer_(
		_renderer),
	texture_creator_(
		renderer_,
		sge::image::color::format::rgba8,
		sge::renderer::filter::linear),
	texture_manager_(
		renderer_,
		texture_creator_),
	texture_map_()
{
}

void
sgetris::texture_manager::load(
	fcppt::filesystem::path const &_fn)
{
	FCPPT_LOG_DEBUG(
		mylogger,
		fcppt::log::_ 
			<< FCPPT_TEXT("loading file ")
			<< _fn.string());

	sge::parse::ini::section_vector s;
	if (!parse_file(_fn,s))
		throw exception(
			FCPPT_TEXT("Invalid texture file \"")+_fn.string()+FCPPT_TEXT("\""));
	
	fcppt::filesystem::path const dir = 
		fcppt::filesystem::remove_filename(
			_fn);
	
	BOOST_FOREACH(sge::parse::ini::section_vector::const_reference r,s)
	{
		BOOST_FOREACH(sge::parse::ini::entry_vector::const_reference e,r.entries)
		{
			if (texture_map_.find(e.name) != texture_map_.end())
				throw exception(
					FCPPT_TEXT("Got two textures named \"")+e.name+FCPPT_TEXT("\""));

			FCPPT_LOG_DEBUG(
				mylogger,
				fcppt::log::_ 
					<< FCPPT_TEXT("adding texture pair ")
					<< e.name
					<< FCPPT_TEXT("=")
					<< (dir/e.value).string());

			texture_map_.insert(
				texture_map::value_type(
					e.name,
					sge::texture::add_image(
						texture_manager_,
						loader_.load(
							dir/e.value))));
		}
	}
}

sge::texture::const_part_ptr const
sgetris::texture_manager::texture(
	fcppt::string const &_s) const
{
	texture_map::const_iterator const i = 
		texture_map_.find(
			_s);
	if (i == texture_map_.end())
		throw exception(
			FCPPT_TEXT("Couldn't find texture \"")+_s+FCPPT_TEXT("\""));
	return 
		i->second;
}
