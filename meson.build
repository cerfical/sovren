project('reni',
  'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'cpp_std=c++20'
  ]
)


supported_platforms = ['windows']
if not supported_platforms.contains(host_machine.system())
  error('The only supported platforms are:', ', '.join(supported_platforms))
endif


subdir('src')
subdir('tests')
