#ifndef SGETRIS_EXCEPTION_HPP_HPP_INCLUDED
#define SGETRIS_EXCEPTION_HPP_HPP_INCLUDED

#include <sge/exception.hpp>
#include <sge/string.hpp>

namespace sgetris
{
class exception 
	: public sge::exception 
{
public:
	explicit 
	exception(
		sge::string const &);
};
}

#endif
