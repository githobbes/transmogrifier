# -*- mode: python -*-
# SCons build file

from __future__ import print_function
import os
import os.path
import sys

# TODO: Make this a configurable command-line option.
local_dir = os.path.join(os.environ['HOME'], '.local')

local_include_dir = os.path.join(local_dir, 'include')
local_gm_include_dir = os.path.join(local_include_dir, 'GraphicsMagick')
local_lib_dir = os.path.join(local_dir, 'lib')
# THE ORDER OF THESE DOES MATTER!!!
gm_libs = Split('GraphicsMagick++ GraphicsMagick lcms tiff freetype jasper jpeg png12 Xext SM ICE X11 bz2 xml2 z m gomp pthread')

env = Environment(
    CXXFLAGS='-pthread',
    CPPPATH=[local_include_dir, local_gm_include_dir], # ,'/usr/include/python2.6'],
    LIBPATH=[local_lib_dir])

# Inherit CXX from the environment if it is set.
try:
    env.Replace(CXX=os.environ['CXX'])
except KeyError:
    pass

transmogrify = env.StaticLibrary(
    target='transmogrify',
    source=['transmogrify.cpp', 'convert.cpp', 'algorithms.cpp', 'pixel.cpp', 'logging.cpp'])

main = env.Program(
    target='main',
    # Specify the full path to the static library to link with that and not the dynamic library.
    source=['main.cpp', os.path.join(local_lib_dir, 'libboost_program_options.a')],
    LIBS=[transmogrify] + gm_libs)


chuck = env.Program(target='chuck', source=['mainChuck.cpp'],
                    CPPPATH='#/vendor/include/eigen3')

transmogripy = env.SharedLibrary(
    target='transmogripy', source=['transmogripy.cpp', transmogrify],
    #LIBS=['boost_python', 'transmogrify'],
    SHLIBPREFIX='',
    #CPPPATH='/usr/include/python2.6'
)

#penrose = env.Program(target='penrose', source=['mainPenrose.cpp', 'pixel.cpp'])

env.Default(main)


# Extra commands


def beautify_code(env, target, source):
    # Indent with tab characters.
    try:
        subprocess.check_call(['astyle', '--indent=tab'] +
                              [str(s) for s in source])
    except OSError:
        print("The `astyle' executable was not found. "
              "Install it from <http://astyle.sf.net/> before continuing.",
              file=sys.stderr)

env.AlwaysBuild(env.Command('beautify', Glob('*.[hc]pp'), beautify_code))
