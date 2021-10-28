#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "type.h"
#define PI 3.14159265


void Initialise(char *filename1,char *filename2)
{
    frameduration=(float*)malloc(sizeof(float)*6);

    timeOnAir=(float*)malloc(sizeof(float)*6);

    DLframeduration=(float*)malloc(sizeof(float)*6);

    sensitivity_SF=(float*)malloc(sizeof(float)*6);

    SINR_SF=(float*)malloc(sizeof(float)*6);

    nb_sf=(int*)malloc(sizeof(int)*6);

    arm=(double*)malloc(sizeof(double)*6);


    //ul time on air in sec
    arm[5]=0.023390178; //SF7
    arm[4]=0.042062532; //SF8
    arm[3]=0.093308528; //SF9
    arm[2]=0.168045942; //SF10
    arm[1]=0.306349239; //SF11
    arm[0]=0.561061;  //SF12

    //ul time on air in sec
    frameduration[0]=0.0617; //SF7
    frameduration[1]=0.1131; //SF8
    frameduration[2]=0.2058; //SF9
    frameduration[3]=0.3707; //SF10
    frameduration[4]=0.8233; //SF11
    frameduration[5]=1.483;  //SF12

    //ul time on air
    timeOnAir[0]=0.041216; //SF7
    timeOnAir[1]=0.072192; //SF8
    timeOnAir[2]=0.144384; //SF9
    timeOnAir[3]=0.247808; //SF10
    timeOnAir[4]=0.495616; //SF11
    timeOnAir[5]=0.991232;  //SF12

    //dl time on air
    DLframeduration[0]=0.0514; //SF7
    DLframeduration[1]=0.092;  //SF8
    DLframeduration[2]=0.1648; //SF9
    DLframeduration[3]=0.3297; //SF10
    DLframeduration[4]=0.6594; //SF11
    DLframeduration[5]=1.319;  //SF12

    //Antenna sensitivity for the different SF
    //(Sensitivity=-174+10log10(bandwidth)+Receiver noise figure+SNR)
    //sensitivity_SF=zeros(6);
    sensitivity_SF[0]=-124; //SF7
    sensitivity_SF[1]=-127; //SF8
    sensitivity_SF[2]=-130; //SF9
    sensitivity_SF[3]=-133; //SF10
    sensitivity_SF[4]=-135; //SF11
    sensitivity_SF[5]=-137; //SF12

    //SINR per SF
    //SINR_SF=zeros(6);
    SINR_SF[0]=-7; //SF7
    SINR_SF[1]=-9; //SF8
    SINR_SF[2]=-11.5; //SF9
    SINR_SF[3]=-14; //SF10
    SINR_SF[4]=-16.5; //SF11
    SINR_SF[5]=-19; //SF12


    d=(float*)malloc(6*sizeof(float));
    margin_dec=(float*)malloc(6*sizeof(float));
    margin_inc=(float*)malloc(6*sizeof(float));
    SNR_margin=10;// 10 dB or 12 dB
    //SF 7
    margin_dec[0]=-5.0;
    margin_inc[0]=-7.5;
    //SF 8
    margin_dec[1]=-7;
    margin_inc[1]=-10;
    //SF 9
    margin_dec[2]=-9;
    margin_inc[2]=-12.5;
    //SF 10
    margin_dec[3]=-11.5;
    margin_inc[3]=-15;
    //SF 11
    margin_dec[4]=-14;
    margin_inc[4]=-17.5;
    //SF 12
    margin_dec[5]=-16.5;
    margin_inc[5]=-20;


    slot=0;
    time_slot_duration=400;
    //R = 2000; //radius of a site
    C_sur_N_cible = 6; // C/N cible en dB
    for(int i=0; i<6; i++)
        {d[i]=frameduration[i]/0.01;
        printf("d=%f\n",d[i]);}


    //generating normally distributed numbers
    randn_nb=(float*)malloc(sizeof(float)*1000000);
    FILE *randnF;
    randnF=fopen("randn.txt","r");
    int ok=0;
    if (randnF == NULL) printf("error\n");
    for(int i=0;i<1000000;i++)
        { ok=fscanf(randnF,"%f\n",&randn_nb[i]);
        if (ok != 1) printf("error\n\n");
        }

    initParam();
    BSread(filename1);
    InitNetwork (filename2);
    alloc();
    total_plays=(int*)malloc(sizeof(int)*Draws);
    total_sentPkts=(int*)malloc(sizeof(int)*Draws);
    total_success=(int*)malloc(sizeof(int)*Draws);
    total_collisions=(int*)malloc(sizeof(int)*Draws);
    total_loss=(int*)malloc(sizeof(int)*Draws);
    totalLossbyN=(int*)malloc(sizeof(int)*Draws);
    energybyN=(float*)malloc(sizeof(float)*Draws);
    sampleComplexity=(int*)malloc(sizeof(int)*Draws);
    exploreTime=(int*)malloc(sizeof(int)*Draws);

    intrnalColl=(int*)malloc(sizeof(int)*Draws);
    loss=(int*)malloc(sizeof(int)*Draws);
    externalColl=(int*)malloc(sizeof(int)*Draws);
    nbPlaysPerArm=(int**)malloc(sizeof(int*)*Draws);
    energyPerArm=(double**)malloc(sizeof(double*)*Draws);
    successPerArm=(int**)malloc(sizeof(int*)*Draws);
    lostPkts=(int**)malloc(sizeof(int*)*Draws);

    fairness=(double*)malloc(sizeof(double)*Draws);
    nPlaysbyarm=(int*)malloc(sizeof(int)*K);
    nColbyarm=(int*)malloc(sizeof(int)*K);
    nlossbyarm=(int*)malloc(sizeof(int)*K);


    wEXP=(float **)malloc(sizeof(float *)*(N-N1));
	for (int i=0;i<(N-N1);i++) wEXP[i]=(float *)malloc(sizeof(float)*K);

    pEXP=(float **)malloc(sizeof(float *)*(N-N1));
	for (int i=0;i<(N-N1);i++) pEXP[i]=(float *)malloc(sizeof(float)*K);

    locateNodes();
    initialize_problem();

    //if playing CBAIMPB
    if(Algorithm==3)
    {

        // server
        Lambda=(int **)malloc(sizeof(int *)*(N-N1));
        for (int i=0;i<N-N1;i++) Lambda[i]=(int *)malloc(sizeof(int)*K);

        omiga=(int **)malloc(sizeof(int *)*N-N1);
        for (int i=0;i<N-N1;i++) omiga[i]=(int *)malloc(sizeof(int)*K);

        nb_plays=(int*)malloc(sizeof(int)*K);
        global_mu=(float*)malloc(sizeof(float)*K);

        for (int i=N1;i<N;i++) node[i].A=(int *)malloc(K*sizeof(int));
        for (int i=N1;i<N;i++) node[i].nonOptimals=(int *)malloc((K-M)*sizeof(int));
        for (int i=N1;i<N;i++) node[i].selected=(int *)malloc((M)*sizeof(int));
        for (int i=N1;i<N;i++) node[i].discarded=(int *)malloc((K-M)*sizeof(int));


        average_mu=(float **)malloc(sizeof(float *)*Draws);
        Rewards=(float *)malloc(sizeof(float )*Draws);
        collRate=(float **)malloc(sizeof(float *)*Draws);
        loss=(int *)malloc(sizeof(int )*Draws);
        muMin=(float *)malloc(sizeof(float )*Draws);
        muMax=(float *)malloc(sizeof(float )*Draws);
        playersMeans=(float **)malloc(sizeof(float *)*Draws);
        playersRewards=(float*)malloc(sizeof(float)*(N-N1));
        for(int i=0;i<N1;i++)
            playersRewards[i]=0.0;
        duration=(int *)malloc(sizeof(int )*Draws);

        for (int i=0;i<Draws;i++) {
            average_mu[i]=(float *)malloc(sizeof(float)*(T/(T/TimeDisplay)+1));
            collRate[i]=(float *)malloc(sizeof(float)*(T/(T/TimeDisplay)+1));
            playersMeans[i]=(int *)malloc(sizeof(int)*(N-N1));
            for (int j=N1-N1;j<N-N1;j++) {playersMeans[i][j]=0.0;}
        }

        for (int i=0;i<Draws;i++) {
            Rewards[i]=0.0;
            loss[i]=0;
            duration[i]=0;
        }

        // Action
        Action=(int *)malloc(sizeof(int)*K);
        globalSelected=(int *)malloc(sizeof(int)*K);
        globalDiscarded=(int *)malloc(sizeof(int)*K);
        Arm=(float *)malloc(sizeof(float)*K);

        // Logs
        int S=T/TimeDisplay;
        Logs=(float **)malloc(sizeof(float *)*Draws);
        for (int i=0;i<Draws;i++) {
            Logs[i]=(float *)malloc(sizeof(float)*S);
            for (int j=0;j<S;j++) Logs[i][j]=0.0;
        }

        SampleC=(int *)malloc(sizeof(int )*Draws);
        collisions=(int *)malloc(sizeof(int )*Draws);
        plays=(int *)malloc(sizeof(int )*Draws);
        for (int i=0;i<Draws;i++) {
            SampleC[i]=0;
            collisions[i]=0;
            plays[i]=0;
        }

        // Tampon mu
        mu=(float *)malloc(sizeof(float)*K);
        phase1=(int *)malloc(sizeof(int)*N);
        armsPlayed=(int **)malloc(sizeof(int *)*N);
        for (int i=0;i<N;i++) armsPlayed[i]=(int *)malloc(sizeof(int)*K);
    }

    exter_colArms=(int**)malloc(sizeof(int*)*6);
    inter_colArms=(int**)malloc(sizeof(int*)*6);
    losArms=(int**)malloc(sizeof(int*)*6);
    for(int i=0;i<6;i++)
    {
        inter_colArms[i]=(int*)malloc(sizeof(int)*5);
        exter_colArms[i]=(int*)malloc(sizeof(int)*5);
        losArms[i]=(int*)malloc(sizeof(int)*5);

    }
    commCost=(int*)malloc(sizeof(int)*Draws);
    for(int i=0;i<Draws;i++)
        {commCost[i]=0;
        intrnalColl[i]=0;
        total_sentPkts[i]=0;
        loss[i]=0;
        externalColl[i]=0;
        totalLossbyN[i]=0;
        energybyN[i]=0;
        nbPlaysPerArm[i]=(int*)malloc(sizeof(int)*K);

        successPerArm[i]=(int*)malloc(sizeof(int)*K);
        lostPkts[i]=(int*)malloc(sizeof(int)*K);
        energyPerArm[i]=(double*)malloc(sizeof(double)*K);
        for(int j=0;j<K;j++)
            {nbPlaysPerArm[i][j]=0;
             successPerArm[i][j]=0;
             lostPkts[i][j]=0;
             energyPerArm[i][j]=0.0;
            }
        }
    realAvg=(float*)malloc(sizeof(float)*K);
    totally_lost=(int*)malloc(sizeof(int)*Draws);
    pdrProgress=(double**)malloc(sizeof(double*)*Draws);
        for(int i=0;i<Draws;i++)
            {totally_lost[i]=0;
             pdrProgress[i]=(double*)malloc(sizeof(double)*TimeDisplay);
             for(int j=0;j<TimeDisplay;j++)
                pdrProgress[i][j]=-1;
            }

    total_pdrPerArm=(double**)malloc(sizeof(double*)*Draws);
    ex_playsPerArm=(int**)malloc(sizeof(int*)*Draws);
    ex_rewardsPerArm=(double**)malloc(sizeof(double*)*Draws);
    for(int i=0;i<Draws;i++)
        {total_pdrPerArm[i]=(double*)malloc(sizeof(double)*K);
        ex_playsPerArm[i]=(int*)malloc(sizeof(int)*K);
        ex_rewardsPerArm[i]=(double*)malloc(sizeof(double)*K);
            for(int k=0;k<K;k++)
                {total_pdrPerArm[i][k]=0.0;
                ex_playsPerArm[i][k]=0;
                ex_rewardsPerArm[i][k]=0.0;
                }
        }



    pdrProgressPerArm=(double***)malloc(sizeof(double**)*Draws);
    for(int i=0;i<Draws;i++)
        {pdrProgressPerArm[i]=(double**)malloc(sizeof(double*)*K);
         for(int j=0;j<K;j++)
            {pdrProgressPerArm[i][j]=(double*)malloc(sizeof(double)*TimeDisplay);
             for(int k=0;k<TimeDisplay;k++)
                pdrProgressPerArm[i][j][k]=-1;
            }
        }


    playsPerArm=(int*)malloc(sizeof(int)*K);
    lossPerArm=(int*)malloc(sizeof(int)*K);
    rewardPerArm=(double*)malloc(sizeof(double)*K);
    for(int i=0;i<N;i++)
        {node[i].last_sinr=(float*)malloc(sizeof(float)*20);
        node[i].nb_sf=(int*)malloc(sizeof(int)*6);
        node[i].nbPlayArms=(int*)malloc(sizeof(int)*K);
        }

    estimations=(double**)malloc(sizeof(double*)*N);
    for(int i=0;i<N;i++) estimations[i]= (double*)malloc(sizeof(double)*K);

    total_energy=(double*)malloc(sizeof(double)*Draws);
    total_rewards=(double*)malloc(sizeof(double)*Draws);

    finalEstimation=(double**)malloc(sizeof(double*)*Draws);
    for(int i=0;i<Draws;i++) finalEstimation[i]= (double*)malloc(sizeof(double)*K);

    interF=fopen("res_interColl.txt","w");
    intraF=fopen("res_intraColl.txt","w");
    intraF2=fopen("res_intra2Coll.txt","w");
    lossF=fopen("res_lossColl.txt","w");
    timeF=fopen("time.txt","w");


}

