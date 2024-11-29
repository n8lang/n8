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

#include <N8.hpp>

#include <n8/ast/ASTNode.hpp>
#include <n8/ast/ASTNodeException.hpp>
#include <n8/ast/TerminativeSignal.hpp>
#include <n8/core/Runtime.hpp>
#include <n8/parser/Parser.hpp>
#include <n8/parser/ParserException.hpp>
#include <n8/parser/Tokenizer.hpp>
#include <n8/util/Print.hpp>

#include <exception>
#include <iostream>
#include <stack>
#include <thread>
#include <vector>

#if defined(__unix__) || defined(__linux__) || defined(__APPLE__)
#   include <dlfcn.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#   include <Windows.h>
#else
#   error "Unsupported architecture for shared objects or dynamic libraries."
#endif

bool Runtime::testMode = false;
std::unordered_map<std::string, void*> Runtime::nativeLibraries;

#ifdef __EMSCRIPTEN__
std::string Runtime::elementId = "";
#endif

bool Runtime::isTestMode() {
    return Runtime::testMode;
}

void Runtime::setTestMode(bool _testMode) {
    Runtime::testMode = _testMode;
}

void Runtime::addLoadedLibrary(std::string libName, void* handle) {
    Runtime::nativeLibraries[libName] = handle;
}

void* Runtime::getLoadedLibrary(std::string libName) {
    return Runtime::nativeLibraries[libName];
}

bool Runtime::hasLoadedLibrary(std::string libName) {
    return Runtime::nativeLibraries.find(libName) !=
        Runtime::nativeLibraries.end();
}

void Runtime::cleanUp() {
    for(const auto& [key, value] : Runtime::nativeLibraries)
        if(value != nullptr)
            #if defined(__unix__) || defined(__linux__) || defined(__APPLE__)
            dlclose(value);
            #elif defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
            FreeLibrary((HMODULE) value);
            #endif

    Runtime::nativeLibraries.clear();
}

static inline bool isBalanced(const std::string& input) {
    std::stack<char> stack;

    for(char ch : input) {
        if(ch == '(' || ch == '{' || ch == '[')
            stack.push(ch);
        else if(ch == ')' || ch == '}' || ch == ']') {
            if(stack.empty())
                return false;

            char top = stack.top();
            if((ch == ')' && top != '(') ||
                (ch == '}' && top != '{') ||
                (ch == ']' && top != '['))
                return false;

            stack.pop();
        }
    }

    return stack.empty();
}

#ifndef __EMSCRIPTEN__
void Runtime::repl() {
    SymbolTable symtab;
    std::string input, line;
    int iterNum = 1;

    std::cout << N8_MAIN_BANNER
        << std::endl << std::endl;
    std::cout << "\u001b[1;32m>>>\u001b[0m ";

    while(std::getline(std::cin, line)) {
        input += line + '\n';

        if(isBalanced(input)) {
            try{
                Tokenizer tokenizer(
                    input,
                    "<repl, iteration: " +
                        std::to_string(iterNum) + ">"
                );
                tokenizer.scan();

                Parser parser(tokenizer.getTokens());
                parser.parse();

                for(const auto& statement : parser.getGlobalStatements())
                    statement->visit(symtab);

                symtab.detachParallelNodes();
            }
            catch(const std::system_error& exc) {
                symtab.waitForThreads();
                Runtime::cleanUp();

                N8Util::printError("[\u001b[1;31mSystem Error\u001b[0m]: \u001b[3;37m");
                N8Util::printError(exc.what());
                N8Util::printError("\u001b[0m\r\n");
            }
            catch(const ASTNodeException& nodeExc) {
                symtab.waitForThreads();
                Runtime::cleanUp();

                N8Util::printError("[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m");
                N8Util::printError(nodeExc.what());
                N8Util::printError("\u001b[0m\r\n                 ");
                N8Util::printError(nodeExc.getAddress()->toString());
                N8Util::printError("\r\n");
            }
            catch(const LexicalAnalysisException& lexAnlExc) {
                symtab.waitForThreads();
                Runtime::cleanUp();

                N8Util::printError("[\u001b[1;31mLexical Error\u001b[0m]:\r\n\t");
                N8Util::printError(lexAnlExc.what());
                N8Util::printError("\r\n");
            }
            catch(const ParserException& parserExc) {
                symtab.waitForThreads();
                Runtime::cleanUp();

                N8Util::printError("[\u001b[1;31mParser Error\u001b[0m]:  \u001b[3;37m");
                N8Util::printError(parserExc.what());
                N8Util::printError("\u001b[0m\r\n                 ");
                N8Util::printError(parserExc.getAddress()->toString());
                N8Util::printError("\r\n");
            }
            catch(const TerminativeBreakSignal& breakExc) {
                symtab.waitForThreads();
                Runtime::cleanUp();

                N8Util::printError(
                    "[\u001b[1;31mRuntime Error\u001b[0m]: "
                    "\u001b[3;37mInvalid break statement signal caught.\u001b[0m"
                    "\r\n                 "
                );
                N8Util::printError(breakExc.getAddress().toString());
                N8Util::printError("\r\n");
            }
            catch(const TerminativeContinueSignal& continueExc) {
                symtab.waitForThreads();
                Runtime::cleanUp();

                N8Util::printError(
                    "[\u001b[1;31mRuntime Error\u001b[0m]: "
                    "\u001b[3;37mInvalid continue statement signal caught.\u001b[0m"
                    "\r\n                 "
                );
                N8Util::printError(continueExc.getAddress().toString());
                N8Util::printError("\r\n");
            }
            catch(const TerminativeReturnSignal& retExc) {
                symtab.waitForThreads();
                Runtime::cleanUp();

                N8Util::printError("\u001b[0;93m");
                N8Util::printError(retExc.getObject().toString());
                N8Util::printError("\u001b[0m\r\n");
            }
            catch(const TerminativeThrowSignal& throwExc) {
                symtab.waitForThreads();
                Runtime::cleanUp();

                N8Util::printError("[\u001b[1;31mUncaught Error\u001b[0m]: \u001b[3;37m");
                N8Util::printError(throwExc.getObject().toString());
                N8Util::printError("\u001b[0m\r\n                  ");
                N8Util::printError(throwExc.getAddress()->toString());
                N8Util::printError("\r\n");
            }
            catch(const std::exception& exc) {
                symtab.waitForThreads();
                Runtime::cleanUp();

                N8Util::printError("[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m");
                N8Util::printError(exc.what());
                N8Util::printError("\u001b[0m\r\n");
            }

            input.clear();
            iterNum++;

            std::cout << std::endl << "\u001b[1;32m>>>\u001b[0m ";
        }
        else std::cout << "\u001b[93m...\u001b[0m ";
    }
}

