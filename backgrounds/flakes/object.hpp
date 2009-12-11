#ifndef SGETRIS_BACKGROUNDS_FLAKES_OBJECT_HPP_HPP_INCLUDED
#define SGETRIS_BACKGROUNDS_FLAKES_OBJECT_HPP_HPP_INCLUDED

#include "flake_fwd.hpp"
#include "flake_count.hpp"
#include "../../texture_manager_fwd.hpp"
#include "../base.hpp"
#include "../../diff_clock.hpp"
#include "../../real.hpp"
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
#include <boost/program_options/variables_map.hpp>
#include <utility>

namespace sgetris
{
namespace backgrounds
{
namespace flakes
{
class object
:
	public base
{
public:
	explicit
	object(
		boost::program_options::variables_map &,
		sge::renderer::device_ptr,
		// The flakes get the loader because in a later version more than one flake image could be loaded
		// from a directory
		texture_manager &);
	
	void
	update(	
		time_delta);

	void
	draw();
private:
	typedef 
	boost::ptr_vector<flake>
	flake_sequence;

	texture_manager &texture_manager_;
	sge::sprite::intrusive::system ss_;
	diff_clock clock_;
	sge::time::timer frame_timer_;
	flake_sequence flakes_;

};
}
}
}

#endif
