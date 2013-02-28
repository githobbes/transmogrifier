# -*- mode: python -*-
# SCons build file

from __future__ import print_function
import os
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

main = env.Program(target='main', source=['main.cpp', 'convert.cpp'],
                   CXXFLAGS=get_gm_flags('cxxflags'),
                   _CPPINCFLAGS=get_gm_flags('cppflags'),
                   _LIBDIRFLAGS=get_gm_flags('ldflags'),
                   _LIBFLAGS=get_gm_flags('libs'))

algorithms = env.Program(target='algorithms', source=['algorithms.cpp'],
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
