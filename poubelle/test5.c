#include "minishell.h"

int main()
{
	int test = 1;

	printf("%d\n", !test);

	test = !test;

	printf("%d\n", test);

	test = !test;

	printf("%d\n", test);
}