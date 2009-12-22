#include "exception.hpp"
#include <fcppt/text.hpp>

sgetris::exception::exception(
	fcppt::string const &w)
: 
	sge::exception(
		FCPPT_TEXT("sgetris: ")+w)
{
}