void init()
{
    nbPkts=0;
    for(int i=0;i<6;i++)
        nb_sf[i]=0;

    for(int k=0;k<K;k++)
        {playsPerArm[k]=0;
         lossPerArm[k]=0;
         rewardPerArm[k]=0.0;
        }

    for(int j=0;j<K;j++)
        {nPlaysbyarm[j]=0;
        nColbyarm[j]=0;
        nlossbyarm[j]=0;
        }

    for(int i=0;i<6;i++)
        {for(int j=0;j<5;j++)
            {inter_colArms[i][j]=0;
            exter_colArms[i][j]=0;
            losArms[i][j]=0;
            }
        }

    arms=(int**)malloc(sizeof(int*)*6);
    for(int i=0;i<6;i++)
        {arms[i]=(int*)malloc(sizeof(int)*5);
        for(int j=0;j<5;j++)
            arms[i][j]=0;
        }

    rewardAvgs=(float**)malloc(sizeof(float*)*6);
    for(int i=0;i<6;i++)
    {
        rewardAvgs[i]=(float*)malloc(sizeof(float)*5);
        for(int j=0;j<5;j++)
            rewardAvgs[i][j]=0.0;
    }

    for(int i=0;i<K;i++)
        for(int j=0;j<N-N1;j++)
            wEXP[j][i]=1.0;


    global_plays=0;
    sinrsum=0.0;
    global_success=0;
    global_loss=0;
    global_rewards=0.0;
    global_collisions=0;
    global_energy=0.0;
    counter=0;
    sum_Nstep=0;
    initNodes();
    find_epsilon2();
    initialize_T_k();
}
/**initiates the structure Param
*/
void initParam(){

    //resolution
    P.resolution = 1; //taille coté pixel en metres

    //Channel Characteristics
    P.bandwidth = 125*pow(10,3); //125 kHz
    P.th_noise = -174+10*log10(P.bandwidth);//  thermal noise
    P.NF = 3; // Gateway noise figure = 3 dB
    P.shadowing = sqrt(pow(12,2)+pow(6,2)); //for indoor nodes

    //Uplink channel parameters
    P.P0 = log2lin(14.0);//14 dBm for uplink and downlink nominal transmission power
    P.alpha = 0.9;// 0.9
}


