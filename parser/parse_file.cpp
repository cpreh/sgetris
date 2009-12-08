#include "parse_file.hpp"
#include "parse_stream.hpp"
#include "../exception.hpp"
#include <sge/text.hpp>
#include <sge/fstream.hpp>

sgetris::parser::stone_sequence const
sgetris::parser::parse_file(
	sge::filesystem::path const &_p)
{
	sge::ifstream f(
		_p);
	if (!f.is_open())
		throw exception(
			SGE_TEXT("Couldn't open file \"")+_p.string()+SGE_TEXT("\""));
	return 
		parse_stream(
			f);
}
