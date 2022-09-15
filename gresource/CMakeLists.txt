
# Generate the GResource XML file.
configure_file(ghexedit.gresource.xml.in ghexedit.gresource.xml)

# Generate resources.c.
# The touch is needed so target_sources can file a file with that name.
file(TOUCH "${CMAKE_CURRENT_BINARY_DIR}/resources.c")
add_custom_target(resources.c
    COMMAND glib-compile-resources
        ghexedit.gresource.xml
        --target=resources.c
        --sourcedir="${CMAKE_CURRENT_SOURCE_DIR}"
        --generate-source
)

add_dependencies(ghexedit resources.c)
target_sources(ghexedit PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/resources.c")
