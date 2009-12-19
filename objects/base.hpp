#ifndef SGETRIS_OBJECTS_BASE_HPP_HPP_INCLUDED
#define SGETRIS_OBJECTS_BASE_HPP_HPP_INCLUDED

#include "../time_delta.hpp"
#include <sge/noncopyable.hpp>

namespace sgetris
{
namespace objects
{
class base
{
SGE_NONCOPYABLE(base)
protected:
	explicit
	base();
public:
	virtual bool
	can_be_removed() const = 0;

	virtual void
	update(
		time_delta) = 0;

	virtual 
	~base();
};
}
}

#endif
