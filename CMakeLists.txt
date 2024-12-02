cmake_minimum_required(VERSION 3.5)

set(LIBRARY_SOURCES 
    "lcl_vect.c" 
    "lcl_err.c" 
    "lcl_list.c" 
    "lcl_string.c"    
    "lcl_bitvect.c"
    "lcl_hash.c"
    "lcl_mpool.c"
    "lcl_rc.c"
    "lcl_hmap.c"
)
set(TEST_SRCS 
    "unit-test/unit-vect.c"
    "unit-test/unit-main.c"
    "unit-test/unit-str.c"
    "unit-test/unit-list.c"
    "unit-test/unit-mpool.c"

    "unit-test/unit-hmap.c"
)

# Check if building for ESP32
if (IDF_TARGET STREQUAL "esp32")
    set(TARGET_ESP32 TRUE)
endif()

if( TARGET_ESP32 )

idf_component_register(SRCS ${LIBRARY_SOURCES}
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES unity)

else()
    # Project name
    project(lcl)


    add_library( lcl ${LIBRARY_SOURCES} )
    target_include_directories( lcl PUBLIC "./include" )

    # Host (desktop) system configuration

    # Fetch Unity if not building for ESP32
    include(FetchContent)
    FetchContent_Declare(
        unity
        GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
        GIT_TAG v2.5.2  # Replace with the desired version
    )
    FetchContent_MakeAvailable(unity)

    add_executable(unit_tests ${TEST_SRCS})
    target_link_libraries(unit_tests lcl unity)
    

    target_include_directories(unit_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    # Define the target for running tests on the host
    add_custom_target(run_tests
                      COMMAND unit_tests
                      DEPENDS unit_tests)

endif()