#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "type.h"


void dofg()
{
    printf("_________________*********************________________dofg______________*********************________________________\n");

    int* sortednodes;
    sortednodes=(int*)malloc(sizeof(int)*(N-N1));
    sortednodes=sortNodes();


    double ** PI;
    double ** L;
    int ** q;

    PI=(double **)malloc(sizeof(double *)*(N-N1));
	for (int i=0;i<N-N1;i++) PI[i]=(double *)malloc(sizeof(double)*K);

	q=(int **)malloc(sizeof(int *)*(N-N1));
	for (int i=0;i<N-N1;i++) q[i]=(int *)malloc(sizeof(int)*K);
	//initialization
	for (int k=0;k<K;k++)
    {
        PI[sortednodes[0]-N1][k]=1.0;
    }
    //start
    for (int n=0;n<N-N1;n++)
    {
        int maxK=0;
        for(int k=0;k<K;k++)
        {if(finalEstimation[dr][k]*PI[sortednodes[n]-N1][k]>=finalEstimation[dr][maxK]*PI[sortednodes[n]-N1][maxK])
                    maxK=k;
        }
        for(int k=0;k<K;k++)
            {if(k==maxK)
                {q[sortednodes[n]-N1][k]=1;
                node[sortednodes[n]].arm=k;
                if(n<N-N1-1)
                    {PI[sortednodes[n+1]-N1][k]=PI[sortednodes[n]-N1][k]*(1-node[sortednodes[n]-N1].pr);
                    }
                }

            else
                {q[sortednodes[n]-N1][k]=0;
                if(n<N-N1-1)
                    {PI[sortednodes[n+1]-N1][k]=PI[sortednodes[n]-N1][k];
                    }
                }

            }
    }

    ///expected mean reward
    float mean=0.0;
    float mult;
    float sum;
    for(int k=0;k<K;k++)
    {
        sum=0.0;
        for(int n=N1;n<N;n++)
            {mult=node[n].pr*q[n-N1][k];
             for(int n1=N1;n1<N;n1++)
                if(n1!=n) mult=mult*(1-node[n1].pr*q[n1-N1][k]);
             sum=sum+mult;
            }
        mean=mean+finalEstimation[dr][k]*sum;
    }
    printf("expected mean with fair greedy is=%f\n", mean);

    for(int k=0;k<K;k++)
    {
        printf("players playing arm %d:****\n",k);
        for(int n=N1;n<N;n++)
        {
            if(node[sortednodes[n-N1]].arm==k)
                printf("%d(%d)-",sortednodes[n-N1],n);
        }
        printf("\n\n");
    }
}



