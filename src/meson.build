sources = []
deps = []


subdir('sovren')
subdir('core')

if host_machine.system() == 'windows'
    sources += files('win32.cpp')
    subdir('dx11')
endif


sovren_lib = static_library('sovren',
    sources: sources,
    include_directories: include_directories('../include/sovren', 'sovren')
)

sovren_dep = declare_dependency(
    include_directories: include_directories('../include', '.'),
    dependencies: deps,
    link_with: sovren_lib
)
