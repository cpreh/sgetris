#ifndef SGETRIS_OBJECTS_BLOCK_HPP_HPP_INCLUDED
#define SGETRIS_OBJECTS_BLOCK_HPP_HPP_INCLUDED

#include "../sprite/vector.hpp"
#include "base.hpp"

namespace sgetris
{
namespace objects
{
class block
	:	public base
{
public:
	virtual void
	move(
		sprite::vector const &) = 0;
	
	virtual void 
	die() = 0;
};
}
}

#endif
