#ifndef SGETRIS_OBJECTS_SPRITE_BLOCK_HPP_HPP_INCLUDED
#define SGETRIS_OBJECTS_SPRITE_BLOCK_HPP_HPP_INCLUDED

#include "block.hpp"
#include "../real_point.hpp"
#include "../sprite/object.hpp"
#include "../sprite/parameters.hpp"

namespace sgetris
{
namespace objects
{
class sprite_block
	: public block
{
public:
	sprite_block(
		sprite::parameters const &);
	
	bool 
	can_be_removed() const;

	void
	update(
		time_delta);

	void
	move(
		sprite::vector const &);
	
	void
	die();
private:
	sprite::object sprite_;
	real_point pos_,target_;
	real speed_;
	bool dead_;
};
}
}

#endif
