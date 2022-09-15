
# Configure version header.
configure_file(version.h.in "${PROJECT_BINARY_DIR}/include/version.h")

# Configure gtk info header.
configure_file(appid.h.in "${PROJECT_BINARY_DIR}/include/appid.h")


target_sources(ghexedit PRIVATE main.c)

add_subdirectory(classes)