/** reads the BS parameter file*/
void BSread(char *nom_fichier)
{
    int k =0,nb_bs=0,ok;
    FILE *fic;


    fic=fopen(nom_fichier,"r");
	if (fic == NULL)
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }

    ok=fscanf(fic, "%d\n",&netwDim); //Network dimension limits the number of stations to consider in the network
    if (ok != 1)  { printf("error in BS read\n");return 0;}
    bs=(struct BS *)malloc(sizeof(struct BS)*netwDim);
    printf("netwDim %d \n", netwDim);

   while(1){

        if(k==netwDim) break;

        ok=fscanf(fic, "%d ",&bs[k].x);
        if (ok != 1)  { printf("error in bs.x\n");return 0;}

        ok=fscanf(fic, "%d ",&bs[k].y);
        if (ok != 1) return 0;

        ok=fscanf(fic, "%d ",&bs[k].type);
        if (ok != 1) return 0;

        ok=fscanf(fic, "%d ",&bs[k].connection);
        if (ok != 1) return 0;

        ok=fscanf(fic, "%d\n",&bs[k].azimuth); //Tri-sectorized or not
        if (ok != 1) bs[k].azimuth= 0;


        if  (bs[k].type == 0) //base station
            nb_bs = nb_bs + 1;

        char snum[5];
        char src[50];

        strcpy(src,  "BS");

        itoa(nb_bs, snum, 10);// convert nb-bs to string
        strcat(src,snum);
        bs[k].name =(char *)malloc(sizeof(char)*20);
        bs[k].name = src;
        printf("name %s \n", bs[k].name);
        bs[k].pilot_power = log2lin(14.0);
        bs[k].data_power = log2lin(14.0);
        bs[k].height = 30; //30m

        k++;

   }

	fclose(fic);
}

