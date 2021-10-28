
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "type.h"
#define PI 3.14159265


/*************************************************__________________________Statistics____________________________***************************************************/

/**prints the per of each sf at the end of the simulation*/
void perc_sf()
    {
        int *perc_sf;
        perc_sf=(int*)malloc(sizeof(int)*6);

        for(int i=0;i<6;i++)
            perc_sf[i]=0;
        for(int n=0;n<N;n++)
            perc_sf[node[n].ind_sf]++;

        for(int i=0;i<6;i++)
            printf("perc_sf[%d]=%d\n",i,perc_sf[i]);
    }


void collectStats()
{

    char nom_fic[256];
    strcpy(nom_fic,"res");
    double ecart=0.0;
	double m=0.0;

    strcpy(filename1,nom_fic);
	strcat(filename1,"_PDRperArm.txt");

	strcpy(filename2,nom_fic);
	strcat(filename2,"_PDRperArm_err.txt");

	file1=fopen(filename1,"a");
	file2=fopen(filename2,"a");

    for(int k=0;k<K;k++)
        {for (int i=0;i<Draws;i++)
            {m=m+total_pdrPerArm[i][k];}
         m=m/(double)Draws;
         for(int i=0;i<Draws;i++) ecart=ecart+(double)(total_pdrPerArm[i][k]-m)*(total_pdrPerArm[i][k]-m);
         ecart=sqrt(ecart/(double)(Draws));
         fprintf(file1,"%f  ",m);
         fprintf(file2,"%f  ",1.96*ecart/(sqrt(Draws)));
         m=0.0;
         ecart=0.0;
        }
    fprintf(file1,"\n");
    fprintf(file2,"\n");

	fclose(file1);
	fclose(file2);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	strcpy(filename1,nom_fic);
	strcat(filename1,"_fairness.txt");

	strcpy(filename2,nom_fic);
	strcat(filename2,"_fairness_err.txt");

	file1=fopen(filename1,"a");
	file2=fopen(filename2,"a");

	ecart=0.0;
	m=0.0;

        {for (int i=0;i<Draws;i++)
            m=m+(double)fairness[i];
         m=m/(double)Draws;
         for(int i=0;i<Draws;i++) ecart=ecart+(double)((double)fairness[i]-m)*((double)fairness[i]-m);
         ecart=sqrt(ecart/(double)(Draws));
         fprintf(file1,"%f  ",m);
         fprintf(file2,"%f  ",1.96*ecart/(sqrt(Draws)));
         m=0.0;
         ecart=0.0;
        }

    fclose(file1);
	fclose(file2);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	strcpy(filename1,nom_fic);
	strcat(filename1,"_samplecomplexity.txt");

	strcpy(filename2,nom_fic);
	strcat(filename2,"_samplecomplexity_err.txt");

	file1=fopen(filename1,"a");
	file2=fopen(filename2,"a");

	ecart=0.0;
	m=0.0;

        {for (int i=0;i<Draws;i++)
            m=m+(double)sampleComplexity[i];
         m=m/(double)Draws;
         for(int i=0;i<Draws;i++) ecart=ecart+(double)((double)sampleComplexity[i]-m)*((double)sampleComplexity[i]-m);
         ecart=sqrt(ecart/(double)(Draws));
         fprintf(file1,"%f  ",m);
         fprintf(file2,"%f  ",1.96*ecart/(sqrt(Draws)));
         m=0.0;
         ecart=0.0;
        }

    fclose(file1);
	fclose(file2);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	strcpy(filename1,nom_fic);
	strcat(filename1,"_exploreDuration.txt");

	strcpy(filename2,nom_fic);
	strcat(filename2,"_exploreDuration_err.txt");

	file1=fopen(filename1,"a");
	file2=fopen(filename2,"a");

	ecart=0.0;
	m=0.0;

        {for (int i=0;i<Draws;i++)
            m=m+(double)exploreTime[i];
         m=m/(double)Draws;
         for(int i=0;i<Draws;i++) ecart=ecart+(double)((double)exploreTime[i]-m)*((double)exploreTime[i]-m);
         ecart=sqrt(ecart/(double)(Draws));
         fprintf(file1,"%f  ",m);
         fprintf(file2,"%f  ",1.96*ecart/(sqrt(Draws)));
         m=0.0;
         ecart=0.0;
        }

    fclose(file1);
	fclose(file2);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	strcpy(filename1,nom_fic);
	strcat(filename1,"_pdr.txt");

	strcpy(filename2,nom_fic);
	strcat(filename2,"_pdr_err.txt");

	file1=fopen(filename1,"a");
	file2=fopen(filename2,"a");

	ecart=0.0;
	m=0.0;

        {for (int i=0;i<Draws;i++)
            m=m+(double)total_success[i]/total_plays[i];
         m=m/(double)Draws;
         for(int i=0;i<Draws;i++) ecart=ecart+(double)((double)total_success[i]/total_plays[i]-m)*((double)total_success[i]/total_plays[i]-m);
         ecart=sqrt(ecart/(double)(Draws));
         fprintf(file1,"%f  ",m);
         fprintf(file2,"%f  ",1.96*ecart/(sqrt(Draws)));
         m=0.0;
         ecart=0.0;
        }

    fclose(file1);
	fclose(file2);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	strcpy(filename1,nom_fic);
	strcat(filename1,"_commCost.txt");

	strcpy(filename2,nom_fic);
	strcat(filename2,"_commCost_err.txt");

	file1=fopen(filename1,"a");
	file2=fopen(filename2,"a");

	ecart=0.0;
	m=0.0;

        {for (int i=0;i<Draws;i++)
            m=m+(double)commCost[i];
         m=m/(double)Draws;
         for(int i=0;i<Draws;i++) ecart=ecart+(double)((double)commCost[i]-m)*((double)commCost[i]-m);
         ecart=sqrt(ecart/(double)(Draws));
         fprintf(file1,"%f  ",m);
         fprintf(file2,"%f  ",1.96*ecart/(sqrt(Draws)));
         m=0.0;
         ecart=0.0;
        }

    fclose(file1);
	fclose(file2);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	strcpy(filename1,nom_fic);
	strcat(filename1,"_internalCol.txt");

	strcpy(filename2,nom_fic);
	strcat(filename2,"_internalCol_err.txt");

	file1=fopen(filename1,"a");
	file2=fopen(filename2,"a");

	ecart=0.0;
	m=0.0;

        {for (int i=0;i<Draws;i++)
            m=m+(double)intrnalColl[i]/total_plays[i];
         m=m/(double)Draws;
         for(int i=0;i<Draws;i++) ecart=ecart+(double)((double)intrnalColl[i]/total_plays[i]-m)*((double)intrnalColl[i]/total_plays[i]-m);
         ecart=sqrt(ecart/(double)(Draws));
         fprintf(file1,"%f  ",m);
         fprintf(file2,"%f  ",1.96*ecart/(sqrt(Draws)));
         m=0.0;
         ecart=0.0;
        }

    fclose(file1);
	fclose(file2);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	strcpy(filename1,nom_fic);
	strcat(filename1,"_extrnalCol.txt");

	strcpy(filename2,nom_fic);
	strcat(filename2,"_externalCol_err.txt");

	file1=fopen(filename1,"a");
	file2=fopen(filename2,"a");

	ecart=0.0;
	m=0.0;

        {for (int i=0;i<Draws;i++)
            m=m+(double)externalColl[i]/total_plays[i];
         m=m/(double)Draws;
         for(int i=0;i<Draws;i++) ecart=ecart+(double)((double)externalColl[i]/total_plays[i]-m)*((double)externalColl[i]/total_plays[i]-m);
         ecart=sqrt(ecart/(double)(Draws));
         fprintf(file1,"%f  ",m);
         fprintf(file2,"%f  ",1.96*ecart/(sqrt(Draws)));
         m=0.0;
         ecart=0.0;
        }

    fclose(file1);
	fclose(file2);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	strcpy(filename1,nom_fic);
	strcat(filename1,"_loss.txt");

	strcpy(filename2,nom_fic);
	strcat(filename2,"_loss_err.txt");

	file1=fopen(filename1,"a");
	file2=fopen(filename2,"a");

	ecart=0.0;
	m=0.0;

        {for (int i=0;i<Draws;i++)
            m=m+(double)loss[i]/total_plays[i];
         m=m/(double)Draws;
         for(int i=0;i<Draws;i++) ecart=ecart+(double)((double)loss[i]/total_plays[i]-m)*((double)loss[i]/total_plays[i]-m);
         ecart=sqrt(ecart/(double)(Draws));
         fprintf(file1,"%f  ",m);
         fprintf(file2,"%f  ",1.96*ecart/(sqrt(Draws)));
         m=0.0;
         ecart=0.0;
        }

    fclose(file1);
	fclose(file2);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	strcpy(filename1,nom_fic);
	strcat(filename1,"_energy.txt");

	strcpy(filename2,nom_fic);
	strcat(filename2,"_energy_err.txt");

	file1=fopen(filename1,"a");
	file2=fopen(filename2,"a");

	ecart=0.0;
	m=0.0;

        {for (int i=0;i<Draws;i++)
            m=m+(double)total_energy[i];
         m=m/(double)Draws;
         for(int i=0;i<Draws;i++) ecart=ecart+(double)((double)total_energy[i]-m)*((double)total_energy[i]-m);
         ecart=sqrt(ecart/(double)(Draws));
         fprintf(file1,"%f\t",m);
         fprintf(file2,"%f\t",1.96*ecart/(sqrt(Draws)));
         m=0.0;
         ecart=0.0;
        }

    fclose(file1);
	fclose(file2);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	strcpy(filename1,nom_fic);
	strcat(filename1,"_totalPlays.txt");

	strcpy(filename2,nom_fic);
	strcat(filename2,"_totalPlays_err.txt");

	file1=fopen(filename1,"a");
	file2=fopen(filename2,"a");

	ecart=0.0;
	m=0.0;

        {for (int i=0;i<Draws;i++)
            m=m+(double)total_plays[i];
         m=m/(double)Draws;
         for(int i=0;i<Draws;i++) ecart=ecart+(double)((double)total_plays[i]-m)*((double)total_plays[i]-m);
         ecart=sqrt(ecart/(double)(Draws));
         fprintf(file1,"%f  ",m);
         fprintf(file2,"%f  ",1.96*ecart/(sqrt(Draws)));
         m=0.0;
         ecart=0.0;
        }

    fclose(file1);
	fclose(file2);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	strcpy(filename1,nom_fic);
	strcat(filename1,"_totallyLost.txt");

	strcpy(filename2,nom_fic);
	strcat(filename2,"_totallyLost_err.txt");

	file1=fopen(filename1,"a");
	file2=fopen(filename2,"a");

	ecart=0.0;
	m=0.0;

        {for (int i=0;i<Draws;i++)
            m=m+(double)totally_lost[i];
         m=m/(double)Draws;
         for(int i=0;i<Draws;i++) ecart=ecart+(double)((double)totally_lost[i]-m)*((double)totally_lost[i]-m);
         ecart=sqrt(ecart/(double)(Draws));
         fprintf(file1,"%f\t",m);
         fprintf(file2,"%f\t",1.96*ecart/(sqrt(Draws)));
         m=0.0;
         ecart=0.0;
        }

    fclose(file1);
	fclose(file2);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	strcpy(filename1,nom_fic);
	strcat(filename1,"_nbLoss.txt");

	strcpy(filename2,nom_fic);
	strcat(filename2,"_nbLoss_err.txt");

	file1=fopen(filename1,"a");
	file2=fopen(filename2,"a");

	ecart=0.0;
	m=0.0;

        {for (int i=0;i<Draws;i++)
            m=m+(double)total_loss[i];
         m=m/(double)Draws;
         for(int i=0;i<Draws;i++) ecart=ecart+(double)((double)total_loss[i]-m)*((double)total_loss[i]-m);
         ecart=sqrt(ecart/(double)(Draws));
         fprintf(file1,"%f  ",m);
         fprintf(file2,"%f  ",1.96*ecart/(sqrt(Draws)));
         m=0.0;
         ecart=0.0;
        }

    fclose(file1);
	fclose(file2);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	strcpy(filename1,nom_fic);
	strcat(filename1,"_nbColl.txt");

	strcpy(filename2,nom_fic);
	strcat(filename2,"_nbColl_err.txt");

	file1=fopen(filename1,"a");
	file2=fopen(filename2,"a");

	ecart=0.0;
	m=0.0;

        {for (int i=0;i<Draws;i++)
            m=m+(double)total_collisions[i];
         m=m/(double)Draws;
         for(int i=0;i<Draws;i++) ecart=ecart+(double)((double)total_collisions[i]-m)*((double)total_collisions[i]-m);
         ecart=sqrt(ecart/(double)(Draws));
         fprintf(file1,"%f  ",m);
         fprintf(file2,"%f  ",1.96*ecart/(sqrt(Draws)));
         m=0.0;
         ecart=0.0;
        }

    fclose(file1);
	fclose(file2);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

    strcpy(filename1,nom_fic);
	strcat(filename1,"_ex_PDRperArm.txt");

	strcpy(filename2,nom_fic);
	strcat(filename2,"_ex_PDRperArm_err.txt");

	file1=fopen(filename1,"a");
	file2=fopen(filename2,"a");

    for(int k=0;k<K;k++)
        {for (int i=0;i<Draws;i++)
            {m=m+ex_rewardsPerArm[i][k]/ex_playsPerArm[i][k];}
         m=m/(double)Draws;
         for(int i=0;i<Draws;i++) ecart=ecart+(double)(ex_rewardsPerArm[i][k]/ex_playsPerArm[i][k]-m)*(ex_rewardsPerArm[i][k]/ex_playsPerArm[i][k]-m);
         ecart=sqrt(ecart/(double)(Draws));
         fprintf(file1,"%f  ",m);
         fprintf(file2,"%f  ",1.96*ecart/(sqrt(Draws)));
         m=0.0;
         ecart=0.0;
        }
    fprintf(file1,"\n");
    fprintf(file2,"\n");
	fclose(file1);
	fclose(file2);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	strcpy(filename1,nom_fic);
	strcat(filename1,"_playperArm.txt");

	strcpy(filename2,nom_fic);
	strcat(filename2,"_playperArm_err.txt");

	file1=fopen(filename1,"a");
	file2=fopen(filename2,"a");

    for(int k=0;k<K;k++)
        {for (int i=0;i<Draws;i++)
            {m=m+nbPlaysPerArm[i][k];}
         m=m/(double)Draws;
         for(int i=0;i<Draws;i++) ecart=ecart+(double)(nbPlaysPerArm[i][k]-m)*(nbPlaysPerArm[i][k]-m);
         ecart=sqrt(ecart/(double)(Draws));
         fprintf(file1,"%f\t",m);
         fprintf(file2,"%f\t",1.96*ecart/(sqrt(Draws)));
         m=0.0;
         ecart=0.0;
        }
    fprintf(file1,"\n");
    fprintf(file2,"\n");
	fclose(file1);
	fclose(file2);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
    strcpy(filename1,nom_fic);
	strcat(filename1,"_estimations.txt");

	strcpy(filename2,nom_fic);
	strcat(filename2,"_estimations_err.txt");

	file1=fopen(filename1,"a");
	file2=fopen(filename2,"a");

    for(int k=0;k<K;k++)
        {for (int i=0;i<Draws;i++)
            {m=m+finalEstimation[i][k];}
         m=m/(double)Draws;
         for(int i=0;i<Draws;i++) ecart=ecart+(double)(finalEstimation[i][k]-m)*(finalEstimation[i][k]-m);
         ecart=sqrt(ecart/(double)(Draws));
         fprintf(file1,"%f\t",m);
         fprintf(file2,"%f\t",1.96*ecart/(sqrt(Draws)));
         m=0.0;
         ecart=0.0;
        }
    fprintf(file1,"\n");
    fprintf(file2,"\n");
	fclose(file1);
	fclose(file2);

}

