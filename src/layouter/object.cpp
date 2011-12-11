#include "object.hpp"
#include "../log/object.hpp"
#include <fcppt/assert.hpp>
#include <fcppt/assert_message.hpp>
#include <fcppt/optional_impl.hpp>
#include <fcppt/math/null.hpp>
#include <fcppt/math/box/output.hpp>
#include <fcppt/math/almost_zero.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <limits>

namespace
{
fcppt::log::object 
mylogger(
	fcppt::log::parameters::inherited(
		sgetris::log::object(),
		FCPPT_TEXT("layouter")));

template<typename T>
T
bisect(
	T const &t)
{
	return 
		t/static_cast<T>(2);
}

template<typename T>
T
center(
	T const &t,
	T const &u)
{
	return 
		bisect(
			t)-
		bisect(
			u);
}
}

sgetris::layouter::object::object(
	rect const &_rect,
	alignment::type const _alignment,
	real const _spacing)
:
	parent_(
		0),
	children_(),
	alignment_(
		_alignment),
	spacing_(
		_spacing),
	size_specs_(),
	fixed_rect_(
		_rect),
	calculated_rect_()
{
	FCPPT_ASSERT(
		spacing_ >= fcppt::math::null<real>() &&
		spacing_ < static_cast<real>(1))
}

sgetris::layouter::object::object(
	object &_parent,
	optional_pair const &_size_specs,
	alignment::type const _alignment,
	real const _spacing)
:
	parent_(
		&_parent),
	children_(),
	alignment_(
		_alignment),
	spacing_(
		_spacing),
	size_specs_(
		_size_specs),
	fixed_rect_(),
	calculated_rect_()
{
	FCPPT_ASSERT(
		spacing_ >= fcppt::math::null<real>() &&
		spacing_ < static_cast<real>(1))
	parent_->add_child(
		*this);
}

void
sgetris::layouter::object::compile()
{
	FCPPT_ASSERT(
		!parent_);

	// should be implied by above assert, but test anyway
	FCPPT_ASSERT(
		fixed_rect_);
	
	compile(
		*fixed_rect_);
}

sgetris::layouter::object::rect const
sgetris::layouter::object::calculated_rect() const
{
	return 
		calculated_rect_;
}

