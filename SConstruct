# -*- mode: python -*-
# SCons build file

from __future__ import print_function
import os
import os.path
import sys
import subprocess


def get_gm_flags(for_flag):
    # Python 2.7
    # return subprocess.check_output(['GraphicsMagick++-config', '--' + for_flag])
    # Python 2.6 compatibiliy
    flags = subprocess.Popen(
        ['GraphicsMagick++-config', '--' + for_flag],
        stdout=subprocess.PIPE).communicate()[0]
    # Strip newlines off the end.
    return flags.rstrip('\n')

env = Environment()
# Inherit the PATH for use with GraphicsMagick++-config.
env.Append(PATH=os.environ['PATH'])
# Inherit CXX from the environment if it is set.
try:
    env.Replace(CXX=os.environ['CXX'])
except KeyError:
    pass

local_dir = os.path.join(os.environ['HOME'], '.local')
local_include_dir = os.path.join(local_dir, 'include')
local_lib_dir = os.path.join(local_dir, 'lib')

main = env.Program(target='main', source=['main.cpp', 'convert.cpp', os.path.join(local_lib_dir, 'libboost_program_options.a')],
                   CXXFLAGS=get_gm_flags('cxxflags'),
                   _CPPINCFLAGS=get_gm_flags('cppflags') + ' -I' + local_include_dir,
                   _LIBDIRFLAGS=get_gm_flags('ldflags') + ' -L' + local_lib_dir,
                   _LIBFLAGS=get_gm_flags('libs'))

chuck = env.Program(target='chuck', source=['mainChuck.cpp'],
                    CPPPATH='#/vendor/include/eigen3')

penrose = env.Program(target='penrose', source=['mainPenrose.cpp', 'pixel.cpp'])

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
