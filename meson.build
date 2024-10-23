project('sovren', 'cpp',
    version: '0.1.0',
    default_options: [
        'cpp_std=c++20',
        'warning_level=3',
        'werror=true'
    ]
)

supported_platforms = ['windows']
if host_machine.system() not in supported_platforms
    error('The only supported platforms are:', ', '.join(supported_platforms))
endif

subdir('src')
subdir('app')
subdir('tests')
subdir('docs')
