#include "parse_stream.hpp"
#include "../exception.hpp"
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/null.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/container/field_impl.hpp>
#include <fcppt/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <algorithm>
#include <vector>

namespace
{
template<typename T>
typename T::value_type::size_type
longest_element(
	T const &t)
{
	typename T::value_type::size_type longest = 
		fcppt::math::null<typename T::value_type::size_type>();
	BOOST_FOREACH(typename T::const_reference s,t)
		longest = 
			std::max(
				longest,
				s.size());
	return 
		longest;
}
}

sgetris::parser::stone_sequence const
sgetris::parser::parse_stream(
	fcppt::io::istream &_i)
{
	typedef
	std::vector<fcppt::string>
	string_sequence;

	stone_sequence stones;
	string_sequence current_stone;

	fcppt::string line;
	bool last_line = true;
	unsigned line_counter = 0;
	while (std::getline(_i,line) || last_line)
	{
		++line_counter;

		if (_i.eof())
			last_line = false;

		if (!line.empty() && !_i.eof())
		{
			current_stone.push_back(
				line);
			continue;
		}

		if (current_stone.empty())
			throw exception(
				FCPPT_TEXT("Too many empty lines encountered"));

		fcppt::string::size_type const longest_line = 
			longest_element(
				current_stone);

		if (longest_line == fcppt::math::null<fcppt::string::size_type>())
			throw exception(
				FCPPT_TEXT("No proper stones found"));

		stone_template t(
			stone_template::dim(
				static_cast<stone_template::scalar>(
					longest_line),
				static_cast<stone_template::scalar>(
					current_stone.size())));


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
				if (current_stone[y].size() <= x)
				{
					t.pos(
						stone_template::vector(
							x,
							y)) = false;
					continue;
				}

				if (current_stone[y][x] != FCPPT_TEXT(' ') && current_stone[y][x] != FCPPT_TEXT('#'))
					throw exception(
						FCPPT_TEXT("Invalid somewhere around line ")+
						fcppt::lexical_cast<fcppt::string>(
							line_counter-y)+
						FCPPT_TEXT(" of stones file: '")+
						current_stone[y][x]+
						FCPPT_TEXT("' numeric code: ")+
						fcppt::lexical_cast<fcppt::string>(
							static_cast<unsigned>(
								current_stone[y][x])));

				t.pos(
					stone_template::vector(
						x,
						y)) = current_stone[y][x] == FCPPT_TEXT(' ') ? false : true;
			}
		}


		current_stone.clear();

		stones.push_back(
			t);
	}

	return stones;
}
