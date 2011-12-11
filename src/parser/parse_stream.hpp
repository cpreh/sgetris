#ifndef SGETRIS_PARSER_PARSE_STREAM_HPP_HPP_INCLUDED
#define SGETRIS_PARSER_PARSE_STREAM_HPP_HPP_INCLUDED

#include "stone_sequence.hpp"
#include <fcppt/io/istream.hpp>

namespace sgetris
{
namespace parser
{
stone_sequence const
parse_stream(
	fcppt::io::istream &);
}
}

#endif
