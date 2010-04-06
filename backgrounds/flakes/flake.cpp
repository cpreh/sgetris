#include "flake.hpp"
#include "../../sprite/system.hpp"
#include "../../sprite/rotation_type.hpp"
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/random/make_inclusive_range.hpp>
#include <fcppt/math/twopi.hpp>
#include <fcppt/math/pi.hpp>

sgetris::backgrounds::flakes::flake::flake(
	real const &_f,
	sge::renderer::screen_size const &_screen_size,
	sprite::parameters const &_params,
	sprite::scalar const _speed)
:
	f_(
		_f),
	sprite_(
		_params.elements()),
	starting_x_(
		static_cast<real>(sprite_.x())),
	position_(
		fcppt::math::vector::structure_cast<point>(
			sprite_.pos())),
	speed_(
		static_cast<real>(
			_speed)),
	screen_size_(
		fcppt::math::dim::structure_cast<sprite::dim>(
			_screen_size)),
	x_rng_(
		fcppt::random::make_inclusive_range(
			static_cast<real>(0),
			static_cast<real>(screen_size_.w())))
{
	FCPPT_ASSERT(
		f_ >= real(0) && f_ <= real(1));
}

void
sgetris::backgrounds::flakes::flake::update(
	real const &_r)
{
	position_.y() += speed_ * _r;
	
	real const 
		inside_sin = 
			position_.y()/
			(f_ * static_cast<real>(screen_size_.w())) * 
			real(
				fcppt::math::twopi<real>()),
		x_variance = 
			static_cast<real>(
				screen_size_.w()/3),
		x_variance_val = 
			f_ * x_variance,
		value = 
			starting_x_ + 
			real(
				std::sin(
					inside_sin))*
			x_variance_val;

	position_.x() = 
		value;

	// TODO: structure_cast here, but it won't work with real yet
	sprite_.x( 
		static_cast<sprite::scalar>(
			position_.x()));
	sprite_.y( 
		static_cast<sprite::scalar>(
			position_.y()));
	sprite_.rotation(
		static_cast<sprite::rotation_type>(
			std::sin(
				inside_sin/
				static_cast<real>(2)) * 
			fcppt::math::pi<real>()/
			static_cast<real>(2)));
	
	if (sprite_.y() > screen_size_.h())
		reset();
}

void
sgetris::backgrounds::flakes::flake::reset()
{
	position_.x() = 
		real(
			x_rng_());
	position_.y() = 
		static_cast<real>(
			-sprite_.h());
}
