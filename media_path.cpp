#include "media_path.hpp"
#include <sge/filesystem/path.hpp>
#include <sge/config/find_path.hpp>
#include <sge/config/make_files.hpp>
#include <sge/text.hpp>
#include <boost/assign/list_of.hpp>

sge::filesystem::path const
sgetris::media_path()
{
	static sge::filesystem::path const ret(
		sge::config::find_path(
			sge::config::make_files(
				SGE_TEXT("sgetris")
			),
			SGE_TEXT("sgetris_media"),
			boost::assign::list_of(
				SGE_TEXT("media")
			)
		)
	);

	return ret;
}