void statsPerdraw()
{
    FILE *f1;
    f1=fopen("res_losArms.txt","w");
    for(int i=0;i<5;i++)
        {for(int j=0;j<6;j++)
            fprintf(f1,"%d\t", losArms[j][i]);

        fprintf(f1,"\n" );
        }
    fclose(f1);
/////////////////////////////////////////////////////////////////////////////////

    f1=fopen("res_inter_colArms.txt","w");
    for(int i=0;i<5;i++)
        {for(int j=0;j<6;j++)
            fprintf(f1,"%d\t", inter_colArms[j][i]);

        fprintf(f1,"\n" );
        }
    fclose(f1);
////////////////////////////////////////////////////////////////////////////////

    f1=fopen("res_exter_colArms.txt","w");
    for(int i=0;i<5;i++)
        {for(int j=0;j<6;j++)
            fprintf(f1,"%d\t", exter_colArms[j][i]);

        fprintf(f1,"\n" );
        }
    fclose(f1);
//////////////////////////////////////////////////////////////////////////////////

    f1=fopen("res_armsPlays.txt","w");
    for(int i=0;i<5;i++)
        {for(int j=0;j<6;j++)
            fprintf(f1,"%d\t", arms[j][i]);

        fprintf(f1,"\n" );
        }
    fclose(f1);
//////////////////////////////////////////////////////////////////////////////////

    f1=fopen("res_avgRewards.txt","w");
    for(int i=0;i<5;i++)
        {for(int j=0;j<6;j++)
            fprintf(f1,"%f\t", rewardAvgs[j][i]);

        fprintf(f1,"\n" );
        }
    fclose(f1);
}

double getFainess()
{
    double min=2;
    double max=-1;

    for(int n=N1;n<N;n++)
    {
        if((double)node[n].nb_success/node[n].nb_plays < min) min=(double)node[n].nb_success/node[n].nb_plays;
        if((double)node[n].nb_success/node[n].nb_plays > max) max=(double)node[n].nb_success/node[n].nb_plays;
    }

    printf("fairness=%f\n",min/max);
    return min/max;
}
