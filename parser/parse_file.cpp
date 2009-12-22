#include "parse_file.hpp"
#include "parse_stream.hpp"
#include "../exception.hpp"
#include <fcppt/text.hpp>
#include <fcppt/io/ifstream.hpp>

sgetris::parser::stone_sequence const
sgetris::parser::parse_file(
	fcppt::filesystem::path const &_p)
{
	fcppt::io::ifstream f(
		_p);
	if (!f.is_open())
		throw exception(
			FCPPT_TEXT("Couldn't open file \"")+_p.string()+FCPPT_TEXT("\""));
	return 
		parse_stream(
			f);
}
