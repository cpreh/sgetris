#include "../layouter/object.hpp"
#include "../real.hpp"
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/signal/scoped_connection.hpp>
#include <sge/config/media_path.hpp>
#include <sge/renderer/refresh_rate_dont_care.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/system.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/var.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/sprite/bounding_rect.hpp>
#include <sge/math/box/output.hpp>
#include <sge/log/global.hpp>
#include <sge/log/activate_levels.hpp>
#include <sge/input/system.hpp>
#include <sge/input/action.hpp>
#include <sge/input/key_pair.hpp>
#include <sge/input/key_code.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/color/rgba8.hpp>
#include <sge/image/color/rgba8_format.hpp>
#include <sge/image/color/init.hpp>
#include <sge/image/color/object_impl.hpp>
#include <sge/sprite/object_impl.hpp>
#include <sge/sprite/system.hpp>
#include <sge/sprite/external_system_impl.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <sge/sprite/choices.hpp>
#include <sge/sprite/type_choices.hpp>
#include <sge/sprite/with_color.hpp>
#include <sge/sprite/with_depth.hpp>
#include <sge/sprite/default_sort.hpp>
#include <sge/sprite/default_equal.hpp>
#include <sge/mainloop/dispatch.hpp>
#include <sge/math/dim/structure_cast.hpp>
#include <sge/cerr.hpp>
#include <sge/exception.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <exception>
#include <ostream>
#include <vector>
#include <cstdlib>

namespace
{

typedef sge::image::color::rgba8_format sprite_color;

typedef 
sge::sprite::choices
<
	sge::sprite::type_choices
	<
		int,
		float,
		sprite_color
	>,
	boost::mpl::vector2
	<
		sge::sprite::with_color,
		sge::sprite::with_depth
	> 
> sprite_choices;

typedef 
sge::sprite::object
<
	sprite_choices
> sprite_object;

}

