/*
 * Copyright (c) 2024 - Nathanne Isip
 * This file is part of N8.
 * 
 * N8 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 * 
 * N8 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with N8. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef N8_CORE_RUNTIME_HPP
#define N8_CORE_RUNTIME_HPP

#include <n8/ast/ASTNode.hpp>

#include <unordered_map>

class Runtime final {
private:
    static bool testMode;
    static std::unordered_map<std::string, void*> nativeLibraries;

    #ifdef __EMSCRIPTEN__
    static std::string elementId;
    #endif

public:
    static void repl();

    static bool isTestMode();
    static void setTestMode(bool _testMode);

    static void addLoadedLibrary(std::string libName, void* handle);
    static void* getLoadedLibrary(std::string libName);
    static bool hasLoadedLibrary(std::string libName);

    static void cleanUp();

    #ifdef __EMSCRIPTEN__
    static void setOutputElementId(std::string id);
    static std::string getOutputElementId();

    static void execute(const char* sourceCode);
    #endif
};

#endif
