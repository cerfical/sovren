doxygen = find_program('doxygen', required: false)
dot = find_program('dot', required: false)

if doxygen.found()
	doxyfile = configure_file(
		input: 'Doxyfile.in',
		output: 'Doxyfile',
		configuration: {
			'PROJECT_NAME': meson.project_name(),
			'CURRENT_BUILD_DIR': meson.current_build_dir(),
			'PROJECT_SOURCE_ROOT': meson.project_source_root(),
			'DOT_FOUND': dot.found() ? 'YES' : 'NO'
		}
	)

	if not dot.found()
		warning('dot tool not found: running Doxygen with dot disabled')
	endif

	run_target('docs', command: [doxygen, doxyfile])
else
	warning('Doxygen not found: documentation disabled')
endif
