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
				"field-width",
				boost::program_options::value<sgetris::real::value_type>()->default_value(
					static_cast<sgetris::real::value_type>(
						0.70)),
				"The size of the playing field in percent of the screen - horizontally")
			(
				"upcoming-height",
				boost::program_options::value<sgetris::real::value_type>()->default_value(
					static_cast<sgetris::real::value_type>(
						0.80)),
				"The size of the upcoming stone box in percent of the screen - vertically");
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
	sge::sprite::rect<sprite::object::unit>::type
	rect;

	typedef
	rectangle_aligner<rect>
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
	rectangle_aligner aligner;

	sge::texture::const_part_ptr const block_texture = 
		context<machine>().texture_manager().texture(
			SGE_TEXT("block"));
	sge::texture::lock_rect::value_type const block_size = 
		block_texture->area().width();

	for(
		upcoming_sequence::size_type i = 
			sge::math::null<upcoming_sequence>(); 
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
