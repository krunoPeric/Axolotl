#include <stdio.h>

#define xmac(x) #x
#define mac(x) (int a=x; xmac(a))

int main()
{
	mac(5);	
	return 0;
}
