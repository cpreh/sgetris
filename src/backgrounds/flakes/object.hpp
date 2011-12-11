#ifndef SGETRIS_BACKGROUNDS_FLAKES_OBJECT_HPP_HPP_INCLUDED
#define SGETRIS_BACKGROUNDS_FLAKES_OBJECT_HPP_HPP_INCLUDED

#include "flake_fwd.hpp"
#include "flake_count.hpp"
#include "../base.hpp"
#include "../../texture_manager_fwd.hpp"
#include "../../diff_timer.hpp"
#include "../../real.hpp"
#include "../../sprite/system.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/program_options/variables_map.hpp>

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
		sge::renderer::device &,
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
	sprite::system ss_;
	sgetris::diff_timer frame_timer_;
	flake_sequence flakes_;

};
}
}
}

#endif
