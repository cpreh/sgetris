#ifndef SGETRIS_SPRITE_SYSTEM_HPP_HPP_INCLUDED
#define SGETRIS_SPRITE_SYSTEM_HPP_HPP_INCLUDED

#include "choices.hpp"
#include <sge/sprite/system.hpp>
#include <sge/sprite/intrusive/system_impl.hpp>

namespace sgetris
{
namespace sprite
{
typedef
sge::sprite::system
<
	choices
>::type
system;
}
}

#endif
