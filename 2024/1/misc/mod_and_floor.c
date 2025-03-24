#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    double f = 0.0031415;
    double x = floor(f);
    int mod = fmod(x, (double)10);
    printf("%lf\t%lf\t%d\n", f, x, mod);

    double hash = floor(1000*((3*rand())%1));
    printf("%lf\n", hash);
    return 0;
}
