#ifndef SGETRIS_OBJECTS_SPRITE_BLOCK_HPP_HPP_INCLUDED
#define SGETRIS_OBJECTS_SPRITE_BLOCK_HPP_HPP_INCLUDED

#include "block.hpp"
#include <sge/sprite/intrusive/object.hpp>

namespace sgetris
{
namespace objects
{
class sprite_block
	: public block
{
public:
	sprite_block(
		sge:sprite::intrusive::parameters const &,
		sge::sprite::intrusive::system &);
	
	bool 
	can_be_removed() const;

	void
	update(
		time_delta);

	void
	move(
		sge::sprite::point const &);
	
	void
	die();
private:
	sge::sprite::intrusive::object sprite_;
	sge::sprite::point target_;
};
}
}

#endif
