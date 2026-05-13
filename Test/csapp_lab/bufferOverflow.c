#include <stdio.h>

void attack()
{
	puts("You have been hacked!!!");
	return;
}

void myfunc()
{
	char input[4];

	gets(input);
	puts("Your input is:");
	puts(input);

	return;
}

int main(int argc, char const *argv[])
{

	myfunc();
	return 0;
}

// To disable ASLR, use the command below before compilation: 
// echo 0 | sudo tee /proc/sys/kernel/randomize_va_space

// use the following command when compiling
// -fno-stack-protector, 
// example: gcc -static -O0 lecture04_01_bufferOverflow.c -fno-stack-protector

// -O0 is necessary, becuase you will lose the attack function,
// if you don't suppress the optimization using -O0.

// To enable ASLR again, change the value 0 to 1 after "echo"