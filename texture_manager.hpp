#ifndef SGETRIS_TEXTURE_MANAGER_HPP_HPP_INCLUDED
#define SGETRIS_TEXTURE_MANAGER_HPP_HPP_INCLUDED

#include <sge/image/multi_loader.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/texture/part_fwd.hpp>
#include <sge/texture/default_creator.hpp>
#include <sge/texture/no_fragmented.hpp>
#include <sge/texture/manager.hpp>
#include <map>

namespace sgetris
{
class texture_manager
{
SGE_NONCOPYABLE(texture_manager)
public:
	explicit
	texture_manager(
		sge::image::multi_loader &,
		sge::renderer::device_ptr);
	
	void
	load(
		sge::filesystem::path const &);
	
	sge::texture::const_part_ptr const
	texture(
		sge::string const &);
private:
	typedef
	std::map
	<
		sge::string,
		sge::texture::const_part_ptr
	>
	texture_map;

	texture_map texture_map_;
	sge::image::multi_loader &loader_;
	sge::renderer::device_ptr const renderer_;
	sge::texture::default_creator<sge::texture::no_fragmented> texture_creator_;
	sge::texture::manager texture_manager_;
};
}

#endif
