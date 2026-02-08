project('txture', 'c', version: '0.1')

cc = meson.get_compiler('c')

m_dep = cc.find_library('m', required: false)

executable(
	'txture',
	'main.c',
	'ppm.c',
	'gen.c',
	'utils.c',
	install: true,
	dependencies: m_dep,
)
