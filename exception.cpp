#include "exception.hpp"
#include <sge/text.hpp>

sgetris::exception::exception(
	sge::string const &w)
: 
	sge::exception(
		SGE_TEXT("sanguis: ")+w)
{
}
