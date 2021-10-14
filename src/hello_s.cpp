#include <hello_s.h>

#include <stdio.h>
#include <cstdio>
#include <cstdlib>

Hello::Hello(const char* name)
{
	this->_name = name;
	printf("Oh hi there %s!\n", name);
}
