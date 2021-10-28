#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "type.h"
#define PI 3.14159265

/*******************************************************LUCB***************************************************/

/**returns the cardinal of vector AD
*/
int Card(int * AD)
{
	int i,s;

	s=0;
	for (i=0;i<K;i++) {
		if (AD[i]==1) s++;
	}
	return s;
}

/** finds the remaining actions
*/
int* RemainingActions(int * AD)
{   int i,j=0;
    int* max;
	max=(int*)malloc(sizeof(int)*(M));

	for (i=0;i<K;i++) {
		if (AD[i]==1)
        {max[j]=i;
        j++;
        }
	}
	return max;
}

/**play arms in round robin
*/
int RoundRobin(int k, int *A)
{
	int i=k+1;

	if (i >= K) i=0;

	while (i<K && A[i] != 1) {
		i++;
		if (i==K) i=0;
	}
	return i;
}

/**returns randomly one arm
*/
int RandomChoice(int *A, int n)
{
    int i=1;
    while(A[(node[n].arm+i)%K]!=1)
        i++;
    return (node[n].arm+i)%K;
}

/**returns the number of plays of an arm k by all nodes
*/
int played(int k)
{
    int sum=0;
    for(int i=N1;i<N;i++)
        sum=sum+node[i].B[k].tk;
    return sum;
}

/**initiates the actions
*/
void InitAction()
{
	int i;

	for (i=0;i<K;i++) Action[i]=1;
	for (i=0;i<K;i++) globalDiscarded[i]=0;
	for (i=0;i<K;i++) globalSelected[i]=0;

}

/**initiates the players
*/
void InitPlayers()
{
	int i,j;
printf("N=%d\n",N);
	for (i=N1;i<N;i++) {
        node[i].epsilon=epsilon/4.0;
        node[i].eta=eta/2;
        node[i].delta=delta/2;
        for (j=0;j<K;j++) {node[i].A[j]=1;}
        for (j=0;j<K-M;j++) {node[i].nonOptimals[j]=-1;}
        for (j=0;j<M;j++) {node[i].selected[j]=-1;}
        for (j=0;j<K-M;j++) {node[i].discarded[j]=-1;}
        node[i].k=0;
        node[i].NbAction=K;
        node[i].t=0;
        node[i].phase1=0;
        node[i].stopCom=0;
        playersRewards[i]=0.0;
	}
}

/**returns 1 if the vectors A and B are equal
*/
int sameVector(int *A, int *B, int n)
{
    for(int i=0; i<n; i++)
        if(A[i]!=B[i]) return 0;
    return 1;
}

/**initiates the server
*/
void InitServer()
{
	int i,j;

	for (i=0;i<N-N1;i++) {
		for (j=0;j<K;j++) Lambda[i][j]=0;
	}
	for (i=0;i<N-N1;i++) {
		for (j=0;j<K;j++) omiga[i][j]=0;
	}
	for(i=0;i<K;i++)
        {global_mu[i]=0.0; nb_plays[i]=0;}
}


/**tests the deletion of actions (arms)
*/
void CallServer()
{
	int i,j,counter,k;
	if(Card(Action)!=M)
    {
    // eliminate arms that enough players want to eliminate
    for (j=0;j<K;j++) {

        //count the number of players that would like to eliminate arm j
		counter=0;
		for (i=0;i<N-N1;i++) {
                counter=counter+Lambda[i][j];
		}
        printf("counter=%d,needed=%d arm=%d played=%d mu=%f*****sc=%d*****\n", counter,(int)ceil(log(delta)/log(eta) ),j,nb_plays[j],global_mu[j],SampleC[dr]);
        // enough players wants to eliminate arm k
		if (counter >= (int)ceil(log(delta)/log(eta) )) {
			for (i=N1;i<N;i++) {
				if (Card(node[i].A) > M && Lambda[i-N1][j] == 0) {
					node[i].A[j]=0;
				}
				if (Card(node[i].A) == M && Lambda[i-N1][j] == 0) {
					k=RoundRobin(j,node[i].A);
					node[i].A[j]=0;
					node[i].A[k]=1;
				}
			}
			if (Card(Action) > M) {Action[j]=0;printf("remaining %d********************************************* \n", Card(Action));}
		}
}
	printf("\n\n");
	}

    // if only M arms are left globally, eliminate all arms except M for all players
    if (Card(Action) == M) {
		for (j=0;j<K;j++)
			for (i=N1;i<N;i++) {
				node[i].A[j]=Action[j];
				if (Action[j]==1) Lambda[i-N1][j]=0;
				else Lambda[i-N1][j]=1;
			}
        }
}


