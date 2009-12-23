#include <boost/next_prior.hpp>
#include <boost/foreach.hpp>
#include <vector>
#include <iostream>

// vector<string> -> vector<vector<string>>
template<typename Container>
std::vector<Container> const
explode(
	Container const &c,
	typename Container::value_type const &separator)
{
	std::vector<Container> r;
	typename Container::const_iterator last = 
		c.begin();
	for (typename Container::const_iterator i = c.begin(); i != c.end(); ++i)
	{
		if (*i != separator)
			continue;

		r.push_back(
			Container(
				last,
				i));

		last = 
			boost::next(
				i);
	}

	r.push_back(
		Container(
			last,
			c.end()));

	return r;
}

int main()
{
	typedef std::vector<std::string> string_vector;

	string_vector s;
	s.push_back("bar");
	s.push_back("foo");

	std::vector<string_vector> exploded = 
		explode(
			s,
			std::string("bar"));
	
	BOOST_FOREACH(string_vector const &p,exploded)
	{
		std::cout << "vector begin:\n";
		BOOST_FOREACH(std::string const &str,p)
			std::cout << str << "\n";
		std::cout << "vector end\n\n";
	}
}
