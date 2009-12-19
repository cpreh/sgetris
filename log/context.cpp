#include "context.hpp"
#include <sge/log/context.hpp>

sge::log::context &
sgetris::log::context()
{
	static sge::log::context global_;
	return global_;
}
