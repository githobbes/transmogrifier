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
env.Append(PATH=os.environ['PATH'])

env.Append(CXXFLAGS=get_gm_flags('cxxflags'))
env.Replace(_CPPINCFLAGS=get_gm_flags('cppflags'))
env.Replace(_LIBDIRFLAGS=get_gm_flags('ldflags'))
env.Replace(_LIBFLAGS=get_gm_flags('libs'))

main = env.Program(target='main', source=['main.cpp', 'convert.cpp'])
env.Default(main)
