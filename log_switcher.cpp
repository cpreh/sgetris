#include "log_switcher.hpp"
#include <sge/log/location.hpp>
#include <sge/log/global.hpp>
#include <sge/log/activate_levels.hpp>
#include <sge/log/context.hpp>
#include <sge/log/level_string.hpp>
#include <sge/text.hpp>
#include <sge/assert.hpp>
#include <sge/exception.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>

namespace
{
typedef 
std::vector<sge::string> 
string_vector;

typedef 
std::pair
<
	sge::string,
	sge::string
>
string_pair;

string_vector const explode(
	sge::string const &e,
	sge::string const &seps)
{
	string_vector v;

	boost::algorithm::split(
		v,
		e,
		boost::algorithm::is_any_of(
			seps));

	return v;
}

string_pair const
split(
	sge::string const &e,
	sge::char_type const sep)
{
	sge::string::size_type const p = 
		e.find(sep);

	string_pair result;
	
	if (p == sge::string::npos)
	{
		result.first = e;
		return result;
	}

	result.first = 
		e.substr(
			static_cast<sge::string::size_type>(
				0),
			p);
	
	result.second = 
		e.substr(
			static_cast<sge::string::size_type>(
				p+1));
	
	return result;
}

template<typename Range>
sge::log::location const 
to_location(
	Range const &r)
{
	SGE_ASSERT(
		!r.empty());
		
	sge::log::location l(
		r.front());
	
	std::for_each(
		boost::next(r.begin()),
		r.end(),
		boost::lambda::var(l) += boost::lambda::_1);
	
	return 
		l;
}
}

sgetris::log_switcher::log_switcher(
	options_callback const &_options_callback,
	sge::string const &_prefix,
	sge::log::context &_context)
:
	prefix_(
		_prefix),
	context_(
		_context)
{
	// add_options::operator() takes a const char * instead of std::string
	sge::string const ugly_po_hack = 
		SGE_TEXT(
			"enable-"+prefix_+"-log");
	_options_callback().add_options()
		(
			ugly_po_hack.c_str(),
			//"kacke",
			boost::program_options::value<string_vector>(),
			"Path specifications enabling either all log levels or just one (example: foo/bar/baz:debug)");
}

void
sgetris::log_switcher::apply(
	boost::program_options::variables_map &_vm)
{
	if (!_vm.count("enable-"+prefix_+"-log"))
		return;

	string_vector const specs =
		_vm["enable-"+prefix_+"-log"].as<string_vector>();
	
	BOOST_FOREACH(sge::string const &s,specs)
	{
		string_pair const splitted = 
			split(
				s,
				SGE_TEXT(':'));

		string_vector const parts = 
			explode(
				splitted.first,
				SGE_TEXT("/"));

		sge::log::location const l =
			to_location(
				parts);
		
		sge::log::object *o =
			context_.find(
				l);

		if (!o)
			throw 
				sge::exception(
					SGE_TEXT("The specified log context ")+l.string()+SGE_TEXT(" wasn't found"));
	
		sge::log::level::type const t = 
			splitted.second.empty()
			?
				sge::log::level::debug
			:	
				sge::log::level_from_string(
					splitted.second);

		sge::log::activate_levels(
			*o,
			t);
	}
}
