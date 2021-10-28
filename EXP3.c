
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "type.h"
#define PI 3.14159265


void p_exp3(int n)
{
    float sum=0;
    for(int i=0;i<K;i++)
        sum=sum+wEXP[n-N1][i];
    for(int i=0;i<K;i++)
        pEXP[n-N1][i]=(1-gamma)*wEXP[n-N1][i]/sum+ gamma/K;
}


int drawArm_exp3(int n)
{
    float sum=0;
    float sum1=0;
    for(int i=0;i<K;i++)
        sum=sum+wEXP[n-N1][i];

    float numy=((float)rand()/(float)(RAND_MAX)) * sum;
    for(int i=0;i<K;i++)
        {
            sum1=sum1+wEXP[n-N1][i];
            if(numy<=sum1)
                return i;
        }


}

void update_wEXP(int n ,float y)
{
    for (int i=0;i<K;i++)
        if(i==node[i].arm) wEXP[n-N1][i]=wEXP[n-N1][i]*exp(gamma*y/pEXP[n-N1][i]/K);

}

