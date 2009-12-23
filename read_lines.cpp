#include "read_lines.hpp"
#include <istream>

std::vector<fcppt::string> const
sgetris::read_lines(
	fcppt::io::istream &_i)
{
	std::vector<fcppt::string> r;
	fcppt::string line;
	while (std::getline(_i,line))
		r.push_back(
			line);
	return 
		r;
}
