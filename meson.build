project('txture', 'c', version: '0.1', default_options: ['c_std=gnu11'])

cc = meson.get_compiler('c')

m_dep = cc.find_library('m', required: false)

if cc.has_function('argp_parse')
	argp_dep = declare_dependency()
else
	argp_dep = cc.find_library('argp', required: true)
endif

deps = [m_dep, argp_dep]

subdir('expr')

executable(
	'txture',
	'main.c',
	'ppm.c',
	'gen.c',
	'utils.c',
	'process.c',
	install: true,
	dependencies: deps,
	link_with: expr_engine_lib,
)
