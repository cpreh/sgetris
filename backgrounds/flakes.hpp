#ifndef SGETRIS_BACKGROUNDS_FLAKES_HPP_HPP_INCLUDED
#define SGETRIS_BACKGROUNDS_FLAKES_HPP_HPP_INCLUDED

#include "base.hpp"
#include "flake.hpp"
#include "../diff_clock.hpp"
#include "../real.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <sge/image/loader_fwd.hpp>
#include <sge/time/timer.hpp>
#include <sge/filesystem/path.hpp>
#include <sge/sprite/intrusive/system.hpp>
#include <sge/sprite/intrusive/object_fwd.hpp>
#include <sge/texture/default_creator.hpp>
#include <sge/texture/no_fragmented.hpp>
#include <sge/texture/manager.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <utility>

namespace sgetris
{
namespace backgrounds
{
class flakes
:
	public base
{
public:
	typedef unsigned flake_count;

	explicit
	flakes(
		sge::renderer::device_ptr,
		// The flakes get the loader because in a later version more than one flake image could be loaded
		// from a directory
		sge::image::loader_ptr,
		flake_count,
		sge::filesystem::path const &);
	
	void
	update(	
		time_delta);

	void
	draw();
private:
	typedef 
	boost::ptr_vector<flake>
	flake_sequence;

	sge::sprite::intrusive::system ss_;
	diff_clock clock_;
	sge::time::timer frame_timer_;
	flake_sequence flakes_;

	sge::texture::default_creator<sge::texture::no_fragmented> texture_creator_;
	sge::texture::manager texture_manager_;
};
}
}

#endif
