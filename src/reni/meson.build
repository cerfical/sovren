# Simple script to generate header for project configuration

# Subdirectory is needed to allow unified consumption of the header from dependent projects
# through the use of <PROJECT_NAME/config.hpp>

buildtype = get_option('buildtype').startswith('debug') ? 'Debug' : 'Release'

configure_file(
    input: '../config.hpp.in',
    output: 'config.hpp',
    configuration: {
        'VERSION': meson.project_version(),
        'CONFIG': buildtype
    }
)
