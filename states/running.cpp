#include "running.hpp"
#include "../real.hpp"
#include "../program_options.hpp"
#include "../game_logics/standard.hpp"
#include "../parser/parse_file.hpp"
#include "../objects/sprite_block.hpp"
#include "../layouter/object.hpp"
#include "../rectangle_aligner.hpp"
#include "../iterator_picker.hpp"
#include "../texture_manager.hpp"
#include "../sprite/scalar.hpp"
#include "../log/object.hpp"
#include <sge/systems/instance.hpp>
#include <fcppt/math/dim/arithmetic.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/dim/output.hpp>
#include <fcppt/math/dim/input.hpp>
#include <fcppt/math/box/output.hpp>
#include <fcppt/container/field_impl.hpp>
#include <fcppt/io/cerr.hpp>
#include <boost/program_options.hpp>

namespace
{
struct program_options
{
public:
	program_options()
	{
		sgetris::program_options().add_options()
			(
				"field-size",
				boost::program_options::value<sgetris::states::running::field::dim>()->default_value(
					sgetris::states::running::field::dim(
						10,
						20)),
				"Field size")
			(
				"upcoming-count",
				boost::program_options::value<sgetris::states::running::upcoming_sequence::size_type>()->default_value(
					static_cast<sgetris::states::running::upcoming_sequence::size_type>(
						2)),
				"The number of upcoming stones to be displayed")
			(
				"spacing",
				boost::program_options::value<sgetris::real>()->default_value(
					static_cast<sgetris::real>(
						0.01)),
				"Size of the spacing between hud/field/upcoming stones in percent of the screen width");
	}
private:
} options;
}

namespace
{
fcppt::log::object 
mylogger(
	fcppt::log::parameters::inherited(
		sgetris::log::object(),
		FCPPT_TEXT("running")));
}

sgetris::states::running::running(
	my_context _ctx)
:
	my_base(
		_ctx),
	logic_(
		new game_logics::standard()),
	possible_stones_(
		parser::parse_file(
			logic_->stone_file())),
	upcoming_(),
	objects_(),
	field_(
		context<machine>().program_options_map()["field-size"].as<field::dim>()),
	ss_(
		context<machine>().systems().renderer())
{
	calculate_rects();
	generate_upcoming_list();
}

boost::statechart::result 
sgetris::states::running::react(
	events::tick const &t)
{
	for (object_sequence::iterator i = objects_.begin(); i != objects_.end();)
	{
		if ((*i)->can_be_removed())
		{
			i = 
				objects_.erase(
					i);
			continue;
		}

		(*i)->update(
			t.delta());
	
		++i;
	}

	return discard_event();
}


void
sgetris::states::running::generate_upcoming_list()
{
	upcoming_sequence::size_type const upcoming_count = 
		context<machine>().program_options_map()["upcoming-count"].as<upcoming_sequence::size_type>();
	
	// Vorgehen: 
	// -n mal folgendes:
	// --Stein zufällig auswählen, Iterator speichern
	// --Größe berechnen (in nxn-Matrix umwandeln, davon die Größe nehmen)
	// --in rectangle_aligner packen, Iterator speichern
	// -Kompilieren
	// -Liste durchlaufen, sprite_blocks instanziieren mit entsprechender
	//  Position, in field<weak_ptr> speichern. Dieses field in upcoming_liste
	//  einfügen.
	
	typedef
	rectangle_aligner<sprite::rect>
	rectangle_aligner;

	typedef 
	std::pair
	<
		parser::stone_sequence::const_iterator,
		rectangle_aligner::iterator
	>
	iterator_pair;

	typedef
	std::vector
	<
		iterator_pair
	>
	iterator_container;

	iterator_container iterators;
	iterator_picker<parser::stone_sequence> picker(
		possible_stones_);
	rectangle_aligner aligner(
		upcoming_rect_,
		rectangle_aligner::y_axis);

	for(
		upcoming_sequence::size_type i = 
			fcppt::math::null<upcoming_sequence::size_type>(); 
		i < upcoming_count; 
		++i)
	{
		parser::stone_sequence::const_iterator const it = 
			picker.pick();

		parser::stone_template::size_type const 
			maxdim = 
				std::max(
					it->dimension().w(),
					it->dimension().h());

		iterators.push_back(
			std::make_pair(
				it,
				aligner.insert(
					sprite::dim(
						static_cast<sprite::scalar>(maxdim) * 
						fcppt::math::dim::structure_cast<sprite::dim>(
							block_dim_)))));
	}

	aligner.compile();

	sge::texture::const_part_ptr const block_texture = 
		context<machine>().texture_manager().texture(
			FCPPT_TEXT("block"));

	BOOST_FOREACH(iterator_container::const_reference its,iterators)
	{
		parser::stone_template const &t = 
			*its.first;
			
		parser::stone_template::size_type const 
			maxdim = 
				std::max(
					t.dimension().w(),
					t.dimension().h());

		field r(
			field::dim(
				maxdim,
				maxdim));

		for (field::size_type y = fcppt::math::null<field::size_type>(); y < maxdim; ++y)
		{
			for (field::size_type x = fcppt::math::null<field::size_type>(); x < maxdim; ++x)
			{
				if (x >= t.dimension().w() || y >= t.dimension().h())
					continue;

				objects_.push_back(
					object_sequence::value_type(
						new objects::sprite_block(
							sprite::parameters()
								.system(&ss_)
								.texture(
									block_texture)
								.order(
									0u)
								.pos(
									its.second->pos() + 
									sprite::vector(
										static_cast<sprite::scalar>(
											x * block_dim_.w()),
										static_cast<sprite::scalar>(
											y * block_dim_.h())))
								.size(
									block_dim_))));

				r.pos(
					field::vector(
						x,
						y)) = objects_.back();
			}
		}
	}
}

