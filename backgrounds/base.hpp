#ifndef SGETRIS_BACKGROUNDS_BASE_HPP_HPP_INCLUDED
#define SGETRIS_BACKGROUNDS_BASE_HPP_HPP_INCLUDED

#include "../time_delta.hpp"
#include <fcppt/noncopyable.hpp>

namespace sgetris
{
namespace backgrounds
{
class base
{
FCPPT_NONCOPYABLE(base)
protected:
	base();
public:
	virtual void
	update(	
		time_delta) = 0;

	virtual void
	draw() = 0;

	virtual
	~base();
};
}
}

#endif
