shaders = {
    'vs_6_0': [],
    'ps_6_0': []
}


cxx = meson.get_compiler('cpp')

deps += cxx.find_library('d3d11', required: true)
deps += cxx.find_library('d2d1', required: true)


dxc = find_program('dxc', required: true)
hlsl_compiler = generator(dxc,
    arguments: ['-T', '@EXTRA_ARGS@', '@INPUT@', '-Fh', '@OUTPUT@'],
    output: '@BASENAME@.hpp'
)


foreach profile, shaders : shaders
    if shaders.length() == 0
        continue
    endif

    compiled_shaders = hlsl_compiler.process(shaders,
        extra_args: profile
    )
    sources += compiled_shaders
endforeach
