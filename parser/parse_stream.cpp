#include "parse_stream.hpp"
#include "../explode.hpp"
#include "../read_lines.hpp"
#include "../exception.hpp"
#include "../longest_element.hpp"
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/null.hpp>
#include <fcppt/math/almost_zero.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/container/field_impl.hpp>
#include <fcppt/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <algorithm>
#include <vector>

sgetris::parser::stone_sequence const
sgetris::parser::parse_stream(
	fcppt::io::istream &_i)
{
	typedef std::vector<fcppt::string> string_vector;

	stone_sequence stones;
	
	BOOST_FOREACH(
		string_vector const &r,
		explode(
			read_lines(
				_i),
			fcppt::string(
				FCPPT_TEXT(""))))
	{
		if (r.empty())
			throw exception(
				FCPPT_TEXT("Encountered empty stone"));

		fcppt::string::size_type const longest_line = 
			longest_element(
				r);

		stone_template t(
			stone_template::dim(
				static_cast<stone_template::scalar>(
					longest_line),
				static_cast<stone_template::scalar>(
					r.size())));

		for(
			stone_template::size_type y = 
				fcppt::math::null<stone_template::size_type>(); 
			y < t.dimension().h(); 
			++y)
		{
			for(
				stone_template::size_type x = 
					fcppt::math::null<stone_template::size_type>(); 
				x < t.dimension().w(); 
				++x)
			{
				if (r[y].size() <= x)
				{
					t.pos(
						stone_template::vector(
							x,
							y)) = false;
					continue;
				}

				if (r[y][x] != FCPPT_TEXT(' ') && r[y][x] != FCPPT_TEXT('#'))
					throw exception(
						fcppt::string(
							FCPPT_TEXT("Invalid character encountered in stones file: "))+
						r[y][x]+
						FCPPT_TEXT("' numeric code: ")+
						fcppt::lexical_cast<fcppt::string>(
							static_cast<unsigned>(
								r[y][x])));

				t.pos(
					stone_template::vector(
						x,
						y)) = r[y][x] == FCPPT_TEXT(' ') ? false : true;
			}
		}

		stones.push_back(
			t);
	}

	return stones;
}
