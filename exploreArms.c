#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "type.h"
#define PI 3.14159265

/*************************************************__________________________Exploration, DORG, DOFG_________________________***************************************************/

/**finds the player of the least active rate
*/
int slowPl()
{
    int min=0;
    for (int n=N1;n<N-N1;n++)
        if(node[min].pr>node[n].pr)
            min=n;
    return min;
}

/**returns 1 if node n finishes estimating
*/
int nodeFinishEstimating(int n)
{
    for(int k=0;k<K;k++)
        if(node[n].B[k].tk<node[n].T_k)
            return 0;
    return 1;
}

/** returns 1 if enough estimations are done
*/
int enoughEstimations()
{
    int sum=0;
    int sumP=0;
    int sum2=0;
    for (int k=0;k<K;k++)
        {for(int n=N1;n<N;n++)
            if(node[n].estimationsSent>=1) {sum=sum+node[n].B[k].tk; sumP=sumP+1;}
         if(sum < ceil(log(2*K/delta)/(2*pow(epsilon2,2))) )return 0;
         sum=0;
        }
    printf("players voting are %d/%d, sum=%d/%f\n", sumP/K,N,sum ,ceil(log(2*K/delta)/(2*pow(epsilon2,2))));

    return 1;
}

/**returns 1 if all nodes finish estimating
*/
int finishEstimating()
{
    for (int n=N1;n<N;n++)
        if(nodeFinishEstimating(n)==0) return 0;
    return 1;
}

/**returns 1 if all nodes play the required number of times
*/
int testFinishRequiredNb()
{
    for(int n=N1;n<N;n++)
        for(int k=0;k<K;k++)
            if(node[n].B[k].tk<node[n].T_k) return 0;
    return 1;
}

/** finds the collision rate
*/
float collisionRate(int n)
{
    float prod=1.0;
    for(int pl=N1;pl<N;pl++)
        if(pl!=n) prod=prod*(1-node[pl].pr/K);
    return prod;
}

/**finds the final estimations after exploration ends
*/
void findFinalEstimation()
{
    float sum1;
    int sum2;
    for(int k=0;k<K;k++)
        {sum1=0.0;
         sum2=0;
         for(int n=N1; n<N;n++)
            if(node[n].estimationsSent>=1)
                {sum1=sum1+estimations[n][k]*node[n].B[k].tk;
                 sum2=sum2+node[n].B[k].tk;
                }
         finalEstimation[dr][k]=(float)sum1/sum2;
         printf("est[%d]=%f/%f, nb=%d\n",k,finalEstimation[dr][k],(float) log(2*K/delta)/(2*pow(epsilon2,2)),sum2);
        }
}

/**assigns to each node the number of samples to do
*/
void initialize_T_k()
{
    float sum=0.0;
    float prod=1.0;
    for(int n=N1;n<N;n++) {sum=sum+node[n].pr; }
    for(int n=N1;n<N;n++)
        {   for(int n1=N1;n1<N;n1++) if(n1!=n)prod=prod*(float)(1.0-node[n1].pr/K);
            node[n].T_k= (int) ceil(node[n].pr*(float)log(2*K/delta)/(2*pow(epsilon2,2)*sum*pow(prod,2)));
            prod=1.0;
        }
}

/**finds the value of epsilon2
*/
void find_epsilon2()
{
    float den=0.0;
    float value1;
    float value2;

    for(int i=N1;i<N;i++)
        den=den+node[i].pr;

    epsilon2=(epsilon-den)/K;
    epsilon2=epsilon;
}

/**sort nodes in decreasing order of their active rates
*/
int* sortNodes()
{
    double* sorted=(double*)malloc(sizeof(double)*(N-N1));
    int* sortedNodes=(int*)malloc(sizeof(int)*(N-N1));
    for(int i=N1;i<N;i++)
        {sorted[i-N1]=node[i].pr;
        sortedNodes[i-N1]=i;}
    int i,j,b;
    double a;
    for (i = 0; i < N-N1; ++i)
        {
            for (j = i + 1; j < N-N1; ++j)
            {

                if (sorted[i] < sorted[j])
                {
                    a =  sorted[i];
                    b = sortedNodes[i];

                    sorted[i] = sorted[j];
                    sortedNodes[i]=sortedNodes[j];

                    sorted[j] = a;
                    sortedNodes[j]=b;
                }
            }
        }
    return sortedNodes;

}

