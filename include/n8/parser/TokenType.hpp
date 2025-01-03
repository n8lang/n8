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

#ifndef N8_PARSER_TOKEN_TYPE_HPP
#define N8_PARSER_TOKEN_TYPE_HPP

#include <string>

enum class TokenType {
    DIGIT,
    STRING,
    REGEX,
    KEYWORD,
    IDENTIFIER,
    OPERATOR
};

inline std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::DIGIT:
            return "digit";
        case TokenType::STRING:
            return "string";
        case TokenType::KEYWORD:
            return "keyword";
        case TokenType::IDENTIFIER:
            return "identifier";
        case TokenType::OPERATOR:
            return "operator";
        case TokenType::REGEX:
            return "regular expression";
        default:
            return "unknown";
    }
}

#endif
