gtest_dep = dependency('gtest_main')


math_test = executable('math-test',
	sources: [
		'Vec2Test.cpp',
		'Vec3Test.cpp',
		'Vec4Test.cpp'
	],
	dependencies: [
		gtest_dep,
		reni_dep
	]
)

test('MathTest', math_test)


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