/**initialize the problem settings from the file param.txt
*/
int InitNetwork (char *filename)
{
	FILE *fic;
	int ok;

    //printf("in init network\n");
	fic=fopen(filename,"r");
	if (fic == NULL) return 0;

	ok=fscanf(fic, "Algorithm=%d\n",&Algorithm);
	if (ok != 1) return 0;

	ok=fscanf(fic, "Problem=%d\n",&Problem);
	if (ok != 1) return 0;

	ok=fscanf(fic, "reward_function=%d\n",&reward_function);
	if (ok != 1) return 0;

	ok=fscanf(fic, "alpha=%f\n",&alpha);
	if (ok != 1) return 0;

	ok=fscanf(fic, "K=%d\n",&K);
	if (ok != 1) return 0;

	ok=fscanf(fic, "M=%d\n",&M);
	if (ok != 1) return 0;

	ok=fscanf(fic, "T=%d\n",&T);
	if (ok != 1) return 0;

	ok=fscanf(fic, "T1=%d\n",&T1);
	if (ok != 1) return 0;

	ok=fscanf(fic, "R=%d\n",&R);
	if (ok != 1) return 0;

	ok=fscanf(fic, "TimeDisplay=%d\n",&TimeDisplay);
	if (ok != 1) return 0;

	ok=fscanf(fic, "delta=%f\n",&delta);
	if (ok != 1) return 0;

	ok=fscanf(fic, "epsilon=%f\n",&epsilon);
	if (ok != 1) return 0;

	ok=fscanf(fic, "eta=%f\n",&eta);
	if (ok != 1) return 0;

	ok=fscanf(fic, "gamma=%f\n",&gamma);
	if (ok != 1) return 0;

	ok=fscanf(fic, "Draws=%d\n",&Draws);
	if (ok != 1) return 0;

	ok=fscanf(fic, "N=%d\n",&N);
	if (ok != 1) return 0;

	ok=fscanf(fic, "N1=%d\n",&N1);
	if (ok != 1) return 0;
    N=N+N1;

	ok=fscanf(fic, "background=%d\n",&backgroundN);
	if (ok != 1) return 0;

    ok=fscanf(fic, "retransmit=%d\n",&retransmit);
	if (ok != 1) return 0;

	fclose(fic);
	return 1;
}

