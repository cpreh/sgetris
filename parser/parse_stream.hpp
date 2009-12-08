#ifndef SGETRIS_PARSER_PARSE_STREAM_HPP_HPP_INCLUDED
#define SGETRIS_PARSER_PARSE_STREAM_HPP_HPP_INCLUDED

#include "stone_sequence.hpp"
#include <sge/istream.hpp>

namespace sgetris
{
namespace parser
{
stone_sequence const
parse_stream(
	sge::istream &);
}
}

#endif
