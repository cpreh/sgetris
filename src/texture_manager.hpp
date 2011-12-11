#ifndef SGETRIS_TEXTURE_MANAGER_HPP_HPP_INCLUDED
#define SGETRIS_TEXTURE_MANAGER_HPP_HPP_INCLUDED

#include <sge/image2d/system_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/texture/const_part_ptr.hpp>
#include <sge/texture/manager.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/string.hpp>
#include <fcppt/filesystem/path.hpp>
#include <map>

namespace sgetris
{
class texture_manager
{
FCPPT_NONCOPYABLE(texture_manager);
public:
	explicit
	texture_manager(
		sge::image2d::system &,
		sge::renderer::device &);

	void
	load(
		fcppt::filesystem::path const &);

	sge::texture::const_part_ptr const
	texture(
		fcppt::string const &) const;
private:
	typedef
	std::map
	<
		fcppt::string,
		sge::texture::const_part_ptr
	>
	texture_map;

	sge::image2d::system &loader_;
	sge::renderer::device &renderer_;
	sge::texture::manager texture_manager_;
	texture_map texture_map_;
};
}

#endif