/**initiates the structure bandit
*/
void InitBandit(struct Bandit *B)
{
	for (int i=0;i<K;i++) {

			B[i].mu=0.0;
			B[i].avg_reward=0.0;
			B[i].tk=0;
			B[i].B=RAND_MAX;
			B[i].UCB=900.0;
			B[i].LCB=-900.0;
			B[i].var=0.0; //variance
			B[i].notified=0;
		}
}

/**initiates the active rates of all the internal and external nodes
*/
void initializeActiveRates()
{
    for (int n=0;n<N1;n++) {
        node[n].pr=0.01;
        node[n].p=0.01;
        }

    p1=(1-exp(log(1-0.1)/(1000-1)))*K;
    pN=p1/4;
    p1=(2*p1-pN);
    float x=(p1-pN)/(N-N1-1);
    float y=pN;
    for (int n=N1;n<N;n++) {
        node[n].pr=y;
        node[n].p=y;
        y=y+x;
        }

}

/**initialize all the nodes
*/
void initNodes(){
	for (int i=0;i<N;i++) {
        InitBandit(node[i].B);

        node[i].ind_sf=5;
        node[i].ch=0;
        node[i].tx=14.0;
        node[i].SNR=0.0;
        node[i].init=0;

        node[i].previous_emission_t=-1.0;
        node[i].current_emission_t=-1.0;

        node[i].nb_plays=0;
        node[i].nb_pkts=0;

        node[i].nb_success=0;
        node[i].nb_collisions=0;
        node[i].nb_loss=0;
        node[i].sum_rewards=0.0;
        node[i].isPlaying=0;
        node[i].cons_coll=0;
        node[i].energy=0.0;

        node[i].estimationsSent=0;
        node[i].finishEstimating=0;
        for(int j=0;j<20;j++)
            node[i].last_sinr[j]=-1.0;
        for(int j=0;j<6;j++)
            node[i].nb_sf[j]=0;
        for(int j=0;j<K;j++)
            node[i].nbPlayArms[j]=0;
        node[i].indx=0;

	}
    initializeActiveRates();
}

