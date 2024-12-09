# Copyright (c) 2024 - Nathanne Isip
# This file is part of N8.
# 
# N8 is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published
# by the Free Software Foundation, either version 3 of the License,
# or (at your option) any later version.
# 
# N8 is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with N8. If not, see <https://www.gnu.org/licenses/>.

import cpuinfo
import os
import platform
import shutil
import subprocess

CORE_VERSION = '1.0.0'
OUT_DIR = 'dist'

if os.path.exists(OUT_DIR):
    shutil.rmtree(OUT_DIR)

OUT_DIR = os.path.join(OUT_DIR, 'n8lang')
os.makedirs(OUT_DIR)

try:
    src = 'modules'

    shutil.copytree(src, os.path.join(OUT_DIR, os.path.basename(src)), dirs_exist_ok=True)
    print(f"Successfully copied '{src}' to '{OUT_DIR}'")

except Exception as e:
    print("Failed to copy modules.")
    exit(0)

PLATFORM = platform.system()
OUTPUT_EXECUTABLE = os.path.join(OUT_DIR, 'bin')
OUTPUT_LIBRARY = os.path.join(OUT_DIR, 'modules', 'core@' + CORE_VERSION, 'lib')

os.makedirs(OUTPUT_EXECUTABLE)
os.makedirs(OUTPUT_LIBRARY)

OUTPUT_EXECUTABLE = os.path.join(OUTPUT_EXECUTABLE, 'n8')
OUTPUT_LIBRARY = os.path.join(OUTPUT_LIBRARY, 'n8-std')

cpp_files = []
cc_files = []

lib_headers = [
    '-Ilib/QuickDigest5/include',
    '-Ilib/MyShell/include',
    '-Ilib/SHA/src'
]
lib_source_files = []

def get_ext_instructions():
    print('Checking extended instruction availability...')
    features_to_check = [
        "abm", "adx", "aes", "avx", "avx2", "bmi",
        "clflushopt", "cx16", "f16c", "fma", "fsgsbase",
        "fxsr", "mmx", "movbe", "rdrnd", "rdseed",
        "rtm", "sgx", "sse", "sse2", "sse3", "sse4",
        "sse4_1", "sse4_2", "vpclmulqdq", "xsave",
        "xsavec", "xsaveopt", "xsave"
    ]

    supported_features = []
    cpu_info = cpuinfo.get_cpu_info()

    if 'flags' in cpu_info:
        for feature in features_to_check:
            print('Checking extended instruction support for ' + feature + '... ', end='')
            if feature in cpu_info['flags']:
                supported_features.append('-m' + feature.replace('_', '.'))
                print('supported')
            else:
                print('not found')

    return supported_features

def include_sha_headers():
    global lib_source_files

    for root, _, files in os.walk('lib/SHA/src'):
        for file in files:
            if file.endswith('.cpp'):
                lib_source_files.append(os.path.join(root, file))

    lib_source_files = [file for file in lib_source_files if not file.endswith("example.cpp")]

for root, dirs, files in os.walk('src'):
    for file in files:
        if file.endswith('.cpp'):
            cpp_files.append(os.path.join(root, file))

for root, dirs, files in os.walk('std'):
    for file in files:
        if file.endswith('.cc'):
            cc_files.append(os.path.join(root, file))

for root, dirs, files in os.walk('lib/QuickDigest5/src'):
    for file in files:
        if file.endswith('.cpp'):
            lib_source_files.append(os.path.join(root, file))

