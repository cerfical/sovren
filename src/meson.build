sources = files(
    'sg.cpp',
)

deps = []


if host_machine.system() == 'windows'
    subdir('win32')
    subdir('dx')
endif

subdir('core')
subdir('sovren')


sovren_lib = static_library('sovren',
    sources: sources,
    include_directories: include_directories('../include/sovren', 'sovren')
)

sovren_dep = declare_dependency(
    include_directories: include_directories('../include', '.'),
    dependencies: deps,
    link_with: sovren_lib
)
