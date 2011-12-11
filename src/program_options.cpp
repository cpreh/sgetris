#include "program_options.hpp"

boost::program_options::options_description &
sgetris::program_options()
{
	static boost::program_options::options_description d("Allowed options");
	return d;
}