/**
sorts an array in ascending order
*/

int * sortArr( float *max)
{
    int i,j;
    float a;
    for (i = 0; i < M; ++i)
        {
            for (j = i + 1; j < M; ++j)
            {
                if (max[i] > max[j])
                {
                    a =  max[i];
                    max[i] = max[j];
                    max[j] = a;
                }
            }
        }

    return max;
}


/**
checks if the arm is one of the M best arms
returns its index in the top arms if yes, and
-1 otherwise
*/
int isOftheBest(int n, float* max, int index, int size)
{
    int i;
    for ( i=0; i < size; i++)
    {
        if (node[n].B[index].avg_reward > max[i])
            return i;
    }

    return -1;
}


/** tests if all node found the M best arms or not
*/
int allPlayersFinish()
{   int i;

    for (i=N1;i<N;i++)
        if( Card(node[i].A) > M)
            return 0;

    return 1;
}

/**returns the set of the m best arms
*/
void findM_best()
{
    M_best=(int*)malloc(sizeof(int)*M);

    int j=0;
    for(int i=0;i<K;i++)
        {if(Action[i]==1)
            {M_best[j]=i;
            j++;
            printf("M_best[%d]=%d\n",j,M_best[j-1]);
            }
        }
}

/**returns an arm uniformly chosen amongst the m best arms
*/
int playM_bestUniformly()
{
    int n = rand() % M ;
    return M_best[n];
}

/**returns the confidence interval of node n on arm k after t plays
*/
float confInt(int t, int n, int k)
{
    return (float)1.96*node[n].B[k].stdd/sqrt(node[n].B[k].tk*(float)log(pow(t,2)/eta));
}

/**returns if player n has not played all the arms in phase 1 yet
*/
int allArmsPlayed(int **armsPlayed,int n)
{
    for(int i=0;i<K;i++)
    {
        if (armsPlayed[n][i]==0)
            return 0;
    }
    return 1;
}

/**
sorts an array in ascending order
*/
float * sort2Arrays( float *max,int* maxArms,int M)
{
    int i,j,k;
    float a;
    for (i = 0; i < M; ++i)
        {
            for (j = i + 1; j < M; ++j)
            {
                if (max[i] > max[j])
                {
                    a =  max[i];
                    k = maxArms[i];

                    max[i] = max[j];
                    maxArms[i] = maxArms[j];

                    max[j] = a;
                    maxArms[j] = k;
                }
            }
        }

    return max;
}

/**finds the optimal M arms for player n
*/
int* findOptimals(int n)
{
    int i,j,k=0;
	float* max;
	int* optimal;

	max=(float*)malloc(sizeof(float)*(M));
	optimal=(int*)malloc(sizeof(int)*(M));

    //initialize array by zeros
    for (i = 0; i<M; i++)
        {max[i] = -1.0;
        optimal[i]=-1;
        }

    // find the best arms
	for (i=0;i<K;i++) {
		if (node[n].A[i] == 1 && isOftheBest(n, max,i,M) !=-1 ) {
            int ind=isOftheBest(n, max,i,M);
			max [ind]= node[n].B[i].avg_reward;
			optimal[ind]=i;
			max=sort2Arrays(max,optimal,M);
		}
	}
    return optimal;
}

/**finds the K-M suboptimal arms
*/
int* findNonOptimals(int n)
{
    int i,j,k=0,yes=0;
	int* nonOptimal;
	int* optimal;
	nonOptimal=(int*)malloc(sizeof(int)*(K-M));
    optimal=(int*)malloc(sizeof(int)*(M));

	optimal=findOptimals(n);

	for(i=0;i<K;i++)
        {for(j=0;j<M; j++)
            if( optimal[j]==i)
                yes=1;
        if(yes==0)
            {nonOptimal[k]=i;
            k++;}
            yes=0;
        }
    return nonOptimal;
}

