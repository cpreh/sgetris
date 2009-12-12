#ifndef SGETRIS_SPRITE_CHOICES_HPP_HPP_INCLUDED
#define SGETRIS_SPRITE_CHOICES_HPP_HPP_INCLUDED

#include "color.hpp"
#include <sge/sprite/choices.hpp>
#include <sge/sprite/type_choices.hpp>
#include <sge/sprite/with_texture.hpp>
#include <sge/sprite/with_color.hpp>
#include <sge/sprite/with_rotation.hpp>
#include <sge/sprite/intrusive/tag.hpp>
#include <boost/mpl/vector.hpp>

namespace sgetris
{
namespace sprite
{
typedef
sge::sprite::choices
<
	sge::sprite::type_choices
	<
		int,
		float,
		color
	>,
	boost::mpl::vector3
	<
	//	sge::sprite::with_color,
		sge::sprite::with_texture,
		sge::sprite::with_rotation,
		sge::sprite::intrusive::tag
	>
> choices;
}
}

#endif
