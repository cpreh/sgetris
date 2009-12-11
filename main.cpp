#include "backgrounds/flakes/object.hpp"
#include "media_path.hpp"
#include "program_options.hpp"
#include "texture_manager.hpp"
#include "sound_manager.hpp"
#include <sge/audio/multi_loader.hpp>
#include <sge/audio/sound.hpp>
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
#include <sge/audio/player.hpp>
#include <sge/log/global.hpp>
#include <sge/log/activate_levels.hpp>
#include <sge/time/time.hpp>
#include <sge/input/system.hpp>
#include <sge/input/action.hpp>
#include <sge/input/key_pair.hpp>
#include <sge/input/key_code.hpp>
#include <sge/image/multi_loader.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/color/rgba8.hpp>
#include <sge/image/color/init.hpp>
#include <sge/sprite/object.hpp>
#include <sge/sprite/system.hpp>
#include <sge/sprite/parameters.hpp>
#include <sge/sprite/texture_animation.hpp>
#include <sge/texture/manager.hpp>
#include <sge/texture/add_image.hpp>
#include <sge/texture/no_fragmented.hpp>
#include <sge/texture/default_creator.hpp>
#include <sge/texture/default_creator_impl.hpp>
#include <sge/mainloop/dispatch.hpp>
#include <sge/math/dim/structure_cast.hpp>
#include <sge/math/dim/input.hpp>
#include <sge/math/dim/output.hpp>
#include <sge/cerr.hpp>
#include <sge/exception.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/program_options.hpp>
#include <exception>
#include <iostream>
#include <ostream>
#include <cstdlib>

#include "media_path.hpp"
#include "parser/parse_file.hpp"
#include <boost/foreach.hpp>
#include <sge/cout.hpp>
#include <sge/container/field_impl.hpp>

int main(
	int _argc,
	char *_argv[])
try
{
	sge::log::activate_levels(
		sge::log::global(),
		sge::log::level::debug);
	
	sgetris::program_options().add_options()
		(
			"help",
			"produce help message")
		(
			"screen-size",
			boost::program_options::value<sge::renderer::screen_size>()->default_value(
				sge::renderer::screen_size(
					1024,
					768)),
			"Change screen resolution (format: (w,h))");
	
	boost::program_options::variables_map vm;
	boost::program_options::store(
		boost::program_options::parse_command_line(
			_argc,
			_argv,
			sgetris::program_options()),
		vm);
	
	boost::program_options::notify(
		vm);

	if (vm.count("help"))
	{
		std::cout << sgetris::program_options() << "\n";
		return 0;
	}

	sge::systems::instance sys(
		sge::systems::list()
		(sge::window::parameters(
			SGE_TEXT("sgetris snowflaketest")
		))
		(sge::renderer::parameters(
			sge::renderer::display_mode(
				vm["screen-size"].as<sge::renderer::screen_size>(),
				sge::renderer::bit_depth::depth32,
				sge::renderer::refresh_rate_dont_care),
			sge::renderer::depth_buffer::off,
			sge::renderer::stencil_buffer::off,
			sge::renderer::window_mode::windowed,
			sge::renderer::vsync::on,
			sge::renderer::no_multi_sampling
		))
		(sge::systems::parameterless::input)
		(sge::systems::parameterless::audio_player)
		(sge::systems::parameterless::image));

	bool running = true;

	sge::audio::multi_loader audio_loader(
		sys.plugin_manager());
	sgetris::sound_manager sm(
		audio_loader,
		sys.audio_player());
	
	sge::image::multi_loader image_loader(
		sys.plugin_manager());
	sgetris::texture_manager tm(
		image_loader,
		sys.renderer());

	sge::signal::scoped_connection const cb(
		sys.input_system()->register_callback(
			sge::input::action(
				sge::input::kc::key_escape,
				boost::phoenix::ref(running) = false)));
	
	sge::scoped_ptr<sgetris::backgrounds::base> bg(
		new sgetris::backgrounds::flakes::object(
			vm,
			sys.renderer(),
			tm));

	sgetris::time_delta t = 
		sge::time::time();
	

	/*
	sgetris::machine m(
		_argv,
		_argv);
	m.initiate();
	*/

	while(running)
	{
		sgetris::time_delta const 
			newtime = 
				sge::time::time(),
			diff = 
				newtime - t;

		t = newtime;

		sge::mainloop::dispatch();
		sge::renderer::scoped_block const block_(
			sys.renderer());
		bg->update(
			diff);
			/*
		m.process_event(
			sgetris::events::tick(
				diff));
				*/
		bg->draw();
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
