shaders = {
	'vs_6_0': [],
	'ps_6_0': []
}

sources += files(
	'DxRenderTarget.cpp',
	'DxRenderContext.cpp',
	'DxRenderBackend.cpp',
	'DxSwapChain.cpp'
)


cxx = meson.get_compiler('cpp')

deps += cxx.find_library('d3d11')
deps += cxx.find_library('d2d1')


dxc = find_program('dxc')
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
