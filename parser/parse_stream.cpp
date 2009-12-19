#include "parse_stream.hpp"
#include "../exception.hpp"
#include <sge/string.hpp>
#include <sge/text.hpp>
#include <sge/math/null.hpp>
#include <sge/math/vector/basic_impl.hpp>
#include <sge/math/dim/basic_impl.hpp>
#include <sge/container/field_impl.hpp>
#include <sge/cerr.hpp>
#include <sge/lexical_cast.hpp>
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
		sge::math::null<typename T::value_type::size_type>();
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
	sge::istream &_i)
{
	typedef
	std::vector<sge::string>
	string_sequence;

	stone_sequence stones;
	string_sequence current_stone;

	sge::string line;
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
				SGE_TEXT("Too many empty lines encountered"));

		sge::string::size_type const longest_line = 
			longest_element(
				current_stone);

		if (longest_line == sge::math::null<sge::string::size_type>())
			throw exception(
				SGE_TEXT("No proper stones found"));

		stone_template t(
			stone_template::dim_type(
				static_cast<stone_template::dim_type::value_type>(
					longest_line),
				static_cast<stone_template::dim_type::value_type>(
					current_stone.size())));

		sge::cerr << "loop beginning\n";

		for(
			stone_template::size_type y = 
				sge::math::null<stone_template::size_type>(); 
			y < t.dim().h(); 
			++y)
		{
			for(
				stone_template::size_type x = 
					sge::math::null<stone_template::size_type>(); 
				x < t.dim().w(); 
				++x)
			{
				sge::cerr << "current length is " << current_stone[y].size() << ", current index is " << x << "\n";
				if (current_stone[y].size() <= x)
				{
					sge::cerr << "doing nothing\n";
					t.pos(
						stone_template::vector_type(
							x,
							y)) = false;
					continue;
				}

				sge::cerr << "doing something\n";
				if (current_stone[y][x] != SGE_TEXT(' ') && current_stone[y][x] != SGE_TEXT('#'))
					throw exception(
						SGE_TEXT("Invalid somewhere around line ")+
						sge::lexical_cast<sge::string>(
							line_counter-y)+
						SGE_TEXT(" of stones file: '")+
						current_stone[y][x]+
						SGE_TEXT("' numeric code: ")+
						sge::lexical_cast<sge::string>(
							static_cast<unsigned>(
								current_stone[y][x])));

				t.pos(
					stone_template::vector_type(
						x,
						y)) = current_stone[y][x] == SGE_TEXT(' ') ? false : true;
			}
		}

		sge::cerr << "loop finished\n";

		current_stone.clear();

		stones.push_back(
			t);
	}

	return stones;
}
