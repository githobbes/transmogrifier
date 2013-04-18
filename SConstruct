# -*- mode: python -*-
# SCons build file

from __future__ import print_function
import os
import os.path
import sys

# Credit: <http://scons.org/wiki/StaticPicLibrary>
# Using this is roughly equivalent to using the StaticLibrary builder and adding `-fPIC' to CXXFLAGS.
def createStaticPicLibraryBuilder(env):
    """This is a utility function that creates the StaticExtLibrary Builder in
    an Environment if it is not there already.

    If it is already there, we return the existing one."""
    import SCons.Action

    try:
        static_extlib = env['BUILDERS']['StaticPicLibrary']
    except KeyError:
        action_list = [SCons.Action.Action("$ARCOM", "$ARCOMSTR")]
        if env.Detect('ranlib'):
            ranlib_action = SCons.Action.Action("$RANLIBCOM", "$RANLIBCOMSTR")
            action_list.append(ranlib_action)

    static_extlib = SCons.Builder.Builder(
        action=action_list,
        emitter='$LIBEMITTER',
        prefix='$LIBPREFIX',
        suffix='$LIBSUFFIX',
        src_suffix='$OBJSUFFIX',
        src_builder='SharedObject')

    env['BUILDERS']['StaticPicLibrary'] = static_extlib
    return static_extlib


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
createStaticPicLibraryBuilder(env)

# Inherit CXX from the environment if it is set.
try:
    env.Replace(CXX=os.environ['CXX'])
except KeyError:
    pass

static_transmogrify = env.StaticPicLibrary(
    target='transmogrify',
    source=['transmogrify.cpp', 'convert.cpp', 'algorithms.cpp', 'pixel.cpp', 'logging.cpp'])

main = env.Program(
    target='main',
    # Specify the full path to the static library to link with that and not the dynamic library.
    source=['main.cpp', os.path.join(local_lib_dir, 'libboost_program_options.a')],
    LIBS=[static_transmogrify] + gm_libs)


chuck = env.Program(target='chuck', source=['mainChuck.cpp'],
                    CPPPATH='#/vendor/include/eigen3')

transmogripy = env.SharedLibrary(
    target='transmogripy', source=['transmogripy.cpp'],
    LIBS=['boost_python', static_transmogrify] + gm_libs,
    SHLIBPREFIX='',
    CPPPATH='/usr/include/python2.6'
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