/**nodes collaborate to estimate the mean rewards of the arms*/
void exploreArms(){

    printf("_________________*********************________________Exploring starts______________*********************________________________\n");

while(enoughEstimations()==0)
{


        for (int n=0;n<N;n++)
        {
        if(node[n].pr >= RAND_R(1.0))
        {

        if(n<N1)
            node[n].arm =selectArm(n);


           if(n>=N1)
            {
                node[n].arm = play_roundRobin(n);
                playsPerArm[node[n].arm]++;
                int k=node[n].arm;
                node[n].B[k].tk=node[n].B[k].tk+1;
                playArm(n,node[n].arm);
                global_plays++;
                global_energy=global_energy+energy(n);

                nbPlaysPerArm[dr][node[n].arm]++;
                energyPerArm[dr][node[n].arm]=energyPerArm[dr][node[n].arm]+energy(n);

                arms[node[n].ind_sf][(int) node[n].tx/3]++;
                if(node[n].nb_success<1) commCost[dr]++;
            }

            node[n].isPlaying=1;
            node[n].nb_plays++;

            node[n].shadowing=P.shadowing*randn();

            float rn=0.0;
            while(rn==0) rn=RAND_R(1.0);
            node[n].rayleigh=-1.0*log(rn);
            node[n].PL=log2lin(pathLoss(n)+P.shadowing*randn());
            node[n].RSSI=log2lin(node[n].tx)*node[n].rayleigh/node[n].PL;
            node[n].energy=node[n].energy+energy(n);

            node[n].nb_sf[node[n].ind_sf]++;
            nb_sf[node[n].ind_sf]++;
            }
        else node[n].isPlaying=0;
    }

    /**collisions*/

    for(int n=0;n<N;n++)
    {
        if(node[n].isPlaying)
            {updateSINR(n);
             int y=Tr_success(n);
             int k=node[n].arm;
             if(y==0&& n>=N1)
                 {
                 node[n].cons_coll++;
                 node[n].pr=node[n].p*8;
                 }

             if(y==1 && n>=N1) global_success++;
             if(n>=N1)
                {successPerArm[dr][k]=successPerArm[dr][k]+y;
                }


             if( (y==1 ||  node[n].cons_coll==retransmit)&& n>=N1)
                 {nbPkts++;
                 node[n].pr=node[n].p;
                 }

                 node[n].nb_pkts++;
                 float rw=getReward(n,y==0,node[n].cons_coll+1);
                 node[n].sum_rewards=node[n].sum_rewards+rw;
                 node[n].B[k].avg_reward= (float)rw/(float)node[n].B[k].tk+(float)(node[n].B[k].tk-1)/(float)node[n].B[k].tk*node[n].B[k].avg_reward;
                 if(n>=N1)  global_rewards=global_rewards+rw;
                 if(n>=N1) {rewardPerArm[k]=rewardPerArm[k]+rw;
                            rewardAvgs[node[n].ind_sf][(int) node[n].tx/3]=rewardAvgs[node[n].ind_sf][(int) node[n].tx/3]+rw;}


             if(y==1)
                {
                node[n].cons_coll=0;
                node[n].nb_success++;
                }

            if(node[n].cons_coll==retransmit&& n>=N1)
                {
                    if(n==N-1) totalLossbyN[dr]++;
                    node[n].cons_coll=0;
                    totally_lost[dr]++;
                    losArms[node[n].ind_sf][(int) node[n].tx/3]++;
                }

             node[n].B[k].mu=(float)y/(float)node[n].B[k].tk+(float)(node[n].B[k].tk-1)/(float)node[n].B[k].tk*node[n].B[k].mu;

             //sending estimations
             if (n>=N1 && nodeFinishEstimating(n)==1 && node[n].finishEstimating==1 && node[n].estimationsSent<1 )
                {if(y==1.0)
                    {node[n].estimationsSent=node[n].estimationsSent+1;
                     if(node[n].estimationsSent>=1 )
                        {for(int nn=0;nn<K;nn++)
                            node[n].nbPlayArms[nn]=node[n].B[nn].tk;
                        }
                    }
                 commCost[dr]++;
                }

             for(int pl=N1;pl<N;pl++)
                {
                 if (nodeFinishEstimating(pl)==1 && node[pl].finishEstimating==0)
                    {if(finishEstimating()) printf("players finish estimating at time %d with node %d\n", slot,n);
                     for(int arm=0;arm<K;arm++)
                        estimations[pl][arm]=node[pl].B[arm].avg_reward/collisionRate(n);

                     node[pl].finishEstimating=1;
                    }
                }

            }

    }


    slotEnd();
    slot++;
 }

findFinalEstimation();

for(int j=0;j<K;j++)
    printf("estimation[%d]=%f(%f)\n",j,finalEstimation[dr][j],ex_rewardsPerArm[dr][j]/ex_playsPerArm[dr][j]);


  FILE *f1;
f1=fopen("res_losArms_explore.txt","w");
for(int i=0;i<5;i++)
    {for(int j=0;j<6;j++)
        fprintf(f1,"%d\t", losArms[j][i]);

    fprintf(f1,"\n" );
    }
fclose(f1);

f1=fopen("res_inter_colArms-explore.txt","w");
for(int i=0;i<5;i++)
    {for(int j=0;j<6;j++)
        fprintf(f1,"%d\t", inter_colArms[j][i]);

    fprintf(f1,"\n" );
    }
fclose(f1);

f1=fopen("res_exter_colArms_explore.txt","w");
for(int i=0;i<5;i++)
    {for(int j=0;j<6;j++)
        fprintf(f1,"%d\t", exter_colArms[j][i]);

    fprintf(f1,"\n" );
    }
fclose(f1);


f1=fopen("res_armsPlays_explore.txt","w");
for(int i=0;i<5;i++)
    {for(int j=0;j<6;j++)
        fprintf(f1,"%d\t", arms[j][i]);

    fprintf(f1,"\n" );
    }
fclose(f1);
}





