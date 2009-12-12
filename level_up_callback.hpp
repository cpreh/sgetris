#ifndef SGETRIS_LEVEL_UP_CALLBACK_HPP_HPP_INCLUDED
#define SGETRIS_LEVEL_UP_CALLBACK_HPP_HPP_INCLUDED

#include "level_up_callback_fn.hpp"
#include <sge/function/object.hpp>

namespace sgetris
{
typedef
sge::function::object
<
	level_up_callback_fn
>
level_up_callback;
}

#endif
