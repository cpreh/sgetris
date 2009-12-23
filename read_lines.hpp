#ifndef SGETRIS_READ_LINES_HPP_HPP_INCLUDED
#define SGETRIS_READ_LINES_HPP_HPP_INCLUDED

#include <fcppt/string.hpp>
#include <fcppt/io/istream.hpp>
#include <vector>

namespace sgetris
{
std::vector<fcppt::string> const
read_lines(
	fcppt::io::istream &);
}

#endif
