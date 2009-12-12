#include "texture_manager.hpp"
#include "exception.hpp"
#include <sge/texture/part.hpp>
#include <sge/texture/add_image.hpp>
#include <sge/texture/default_creator_impl.hpp>
#include <sge/filesystem/remove_filename.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/parse/ini/parse_file.hpp>
#include <sge/parse/ini/section_vector.hpp>
#include <sge/parse/ini/section.hpp>
#include <sge/parse/ini/entry_vector.hpp>
#include <sge/parse/ini/entry.hpp>
#include <boost/foreach.hpp>

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
	sge::filesystem::path const &_fn)
{
	sge::parse::ini::section_vector s;
	if (!parse_file(_fn,s))
		throw exception(
			SGE_TEXT("Invalid texture file \"")+_fn.string()+SGE_TEXT("\""));
	
	sge::filesystem::path const dir = 
		sge::filesystem::remove_filename(
			_fn);
	
	BOOST_FOREACH(sge::parse::ini::section_vector::const_reference r,s)
	{
		BOOST_FOREACH(sge::parse::ini::entry_vector::const_reference e,r.entries)
		{
			if (texture_map_.find(e.name) != texture_map_.end())
				throw exception(
					SGE_TEXT("Got two textures named \"")+e.name+SGE_TEXT("\""));

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
	sge::string const &_s)
{
	texture_map::const_iterator const i = 
		texture_map_.find(
			_s);
	if (i == texture_map_.end())
		throw exception(
			SGE_TEXT("Couldn't find texture \"")+_s+SGE_TEXT("\""));
	return 
		i->second;
}
