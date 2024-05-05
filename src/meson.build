lib_sources = files(
  'print.cpp',
  'RenderWindow.cpp',
  'rg.cpp'
)

lib_deps = []


if host_machine.system() == 'windows'
  lib_sources += files('pal/win32/Platform.cpp')

  cxx = meson.get_compiler('cpp')
  lib_deps += cxx.find_library('d3d11', required : true)
  lib_deps += cxx.find_library('d2d1', required : true)
endif


reni_lib = static_library('reni',
  lib_sources,
  include_directories : '../include/reni'
)

reni_dep = declare_dependency(
  include_directories : '../include',
  dependencies: lib_deps,
  link_with: reni_lib
)
