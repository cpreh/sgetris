#ifndef SGETRIS_LOG_SWITCHER_HPP_HPP_INCLUDED
#define SGETRIS_LOG_SWITCHER_HPP_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>
#include <fcppt/string.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/log/context_fwd.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

namespace sgetris
{
class log_switcher
{
FCPPT_NONCOPYABLE(log_switcher)
public:
	typedef
	boost::program_options::options_description &
	options_callback_fn();

	typedef
	fcppt::function::object<options_callback_fn>
	options_callback;

	explicit
	log_switcher(
		options_callback const &,
		fcppt::string const &prefix,
		fcppt::log::context &);

	void
	apply(
		boost::program_options::variables_map &);
private:
	fcppt::string const prefix_;
	fcppt::log::context &context_;
};
}

#endif
