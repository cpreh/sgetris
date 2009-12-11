#ifndef SGETRIS_OBJECTS_WEAK_PTR_HPP_HPP_INCLUDED
#define SGETRIS_OBJECTS_WEAK_PTR_HPP_HPP_INCLUDED

#include "base_fwd.hpp"
#include <sge/weak_ptr.hpp>

namespace sgetris
{
namespace objects
{
typedef
sge::weak_ptr<base>
weak_ptr;
}
}

#endif
