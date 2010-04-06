#ifndef SGETRIS_LAYOUTER_OBJECT_HPP_HPP_INCLUDED
#define SGETRIS_LAYOUTER_OBJECT_HPP_HPP_INCLUDED

#include "alignment.hpp"
#include "stretch.hpp"
#include "pair.hpp"
#include "../sprite/rect.hpp"
#include "../sprite/dim.hpp"
#include "../sprite/scalar.hpp"
#include "../sprite/vector.hpp"
#include "../real.hpp"
#include <boost/intrusive/list.hpp>
#include <fcppt/optional_decl.hpp>
#include <utility>

namespace sgetris
{
namespace layouter
{
class object
:
	public 
	boost::intrusive::list_base_hook
	<
		boost::intrusive::link_mode
		<
			boost::intrusive::auto_unlink
		>
	>
{
public:
	typedef 
	sgetris::real
	real;

	typedef 
	sprite::scalar 
	scalar;

	typedef
	sprite::vector
	vector;

	typedef
	sprite::dim
	dim;

	typedef
	sprite::rect
	rect;

	typedef
	std::pair
	<
		stretch::type,
		scalar
	>
	dimension_pair;

	// A widget can be variable sized, fixed sized, or non-sized on the master
	// axis and can be either fixed size or non-sized on the slave axis
	typedef
	pair
	<
		fcppt::optional<dimension_pair>,
		fcppt::optional<scalar>
	>
	optional_pair;

	explicit
	object(
		rect const &,
		alignment::type,
		real spacing);

	explicit
	object(
		object &,
		optional_pair const &,
		alignment::type,
		real spacing);

	void
	compile();
	
	rect const
	calculated_rect() const;
private:
	typedef
	boost::intrusive::list
	<
		object,
		boost::intrusive::constant_time_size<false>
	>
	child_sequence;

	object * const parent_;
	child_sequence children_;
	alignment::type const alignment_;
	real const spacing_;
	optional_pair const size_specs_;
	fcppt::optional<rect> const fixed_rect_;
	rect calculated_rect_;

	void
	compile(
		rect const &);

	void
	add_child(
		object &);
	
	dim::size_type
	axis() const;

	dim::size_type
	slave_axis() const;

	dim const
	make_dim(
		scalar master,
		scalar slave) const;

	vector const
	make_vector(
		scalar master,
		scalar slave) const;
};
}
}

#endif