/**locate the nodes (geographically)
*/
int locateNodes()
{
    FILE *fic;
	int ok;
    char filename1[200];
    char filename2[200];
    char str[200];

    //allocate nodes using .txt files
    sprintf(filename2,"hex_%d.txt",R);
    sprintf(filename1,"hex_%d.txt",R);
	fic=fopen(filename2,"r");
	if (fic == NULL) {printf("null file*****************************\n"); return 0;}
    float x;
    float y;
	for(int i=0;i<N1; i++)
        {ok=fscanf(fic, "%f %f\n",&x,&y);
         node[i].x=x;
         node[i].y=y;
         if (ok != 2) return 0;
        }
    fic=fopen(filename1,"r");
	if (fic == NULL) {printf("null file*****************************\n"); return 0;}
	for(int i=0;i<N-N1; i++)
        {ok=fscanf(fic, "%f %f\n",&x,&y);
         node[i+N1].x=x;
         node[i+N1].y=y;
         if (ok != 2) return 0;
        }
}

/**memory allocation
*/
void alloc()
{
    node=(struct node*)malloc(sizeof(struct node)*N);
    for (int i=0;i<N;i++)
        {node[i].B=(struct Bandit *)malloc(K*sizeof(struct Bandit));
         node[i].nbPlayArms=(int*)malloc(sizeof(int)*K);
        }
}
