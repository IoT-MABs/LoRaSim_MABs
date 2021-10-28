#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "type.h"
#define PI 3.14159265


int main(int argc, char* argv[])
{

    srand ( time(NULL) );

    if (argc <= 2) {
		printf("Include the files of the base station and network settings \n");
		return 0;
	}

	if (argc > 2) {
        Initialise(argv[1],argv[2]);//argv[1]=BS_File.txt, argv[2]=param.txt
	}

	if(Algorithm < 1 || Algorithm > 6 )
        {printf("This algorithm does not exist\n");
        return 0;}


    for(dr=0;dr<Draws;dr++)
        {
            init();

            background();
            perc_sf();

            switch(Algorithm)
            {

                case 1: //DORG
                    //exploration
                    exploreArms();
                    sampleComplexity[dr]=global_plays-commCost[dr];
                    exploreTime[dr]=slot-T1;
                    //exploiation
                    dorg();
                    exploitGreedy();
                    break;

                case 2: //DOFG
                    //exploration
                    exploreArms();
                    sampleComplexity[dr]=global_plays-commCost[dr];
                    exploreTime[dr]=slot-T1;
                    //exploiation
                    dofg();
                    exploitGreedy();
                    break;

                case 3: //LUCB (CBAIMPB)
                    //exploration
                    LUCB();
                    //exploitation
                    exploitLUCB();
                    break;

                case 4: //UCB
                    exploitUCB();
                    break;

                case 5: // ADR
                    exploitADR();
                    break;

                case 6: // EXP3
                    exploitEXP3();
                    break;

                default:
                    printf("algorithm does not exist\n");
            }

            total_plays[dr]=global_plays;
            total_success[dr]=global_success;
            total_collisions[dr]=global_collisions;
            total_loss[dr]=global_loss;
            total_energy[dr]=global_energy;
            total_rewards[dr]=global_rewards;

            for(int k=0;k<K;k++)
                total_pdrPerArm[dr][k]=rewardPerArm[k]/playsPerArm[k];

            /**Stats after each draw*/
            statsPerdraw();
            fairness[dr]=getFainess();

            printf("dr=%d, total_plays=%d,total_success=%d,total_collisions=%d,total_loss=%d, total_energy=%f, total_rewards=%f, pdr=%f\n\n",dr,total_plays[dr],total_success[dr],total_collisions[dr],total_loss[dr],total_energy[dr],total_rewards[dr],(double)total_success[dr]/total_plays[dr] );

        }
    for(int i=0;i<Draws;i++)
            printf("dr=%d, total_plays=%d,total_success=%d,total_collisions=%d,total_loss=%d, total_energy=%f, total_rewards=%f, pdr=%f\n",i,total_plays[i],total_success[i],total_collisions[i],total_loss[i],total_energy[i],total_rewards[i],(float)total_success[i]/total_plays[i] );


    printf("Go collect stats\n");

    //global stats
    collectStats();
    return 0;
}

