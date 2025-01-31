# Copyright (c) 2025 - Nathanne Isip
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

import os
import platform
import subprocess

PROJECT_NAME    = 'n8-lang'
VERSION         = '1.0.0'
ARCHITECTURE    = platform.machine().lower()
OUTPUT_EXE      = os.path.join(
    'dist',
    f'{PROJECT_NAME}_{VERSION}_{ARCHITECTURE}.exe'
)

subprocess.run([
    'g++', '-static', '-o', OUTPUT_EXE,
    'tools\\windows_installer\\installer.cpp',
    '-lshlwapi', '-ladvapi32'
])
