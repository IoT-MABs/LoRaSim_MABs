
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "type.h"
#define PI 3.14159265

/**returns a random number between 0 and x
*/
float RAND_R(float x)
{
    float num=(float)rand() / (double)(RAND_MAX)*x;
    return num;
}

/**Log2Lin conversion from logarithmic to linear scale
*/

float log2lin(float login){
    return pow(10,(float)(login/10.0));
}

/**Lin2Log conversion from linear to logarithmic scale
*/

float lin2log(float linin){
    return 10*log10(linin);
}

/**returns a random number normally distributed
it selects uniformly one number from the normally
distributed numbers file
*/
float randn()
{
    int x=(int)  RAND_R(1000000.5);
    float rndmn=randn_nb[x];

    return rndmn;
}


/**return absolute value of a float
*/
float absf(float x)
{
    if (x<0)
        return -1*x;
    else
        return x;
}
