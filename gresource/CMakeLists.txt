
# Generate the GResource XML file.
configure_file(ghexedit.gresource.xml.in ghexedit.gresource.xml)

# Generate resources.c.
add_custom_target(resources.c
    COMMAND glib-compile-resources
        ghexedit.gresource.xml
        --target=resources.c
        --sourcedir="${CMAKE_CURRENT_SOURCE_DIR}"
        --generate-source
)