/**find the arm of the highest upper confidence bound in the K-M suboptimal arms
*/
float findBestNonOptimal(int n)
{

    int* nonOptimals;
	nonOptimals=findNonOptimals(n);
	if(nonOptimals==NULL)printf("problem *****\n");

	float max=-INFINITY;
	int maxArm=-1;
	int i;

	for(i=0;i< K-M; i++)
    {
        if(node[n].B[nonOptimals[i]].UCB > max && node[n].A[i] == 1 )
        {
            max=node[n].B[nonOptimals[i]].UCB;
            maxArm=nonOptimals[i];
        }
    }
	return max;
}

/**find the arm of the lowest lower confidence bound in the M optimal arms
*/

float findWorstOptimal(int n)
{
    int* Optimals;
	Optimals=findOptimals(n);
	if(Optimals==NULL)printf("probleeeem************** \n");

	float min=INFINITY;
	int minArm=-1;
	int i;

	for(i=0;i< M; i++)
    {if(node[n].B[Optimals[i]].LCB < min)
        {
            min=node[n].B[Optimals[i]].LCB;
            minArm=Optimals[i];
        }
    }
	return min;
}

/**tests whether the player stops searching and sends messages
*/
int stop_reset(int n)
{
    float max=findBestNonOptimal(n);

    float min=findWorstOptimal(n);

    if(max-min < epsilon  &&(max!=min!=0))
    {
        int* nonOptimals;
        nonOptimals=findNonOptimals(n);
        for(int i=0; i<K-M; i++)
            {Lambda[n-N1][nonOptimals[i]]++;
            printf("node %d eliminates arm %d\n",n,nonOptimals[i]);
            }
        for(int j=0;j<K-M;j++)
            node[n].nonOptimals[j]=nonOptimals[j];
            for (int i=0;i<K;i++) {
                node[n].B[i].mu=0.0;
                node[n].B[i].avg_reward=0.0;
                node[n].B[i].tk=0;
                node[n].B[i].UCB=INFINITY;
                node[n].B[i].LCB=-INFINITY;
                node[n].B[i].var=0;
                node[n].B[i].stdd=0.0;
                armsPlayed[n][i]=0;
            }
            phase1[n]=0;
            node[n].t=0;
        return 1;
    }
    return 0;
}

/**node n plays an arm in round robin
*/
int RoundRobinn(int k, int **A, int n)
{
	int i=k+1;

	if (i >= K) i=0;

	while (i<K && A[n][i] == 1) {
		i++;
		if (i==K) i=0;
	}
	return i;
}

/**retruns an arm not yet played by node n
*/
int chooseNotPlayedArm(int **A, int n)
{
	int i,k;
	float a;
	a=floor(RAND_R((float)K));
	k=(int)a;

	i=RoundRobinn(k,A,n);
	return i;
}


/** This is the decentralized LUCB algorithm
    */
