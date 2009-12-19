#include "running.hpp"
#include "../real.hpp"
#include "../program_options.hpp"
#include "../game_logics/standard.hpp"
#include "../parser/parse_file.hpp"
#include "../objects/sprite_block.hpp"
#include "../rectangle_aligner.hpp"
#include "../iterator_picker.hpp"
#include "../texture_manager.hpp"
#include "../sprite/scalar.hpp"
#include <sge/systems/instance.hpp>
#include <sge/math/dim/arithmetic.hpp>
#include <sge/math/dim/structure_cast.hpp>
#include <sge/math/dim/basic_impl.hpp>
#include <sge/math/dim/output.hpp>
#include <sge/math/dim/input.hpp>
#include <sge/container/field_impl.hpp>
#include <sge/cerr.hpp>
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
		/*context<machine>().program_options_map()["field-size"].as<field::dim_type>()*/),
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
	sge::cerr << "tick event\n";
	for (object_sequence::iterator i = objects_.begin(); i != objects_.end();)
	{
		if ((*i)->can_be_removed())
		{
			sge::cerr << "erasing object\n";
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
			sge::math::null<upcoming_sequence::size_type>(); 
		i < upcoming_count; 
		++i)
	{
		parser::stone_sequence::const_iterator const it = 
			picker.pick();

		parser::stone_template::size_type const 
			maxdim = 
				std::max(
					it->dim().w(),
					it->dim().h());

		iterators.push_back(
			std::make_pair(
				it,
				aligner.insert(
					sprite::dim(
						static_cast<sprite::scalar>(maxdim) * 
						sge::math::dim::structure_cast<sprite::dim>(
							block_dim_)))));
	}

	aligner.compile();

	sge::texture::const_part_ptr const block_texture = 
		context<machine>().texture_manager().texture(
			SGE_TEXT("block"));

	BOOST_FOREACH(iterator_container::const_reference its,iterators)
	{
		parser::stone_template const &t = 
			*its.first;
			
		parser::stone_template::size_type const 
			maxdim = 
				std::max(
					t.dim().w(),
					t.dim().h());

		field r(
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

	sprite::scalar const 
		margin_value = 
			static_cast<sprite::scalar>(
				static_cast<real::value_type>(
					screen_size.h()) * 
				context<machine>().program_options_map()["margins"].as<real::value_type>()),
		field_height = 
			static_cast<sprite::scalar>(
				screen_size.h()) - 
			static_cast<sprite::scalar>(
				2)*
			margin_value,
		block_height = 
			field_height/
			static_cast<sprite::scalar>(
				field_.dim().h());

	block_dim_ = 
		sprite::dim(
			block_height,
			block_height);
	
	field_rect_ = 
		sprite::rect(
			sprite::vector(
				margin_value,
				margin_value),
			sprite::dim(
				static_cast<sprite::scalar>(
					block_height * 
					static_cast<sprite::scalar>(
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
	
	sprite::scalar const 
		upcoming_height = 
			static_cast<sprite::scalar>(
				max_height)*
			static_cast<sprite::scalar>(
				context<machine>().program_options_map()["upcoming-count"].as<upcoming_sequence::size_type>()),
		remaining_width = 
			screen_size.w() - 
			margin_value - 
			field_rect_.w() - 
			margin_value - 
			/* insert hud and upcoming here */
			margin_value;
	
	upcoming_rect_ = 
		sprite::rect(
			sprite::vector(
				margin_value,
				field_rect_.w() + 
				margin_value),
			sprite::dim(
				remaining_width,
				upcoming_height));
	
	hud_rect_ = 
		sprite::rect(
			sprite::vector(
				upcoming_rect_.left(),
				upcoming_rect_.bottom() + margin_value),
			sprite::dim(
				upcoming_rect_.w(),
				static_cast<sprite::scalar>(
					screen_size.h() - 
					margin_value - 
					upcoming_rect_.h() - 
					margin_value)));
}
