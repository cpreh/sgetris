#include "parse_stream.hpp"
#include "../exception.hpp"
#include <sge/string.hpp>
#include <sge/text.hpp>
#include <sge/math/null.hpp>
#include <sge/math/vector/basic_impl.hpp>
#include <sge/math/dim/basic_impl.hpp>
#include <sge/container/field_impl.hpp>
#include <sge/cerr.hpp>
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
	while (std::getline(_i,line) || last_line)
	{
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
				if (current_stone[y][x] != SGE_TEXT(' ') && current_stone[y][x] != SGE_TEXT('#'))
					throw exception(
						SGE_TEXT("Invalid character in stones file"));

				t.pos(
					stone_template::vector_type(
						x,
						y)) = current_stone[y][x] == SGE_TEXT(' ') ? false : true;
			}
		}

		current_stone.clear();

		stones.push_back(
			t);
	}

	return stones;
}
