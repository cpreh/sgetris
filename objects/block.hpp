#ifndef SGETRIS_OBJECTS_BLOCK_HPP_HPP_INCLUDED
#define SGETRIS_OBJECTS_BLOCK_HPP_HPP_INCLUDED

#include <sge/sprite/point.hpp>
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
		sge::sprite::point const &) = 0;
	
	virtual void 
	die() = 0;
};
}
}

#endif
