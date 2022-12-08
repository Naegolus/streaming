
project(
	'Gaming',
	'c',
	'cpp',
	default_options : [],
)

fs = import('fs')

srcsCore = [
	'Processing.cpp',
	'Log.cpp',
	'TcpListening.cpp',
	'TcpTransfering.cpp',
	'SystemDebugging.cpp',
	'SystemCommanding.cpp',
]

srcsCommon = [
	'Res.cpp',
]

srcsSpecific = [
	'main.cpp',
	'AdventCoding.cpp',
	'AocTree.cpp',
]

srcs = []
srcs += srcsSpecific

foreach f : srcsCore
	srcs += join_paths('../../common/core', f)
endforeach

foreach f : srcsCommon
	srcs += join_paths('../../common', f)
endforeach

cxx = meson.get_compiler('cpp')

incs = [
	'../../common/core',
	'../../common',
]

mr = meson.source_root()

incsAbs = []
foreach p : incs
	incsAbs += include_directories(p)
endforeach

toCppConvert = ['bin2cpp', '@OUTPUT@', '@INPUT@']

cmd = run_command('ls', 'resources/', check : true)
resources = cmd.stdout().strip().split('\n')

message('Integrating resources')

resPaths = []
foreach r : resources
	message('Global: ' + r)
	resPaths += join_paths('./resources/', r)
endforeach

cust = custom_target(
	'targetRes',
	input	: resPaths,
	output	: ['resources.cpp'],
	command	: toCppConvert,
)
srcs += cust

message('Integrating resources: Done')

deps = []
deps += dependency('threads')
deps += dependency('jsoncpp')
deps += dependency('tclap')
deps += dependency('libcrypto++', static : true)

cppArgs = [
	'-DdPackageName="' + meson.project_name() + '"',
	'-DdAppName="gaming"',
	'-DHAVE_SSTREAM',
	'-DCONFIG_PROC_HAVE_LOG=1',
	'-DCONFIG_PROC_HAVE_DRIVERS=1',
	'-DCONFIG_PROC_USE_STD_LISTS=1',
	'-DCONFIG_PROC_SHOW_ADDRESS_IN_ID=0',
	'-DCONFIG_PROC_INFO_BUFFER_SIZE=1024',
	'-std=gnu++11',
]

# https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
warnings = [
	'-Wall',
	'-Wextra',
	'-Wpedantic',
	'-Werror',
	'-Wfatal-errors',
	'-Wreorder',
	'-Wswitch-enum',
	'-Wuseless-cast',
	'-Wparentheses',
	'-Wshift-overflow',
	'-Wsign-compare',
	'-Wzero-as-null-pointer-constant',
	'-Wcast-align',
	'-Wcast-qual',
	'-Wcatch-value',
	'-Wchar-subscripts',
	'-Wswitch-default',
	'-Wctor-dtor-privacy',
	'-Wduplicated-branches',
	'-Wduplicated-cond',
	'-Wempty-body',
	'-Wextra-semi',
	'-Wfloat-equal',
	'-Wformat',
	'-Wformat-extra-args',
	'-Wimplicit-fallthrough',
	'-Wmissing-field-initializers',
	'-Wnull-dereference',
	'-Wshadow',
]

cppArgs += warnings

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
		cppArgs,
	],
)
