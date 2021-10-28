#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "type.h"
#define PI 3.14159265

/** returns the energy consumed during the transmission*/
float energy(int n)
{
    return timeOnAir[node[n].ind_sf]*mwTojoulePersec(log2lin(node[n].tx));
}

/**returns the normalized value of the energy consumption of player n*/
float energy_N(int n, int nbRetransm)
{
    float energy_max = timeOnAir[5]*mwTojoulePersec(log2lin(14.0));
    float energy_min = timeOnAir[0]*mwTojoulePersec(log2lin(2.0));

    float energyN=(energy(n))/(energy_max);
    return energyN;
}


/**returns the reward of node n transmission*/
double getReward(int n, int fail, int nbRetransm)
{
    if(fail==1) nbRetransm=nbRetransm-1;
    int q=1;
    if(reward_function==2) return (double)!fail;
    if(reward_function==3) return (double)1.0-(energy_N(n,nbRetransm)+fail*alpha)/(1.0+alpha);
    if(reward_function==4) return (double)1.0- energy_N(n,1);
    if(reward_function==5) return (double)1.0-(energy_N(n,nbRetransm)+(float)(nbRetransm-1.0)/(retransmit-1.0)*alpha)/(1.0+alpha);
    if(reward_function==7) return (double)1.0-((1.0-alpha)*energy_N(n,1)+fail*alpha);
    if(reward_function==8) return (double) (1.0-alpha*energy_N(n,1))*(1.0-(double)fail);
    if(reward_function==9) return (double) (1.0-alpha*sqrt(energy_N(n,1)))*(1.0-(double)fail);
    if(reward_function==10) return (double) pow((1.0-alpha*sqrt(energy_N(n,1))),4)*(1.0-(double)fail);
    if(reward_function==11)
        {//printf("rw=%f, fail=%d\n",pow((1.0-alpha*(energy_N(n,1))),q)*(1.0-(double)fail),fail);
        return (double) pow((1.0-alpha*(energy_N(n,1))),q)*(1.0-(double)fail); }

}

/**arm selected by background nodes
*/
int selectArm(int n)
{
    float x=RAND_R(1.0);
    int tp = rand() % 5 ;
    node[n].tx=tp*3.0+2.0;

    if(backgroundN==1)
        {if(x>=0.6)
            {node[n].ind_sf=5;
             return node[n].ind_sf+tp*6;
            }
        if(x>=0.35)
            {node[n].ind_sf=4;
             return node[n].ind_sf+tp*6;
            }
        if(x>=0.2)
            {node[n].ind_sf=3;
             return node[n].ind_sf+tp*6;
            }
        if(x>=0.1)
            {node[n].ind_sf=2;
             return node[n].ind_sf+tp*6;
            }
        if(x>=0.0)
            {node[n].ind_sf=1;
             return node[n].ind_sf+tp*6;
            }
        }

    if(backgroundN==2)
        {if(x>=0.6)
            {node[n].ind_sf=0;
             return node[n].ind_sf+tp*6;
            }
        if(x>=0.35)
            {node[n].ind_sf=1;
             return node[n].ind_sf+tp*6;
            }
        if(x>=0.2)
            {node[n].ind_sf=2;
             return node[n].ind_sf+tp*6;
            }
        if(x>=0.1)
            {node[n].ind_sf=3;
             return node[n].ind_sf+tp*6;
            }
        if(x>=0.0)
            {node[n].ind_sf=4;
             return node[n].ind_sf+tp*6;
            }
        }
    if(backgroundN==3)
        {if(x>=0.8631)
            {node[n].ind_sf=0;
             return node[n].ind_sf+tp*6;
            }
        if(x>=0.6231)
            {node[n].ind_sf=1;
             return node[n].ind_sf+tp*6;
            }
        if(x>=0.4392)
            {node[n].ind_sf=2;
             return node[n].ind_sf+tp*6;
            }
        if(x>=0.3159)
            {node[n].ind_sf=3;
             return node[n].ind_sf+tp*6;
            }
        if(x>=0.0760)
            {node[n].ind_sf=4;
             return node[n].ind_sf+tp*6;
            }
        if(x>=0.0)
            {node[n].ind_sf=5;
             return node[n].ind_sf+tp*6;
            }
        }
}


/**initializes the value of K according to the considered problem
*/
void initialize_problem()
{
    //the arms are the 6 sf
    if(Problem==1)
        K=6;

    //the arms are the thirty pairs (sf,tx)
    if(Problem ==2)
        K=30;
}

/**returns the sf and tx of the corresponding selected arm
*/
void playArm(int n, int k)
{

    if(Problem ==1)
        node[n].ind_sf=k;



    if(Problem ==2)
    switch(k)
    {

        case 0:
            node[n].ind_sf=0;
            node[n].tx=2.0;
            break;

        case 1:
            node[n].ind_sf=1;
            node[n].tx=2.0;
            break;

        case 2:
            node[n].ind_sf=2;
            node[n].tx=2.0;
            break;

        case 3:
            node[n].ind_sf=3;
            node[n].tx=2.0;
            break;

        case 4:
            node[n].ind_sf=4;
            node[n].tx=2.0;
            break;

        case 5:
            node[n].ind_sf=5;
            node[n].tx=2.0;
            break;

        case 6:
            node[n].ind_sf=0;
            node[n].tx=5.0;
            break;

        case 7:
            node[n].ind_sf=1;
            node[n].tx=5.0;
            break;

        case 8:
            node[n].ind_sf=2;
            node[n].tx=5.0;
            break;

        case 9:
            node[n].ind_sf=3;
            node[n].tx=5.0;
            break;

        case 10:
            node[n].ind_sf=4;
            node[n].tx=5.0;
            break;

        case 11:
            node[n].ind_sf=5;
            node[n].tx=5.0;
            break;

        case 12:
            node[n].ind_sf=0;
            node[n].tx=8.0;
            break;

        case 13:
            node[n].ind_sf=1;
            node[n].tx=8.0;
            break;

        case 14:
            node[n].ind_sf=2;
            node[n].tx=8.0;
            break;

        case 15:
            node[n].ind_sf=3;
            node[n].tx=8.0;
            break;

        case 16:
            node[n].ind_sf=4;
            node[n].tx=8.0;
            break;

        case 17:
            node[n].ind_sf=5;
            node[n].tx=8.0;
            break;

        case 18:
            node[n].ind_sf=0;
            node[n].tx=11.0;
            break;

        case 19:
            node[n].ind_sf=1;
            node[n].tx=11.0;
            break;

        case 20:
            node[n].ind_sf=2;
            node[n].tx=11.0;
            break;

        case 21:
            node[n].ind_sf=3;
            node[n].tx=11.0;
            break;

        case 22:
            node[n].ind_sf=4;
            node[n].tx=11.0;
            break;

        case 23:
            node[n].ind_sf=5;
            node[n].tx=11.0;
            break;

        case 24:
            node[n].ind_sf=0;
            node[n].tx=14.0;
            break;

        case 25:
            node[n].ind_sf=1;
            node[n].tx=14.0;
            break;

        case 26:
            node[n].ind_sf=2;
            node[n].tx=14.0;
            break;

        case 27:
            node[n].ind_sf=3;
            node[n].tx=14.0;
            break;

        case 28:
            node[n].ind_sf=4;
            node[n].tx=14.0;
            break;

        case 29:
            node[n].ind_sf=5;
            node[n].tx=14.0;
            break;


        default:
            printf("arm %d does not exist \n",k);
    }

}
