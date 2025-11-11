#include <stdio.h>
#include <unistd.h>

int x = 0;

int main()
{
  fork();
  printf("Hello World. x = %d (%p)\n", x, &x);
}
