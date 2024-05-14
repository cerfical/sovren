gtest_dep = dependency('gtest_main')

render_test = executable('render-test',
	sources: 'RenderTest.cpp',
	dependencies: [
		gtest_dep,
		reni_dep
	]
)

test('RenderTest', render_test,
	is_parallel: false,
	timeout: -1
)
