#include "context.hpp"
#include <fcppt/log/context.hpp>

fcppt::log::context &
sgetris::log::context()
{
	static fcppt::log::context global_;
	return global_;
}
