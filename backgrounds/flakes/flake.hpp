#ifndef SGETRIS_BACKGROUNDS_FLAKES_FLAKE_HPP_HPP_INCLUDED
#define SGETRIS_BACKGROUNDS_FLAKES_FLAKE_HPP_HPP_INCLUDED

#include "../../real.hpp"
#include "../../point.hpp"
#include <sge/noncopyable.hpp>
#include <sge/renderer/screen_size.hpp>
#include <sge/sprite/intrusive/parameters.hpp>
#include <sge/sprite/intrusive/object.hpp>
#include <sge/sprite/unit.hpp>
#include <sge/math/vector/basic_impl.hpp>
#include <sge/random/uniform.hpp>

namespace sgetris
{
namespace backgrounds
{
namespace flakes
{
class flake
{
SGE_NONCOPYABLE(flake)
public:
	explicit
	flake(
		real const &,
		sge::renderer::screen_size const &,
		sge::sprite::intrusive::parameters const &,
		sge::sprite::unit const &speed);
	
	void
	update(
		real const &);
private:
	real f_;
	sge::sprite::intrusive::object sprite_;
	real starting_x_;
	point position_;
	real speed_;
	sge::sprite::dim screen_size_;
	sge::random::uniform<real::value_type> x_rng_;

	void
	reset();
};
}
}
}

#endif
