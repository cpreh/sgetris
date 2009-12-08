#ifndef SGETRIS_PARSER_PARSE_FILE_HPP_HPP_INCLUDED
#define SGETRIS_PARSER_PARSE_FILE_HPP_HPP_INCLUDED

#include "stone_sequence.hpp"
#include <sge/filesystem/path.hpp>

namespace sgetris
{
namespace parser
{
stone_sequence const
parse_file(
	sge::filesystem::path const &);
}
}

#endif
