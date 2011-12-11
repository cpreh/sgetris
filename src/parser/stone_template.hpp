#ifndef SGETRIS_PARSER_STONE_TEMPLATE_HPP_HPP_INCLUDED
#define SGETRIS_PARSER_STONE_TEMPLATE_HPP_HPP_INCLUDED

#include <fcppt/container/field_decl.hpp>
#include <fcppt/container/raw_vector.hpp>

namespace sgetris
{
namespace parser
{
typedef
fcppt::container::field
<
	fcppt::container::raw_vector
	<
		bool
	>
>
stone_template;
}
}

#endif