void
sgetris::layouter::object::compile(
	rect const &top_rect)
{
	calculated_rect_ = 
		top_rect;

	if(children_.empty())
		return;
	
	scalar remaining_space = 
		top_rect.dimension()[axis()];
	
	FCPPT_LOG_DEBUG(
		mylogger,
		fcppt::log::_ 
			<< FCPPT_TEXT("In compile of rect ")
			<< top_rect
			<< FCPPT_TEXT(", setting remaining space to ")
			<< remaining_space);
	
	child_sequence::size_type 
		nonsized_widgets = 
			fcppt::math::null<child_sequence::size_type>(),
		minimum_widgets = 
			fcppt::math::null<child_sequence::size_type>();
	for(
		child_sequence::iterator i = 
			children_.begin(); 
		i != children_.end(); 
		++i)
	{
		optional_pair const &s = 
			i->size_specs_;

		// There was no fixed or minimum size given, then continue
		if (!s.master)
		{
			nonsized_widgets++;
			continue;
		}

		switch (s.master->first)
		{
			case stretch::fixed:
			break;
			case stretch::minimal:
				minimum_widgets++;
			break;
		}

		remaining_space -= s.master->second;
	}

	FCPPT_LOG_DEBUG(
		mylogger,
		fcppt::log::_ 
			<< FCPPT_TEXT("First loop finished, remaining space is ")
			<< remaining_space
			<< FCPPT_TEXT(", there are ")
			<< minimum_widgets
			<< FCPPT_TEXT(" widgets which have a minimum size and ")
			<< nonsized_widgets
			<< FCPPT_TEXT(" widgets which have no size (yet)"));

	FCPPT_ASSERT_MESSAGE(
		remaining_space >= fcppt::math::null<scalar>(),
		FCPPT_TEXT("The accumulated size of the fixed items is larger than the top rectangle"));
	
	scalar const 
		spacing_size_theoretical = 
			static_cast<scalar>(
				static_cast<real>(
					top_rect.dimension()[axis()]) * 
				spacing_),
		spacing_size = 
			remaining_space > spacing_size_theoretical
			? 
				spacing_size_theoretical
			:
				fcppt::math::null<scalar>(),
		remaining_space_without_spacing = 
			remaining_space - spacing_size,
		calculated_spacing = 
			fcppt::math::almost_zero(nonsized_widgets) && 
			fcppt::math::almost_zero(minimum_widgets)
			?
				remaining_space/
				static_cast<scalar>(
					children_.size()+1)
			:
				spacing_size/
				static_cast<scalar>(
					children_.size()+1);

	FCPPT_LOG_DEBUG(
		mylogger,
		fcppt::log::_ 
			<< FCPPT_TEXT("Intermediate calculation finished, theoretical spacing size is ")
			<< spacing_size_theoretical
			<< FCPPT_TEXT(", real spacing size is ")
			<< spacing_size
			<< FCPPT_TEXT(" the remaining space without the spacing is ")
			<< remaining_space_without_spacing
			<< FCPPT_TEXT(" and the calculated spacing is ")
			<< calculated_spacing);
	
	// Now we know the space available for
	// -The non-sized widgets
	// -The minimal-sized widgets
	// We first distribute the space among the non-sized widgets, then among the
	// minimal-sized ones
	
	scalar master_position = 
		top_rect.pos()[axis()]+calculated_spacing;

	for(
		child_sequence::iterator i = 
			children_.begin(); 
		i != children_.end(); 
		++i)
	{
		FCPPT_LOG_DEBUG(
			mylogger,
			fcppt::log::_ 
				<< FCPPT_TEXT("Inside critical loop, master_position is  ")
				<< master_position);
		optional_pair const &s = 
			i->size_specs_;
		scalar master_component = 
			std::numeric_limits<scalar>::max();
		if (s.master)
		{
			switch (s.master->first)
			{
				case stretch::fixed:
					master_component = s.master->second;
				break;
				case stretch::minimal:
					master_component = 
						s.master->second;
					// We can stretch this widget, but only if there are no widgets which
					// have no size
					if (!nonsized_widgets)
					{
						FCPPT_ASSERT(
							minimum_widgets);
						master_component += 
							remaining_space_without_spacing/
							static_cast<scalar>(
								minimum_widgets);
					}
				break;
			}
		}
		else
		{
			FCPPT_ASSERT(
				nonsized_widgets);
			master_component = 
				remaining_space_without_spacing/
				static_cast<scalar>(
					nonsized_widgets);
		}

		scalar slave_component = 
			s.slave
			?
				*s.slave
			:
				top_rect.dimension()[slave_axis()];

		FCPPT_LOG_DEBUG(
			mylogger,
			fcppt::log::_ 
				<< FCPPT_TEXT("Master component is  ")
				<< master_component
				<< FCPPT_TEXT(", slave component is  ")
				<< slave_component
				<< FCPPT_TEXT(", rectangle is ")
				<< rect(
				make_vector(
					master_position,
					top_rect.pos()[slave_axis()]+
					center(
						top_rect.dimension()[slave_axis()],
						slave_component)),
				make_dim(
					master_component,
					slave_component)));

		i->compile(
			rect(
				make_vector(
					master_position,
					top_rect.pos()[slave_axis()]+
					center(
						top_rect.dimension()[slave_axis()],
						slave_component)),
				make_dim(
					master_component,
					slave_component)));

		master_position += 
			master_component + calculated_spacing;
	}
}


void
sgetris::layouter::object::add_child(
	object &p)
{
	children_.push_back(
		p);
}

sgetris::layouter::object::dim::size_type
sgetris::layouter::object::axis() const
{
	switch (alignment_)
	{
		case alignment::horizontal:
			return 
				static_cast<dim::size_type>(
					0);
		case alignment::vertical:
			return 
				static_cast<dim::size_type>(
					1);
	}

	FCPPT_ASSERT(
		false);

	return 
		static_cast<dim::size_type>(
			-1);
}

sgetris::layouter::object::dim::size_type
sgetris::layouter::object::slave_axis() const
{
	switch (alignment_)
	{
		case alignment::horizontal:
			return 
				static_cast<dim::size_type>(
					1);
		case alignment::vertical:
			return 
				static_cast<dim::size_type>(
					0);
	}

	FCPPT_ASSERT(
		false);

	return 
		static_cast<dim::size_type>(
			-1);
}

sgetris::layouter::object::dim const
sgetris::layouter::object::make_dim(
	scalar const _master,
	scalar const _slave) const
{
	dim d;
	d[axis()] = 
		_master;
	d[slave_axis()] = 
		_slave;
	return 
		d;
}

sgetris::layouter::object::vector const
sgetris::layouter::object::make_vector(
	scalar const _master,
	scalar const _slave) const
{
	vector d;
	d[axis()] = 
		_master;
	d[slave_axis()] = 
		_slave;
	return 
		d;
}
