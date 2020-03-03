#include "TinyCInterpreter.h"
#include <stdio.h>


int  c99Interpreter::tcc_state_used_  = 0;

c99Interpreter::c99Interpreter() {
    tcc_sym_ = NULL;
    scripts_.clear();
    c99Interpreter::tcc_state_used_  = 0;
    searchPath_ = "./";
}

c99Interpreter::~c99Interpreter() {
    if (tcc_sym_)  tcc_delete(tcc_sym_);

    for (auto& script : scripts_) {
        free(script.second.code_mem_);
        script.second.funcs_.clear();
    }
    
    c99Interpreter::tcc_state_used_  = 0;
}

int     
c99Interpreter::setSearchPath(const char* path) {
    if (!path) return -1;

    searchPath_ = path;
    return 0;
}

int 
c99Interpreter::compileScript(const char* scriptContent, const char* scriptName) {
    if (!scriptContent || !scriptName) {
        fprintf(stderr, "param null.\n");
        return -1;
    }
    
    if (scripts_.count(scriptName) > 0) {
        fprintf(stderr, "script name repeat.\n");
        return -1;
    }

    if (c99Interpreter::tcc_state_used_ == 1) {
        fprintf(stderr, "tcc state in use.\n");
        return -1;
    }

    static char source_head[] = {"#define c99_export  \n"};
    c99Interpreter::tcc_state_used_ = 1;
    std::string full_source;

    int ret = 0;
    tcc_sym_ = tcc_new();


    if (!tcc_sym_) {
        fprintf(stderr, "could not create tcc state\n");
        ret = -1;
        goto compile_err_exit;
    }
    {

    tcc_set_lib_path(tcc_sym_, searchPath_.c_str());

    tcc_set_output_type(tcc_sym_, TCC_OUTPUT_MEMORY);
    }
    full_source = std::string(source_head) + scriptContent;
    if (tcc_compile_string(tcc_sym_, full_source.c_str()) == -1) {
        fprintf(stderr, "compile error.\n");
        ret = -1;
        goto compile_err_exit;
    }

    {
    //get all export symbol
    scripts_.insert(std::make_pair(scriptName, tcc_func_list()));
    auto& func_list = scripts_[scriptName];
    func_list.scriptContent_ = scriptContent;
    }
    return ret;
compile_err_exit:
    if (tcc_sym_) tcc_delete(tcc_sym_);
    tcc_sym_ = NULL;
    c99Interpreter::tcc_state_used_ = 0;
    return ret;
}

int     
c99Interpreter::addCustomSymbols(const char* symbol_name,  void* func) {
    if (!tcc_sym_) {
        return -1;
    }

    tcc_add_symbol(tcc_sym_, symbol_name, (void*)func);
    return 0;
}

int     
c99Interpreter::buildSymbol(const char* scriptName) {
    if (scripts_.count(scriptName) <= 0) {
        return -1;
    }

    if (!tcc_sym_) {
        return -1;
    }

    int ret = 0;
    auto& func_list = scripts_[scriptName];

    systemSymbols();
    utilsSymbols();

    int size = tcc_relocate(tcc_sym_, NULL);
    if (size == -1) {
        fprintf(stderr, "tcc relocate error.\n");
        ret = -1;
        goto build_exit;
    }

    func_list.code_mem_ = malloc(size);
    tcc_relocate(tcc_sym_, func_list.code_mem_);

    {
    //analasy source code to get all export function
    std::vector<std::string> funcs;
    analyse_export_funcs(func_list.scriptContent_.c_str(), funcs);

    for (auto& name : funcs) {
        func_list.funcs_.insert(std::make_pair(name, tcc_get_symbol(tcc_sym_, name.c_str())));
    }
    }
build_exit:
    tcc_delete(tcc_sym_);
    tcc_sym_ = NULL;
    c99Interpreter::tcc_state_used_ = 0;
    return ret;
}

const tcc_func_list*   
c99Interpreter::getScript(const char* scriptName) {
    if (!scriptName) 
        return NULL;

    if (scripts_.count(scriptName) <= 0)
        return NULL;

    return &(scripts_[scriptName]);
}

const void*            
c99Interpreter::getFunInScript(const char* scriptName, const char* funcName) {
    if (!scriptName || !funcName) 
        return NULL;

    if (scripts_.count(scriptName) <= 0)
        return NULL;

    if (scripts_[scriptName].funcs_.count(funcName) <= 0) {
        return NULL;
    }    

    return scripts_[scriptName].funcs_[funcName];
}


int 
c99Interpreter::analyse_export_funcs(const char* source, std::vector<std::string>& funcs) {
    if (!source) 
        return -1;

    //c99_export xxx_fun_name
    static char expect[] = {'c', '9', '9', '_', 'e', 'x', 'p', 'o', 'r', 't', 1};
    int state_index = 0;
    std::string func_name = "";
    for (int i = 0; source[i] != 0; i++) {
        if (expect[state_index] == 1) {
            
            if (source[i] == '(') {
                if (func_name != "")
                    funcs.push_back(func_name);
                
                state_index = 0;
                func_name = "";
            }
            else if (!isalpha(source[i]) && !isalnum(source[i]) && source[i] != '_') {
                if (source[i] == ' ' || source[i] == '\t' || source[i] == '\n') {
                    if (func_name == "")
                        continue;
                }
                else {
                    state_index = 0;
                    func_name = "";
                }
            }
            else  {
                func_name.append(1, source[i]);
            }
        }
        else if (source[i] == expect[state_index]) {
            state_index++;
            printf("%c %d\n", source[i], state_index-1);
            continue;
        }
    }

    return 0;
}

void c99Interpreter::systemSymbols() {

}

void c99Interpreter::utilsSymbols() {

}
