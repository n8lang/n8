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

#ifndef N8_UTIL_ARGUMENT_PARSER_HPP
#define N8_UTIL_ARGUMENT_PARSER_HPP

#include <string>
#include <unordered_map>
#include <vector>

namespace N8Util {

class ArgumentParser final {
private:
    int argCount;
    char** argValues;
    std::unordered_map<std::string, std::string> parameters;
    std::unordered_map<std::string, std::string> descriptions;

public:
    ArgumentParser(int _argCount, char** _argValues) :
        argCount(_argCount),
        argValues(_argValues),
        parameters({}),
        descriptions({}) {}

    ArgumentParser(const ArgumentParser& other) :
        argCount(other.argCount),
        argValues(other.argValues),
        parameters(other.parameters),
        descriptions(other.descriptions) {}

    ArgumentParser& operator=(const ArgumentParser& other);
    void defineParameter(
        const std::string& paramShort,
        const std::string& paramLong,
        const std::string& description
    );

    void printAllParamWithDesc() const;
    bool hasParameter(const std::string& paramShort) const;

    std::string getProgramFileName() const;
    std::vector<std::string> getInputFiles() const;
};

}

#endif
