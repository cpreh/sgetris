#include "sprite_block.hpp"
#include <sge/math/vector/structure_cast.hpp>
#include <sge/math/vector/arithmetic.hpp>

sgetris::objects::sprite_block::sprite_block(
	sprite::parameters const &_params)
:
	sprite_(
		_params.elements()),
	pos_(
		sge::math::vector::structure_cast<real_point>(
			sprite_.pos())),
	target_(
		pos_),
	speed_(
		static_cast<real::value_type>(
			20)),
	dead_(
		false)
{
}

bool 
sgetris::objects::sprite_block::can_be_removed() const
{
	return 
		dead_;
}

void
sgetris::objects::sprite_block::update(
	time_delta const _delta)
{
	sprite_.pos(
		sge::math::vector::structure_cast<sprite::vector>(
			pos_ + 
			((pos_-target_)/speed_.value())*static_cast<real::value_type>( _delta)));
}

void
sgetris::objects::sprite_block::move(
	sprite::vector const &_target)
{
	target_ = 
		sge::math::vector::structure_cast<real_point>(
			_target);
}
	
void
sgetris::objects::sprite_block::die()
{
	dead_ = true;
}
