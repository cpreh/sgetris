#include "object.hpp"
#include "flake.hpp"
#include "../../sprite/object.hpp"
#include "../../sprite/system.hpp"
#include "../../texture_manager.hpp"
#include "../../media_path.hpp"
#include "../../program_options.hpp"
#include <sge/math/dim/structure_cast.hpp>
#include <sge/math/null.hpp>
#include <sge/renderer/device.hpp>
#include <sge/image/loader.hpp>
#include <sge/filesystem/path.hpp>
#include <sge/time/second.hpp>
#include <sge/time/unit.hpp>
#include <sge/random/uniform.hpp>
#include <sge/random/inclusive_range.hpp>
#include <sge/math/dim/make.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/color.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/image/colors.hpp>
#include <sge/sprite/default_equal.hpp>

#include <sge/math/vector/output.hpp>
#include <sge/math/dim/output.hpp>
#include <sge/text.hpp>

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
				"flakes-count",
				boost::program_options::value<sgetris::backgrounds::flakes::flake_count>()->default_value(
					static_cast<sgetris::backgrounds::flakes::flake_count>(
						20)),
				"Number of flakes")
			(
				"flakes-size-min",
				boost::program_options::value<sgetris::real::value_type>()->default_value(
					static_cast<sgetris::real::value_type>(
						0.05)),
				"Minimum flake size (in percent of the screen)")
			(
				"flakes-size-max",
				boost::program_options::value<sgetris::real::value_type>()->default_value(
					static_cast<sgetris::real::value_type>(
						0.20)),
				"Maximum flake size (in percent of the screen)")
			(
				"flakes-speed-min",
				boost::program_options::value<sgetris::real::value_type>()->default_value(
					static_cast<sgetris::real::value_type>(
						10)),
				"Minimum flake speed (in pixels per second)")
			(
				"flakes-speed-max",
				boost::program_options::value<sgetris::real::value_type>()->default_value(
					static_cast<sgetris::real::value_type>(
						200)),
				"Maximum flake speed (in pixels per second)");
	}
private:
} options;
}

sgetris::backgrounds::flakes::object::object(
	boost::program_options::variables_map &_program_options,
	sge::renderer::device_ptr const _renderer,
	// The flakes get the loader because in a later version more than one flake image could be loaded
	// from a directory
	texture_manager &_texture_manager)
:
	texture_manager_(
		_texture_manager),
	ss_(
		_renderer),
	clock_(),
	frame_timer_(
		sge::time::second(
			static_cast<sge::time::unit>(
				1)),
		sge::time::activation_state::active,
		clock_.callback()),
	flakes_()
{
	texture_manager_.load(
		media_path()/SGE_TEXT("backgrounds")/SGE_TEXT("flakes")/SGE_TEXT("textures.ini"));
	sge::random::uniform<sprite::object::unit> 
		xposition_rng(
			sge::random::make_inclusive_range(
				sge::math::null<sprite::object::unit>(),
				static_cast<sprite::object::unit>(
					_renderer->screen_size().w()))),
		yposition_rng(
			sge::random::make_inclusive_range(
				sge::math::null<sprite::object::unit>(),
				static_cast<sprite::object::unit>(
					_renderer->screen_size().h())));

	// Those pairs are real::value_type to avoid ugly casting below, they'll be cast
	// one time sprite::object::unit
	std::pair<real::value_type,real::value_type> 
		size_range(
			static_cast<real::value_type>(
				_renderer->screen_size().w())*
			_program_options["flakes-size-min"].as<real::value_type>(),
			static_cast<real::value_type>(
				_renderer->screen_size().w())*
			_program_options["flakes-size-max"].as<real::value_type>());
	
	std::pair<real::value_type,real::value_type>
		speed_range(
			_program_options["flakes-speed-min"].as<real::value_type>(),
			_program_options["flakes-speed-max"].as<real::value_type>());
	
	sge::random::uniform<real::value_type> rng(
		sge::random::make_inclusive_range(
			static_cast<real::value_type>(
				0),
			static_cast<real::value_type>(
				1)));

	for(
		flake_count i = sge::math::null<flake_count>(),
		fc = _program_options["flakes-count"].as<flake_count>(); 
		i < fc; 
		++i)
	{
		// Roll the dice
		real::value_type const v = 
			rng();

		sprite::object::point const position(
			xposition_rng(),
			yposition_rng());

		sprite::object::dim const size(
			sge::math::dim::structure_cast<sprite::object::dim>(
				sge::math::dim::make(
					size_range.first + v * (size_range.second - size_range.first),
					size_range.first + v * (size_range.second - size_range.first))));

		sprite::object::unit const speed = 
			static_cast<sprite::object::unit>(
				speed_range.first + v * (speed_range.second - speed_range.first));
		
		flakes_.push_back(
			new flake(
				real(
					v),
				_renderer->screen_size(),
				sprite::parameters()
					.system(
						&ss_)
					.order(
						0u)
					.pos(
						position)
					.texture(
						texture_manager_.texture(
							SGE_TEXT("flake")))
					.size(
						size),
				speed));
	}

	_renderer->state(
		sge::renderer::state::list
			(sge::renderer::state::bool_::clear_backbuffer = true)
			(sge::renderer::state::color::clear_color
				= sge::image::colors::black()
			)
	);
}

void
sgetris::backgrounds::flakes::object::update(
	time_delta const _td)
{
	clock_.update(
		_td);
	real const d(
		sge::math::real_cast<real>(
			frame_timer_.elapsed_frames()));
	frame_timer_.reset();
	for (flake_sequence::iterator i = flakes_.begin(); i != flakes_.end(); ++i)
		i->update(
			d);
}

void
sgetris::backgrounds::flakes::object::draw()
{
	ss_.render_all(
		sge::sprite::default_equal());
}
