#include "running.hpp"
#include "../program_options.hpp"
#include "../game_logics/standard.hpp"
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
				boost::program_options::value<sgetris::states::running::field::dim_type>()->default_value(
					sgetris::states::running::field::dim_type(
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
				"margins",
				boost::program_options::value<sgetris::real::value_type>()->default_value(
					static_cast<sgetris::real::value_type>(
						0.01)),
				"Size of the margins between hud/field/upcoming stones in percent of the screen height");
	}
private:
} options;
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
		context<machine>().program_options_map()["field-size"].as<field::dim_type>()),
	ss_(
		context<machine>().systems().renderer())
{
	calculate_dims();
	generate_upcoming_list();
}

boost::statechart::result 
sgetris::states::running::react(
	events::tick const &t)
{
	for (object_sequence::iterator i = objects_.begin(); i != objects_.end();)
	{
		if (i->can_be_removed())
		{
			i = 
				objects_.erase(
					i);
			continue;
		}

		i->update(
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
	iterator_picker<parser::stone_sequence> picker;
	rectangle_aligner aligner(
		upcoming_rect_);

	for(
		upcoming_sequence::size_type i = 
			sge::math::null<upcoming_sequence::size_type>(); 
		i < upcoming_count; 
		++i)
	{
		parser::stone_sequence::const_iterator const it = 
			picker.pick();

		stone_template::size_type const 
			maxdim = 
				std::max(
					it->dim().w(),
					it->dim().h());

		iterators.push_back(
			std::make_pair(
				it,
				aligner.insert(
					rect(
						rect::pos_type(),
						rect::dim_type(
							static_cast<rect::value_type>(
								block_size),
							static_cast<rect::value_type>(
								block_size))))));
	}

	aligner.compile();

	BOOST_FOREACH(iterator_container::const_reference r,iterators)
	{
		parser::stone_template const &t = 
			*r.first;
			
		stone_template::size_type const 
			maxdim = 
				std::max(
					t.dim().w(),
					t.dim().h());

		r.resize(
			field::dim_type(
				maxdim,
				maxdim));

		for (field::size_type y = sge::math::null<field::size_type>(); y < maxdim; ++y)
		{
			for (field::size_type x = sge::math::null<field::size_type>(); x < maxdim; ++x)
			{
				if (x >= t.dim().w() || y >= t.dim().h())
					continue;

				objects_.push_back(
					new objects::sprite_block(
						sprite::parameters()
							.system(&ss_)
							.texture(
								block_texture)
							.order(
								0u)
							// FIXME: add position here
							.pos(
								)
							.texture_size()));

				r.pos(
					field::vector_type(
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

	sprite::object::unit const 
		margin_value = 
			static_cast<real::value_type>(
				screen_size.h()) * 
			vm["margins"].as<real::value_type>(),
		field_height = 
			static_cast<sprite::object::unit>(
				screen_size.h()) - 
			static_cast<sprite::object::unit>(
				2)*
			margin_value,
		block_height = 
			field_height/
			static_cast<sprite::object::unit>(
				field_.dim().h());

	block_dim_ = 
		sprite::object::dim(
			block_height,
			block_height);
	
	field_rect_ = 
		rect(
			sprite::object::point(
				margin_value,
				margin_value),
			sprite::dim(
				static_cast<sprite::object::unit>(
					block_height * 
					static_cast<sprite::object::unit>(
						field_.dim().w())),
				field_height));
	
	// calculate the height of the largest stone, multiply by the number of upcoming pieces and get the height of the upcoming box
	parser::stone_template::size_type max_height = 
		possible_stones_.front().dim().h();
	BOOST_FOREACH(parser::stone_template const &r,possible_stones_)
		max_height = 
			std::max(
				max_height,
				r.dim().h());
	
	sprite::object::unit const 
		upcoming_height = 
			static_cast<sprite::object::unit>(
				max_height)*
			static_cast<sprite::object::unit>(
				context<machine>().program_options_map()["upcoming-count"].as<upcoming_sequence::size_type>()),
		remaining_width = 
			screen_size.w() - 
			margin_value - 
			field_rect_.w() - 
			margin_value - 
			/* insert hud and upcoming here */
			margin_value;
	
	upcoming_rect_ = 
		rect(
			sprite::object::point(
				margin_value,
				field_rect_.w() + 
				margin_value),
			sprite::object::dim(
				remaining_width,
				upcoming_height));
	
	hud_rect_ = 
		rect(
			sprite::object::point(
				upcoming_rect_.left(),
				upcoming_rect_.bottom() + margin_value),
			sprite::object::dim(
				static_cast<sprite::object::unit>(
					screen_size.h() - 
					margin_value - 
					upcoming_rect_.h() - 
					margin_value)));
}
