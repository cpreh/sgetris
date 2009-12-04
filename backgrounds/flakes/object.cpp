#include "object.hpp"
#include <sge/image/color/format.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/sprite/intrusive/object.hpp>
#include <sge/sprite/intrusive/parameters.hpp>
#include <sge/math/dim/structure_cast.hpp>
#include <sge/math/null.hpp>
#include <sge/renderer/device.hpp>
#include <sge/image/loader.hpp>
#include <sge/filesystem/path.hpp>
#include <sge/time/second.hpp>
#include <sge/time/unit.hpp>
#include <sge/texture/part.hpp>
#include <sge/texture/add_image.hpp>
#include <sge/texture/default_creator_impl.hpp>
#include <sge/random/uniform.hpp>
#include <sge/sprite/unit.hpp>
#include <sge/random/inclusive_range.hpp>
#include <sge/math/dim/make.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/color.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/image/colors.hpp>

#include <sge/math/vector/output.hpp>
#include <sge/math/dim/output.hpp>
#include <sge/cerr.hpp>

sgetris::backgrounds::flakes::object::object(
	sge::renderer::device_ptr const _renderer,
	// The flakes get the loader because in a later version more than one flake image could be loaded
	// from a directory
	sge::image::loader_ptr const _image_loader,
	flake_count const _flakes,
	sge::filesystem::path const &_image_path)
:
	texture_creator_(
		_renderer,
		sge::image::color::format::rgba8,
		sge::renderer::filter::linear),
	texture_manager_(
		_renderer,
		texture_creator_),
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
	sge::texture::const_part_ptr const t(
		sge::texture::add_image(
			texture_manager_,
			_image_loader->load(
				_image_path)));
	
	sge::random::uniform<sge::sprite::unit> 
		xposition_rng(
			sge::random::make_inclusive_range(
				sge::math::null<sge::sprite::unit>(),
				static_cast<sge::sprite::unit>(
					_renderer->screen_size().w()))),
		yposition_rng(
			sge::random::make_inclusive_range(
				sge::math::null<sge::sprite::unit>(),
				static_cast<sge::sprite::unit>(
					_renderer->screen_size().h())));

	// Those pairs are real::value_type to avoid ugly casting below, they'll be cast
	// one time sge::sprite::unit
	std::pair<real::value_type,real::value_type> 
		size_range(
			static_cast<real::value_type>(
				_renderer->screen_size().w())*
			static_cast<real::value_type>(
				0.05),
			static_cast<real::value_type>(
				_renderer->screen_size().w())*
			static_cast<real::value_type>(
				0.20));
	
	std::pair<real::value_type,real::value_type>
		speed_range(
			static_cast<real::value_type>(
				10),
			static_cast<real::value_type>(
				200));
	
	sge::random::uniform<real::value_type> rng(
		sge::random::make_inclusive_range(
			static_cast<real::value_type>(
				0),
			static_cast<real::value_type>(
				1)));

	for (flake_count i = sge::math::null<flake_count>(); i < _flakes; ++i)
	{
		// Roll the dice
		real::value_type const v = 
			rng();

		sge::sprite::point const position(
			xposition_rng(),
			yposition_rng());

	//	sge::cerr << "adding flake at position: " << position << "\n";

		sge::sprite::dim const size(
			sge::math::dim::structure_cast<sge::sprite::dim>(
				sge::math::dim::make(
					size_range.first + v * (size_range.second - size_range.first),
					size_range.first + v * (size_range.second - size_range.first))));

		sge::sprite::unit const speed = 
			static_cast<sge::sprite::unit>(
				speed_range.first + v * (speed_range.second - speed_range.first));
		
		flakes_.push_back(
			new flake(
				real(
					v),
				_renderer->screen_size(),
				sge::sprite::intrusive::parameters(
					ss_,
					sge::sprite::intrusive::order())
					.pos(
						position)
					.texture(
						t)
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
	ss_.render();
}
