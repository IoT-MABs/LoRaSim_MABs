#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "type.h"
#define PI 3.14159265

/**returns 1 if each arm has beed played at least one with node n*/
int allArmsPlayedOnce(int n)
{
    for(int k=0;k<K;k++)
        if(node[n].B[k].tk<1) return 0;
    return 1;
}

/**returns the arm k to be played in round robin*/
int play_roundRobin(int n)
{
    int k;
    if(node[n].nb_pkts<1) k=playUniform(K);
    else {if(node[n].arm==K-1) k= 0;
          else k = node[n].arm+1;
        }

    return k;
}

/**return an arm unfiromly chosen*/
int playUniform(int K_arms)
{
    return rand() % K_arms;
}

/**select an arm using UCB*/
int recommendArm_UCB (int n)
{
    int max=0;

    for(int k=0;k<K;k++)
        {if(node[n].B[k].avg_reward+node[n].B[k].UCB > node[n].B[max].avg_reward+node[n].B[max].UCB)
            max=k;
        }
    for(int k=0;k<K;k++)

    return max;
}

/**updates the values of UCB of each arm*/
void updateUCB1 (int n)
{
    for(int k=0;k<K;k++)
        node[n].B[k].UCB=sqrt(2*log(node[n].nb_pkts)/(node[n].B[k].tk));
}
