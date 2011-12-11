#ifndef SGETRIS_OBJECTS_WEAK_PTR_HPP_HPP_INCLUDED
#define SGETRIS_OBJECTS_WEAK_PTR_HPP_HPP_INCLUDED

#include "base_fwd.hpp"
#include <fcppt/weak_ptr.hpp>

namespace sgetris
{
namespace objects
{
typedef
fcppt::weak_ptr<base>
weak_ptr;
}
}

#endif
