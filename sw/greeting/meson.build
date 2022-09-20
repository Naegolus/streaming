
project(
	'Greeting',
	'c',
	'cpp',
	default_options : ['cpp_std=c++11'],
)

srcsCommon = [
	'Processing.cpp',
]

srcsSpecific = [
	'main.cpp',
	'Greeting.cpp',
]

srcs = []
srcs += srcsSpecific

foreach file : srcsCommon
	srcs += join_paths('../common/core', file)
endforeach

cxx = meson.get_compiler('cpp')

deps = []
deps += dependency('threads')

incs = []
incs += '../common/core'

myApp = executable(
	'greeting',
	srcs,
	include_directories : include_directories([
		incs,
	]),
	dependencies : [
		deps,
	],
	cpp_args : [
		'-DdPackageName="' + meson.project_name() + '"',
		'-DHAVE_SSTREAM',
		'-DCONFIG_PROC_HAVE_LOG=0',
		'-DCONFIG_PROC_HAVE_DRIVERS=1',
		'-DCONFIG_PROC_USE_STD_LISTS=1',
	],
)
