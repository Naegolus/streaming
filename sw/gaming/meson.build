
project(
	'Gaming',
	'c',
	'cpp',
	default_options : ['cpp_std=c++11'],
)

srcsCommon = [
	'Processing.cpp',
	'Log.cpp',
	'TcpListening.cpp',
	'TcpTransfering.cpp',
	'SystemDebugging.cpp',
	'SystemCommanding.cpp',
]

srcsSpecific = [
	'main.cpp',
	'GameSupervising.cpp',
	'GameServing.cpp',
	'Gaming.cpp',
	'GamerInteracting.cpp',
	'TicTacToeGaming.cpp',
]

srcs = [srcsSpecific]
foreach file : srcsCommon
	srcs += join_paths('../common/core', file)
endforeach

cxx = meson.get_compiler('cpp')

deps = []
deps += dependency('threads')
deps += dependency('jsoncpp')

myApp = executable(
	'gaming',
	srcs,
	include_directories : include_directories([
		'../common/core',
	]),
	dependencies : [
		deps,
	],
	cpp_args : [
		'-DdPackageName="' + meson.project_name() + '"',
		'-DHAVE_SSTREAM',
		'-DCONFIG_PROC_HAVE_LOG=1',
		'-DCONFIG_PROC_HAVE_DRIVERS=1',
		'-DCONFIG_PROC_USE_STD_LISTS=1',
	],
)
