#include "string.h"
#include <stdbool.h>

void *memset(void *s, int c, size_t n)
{
    unsigned char* p=s;
    while(n--)
	*p++=(unsigned char) c;
    return s;
}

void memcpy(void* dest, void* src, size_t n)
{
    size_t i;
    char* d = (char*) dest;
    char* s = (char*) src;
    for (i = 0; i < n; ++i)
    {
        d[i] = s[i];
    }
}

void reverse ( char s[] )
{
    int c, i, j;

    for (i=0, j= strlen(s)-1;i<j;i++,j--)
    {
	c = s[i];
	s[i] = s[j];
	s[j] = c;
    }
}

void itoa(int n, char s[])
{
     int i, sign;

     if ((sign = n) < 0)  /* record sign */
     {
         n = -n;          /* make n positive */
     }
     i = 0;
     do       /* generate digits in reverse order */
     {
	 s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
     {
         s[i++] = '-';
     }
     s[i] = '\0';
     reverse(s);
}


size_t strlen(const char* str)
{
    size_t ret = 0;
    while ( str[ret] != 0 )
	ret++;
    return ret;
}

int strcmp(const char* s1, const char* s2)
{
    while(*s1 && (*s1==*s2))
    {
        s1++,s2++;
    }
    return *(const unsigned char*)s1-*(const unsigned char*)s2;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
    for ( ; n > 0; s1++, s2++, --n)
	if (*s1 != *s2)
	    return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
	else if (*s1 == '\0')
	    return 0;
    return 0;
}

int atoi(const char *p)
{
    int k = 0;
    while (*p) {
        k = (k<<3)+(k<<1)+(*p)-'0';
        p++;
     }
     return k;
}

bool isNumericChar(char x)
{
    return (x >= '0' && x <= '9')? true: false;
}

int stoi(const char *str)
{
    if ((*str) == 0)
       return 0;

    int res = 0;
    int sign = 1;
    int i = 0;

    if (str[0] == '-')
    {
        sign = -1;
        i++;
    }

    for (; str[i] != '\0'; ++i)
    {
        if (isNumericChar(str[i]) == false)
            return 0;
        res = res*10 + str[i] - '0';
    }

    return sign*res;
}
double atof(const char* num)
{
    if (!num || !*num)
	return 0;
    double integerPart = 0;
    double fractionPart = 0;
    int divisorForFraction = 1;
    int sign = 1;
    bool inFraction = false;

    if (*num == '-')
    {
	++num;
	sign = -1;
    }
    else if (*num == '+')
    {
	++num;
    }
    while (*num != '\0')
    {
	if (*num >= '0' && *num <= '9')
	{
	    if (inFraction)
            {
		fractionPart = fractionPart*10 + (*num - '0');
		divisorForFraction *= 10;
	    }
	    else
            {
		integerPart = integerPart*10 + (*num - '0');
	    }
	}
	else if (*num == '.')
	{
	    if (inFraction)
		return sign * (integerPart + fractionPart/divisorForFraction);
	    else
		inFraction = true;
	}
	else
        {
	    return sign * (integerPart + fractionPart/divisorForFraction);
	}
	++num;
    }
    return sign * (integerPart + fractionPart/divisorForFraction);
 }
