#include "backgrounds/flakes/object.hpp"
#include "media_path.hpp"
#include "program_options.hpp"
#include "texture_manager.hpp"
#include "sound_manager.hpp"
#include "machine.hpp"
#include "states/running.hpp"
#include "events/tick.hpp"
#include "log/context.hpp"
#include "log_switcher.hpp"
#include "media_path.hpp"
#include "parser/parse_file.hpp"
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/systems/parameterless.hpp>
#include <sge/systems/image_loader.hpp>
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
#include <sge/audio/sound.hpp>
#include <sge/audio/player.hpp>
#include <sge/audio/loader_capabilities_field.hpp>
#include <sge/log/global.hpp>
#include <sge/time/frames_counter.hpp>
#include <sge/time/duration.hpp>
#include <sge/time/now.hpp>
#include <sge/font/metrics.hpp>
#include <sge/font/object.hpp>
#include <sge/font/drawer_3d.hpp>
#include <sge/font/system.hpp>
#include <sge/font/text_size.hpp>
#include <sge/log/global_context.hpp>
#include <sge/input/system.hpp>
#include <sge/input/action.hpp>
#include <sge/input/key_pair.hpp>
#include <sge/input/key_code.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/capabilities_field.hpp>
#include <sge/mainloop/dispatch.hpp>
#include <sge/extension_set.hpp>
#include <sge/all_extensions.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/dim/input.hpp>
#include <fcppt/math/dim/output.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/container/field_impl.hpp>
#include <fcppt/container/bitfield/basic_impl.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/chrono/duration_arithmetic.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/text.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include <exception>
#include <iostream>
#include <ostream>
#include <cstdlib>

int main(
	int _argc,
	char *_argv[])
try
{
	fcppt::log::activate_levels(
		sge::log::global(),
		fcppt::log::level::debug);
	
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
			"Change screen resolution (format: (w,h))")
		(
			"fps",
			boost::program_options::value<bool>()->default_value(
				true),
			"Show frames per second");
			
	sgetris::log_switcher 
		sge_log(
			&sgetris::program_options,
			FCPPT_TEXT("sge"),
			sge::log::global_context()),
		sgetris_log(
			&sgetris::program_options,
			FCPPT_TEXT("sgetris"),
			sgetris::log::context());
	
	boost::program_options::variables_map vm;
	boost::program_options::store(
		boost::program_options::parse_command_line(
			_argc,
			_argv,
			sgetris::program_options()),
		vm);
	
	boost::program_options::notify(
		vm);

	sge_log.apply(
		vm);

	sgetris_log.apply(
		vm);

	if (vm.count("help"))
	{
		std::cout << sgetris::program_options() << "\n";
		return 0;
	}

	sge::renderer::screen_size const screens =
		vm["screen-size"].as<sge::renderer::screen_size>();

	sge::systems::instance sys(
		sge::systems::list()
		(
			sge::window::parameters(
				FCPPT_TEXT("sgetris snowflaketest")
			)
		)
		(	sge::renderer::parameters(
				sge::renderer::display_mode(
					screens,
					sge::renderer::bit_depth::depth32,
					sge::renderer::refresh_rate_dont_care
				),
				sge::renderer::depth_buffer::off,
				sge::renderer::stencil_buffer::off,
				sge::renderer::window_mode::windowed,
				sge::renderer::vsync::on,
				sge::renderer::no_multi_sampling
			)
		)
		(sge::systems::parameterless::input)
		(sge::systems::parameterless::audio_player)
		(
			sge::systems::image_loader(
				sge::image::capabilities_field::null(),
				fcppt::assign::make_container<
					sge::extension_set
				>(
					FCPPT_TEXT("png")
				)
			)
		)
		(
			sge::systems::audio_loader(
				sge::audio::loader_capabilities_field::null(),
				sge::all_extensions
			)
		)
		(sge::systems::parameterless::font)
	);

	bool running = true;

	sgetris::sound_manager sm(
		sys.audio_loader(),
		sys.audio_player());
	
	sgetris::texture_manager tm(
		sys.image_loader(),
		sys.renderer()
	);

	fcppt::signal::scoped_connection const cb(
		sys.input_system()->register_callback(
			sge::input::action(
				sge::input::kc::key_escape,
				boost::phoenix::ref(running) = false)));
	
	fcppt::scoped_ptr<sgetris::backgrounds::base> bg(
		new sgetris::backgrounds::flakes::object(
			vm,
			sys.renderer(),
			tm));

	sge::font::metrics_ptr const metrics(
		sys.font_system()->create_font(
			sge::config::media_path() / FCPPT_TEXT("fonts") / FCPPT_TEXT("default.ttf"),
			static_cast<sge::font::size_type>(
				15)));

	sge::font::drawer_ptr const drawer(
		fcppt::make_shared_ptr<
			sge::font::drawer_3d
		>(
			sys.renderer(),
			sge::image::colors::green()));

	sge::font::object font(
		metrics,
		drawer);
	
	sge::time::frames_counter frames_counter;

	sgetris::machine m(
		vm,
		sys,
		tm);
	m.initiate();

	sge::time::duration t = 
		sge::time::now().time_since_epoch();

	while(running)
	{
		frames_counter.update();

		sge::time::duration const
			newtime = 
				sge::time::now().time_since_epoch(),
			diff_duration = 
				newtime - t;

		t = newtime;

		sgetris::time_delta const diff(
			diff_duration.count()
		);

		sge::mainloop::dispatch();
		sge::renderer::scoped_block const block_(
			sys.renderer());
		bg->update(
			diff);
		m.process_event(
			sgetris::events::tick(
				diff));
		bg->draw();

		if (vm["fps"].as<bool>())
			font.draw_text(
				FCPPT_TEXT("fps: ")+frames_counter.frames_str(),
				sge::font::pos::null(),
				fcppt::math::dim::structure_cast<sge::font::dim>(
					sys.renderer()->screen_size()),
				sge::font::align_h::left,
				sge::font::align_v::top);
	}

	fcppt::io::cerr << "Am Ende von main\n";
}
catch(fcppt::exception const &e)
{
	fcppt::io::cerr << e.string() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
catch(std::exception const &e)
{
	std::cerr << e.what() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
catch (...)
{
	fcppt::io::cerr << FCPPT_TEXT("Caught someting unknown");
}
