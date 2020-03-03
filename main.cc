#include "TinyCInterpreter.h"
#include <stdio.h>
#include <fstream>
#include <string.h>

int static read_full_content(const std::string& filePath, std::string& out) {
	std::ifstream ifs(filePath.c_str());

	if (!ifs.is_open()) {
		return -1; 
	}   

	ifs.seekg(0, std::ios::end);
	int size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	char buff[size+1];
	ifs.read(buff, size);
	ifs.close();

	out.assign(buff, size);

	return 0;
}   

double foo(int a) {
	return a + 1;
}

int bar() {
	return 10 + 2;
}

typedef double (*proc_wrapped)(int arg);
typedef double (*proc_wrapped1)();
int main()
{
	c99Interpreter c99;

	{
		std::string content;
		read_full_content("../test.tcc", content);

		c99.setSearchPath("../");
		c99.compileScript(content.c_str(), "test.tcc");
		c99.addCustomSymbols("foo", (void*)foo);
		c99.buildSymbol("test.tcc");
		proc_wrapped func = (proc_wrapped)c99.getFunInScript("test.tcc", "wrapped");
		printf("%p, %lf\n", func, func(100));
	}

	return 0;
}

