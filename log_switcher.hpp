#ifndef SGETRIS_LOG_SWITCHER_HPP_HPP_INCLUDED
#define SGETRIS_LOG_SWITCHER_HPP_HPP_INCLUDED

#include <sge/noncopyable.hpp>
#include <sge/string.hpp>
#include <sge/function/object.hpp>
#include <sge/log/context_fwd.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

namespace sgetris
{
class log_switcher
{
SGE_NONCOPYABLE(log_switcher)
public:
	typedef
	boost::program_options::options_description &
	options_callback_fn();

	typedef
	sge::function::object<options_callback_fn>
	options_callback;

	explicit
	log_switcher(
		options_callback const &,
		sge::string const &prefix,
		sge::log::context &);

	void
	apply(
		boost::program_options::variables_map &);
private:
	sge::string const prefix_;
	sge::log::context &context_;
};
}

#endif
