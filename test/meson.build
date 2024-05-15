gtest_dep = dependency('gtest_main')


math_test = executable('math-test',
	sources: [
		'Vec4Test.cpp',
		'Vec3Test.cpp',
		'Vec2Test.cpp',

		'Mat4x4Test.cpp',
		'Mat3x3Test.cpp',
		'Mat2x2Test.cpp',
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
