sources = files(
    'print.cpp',
    'rg.cpp',
)

deps = []


if host_machine.system() == 'windows'
    subdir('win32')
    subdir('dx')
endif

subdir('core')
subdir('log')
subdir('reni')


reni_lib = static_library('reni',
    sources: sources,
    include_directories: include_directories('../include/reni', 'reni')
)

reni_dep = declare_dependency(
    include_directories: include_directories('../include', '.'),
    dependencies: deps,
    link_with: reni_lib
)