void
sgetris::states::running::calculate_rects()
{
	sge::renderer::screen_size const screen_size = 
		context<machine>().systems().renderer()->screen_size();

	real const spacing = 
		context<machine>().program_options_map()["spacing"].as<real>();
	
	sgetris::layouter::object top_layout(
		sprite::rect(
			sprite::vector::null(),
			fcppt::math::dim::structure_cast<sprite::dim>(
				screen_size)),
		sgetris::layouter::alignment::horizontal,
		spacing);

	sprite::scalar const 
		field_height = 
			static_cast<sprite::scalar>(
				screen_size.h()),
		block_size = 
			field_height/
			static_cast<sprite::scalar>(
				field_.dimension().h());
	
	// FIXME: log what's calculated here
	FCPPT_LOG_DEBUG(
		mylogger,
		fcppt::log::_ 
			<< FCPPT_TEXT("field height is ")
			<< field_height
			<< FCPPT_TEXT(", field width is ")
			<< (block_size * field_.dimension().w())
			<< FCPPT_TEXT(", block_size is ")
			<< block_size);

	block_dim_ = 
		sprite::dim(
			block_size,
			block_size);
	
	layouter::object field_layout(
		top_layout,
		layouter::object::optional_pair(
			layouter::object::dimension_pair(
				layouter::stretch::fixed,
				static_cast<sprite::scalar>(
					block_size * 
					static_cast<sprite::scalar>(
						field_.dimension().w()))),
				fcppt::optional<sprite::scalar>()),
		layouter::alignment::horizontal,
		fcppt::math::null<real>());
	
	layouter::object right_layout(
		top_layout,
		layouter::object::optional_pair(),
		layouter::alignment::vertical,
		spacing);

	// calculate the maximum height and width of a stone multiply by the number of upcoming pieces and get the height/width of the upcoming box
	parser::stone_template::size_type 
		max_height = 
			possible_stones_.front().dimension().h(),
		max_width = 
			possible_stones_.front().dimension().w();

	BOOST_FOREACH(parser::stone_template const &r,possible_stones_)
	{
		max_height = 
			std::max(
				max_height,
				r.dimension().h());
		max_width = 
			std::max(
				max_width,
				r.dimension().w());
	}

	FCPPT_LOG_DEBUG(
		mylogger,
		fcppt::log::_ 
			<< FCPPT_TEXT("maximum height is  ")
			<< max_height
			<< FCPPT_TEXT(", maximum width is ")
			<< max_width);

	layouter::object upcoming_layout(
		right_layout,
		layouter::object::optional_pair(
			layouter::object::dimension_pair(
				layouter::stretch::minimal,
				static_cast<sprite::scalar>(
					max_height)*
				block_size*
				static_cast<sprite::scalar>(
					context<machine>().program_options_map()["upcoming-count"].as<upcoming_sequence::size_type>())),
			static_cast<sprite::scalar>(
				max_width*block_size)),
		layouter::alignment::horizontal,
		fcppt::math::null<real>());
	
	layouter::object hud_layout(
		right_layout,
		layouter::object::optional_pair(),
		layouter::alignment::horizontal,
		fcppt::math::null<real>());
	
	top_layout.compile();

	FCPPT_LOG_DEBUG(
		mylogger,
		fcppt::log::_ 
			<< FCPPT_TEXT("calculated rects: top: ")
			<< top_layout.calculated_rect()
			<< FCPPT_TEXT(", field_layout: ")
			<< field_layout.calculated_rect()
			<< FCPPT_TEXT(", right_layout: ")
			<< right_layout.calculated_rect()
			<< FCPPT_TEXT(", upcoming_layout: ")
			<< upcoming_layout.calculated_rect()
			<< FCPPT_TEXT(", hud_layout: ")
			<< hud_layout.calculated_rect());
	
	field_rect_ = 
		field_layout.calculated_rect();
	upcoming_rect_ =
		upcoming_layout.calculated_rect();
	hud_rect_ =
		hud_layout.calculated_rect();
}
