#include "object.hpp"
#include "context.hpp"
#include <fcppt/log/parameters/root.hpp>
#include <fcppt/log/parameters/all.hpp>
#include <fcppt/text.hpp>
#include <fcppt/io/cout.hpp>

fcppt::log::object &
sgetris::log::object()
{
	static fcppt::log::object global_(
		fcppt::log::parameters::root(
			fcppt::io::cout
		)
		.prefix(
			FCPPT_TEXT("sgetris")
		)
		.enabled(
			true
		)
		.level(
			fcppt::log::level::warning
		)
		.context(
			context()
		)
		.create()
	);

	return global_;
}
