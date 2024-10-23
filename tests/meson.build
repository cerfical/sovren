catch2_dep = dependency('catch2-with-main',
    default_options: [
        'warning_level=0',
        'werror=false'
    ]
)


math_test = executable('math-test',
    sources: [
        'matrix_math.cpp',
        'vector_math.cpp',
    ],
    dependencies: [
        catch2_dep,
        sovren_dep
    ]
)

test('MathTest', math_test)


render_test = executable('render-test',
    sources: 'render.cpp',
    dependencies: [
        catch2_dep,
        sovren_dep
    ]
)

test('RenderTest', render_test,
    is_parallel: false,
    timeout: -1
)
