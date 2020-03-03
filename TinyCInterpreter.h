#ifndef C99_INTERPRETER_H
#define C99_INTERPRETER_H

#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <assert.h>
#include <map>
#include <vector>
#include "libtcc.h"

struct tcc_func_list {
    std::string scriptContent_;
    std::map<std::string, void*> funcs_;
    void* code_mem_;
};

class c99Interpreter {
    public:
        c99Interpreter();
        ~c99Interpreter();
        int     setSearchPath(const char* path);
        int     compileScript(const char* scriptContent, const char* scriptName);
        int     addCustomSymbols(const char* symbol_name,  void* func);
        int     buildSymbol(const char* scriptName);

        const tcc_func_list*   getScript(const char* scriptName);
        const void*            getFunInScript(const char* scriptName, const char* funcName);

    private:
        void    systemSymbols();
        void    utilsSymbols();
        int analyse_export_funcs(const char* source, std::vector<std::string>& funcs);

    private:
        std::string searchPath_;
        TCCState* tcc_sym_;
        std::map<std::string, tcc_func_list> scripts_;
        static int  tcc_state_used_;
};
#endif
