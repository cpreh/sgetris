#ifndef SGETRIS_BACKGROUNDS_FLAKES_FLAKE_HPP_HPP_INCLUDED
#define SGETRIS_BACKGROUNDS_FLAKES_FLAKE_HPP_HPP_INCLUDED

#include "../../real.hpp"
#include "../../point.hpp"
#include "../../sprite/parameters.hpp"
#include "../../sprite/object.hpp"
#include "../../sprite/scalar.hpp"
#include "../../sprite/dim.hpp"
#include <fcppt/noncopyable.hpp>
#include <sge/renderer/screen_size.hpp>
#include <fcppt/random/uniform.hpp>

namespace sgetris
{
namespace backgrounds
{
namespace flakes
{
class flake
{
FCPPT_NONCOPYABLE(flake);
public:
	explicit
	flake(
		real const &,
		sge::renderer::screen_size const &,
		sprite::parameters const &,
		sprite::scalar speed);
	
	void
	update(
		real const &);
private:
	real f_;
	sprite::object sprite_;
	real starting_x_;
	point position_;
	real speed_;
	sprite::dim screen_size_;
	fcppt::random::uniform<real> x_rng_;

	void
	reset();
};
}
}
}

#endif
