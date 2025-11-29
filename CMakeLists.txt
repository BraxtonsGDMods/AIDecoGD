cmake_minimum_required(VERSION 3.20)
project(AI_Deco LANGUAGES CXX)

# This file is a minimal starting CMake file for building the AI Deco mod.
# In a proper Geode dev environment you may need to provide include paths and
# link against Geode and Geometry Dash libraries. This file organizes sources
# and sets C++ standard to C++17 (as requested).

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Provide nlohmann/json via FetchContent so presets can be parsed robustly.
FetchContent_Declare(
	nlohmann_json
	GIT_REPOSITORY https://github.com/nlohmann/json.git
	GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
# Gather any platform/Geode specific sources separately so we can conditionally
# include them only on supported hosts. This prevents Linux/unsupported-host
# builds from trying to compile headers that expect Windows/other platforms.
file(GLOB_RECURSE GEODE_SRCS "src/*_geode.cpp")
foreach(g ${GEODE_SRCS})
	list(REMOVE_ITEM SOURCES ${g})
endforeach()
# Exclude test sources from the main module target (they will be built as separate executables)
file(GLOB_RECURSE TEST_SRCS "src/tests/*.cpp")
foreach(t ${TEST_SRCS})
	list(REMOVE_ITEM SOURCES ${t})
endforeach()
file(GLOB_RECURSE HEADERS "include/*.hpp")

add_library(ai_deco MODULE ${SOURCES} ${HEADERS})
target_link_libraries(ai_deco PRIVATE nlohmann_json::nlohmann_json)

# Optionally enable Geode build (user should configure GEODE include/library paths)
option(BUILD_GEODE "Build with Geode integration (requires Geode SDK on system)" OFF)
if (BUILD_GEODE)
	# Decide whether we should actually compile the _geode.cpp sources on the
	# current host. Geode code often targets Windows, Android, or macOS; trying
	# to compile them on an unsupported host (like Linux in this dev container)
	# will trigger header/platform errors. Only add geode sources when the host
	# looks supported.
	set(GEODE_BUILD_ON_HOST OFF)
	if (WIN32 OR APPLE OR ANDROID)
		set(GEODE_BUILD_ON_HOST ON)
	endif()

	if(GEODE_BUILD_ON_HOST)
		message(STATUS "BUILD_GEODE=ON and host supports Geode; including geode sources")
		list(APPEND SOURCES ${GEODE_SRCS})
		target_compile_definitions(ai_deco PRIVATE GEODE)
	else()
		message(WARNING "BUILD_GEODE=ON but host platform is not a supported Geode target; _geode.cpp sources will be skipped so the project can still build on this machine.")
	endif()
	# If a third_party copy of Geode exists, default GEODE_INCLUDE_DIR to its loader include
	if (NOT DEFINED GEODE_INCLUDE_DIR)
		if (EXISTS ${CMAKE_SOURCE_DIR}/third_party/geode/loader/include)
			set(GEODE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/third_party/geode/loader/include")
		endif()
	endif()

	if (DEFINED GEODE_INCLUDE_DIR)
		# Ensure the provided include dir actually contains required Geode headers
		if (EXISTS "${GEODE_INCLUDE_DIR}/Geode/Enums.hpp")
			message(STATUS "Using GEODE include dir: ${GEODE_INCLUDE_DIR}")
			target_include_directories(ai_deco PRIVATE ${GEODE_INCLUDE_DIR})
			# Warn if game headers are missing
			if (NOT EXISTS "${GEODE_INCLUDE_DIR}/cocos-ext.h" AND NOT EXISTS "${GEODE_INCLUDE_DIR}/cocos2d.h")
				message(WARNING "GEODE include dir does not contain cocos headers (cocos-ext.h / cocos2d.h). You must add the game's headers to ${GEODE_INCLUDE_DIR} (run tools/prepare_game_headers.sh) to enable true in-editor spawning.)")
			endif()
		else()
			message(FATAL_ERROR "GEODE include dir found (${GEODE_INCLUDE_DIR}) but required header Geode/Enums.hpp is missing. Provide a full Geode SDK or correct GEODE_INCLUDE_DIR.")
		endif()
	else()
		message(WARNING "BUILD_GEODE=ON but GEODE_INCLUDE_DIR not provided and no third_party/geode detected. Set -DGEODE_INCLUDE_DIR to compile GEODE code.")
	endif()
	# Users should provide GEODE_INCLUDE_DIR and GEODE_LIBS when invoking CMake
	# If GEODE_LIBS is provided (e.g. a path or a list), link them into the
	# target so the resulting DLL resolves imports at link time on Windows.
	if (DEFINED GEODE_LIBS AND NOT "${GEODE_LIBS}" STREQUAL "")
		# Allow users to pass multiple libraries separated by ; or whitespace.
		string(REPLACE " " ";" GEODE_LIBS_SEMICOLON "${GEODE_LIBS}")
		message(STATUS "Linking GEODE libs: ${GEODE_LIBS_SEMICOLON}")
		target_link_libraries(ai_deco PRIVATE ${GEODE_LIBS_SEMICOLON})
	endif()
endif()

# Example include path for Geode (change to your Geode SDK path)
# target_include_directories(ai_deco PRIVATE "${CMAKE_SOURCE_DIR}/path/to/geode/include")

# Example linking (replace with actual Geode libs)
# target_link_libraries(ai_deco PRIVATE geode)

# Set output name appropriate for Geode/Geometry Dash DLL naming on Windows
set_target_properties(ai_deco PROPERTIES PREFIX "" SUFFIX ".dll")

message(STATUS "SOURCES: ${SOURCES}")
message(STATUS "HEADERS: ${HEADERS}")

# Optional test targets to help local development (not required by Geode)
option(BUILD_TESTS "Build local smoke tests and engine tests" ON)
if (BUILD_TESTS)
	file(GLOB_RECURSE ENGINE_SRCS "src/engine/*.cpp")
	file(GLOB_RECURSE PRESET_SRCS "src/presets/*.cpp")

	add_executable(engine_test ${ENGINE_SRCS} ${PRESET_SRCS} src/tests/engine_test.cpp)
		add_executable(preset_test ${PRESET_SRCS} src/tests/preset_test.cpp)
		target_include_directories(preset_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
		target_link_libraries(preset_test PRIVATE nlohmann_json::nlohmann_json)
	target_include_directories(engine_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
	target_link_libraries(engine_test PRIVATE nlohmann_json::nlohmann_json)

	# Smoke test: compile a small standalone binary that runs placeholders
	add_executable(aid_deco_test src/main.cpp)
	target_include_directories(aid_deco_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
	target_link_libraries(aid_deco_test PRIVATE nlohmann_json::nlohmann_json)
endif()
