/*
 * Copyright (c) 2024 - Nathanne Isip
 * This file is part of Zhivo.
 * 
 * Zhivo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 * 
 * Zhivo is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Zhivo. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef ZHIVO_AST_EXPR_ARRAY_ACCESS_HPP
#define ZHIVO_AST_EXPR_ARRAY_ACCESS_HPP

#include <ast/ASTNode.hpp>
#include <ast/TerminativeSignal.hpp>

#include <memory>
#include <vector>

class ArrayAccessExpression : public ASTNode {
private:
    std::unique_ptr<ASTNode> array;
    std::unique_ptr<ASTNode> index;

public:
    explicit ArrayAccessExpression(
        std::unique_ptr<Token> _address,
        std::unique_ptr<ASTNode> _array,
        std::unique_ptr<ASTNode> _index
    ) : array(std::move(_array)),
        index(std::move(_index)) {
        this->address = std::move(_address);
    }

    ASTNode* getArrayExpression() const;
    ASTNode* getIndexExpression() const;

    [[nodiscard]] DynamicObject visit(SymbolTable& symbols) override;
};

#endif
