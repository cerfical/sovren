shaders = {
    'vs_5_0': files(
        'shaders/vs.hlsl',
    ),
    'ps_5_0': files(
        'shaders/ps.hlsl',
    )
}

sources += files(
    'DxRenderTarget.cpp',
    'DxRenderContext.cpp',
    'DxRenderBackend.cpp',
    'DxSwapChain.cpp',
    'DxVertexBuffer.cpp',
    'ShaderCode.cpp',
)


cxx = meson.get_compiler('cpp')

deps += cxx.find_library('d3d11')
deps += cxx.find_library('d2d1')


fxc = find_program('fxc')
hlsl_compiler = generator(fxc,
    arguments: ['-T', '@EXTRA_ARGS@', '@INPUT@', '-Fh', '@OUTPUT@', '-Vn', '@BASENAME@'],
    output: '@BASENAME@.hpp'
)


foreach profile, shaders : shaders
    if shaders.length() == 0
        continue
    endif

    compiled_shaders = hlsl_compiler.process(shaders,
        preserve_path_from: meson.current_source_dir(),
        extra_args: profile
    )
    sources += compiled_shaders
endforeach
