#include "object.hpp"
#include "context.hpp"
#include <sge/log/parameters/root.hpp>
#include <sge/log/parameters/all.hpp>
#include <sge/text.hpp>
#include <sge/cout.hpp>

sge::log::object &
sgetris::log::object()
{
	static sge::log::object global_(
		sge::log::parameters::root(
			sge::cout
		)
		.prefix(
			SGE_TEXT("sge")
		)
		.enabled(
			true
		)
		.level(
			sge::log::level::warning
		)
		.context(
			context()
		)
		.create()
	);

	return global_;
}
