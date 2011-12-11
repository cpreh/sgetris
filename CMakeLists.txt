CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

PROJECT(sgetris)

INCLUDE(FcpptCMakeUtils)

FIND_PACKAGE(
	Boost 1.41.0 REQUIRED COMPONENTS
	program_options
)

INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIR})

LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

# Find FCPPT

FIND_PACKAGE(
	Fcppt REQUIRED
)

INCLUDE_DIRECTORIES(${FCPPT_INCLUDE_DIRS})

LINK_DIRECTORIES(${FCPPT_LIBRARY_DIRS})

# Find SGE

FIND_PACKAGE(
	SGE REQUIRED
)

INCLUDE_DIRECTORIES(${SGE_INCLUDE_DIRS})

SET(
	INSTALL_MEDIA_DIR
	${INSTALL_DATA_DIR}/media
)

INCLUDE(
	SGECustomPath
)

SGE_CONFIG_ADD_CUSTOM_PATH(
	sgetris
	media
	${CMAKE_SOURCE_DIR}/media
	${INSTALL_MEDIA_DIR}
)

INSTALL(
	DIRECTORY
	${CMAKE_SOURCE_DIR}/media/
	DESTINATION
	${INSTALL_MEDIA_DIR}
)

SET(
	SGETRIS_FILES
	src/backgrounds/base.cpp
	src/backgrounds/base.hpp
	src/backgrounds/flakes/flake.cpp
	src/backgrounds/flakes/flake.hpp
	src/backgrounds/flakes/flake_count.hpp
	src/backgrounds/flakes/flake_fwd.hpp
	src/backgrounds/flakes/object.cpp
	src/backgrounds/flakes/object.hpp
	src/clock.hpp
	src/diff_clock.hpp
	src/events/tick.cpp
	src/events/tick.hpp
	src/exception.cpp
	src/exception.hpp
	src/explode.hpp
	src/game_logics/base.cpp
	src/game_logics/base.hpp
	src/game_logics/standard.cpp
	src/game_logics/standard.hpp
	src/iterator_picker.hpp
	src/iterator_picker_impl.hpp
	src/layouter/alignment.hpp
	src/layouter/object.cpp
	src/layouter/object.hpp
	src/layouter/pair.hpp
	src/layouter/stretch.hpp
	src/level.hpp
	src/level_up_callback.hpp
	src/level_up_callback_fn.hpp
	src/log/context.cpp
	src/log/context.hpp
	src/log/object.cpp
	src/log/object.hpp
	src/log_switcher.cpp
	src/log_switcher.hpp
	src/longest_element.hpp
	src/machine.cpp
	src/machine.hpp
	src/main.cpp
	src/media_path.cpp
	src/media_path.hpp
	src/objects/base.cpp
	src/objects/base.hpp
	src/objects/base_fwd.hpp
	src/objects/block.hpp
	src/objects/sprite_block.cpp
	src/objects/sprite_block.hpp
	src/objects/weak_ptr.hpp
	src/parser/parse_file.cpp
	src/parser/parse_file.hpp
	src/parser/parse_stream.cpp
	src/parser/parse_stream.hpp
	src/parser/stone_sequence.hpp
	src/parser/stone_template.hpp
	src/point.hpp
	src/program_options.cpp
	src/program_options.hpp
	src/read_lines.cpp
	src/read_lines.hpp
	src/real.hpp
	src/real_point.hpp
	src/rectangle_aligner.hpp
	src/rectangle_aligner_impl.hpp
	src/score.hpp
	src/sound_manager.cpp
	src/sound_manager.hpp
	src/sprite/choices.hpp
	src/sprite/color.hpp
	src/sprite/dim.hpp
	src/sprite/object.hpp
	src/sprite/parameters.hpp
	src/sprite/rect.hpp
	src/sprite/rotation_type.hpp
	src/sprite/scalar.hpp
	src/sprite/system.hpp
	src/sprite/vector.hpp
	src/states/running.cpp
	src/states/running.hpp
	src/states/running_fwd.hpp
	src/texture_manager.cpp
	src/texture_manager.hpp
	src/texture_manager_fwd.hpp
	src/time_delta.hpp
)

ADD_EXECUTABLE(
	sgetris
	${SGETRIS_FILES}
)

TARGET_LINK_LIBRARIES(
	sgetris
	${SGE_LIBRARIES}
)

IF(NOT MSVC)
	TARGET_LINK_LIBRARIES(sgetris ${Boost_LIBRARIES})
ENDIF()

INSTALL(
	TARGETS
	sgetris
	DESTINATION
	${INSTALL_BINARY_DIR}
)