void LUCB()
{
    printf("in LUCB\n");
    int i,j,t,display,s,sc;
	float R,y;

    {
		InitServer();
		InitPlayers();
		InitAction();
		t=1;display=0;R=0.0;s=0;sc=0;

		for (int n=0; n<N; n++)
            {
                phase1[n]=0;
                for(int i=0; i<K; i++)
                    armsPlayed[n][i]=0;
            }
        SampleC[dr]=0;

		while (Card(Action) > M) //until M arms are left
        {
            duration[dr]++;
            for (int n=0; n<N; n++)
            {
                if(node[n].pr >= RAND_R(1.0))
                    {
                        if(n<N1)
                            node[n].arm =selectArm(n);

                        if(n>=N1)
                        {
                            SampleC[dr]++;
                            plays[dr]=plays[dr]+1;

                            // test if no arms are available to player n
                            if (Card(node[n].A) == 0) printf("zut %d\n",n);

                            // node n plays the arms in phase 1 which is the phase when a node selects each arm once
                            if(phase1[n]==0){

                                node[n].arm=chooseNotPlayedArm(armsPlayed,n);
                                node[n].k=node[n].arm;
                                playsPerArm[node[n].arm]++;
                                int k=node[n].arm;
                                node[n].B[k].tk=node[n].B[k].tk+1;
                                playArm(n,node[n].arm);

                                global_plays++;
                                global_energy=global_energy+energy(n);

                                nb_plays[k]++;
                                armsPlayed[n][k]=1;
                                nbPlaysPerArm[dr][node[n].arm]++;
                                energyPerArm[dr][node[n].arm]=energyPerArm[dr][node[n].arm]+energy(n);


                                arms[node[n].ind_sf][(int) node[n].tx/3]++;
                                if(node[n].nb_success<1) commCost[dr]++;
                                if(allArmsPlayed(armsPlayed,n)==1)
                                    phase1[n]=1;
                                }
                        // player n selects one arm randomly after phase 1 ends
                            else {
                                node[n].arm=RandomChoice(node[n].A,n);
                                node[n].k=node[n].arm;
                                playsPerArm[node[n].arm]++;
                                int k=node[n].arm;
                                node[n].B[k].tk=node[n].B[k].tk+1;
                                playArm(n,node[n].arm);

                                global_plays++;
                                global_energy=global_energy+energy(n);

                                nb_plays[k]++;
                                armsPlayed[n][k]=1;
                                nbPlaysPerArm[dr][node[n].arm]++;
                                energyPerArm[dr][node[n].arm]=energyPerArm[dr][node[n].arm]+energy(n);
                                arms[node[n].ind_sf][(int) node[n].tx/3]++;
                                if(node[n].nb_success<1) commCost[dr]++;
                        }
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

        for (int n=0; n<N; n++)
            {

                if(node[n].isPlaying)
                    {
                     updateSINR(n);
                     int y=Tr_success(n);
                     int k=node[n].arm;
                     if(y==0&& n>=N1)
                         {
                         node[n].cons_coll++;
                         node[n].pr=node[n].p*8;
                         }

                     if(y==1 && n>=N1) global_success++;
                     if(n>=N1) {successPerArm[dr][k]=successPerArm[dr][k]+y;}


                     if( (y==1 ||  node[n].cons_coll==retransmit)&& n>=N1)
                         {nbPkts++;
                         node[n].pr=node[n].p;}

                         node[n].nb_pkts++;

                         float rw=(float)getReward(n,y==0,node[n].cons_coll+1);
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

                    if(n>=N1)
                        {
                            Rewards[dr]=Rewards[dr]+rw;
                            playersRewards[n-N1]=playersRewards[n-N1]+rw;
                            global_mu[k]=(float)rw/(float )nb_plays[k]+(float)(nb_plays[k]-1)/(float)nb_plays[k]*global_mu[k];
                            node[n].B[k].stdd=sqrt(((float )node[n].B[k].stdd*((float )node[n].B[k].tk-1)+pow((float)rw/collisionRate(n)-node[n].B[k].avg_reward/collisionRate(n),2))/((float )node[n].B[k].tk));
                            node[n].t = node[n].t+1;
                            node[n].B[k].LCB = node[n].B[k].avg_reward/collisionRate(n) -confInt(node[n].t,n,k);
                            node[n].B[k].UCB = node[n].B[k].avg_reward/collisionRate(n) +confInt(node[n].t,n,k);

                            if (phase1[n]==1&&(stop_reset(n)) ) {
                                CallServer();
                            }
                            R=R+Arm[0]-(float)y;

                            if (display == TimeDisplay) {
                                display=0;
                                Logs[dr][s]=R;
                                s++;
                            }
                            }
                }
            }
			if ( Card(Action) ==M ) {

				SampleC[dr]=sc;
				printf("SC[%d]=%d  plays=%d collisions=%d, nbPkts=%d********************\n",dr,SampleC[dr],plays[dr],collisions[dr],nbPkts);

				//the remaining action
				for (int j=0;j<K;j++) {if (Action[j]==1) printf ("k*=%d played=%d mu=%f\n",j,nb_plays[j], global_mu[j]);
                                        else printf ("k=%d played=%d mu=%f\n",j,nb_plays[j], global_mu[j]);printf("\n");
				}

			}

		}

        }

	return 1;
}

