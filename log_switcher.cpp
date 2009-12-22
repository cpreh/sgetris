#include "log_switcher.hpp"
#include <fcppt/log/location.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/log/context.hpp>
#include <fcppt/log/level_from_string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert.hpp>
#include <sge/log/global.hpp>
#include <sge/exception.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>

namespace
{
typedef 
std::vector<fcppt::string> 
string_vector;

typedef 
std::pair
<
	fcppt::string,
	fcppt::string
>
string_pair;

string_vector const explode(
	fcppt::string const &e,
	fcppt::string const &seps)
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
	fcppt::string const &e,
	fcppt::char_type const sep)
{
	fcppt::string::size_type const p = 
		e.find(sep);

	string_pair result;
	
	if (p == fcppt::string::npos)
	{
		result.first = e;
		return result;
	}

	result.first = 
		e.substr(
			static_cast<fcppt::string::size_type>(
				0),
			p);
	
	result.second = 
		e.substr(
			static_cast<fcppt::string::size_type>(
				p+1));
	
	return result;
}

template<typename Range>
fcppt::log::location const 
to_location(
	Range const &r)
{
	FCPPT_ASSERT(
		!r.empty());
		
	fcppt::log::location l(
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
	fcppt::string const &_prefix,
	fcppt::log::context &_context)
:
	prefix_(
		_prefix),
	context_(
		_context)
{
	// add_options::operator() takes a const char * instead of std::string
	fcppt::string const ugly_po_hack = 
		FCPPT_TEXT(
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
	
	BOOST_FOREACH(fcppt::string const &s,specs)
	{
		string_pair const splitted = 
			split(
				s,
				FCPPT_TEXT(':'));

		string_vector const parts = 
			explode(
				splitted.first,
				FCPPT_TEXT("/"));

		fcppt::log::location const l =
			to_location(
				parts);
		
		fcppt::log::object *o =
			context_.find(
				l);

		if (!o)
			throw 
				sge::exception(
					FCPPT_TEXT("The specified log context ")+l.string()+FCPPT_TEXT(" wasn't found"));
	
		fcppt::log::level::type const t = 
			splitted.second.empty()
			?
				fcppt::log::level::debug
			:	
				fcppt::log::level_from_string(
					splitted.second);

		fcppt::log::activate_levels(
			*o,
			t);
	}
}
