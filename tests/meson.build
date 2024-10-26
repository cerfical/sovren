catch2_dep = dependency('catch2-with-main',
    default_options: [
        'warning_level=0',
        'werror=false'
    ]
)


math_test = executable('math-test',
    sources: [
        'matrix_math_test.cpp',
        'vector_math_test.cpp',
    ],
    dependencies: [
        catch2_dep,
        sovren_dep
    ]
)

test('MathTest', math_test)
