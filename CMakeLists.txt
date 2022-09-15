
cmake_minimum_required(VERSION 3.21)

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()


project(GHexEdit
    VERSION 0.1.0
    DESCRIPTION "GUI Hex Editor"
    LANGUAGES C
)

# Set Application ID
set(APPLICATION_ID com.github.treecase.ghexedit)
# Set GResource prefix
set(GRESOURCE_PREFIX /com/github/treecase/ghexedit/)

# Dependencies
# GTK4
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)


configure_file(
    "ghexedit.desktop.in"
    "${APPLICATION_ID}.desktop"
)

configure_file(
    "ghexedit.gschema.xml.in"
    "ghexedit.gschema.xml"
)

# Generate gschemas.compiled.
add_custom_target(
    gschemas.compiled
    COMMAND
        glib-compile-schemas
        --strict
        --targetdir=.
        --schema-file=ghexedit.gschema.xml
)

# Main executable target
add_executable(ghexedit)
add_dependencies(ghexedit gschemas.compiled)
target_compile_features(ghexedit PRIVATE c_std_11)
set_target_properties(ghexedit PROPERTIES C_EXTENSIONS OFF)
target_include_directories(ghexedit PRIVATE "${PROJECT_BINARY_DIR}/include")
target_include_directories(ghexedit PRIVATE "${GTK4_INCLUDE_DIRS}")
target_link_directories(ghexedit PRIVATE "${GTK4_LIBRARY_DIRS}")
target_link_libraries(ghexedit PRIVATE "${GTK4_LIBRARIES}")

# Generate GResource data
add_subdirectory(gresource)

# Generate include headers, add source files
add_subdirectory(src)


# Install rules
include(GNUInstallDirs)
install(TARGETS ghexedit)
install(FILES "${PROJECT_BINARY_DIR}/${APPLICATION_ID}.desktop"
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications"
)
install(DIRECTORY icons/
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons"
)
install(FILES "${PROJECT_BINARY_DIR}/ghexedit.gschema.xml"
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/glib-2.0/schemas"
    RENAME "${APPLICATION_ID}.gschema.xml"
)
