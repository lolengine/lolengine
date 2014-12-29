import os
import ycm_core

flags = [
    '-xc++',
    '-std=c++11',
    # Engine includes
    '-I.',
    '-I..',
    '-Isrc',
    # System includes
    '-I/usr/include/SDL2',
    '-I/usr/include/GL',
    '-I/usr/include/libdrm',
    '-I/usr/include/libpng15',
    # Macros
    '-DHAVE_CONFIG_H',
    '-D_REENTRANT',
]

def FlagsForFile(filename, **kwargs):

    extension = os.path.splitext(filename)[1]
    if extension not in [ 'cpp', 'h' ]:
        return None

    final_flags = flags

    return {
        'flags': final_flags,
        'do_cache': True
    }

