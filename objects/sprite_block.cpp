#include "sprite_block.hpp"

sgetris::objects::sprite_block::sprite_block(
	sge::sprite::intrusive::parameters const &_params,
	sge::sprite::intrusive::system &_ss)
:
	sprite_(
		_params),
	target_(
		sprite_.pos())
{
}

bool 
sgetris::objects::sprite_block::can_be_removed() const
{
	
}

void
sgetris::objects::sprite_block::update(
	time_delta)
{
}

void
sgetris::objects::sprite_block::move(
	sge::sprite::point const &)
{
}
	
void
sgetris::objects::sprite_block::die()
{
}