#else

void Runtime::setOutputElementId(std::string id) {
    Runtime::elementId = id;
}

std::string Runtime::getOutputElementId() {
    return Runtime::elementId;
}

void Runtime::execute(const char* sourceCode) {
    SymbolTable symtab;
    try{
        Tokenizer tokenizer(std::string(sourceCode), "<exec_engine>");
        tokenizer.scan();

        Parser parser(tokenizer.getTokens());
        parser.parse();

        for(const auto& statement : parser.getGlobalStatements())
            statement->visit(symtab);

        symtab.detachParallelNodes();
    }
    catch(const std::system_error& exc) {
        symtab.waitForThreads();
        Runtime::cleanUp();

        N8Util::printError("[\u001b[1;31mSystem Error\u001b[0m]: \u001b[3;37m");
        N8Util::printError(exc.what());
        N8Util::printError("\u001b[0m\r\n");
    }
    catch(const ASTNodeException& nodeExc) {
        symtab.waitForThreads();
        Runtime::cleanUp();

        N8Util::printError("[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m");
        N8Util::printError(nodeExc.what());
        N8Util::printError("\u001b[0m\r\n                 ");
        N8Util::printError(nodeExc.getAddress()->toString());
        N8Util::printError("\r\n");
    }
    catch(const LexicalAnalysisException& lexAnlExc) {
        symtab.waitForThreads();
        Runtime::cleanUp();

        N8Util::printError("[\u001b[1;31mLexical Error\u001b[0m]:\r\n\t");
        N8Util::printError(lexAnlExc.what());
        N8Util::printError("\r\n");
    }
    catch(const ParserException& parserExc) {
        symtab.waitForThreads();
        Runtime::cleanUp();

        N8Util::printError("[\u001b[1;31mParser Error\u001b[0m]:  \u001b[3;37m");
        N8Util::printError(parserExc.what());
        N8Util::printError("\u001b[0m\r\n                 ");
        N8Util::printError(parserExc.getAddress()->toString());
        N8Util::printError("\r\n");
    }
    catch(const TerminativeBreakSignal& breakExc) {
        symtab.waitForThreads();
        Runtime::cleanUp();

        N8Util::printError(
            "[\u001b[1;31mRuntime Error\u001b[0m]: "
            "\u001b[3;37mInvalid break statement signal caught.\u001b[0m"
            "\r\n                 "
        );
        N8Util::printError(breakExc.getAddress().toString());
        N8Util::printError("\r\n");
    }
    catch(const TerminativeContinueSignal& continueExc) {
        symtab.waitForThreads();
        Runtime::cleanUp();

        N8Util::printError(
            "[\u001b[1;31mRuntime Error\u001b[0m]: "
            "\u001b[3;37mInvalid continue statement signal caught.\u001b[0m"
            "\r\n                 "
        );
        N8Util::printError(continueExc.getAddress().toString());
        N8Util::printError("\r\n");
    }
    catch(const TerminativeReturnSignal& retExc) {
        symtab.waitForThreads();
        Runtime::cleanUp();

        N8Util::printError("\u001b[0;93m");
        N8Util::printError(retExc.getObject().toString());
        N8Util::printError("\u001b[0m\r\n");
    }
    catch(const TerminativeThrowSignal& throwExc) {
        symtab.waitForThreads();
        Runtime::cleanUp();

        N8Util::printError("[\u001b[1;31mUncaught Error\u001b[0m]: \u001b[3;37m");
        N8Util::printError(throwExc.getObject().toString());
        N8Util::printError("\u001b[0m\r\n                  ");
        N8Util::printError(throwExc.getAddress()->toString());
        N8Util::printError("\r\n");
    }
    catch(const std::exception& exc) {
        symtab.waitForThreads();
        Runtime::cleanUp();

        N8Util::printError("[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m");
        N8Util::printError(exc.what());
        N8Util::printError("\u001b[0m\r\n");
    }
}

#endif
