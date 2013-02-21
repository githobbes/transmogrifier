# -*- mode: python -*-
# SCons build file

import os
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
env.Default(main)