int main()
try
{
	sge::log::activate_levels(
		sge::log::global(),
		sge::log::level::debug
	);

	sge::renderer::screen_size const screen_size(
		1024,
		768);

	sge::systems::instance sys(
		sge::systems::list()
		(
			sge::window::parameters(
				SGE_TEXT("layouter test")))
		(
			sge::renderer::parameters(
				sge::renderer::display_mode(
					screen_size,
					sge::renderer::bit_depth::depth32,
					sge::renderer::refresh_rate_dont_care),
				sge::renderer::depth_buffer::off,
				sge::renderer::stencil_buffer::off,
				sge::renderer::window_mode::windowed,
				sge::renderer::vsync::on,
				sge::renderer::no_multi_sampling))
		(sge::systems::parameterless::input));

	sgetris::layouter::object top(
		sgetris::sprite::rect(
			sgetris::sprite::vector(
				10,10),
			sgetris::sprite::dim(
				300,300)),
		sgetris::layouter::alignment::horizontal,
		static_cast<sgetris::real::value_type>(
			0.1));
	
	sgetris::layouter::object left(
		top,
		sgetris::layouter::object::optional_pair(),
		sgetris::layouter::alignment::horizontal,
		static_cast<sgetris::real::value_type>(
			0));

	sgetris::layouter::object right(
		top,
		sgetris::layouter::object::optional_pair(),
		sgetris::layouter::alignment::vertical,
		static_cast<sgetris::real::value_type>(
			0.1));

	sgetris::layouter::object right_top(
		right,
		sgetris::layouter::object::optional_pair(
			sgetris::layouter::object::dimension_pair(
				sgetris::layouter::stretch::fixed,
				static_cast<sgetris::sprite::scalar>(
					200)),
			static_cast<sgetris::sprite::scalar>(
				100)),
		sgetris::layouter::alignment::vertical,
		static_cast<sgetris::real::value_type>(
			0));

	sgetris::layouter::object right_bottom(
		right,
		sgetris::layouter::object::optional_pair(),
		sgetris::layouter::alignment::vertical,
		static_cast<sgetris::real::value_type>(
			0));
	
	top.compile();

	typedef sge::sprite::system
	<
		sprite_choices
	>::type sprite_system;

	typedef sge::sprite::parameters
	<
		sprite_choices
	> sprite_parameters;

	sprite_system ss(
		sys.renderer()
	);

	sprite_object top_sprite(
		sprite_parameters()
		.pos(
			top.calculated_rect().pos())
		.size(
			top.calculated_rect().dim())
		.color(
			sge::image::color::rgba8(
				sge::image::color::init::red %= 1.0,
				sge::image::color::init::green %= 0.0,
				sge::image::color::init::blue %= 0.0,
				sge::image::color::init::alpha %= 0.5))
		.depth(
			static_cast<sprite_object::depth_type>(
				10))
		.elements()
	);

	sprite_object left_sprite(
		sprite_parameters()
		.depth(
			static_cast<sprite_object::depth_type>(
				0))
		.pos(
			left.calculated_rect().pos())
		.size(
			left.calculated_rect().dim())
		.depth(
			static_cast<sprite_object::depth_type>(
				8))
		.color(
			sge::image::color::rgba8(
				sge::image::color::init::red %= 0.0,
				sge::image::color::init::green %= 0.0,
				sge::image::color::init::blue %= 1.0,
				sge::image::color::init::alpha %= 1.0))
		.elements());

	sprite_object right_sprite(
		sprite_parameters()
		.depth(
			static_cast<sprite_object::depth_type>(
				8))
		.pos(
			right.calculated_rect().pos())
		.size(
			right.calculated_rect().dim())
		.color(
			sge::image::color::rgba8(
				sge::image::color::init::red %= 0.0,
				sge::image::color::init::green %= 1.0,
				sge::image::color::init::blue %= 0.0,
				sge::image::color::init::alpha %= 1.0))
		.elements());

	sprite_object right_top_sprite(
		sprite_parameters()
		.depth(
			static_cast<sprite_object::depth_type>(
				12))
		.pos(
			right_top.calculated_rect().pos())
		.size(
			right_top.calculated_rect().dim())
		.color(
			sge::image::color::rgba8(
				sge::image::color::init::red %= 1.0,
				sge::image::color::init::green %= 1.0,
				sge::image::color::init::blue %= 0.0,
				sge::image::color::init::alpha %= 1.0))
		.elements());

	sprite_object right_bottom_sprite(
		sprite_parameters()
		.depth(
			static_cast<sprite_object::depth_type>(
				12))
		.pos(
			right_bottom.calculated_rect().pos())
		.size(
			right_bottom.calculated_rect().dim())
		.color(
			sge::image::color::rgba8(
				sge::image::color::init::red %= 1.0,
				sge::image::color::init::green %= 1.0,
				sge::image::color::init::blue %= 1.0,
				sge::image::color::init::alpha %= 1.0))
		.elements());
	
	sge::cerr << "right bottom sprite: " << sge::sprite::bounding_rect(right_bottom_sprite) << ", right top sprite: " << sge::sprite::bounding_rect(right_top_sprite) << "\n";

	bool running = true;

	sge::signal::scoped_connection const cb(
		sys.input_system()->register_callback(
			sge::input::action(
				sge::input::kc::key_escape,
				boost::phoenix::ref(running) = false
			)
		)
	);

	sys.renderer()->state(
		sge::renderer::state::list
			(sge::renderer::state::bool_::clear_backbuffer = true)
			(sge::renderer::state::color::clear_color = sge::image::colors::black())
	);

	while(running)
	{
		sge::mainloop::dispatch();

		sge::renderer::scoped_block const block_(
			sys.renderer()
		);

		std::vector<
			sprite_object
		> sprites;

		sprites.push_back(top_sprite);
		sprites.push_back(left_sprite);
		sprites.push_back(right_sprite);
		sprites.push_back(right_top_sprite);
		sprites.push_back(right_bottom_sprite);

		ss.render(
			sprites.begin(),
			sprites.end(),
			sge::sprite::default_sort(),
			sge::sprite::default_equal()
		);
	}
}
catch(sge::exception const &e)
{
	sge::cerr << e.string() << SGE_TEXT('\n');
	return EXIT_FAILURE;
}
catch(std::exception const &e)
{
	sge::cerr << e.what() << SGE_TEXT('\n');
	return EXIT_FAILURE;
}