try:
    ext_instructions = get_ext_instructions()

    if PLATFORM == 'Windows':
        exe_build_args= [
            'g++', '-Iinclude', '-Wall', '-pedantic', '-Wdisabled-optimization',
            '-pedantic-errors', '-Wextra', '-Wcast-align', '-Wcast-qual',
            '-Wchar-subscripts', '-Wcomment', '-Wconversion', '-Werror',
            '-Wfloat-equal', '-Wformat', '-Wformat=2', '-Wformat-nonliteral',
            '-Wformat-security', '-Wformat-y2k', '-Wimport', '-Winit-self',
            '-Winvalid-pch', '-Wunsafe-loop-optimizations', '-Wlong-long',
            '-Wmissing-braces', '-Wmissing-field-initializers', '-Wmissing-format-attribute',
            '-Wmissing-include-dirs', '-Weffc++', '-Wpacked', '-Wparentheses',
            '-Wpointer-arith', '-Wredundant-decls', '-Wreturn-type', '-Wsequence-point',
            '-Wshadow', '-Wsign-compare', '-Wstack-protector', '-Wstrict-aliasing',
            '-Wstrict-aliasing=2', '-Wswitch', '-Wswitch-default', '-Wswitch-enum',
            '-Wtrigraphs', '-Wuninitialized', '-Wunknown-pragmas', '-Wunreachable-code',
            '-Wunused', '-Wunused-function', '-Wunused-label', '-Wunused-parameter',
            '-Wunused-value', '-Wunused-variable', '-Wvariadic-macros',
            '-Wvolatile-register-var', '-Wwrite-strings', '-pipe', '-Ofast', '-s',
            '-std=c++17', '-fopenmp'] + ext_instructions + ['-mfpmath=sse',
            '-march=native', '-funroll-loops', '-ffast-math', '-static', '-static-libgcc',
            '-static-libstdc++'
        ] + lib_headers + lib_source_files + cpp_files + ['-o', OUTPUT_EXECUTABLE]

        include_sha_headers()
        lib_build_args = [
            'g++', '-static', '-static-libgcc', '-Iinclude',
            '-Istd', '-shared', '-o', OUTPUT_LIBRARY + '.dll'
        ] + ext_instructions + lib_headers + lib_source_files + cpp_files + cc_files

        print("Executing:")
        print(' '.join(exe_build_args))
        subprocess.run(exe_build_args)

        print("Executing:")
        print(' '.join(lib_build_args))
        subprocess.run(lib_build_args)

    elif "TERMUX_VERSION" in os.environ:
        ext_instructions.remove('-maes')
        exe_build_args= [
            'g++', '-Iinclude', '-Wall', '-pedantic', '-Wdisabled-optimization',
            '-pedantic-errors', '-Wextra', '-Wcast-align', '-Wcast-qual',
            '-Wchar-subscripts', '-Wcomment', '-Wconversion', '-Werror',
            '-Wfloat-equal', '-Wformat', '-Wformat=2', '-Wformat-nonliteral',
            '-Wformat-security', '-Wformat-y2k', '-Wimport', '-Winit-self',
            '-Winvalid-pch', '-Wno-header-guard', '-Wlong-long',
            '-Wmissing-braces', '-Wmissing-field-initializers', '-Wmissing-format-attribute',
            '-Wmissing-include-dirs', '-Weffc++', '-Wpacked', '-Wparentheses',
            '-Wpointer-arith', '-Wredundant-decls', '-Wreturn-type', '-Wsequence-point',
            '-Wshadow', '-Wsign-compare', '-Wstack-protector', '-Wstrict-aliasing',
            '-Wstrict-aliasing=2', '-Wswitch', '-Wswitch-default', '-Wswitch-enum',
            '-Wtrigraphs', '-Wuninitialized', '-Wunknown-pragmas', '-Wunreachable-code',
            '-Wunused', '-Wunused-function', '-Wunused-label', '-Wunused-parameter',
            '-Wunused-value', '-Wunused-variable', '-Wvariadic-macros', '-O3',
            '-Wvolatile-register-var', '-Wwrite-strings', '-pipe', '-ffast-math', '-s',
            '-std=c++20', '-fopenmp'] + ext_instructions + ['-march=native',
            '-funroll-loops', '-ffast-math', '-D__TERMUX__'
        ] + lib_headers + lib_source_files + cpp_files + ['-o', OUTPUT_EXECUTABLE]

        include_sha_headers()
        lib_build_args = [
            'g++', '-Iinclude', '-Istd', '-fPIC',
            '-shared', '-o', OUTPUT_LIBRARY + '.so'
        ] + ext_instructions + lib_headers + lib_source_files + cpp_files + cc_files

        print("Executing:")
        print(' '.join(exe_build_args))
        subprocess.run(exe_build_args)

        print("Executing:")
        print(' '.join(lib_build_args))
        subprocess.run(lib_build_args)

    elif PLATFORM == 'Linux':
        exe_build_args= [
            'g++', '-Iinclude', '-Wall', '-pedantic', '-Wdisabled-optimization',
            '-pedantic-errors', '-Wextra', '-Wcast-align', '-Wcast-qual',
            '-Wchar-subscripts', '-Wcomment', '-Wconversion', '-Werror',
            '-Wfloat-equal', '-Wformat', '-Wformat=2', '-Wformat-nonliteral',
            '-Wformat-security', '-Wformat-y2k', '-Wimport', '-Winit-self',
            '-Winvalid-pch', '-Wunsafe-loop-optimizations', '-Wlong-long',
            '-Wmissing-braces', '-Wmissing-field-initializers', '-Wmissing-format-attribute',
            '-Wmissing-include-dirs', '-Weffc++', '-Wpacked', '-Wparentheses',
            '-Wpointer-arith', '-Wredundant-decls', '-Wreturn-type', '-Wsequence-point',
            '-Wshadow', '-Wsign-compare', '-Wstack-protector', '-Wstrict-aliasing',
            '-Wstrict-aliasing=2', '-Wswitch', '-Wswitch-default', '-Wswitch-enum',
            '-Wtrigraphs', '-Wuninitialized', '-Wunknown-pragmas', '-Wunreachable-code',
            '-Wunused', '-Wunused-function', '-Wunused-label', '-Wunused-parameter',
            '-Wunused-value', '-Wunused-variable', '-Wvariadic-macros', '-O3',
            '-Wvolatile-register-var', '-Wwrite-strings', '-pipe', '-ffast-math', '-s',
            '-std=c++20', '-fopenmp'] + ext_instructions + ['-mfpmath=sse',
            '-march=native', '-funroll-loops', '-ffast-math'
        ] + lib_headers + lib_source_files + cpp_files + ['-o', OUTPUT_EXECUTABLE]

        include_sha_headers()
        lib_build_args = [
            'g++', '-Iinclude', '-Istd', '-fPIC',
            '-shared', '-o', OUTPUT_LIBRARY + '.so'
        ] + ext_instructions + lib_headers + lib_source_files + cpp_files + cc_files

        print("Executing:")
        print(' '.join(exe_build_args))
        subprocess.run(exe_build_args)

        print("Executing:")
        print(' '.join(lib_build_args))
        subprocess.run(lib_build_args)

    elif PLATFORM == 'Darwin':
        exe_build_args = [
            '/opt/homebrew/opt/llvm/bin/clang++', '-Iinclude', '-Wall',
            '-pedantic', '-Wdisabled-optimization', '-pedantic-errors',
            '-Wextra', '-Wcast-align', '-Wcast-qual', '-Wchar-subscripts',
            '-Wcomment', '-Wconversion', '-Werror', '-Wfloat-equal',
            '-Wformat', '-Wformat=2', '-Wformat-nonliteral', '-Wformat-security',
            '-Wformat-y2k', '-Wimport', '-Winit-self', '-Winvalid-pch',
            '-Wlong-long', '-Wmissing-braces', '-Wmissing-field-initializers',
            '-Wmissing-format-attribute', '-Wmissing-include-dirs', '-Wpacked',
            '-Wparentheses', '-Wpointer-arith', '-Wredundant-decls',
            '-Wsequence-point', '-Wshadow', '-Wsign-compare', '-Wstack-protector',
            '-Wstrict-aliasing', '-Wstrict-aliasing=2', '-Wswitch',
            '-Wswitch-default', '-Wswitch-enum', '-Wtrigraphs', '-Wuninitialized',
            '-Wunknown-pragmas', '-Wunreachable-code', '-Wunused',
            '-Wunused-function', '-Wunused-label', '-Wunused-parameter',
            '-Wunused-value', '-Wunused-variable', '-Wvariadic-macros',
            '-Wwrite-strings', '-Wno-return-type-c-linkage', '-pipe',
            '-std=c++17', '-fopenmp', '-march=native', '-funroll-loops',
            '-ffast-math', '-flto=auto', '-Xpreprocessor', '-O3',
            '-Wno-header-guard', '-Wno-pessimizing-move'
        ] + lib_headers + lib_source_files + cpp_files + ['-o', OUTPUT_EXECUTABLE]

        include_sha_headers()
        lib_build_args = [
            '/opt/homebrew/opt/llvm/bin/clang++', '-Iinclude',
            '-Istd', '-shared', '-o', OUTPUT_LIBRARY + '.dylib'
        ] + ext_instructions + lib_headers + lib_source_files + cpp_files + cc_files

        print("Executing:")
        print(' '.join(exe_build_args))
        subprocess.run(exe_build_args)

        print("Executing:")
        print(' '.join(lib_build_args))
        subprocess.run(lib_build_args)

except Exception as e:
    print(f"Compilation failed with error: {e}")
    exit(1)
