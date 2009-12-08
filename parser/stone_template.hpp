#ifndef SGETRIS_PARSER_STONE_TEMPLATE_HPP_HPP_INCLUDED
#define SGETRIS_PARSER_STONE_TEMPLATE_HPP_HPP_INCLUDED

#include <sge/container/field_decl.hpp>
#include <sge/container/raw_vector.hpp>

namespace sgetris
{
namespace parser
{
typedef
sge::container::field
<
	bool,
	sge::container::raw_vector
>
stone_template;
}
}

#endif
