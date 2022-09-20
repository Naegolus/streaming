
project(
	'Gaming',
	'c',
	'cpp',
	default_options : ['cpp_std=c++11'],
)

srcsCore = [
	'Processing.cpp',
	'Log.cpp',
	'TcpListening.cpp',
	'TcpTransfering.cpp',
	'SystemDebugging.cpp',
	'SystemCommanding.cpp',
]

srcsCommon = [
]

srcsSpecific = [
	'main.cpp',
	'LibGaming.cpp',
	'GameSupervising.cpp',
	'GameServing.cpp',
	'Gaming.cpp',
	'GamerInteracting.cpp',
	'GameSelecting.cpp',
	'ListIdx.cpp',
	'TelnetAuthorizing.cpp',
	'games/TicTacToe/TicTacToeGaming.cpp',
	'games/ConnectFour/ConnectFourGaming.cpp',
	'games/ConnectFour/ConnectFourLobbying.cpp',
	'games/ConnectFour/ConnectFourMatching.cpp',
]

srcs = []
srcs += srcsSpecific

foreach f : srcsCore
	srcs += join_paths('../common/core', f)
endforeach

foreach f : srcsCommon
	srcs += join_paths('../common', f)
endforeach

cxx = meson.get_compiler('cpp')

deps = []
deps += dependency('threads')
deps += dependency('jsoncpp')

incs = [
	'../common/core',
	'../common',
	'games/TicTacToe',
	'games/ConnectFour',
]

mr = meson.source_root()

incsAbs = []
foreach p : incs
	incsAbs += include_directories(p)
endforeach

myApp = executable(
	'gaming',
	srcs,
	include_directories : [
		incsAbs,
	],
	dependencies : [
		deps,
	],
	cpp_args : [
		'-DdPackageName="' + meson.project_name() + '"',
		'-DHAVE_SSTREAM',
		'-DCONFIG_PROC_HAVE_LOG=1',
		'-DCONFIG_PROC_HAVE_DRIVERS=1',
		'-DCONFIG_PROC_USE_STD_LISTS=1',
		'-DCONFIG_PROC_SHOW_ADDRESS_IN_ID=0',
		'-DCONFIG_PROC_INFO_BUFFER_SIZE=1024',
	],
)
