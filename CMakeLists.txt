cmake_minimum_required(VERSION 3.5)

set(LIBRARY_SOURCES "lcl_vect.c" "lcl_err.c" "lcl_list.c" "lcl_string.c")

# Check if building for ESP32
if (IDF_TARGET STREQUAL "esp32")
    set(TARGET_ESP32 TRUE)
endif()

if( TARGET_ESP32 )

idf_component_register(SRCS ${LIBRARY_SOURCES}
                    INCLUDE_DIRS "include")

else()
    # Project name
    project(lcl)


    add_library( lcl ${LIBRARY_SOURCES} )
    target_include_directories( lcl PUBLIC "./include" )

endif()