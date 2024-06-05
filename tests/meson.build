gtest_dep = dependency('gtest_main')


math_test = executable('math-test',
    sources: [
        'VectorOperations4D.cpp',
        'VectorOperations3D.cpp',
        'VectorOperations2D.cpp',

        'MatrixOperations4D.cpp',
        'MatrixOperations3D.cpp',
        'MatrixOperations2D.cpp',
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
