#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "type.h"
#define PI 3.14159265


/*********_________________________LoRa__________________________****************/


/**returns the transmission duration
*/
double calcTimeonAir(int SF, int BW, int CR, int H, int n_preamble, int PL )
{
   SF=SF+7;
   double Tpacket;
   int DE;
    if (SF<=10)
        DE=0;
    else
        DE=1;

    double Tsym= pow(2,SF)/BW;
    double Tpreamble=(n_preamble+ 4.25)*Tsym;
    double payloadSymbNb;
    if(ceil(((8.0*PL)-(4.0*SF)+(28.0)+(0.0*16.0)-(20.0*H))/(4.0*(SF-(2.0*DE))))*(CR+4.0) > 0)
         payloadSymbNb= 8.0+ceil(((8.0*PL)-(4.0*SF)+(28.0)+(0.0*16.0)-(20.0*H))/(4.0*(SF-(2.0*DE))))*(CR+4.0);
    else
         payloadSymbNb= 8.0;
    double Tpayload=payloadSymbNb*Tsym;
    Tpacket=Tpreamble+Tpayload;
    Tpacket=Tpacket/1000.0;

 return Tpacket;
}

/**updates the emission time
*/
void updateEmissionTime(int n,int slot)
{
    float new_t;
    if(node[n].previous_emission_t<0.0)
        {node[n].previous_emission_t=(slot)*(float)time_slot_duration;
         node[n].current_emission_t=node[n].previous_emission_t;
         }
        else {
              do {new_t=(slot)*(float)time_slot_duration;
              if(new_t - node[n].previous_emission_t < d[node[n].ind_sf]) printf("error***********n=%d sf=%d, new_t=%f,previous_emission_t=%f \n",n,node[n].ind_sf,new_t , node[n].previous_emission_t);
                 } while(new_t - node[n].previous_emission_t < d[node[n].ind_sf]);

              node[n].previous_emission_t=node[n].current_emission_t;
              node[n].current_emission_t=new_t;

            }
}


/**returns one if the two nodes frames are overlapping*/
int overlappingTransmissions(int n1,int n2)
{
if(node[n1].current_emission_t+timeOnAir[node[n1].ind_sf] > node[n2].current_emission_t )
        return 1;
    return 0;
}

/**finds the path loss after each transmission without shadowing*/
float pathLoss(int n)
{
    float device_height=1.5; //end-device height
    float distance = sqrt(pow(node[n].x-bs[0].x,2)+pow(node[n].y-bs[0].y,2)+pow(bs[0].height-device_height,2));
    // HATA model for large and dense urban city
    float pathloss_denseurban = 69.55+26.16*log10(868)-13.82*log10(bs[0].height)+4.97-3.2*pow(log10(11.75*device_height),2)+(44.9-6.55*log10(bs[0].height))*log10(distance/1000);
    // HATA model for small and medium urban city
    float pathloss_suburban = 69.55+26.16*log10(868)-13.82*log10(bs[0].height)-0.8 - device_height*(1.1*log10(868)-0.7) + 1.56*log10(868) +(44.9-6.55*log10(bs[0].height))*log10(distance/1000);
    float pathloss_rural = pathloss_suburban - 4.78*pow(log10(868),2) + 18.33*log10(868) - 40.94; // HATA model for open areas
    float penetrationloss=15.0; //18 dB for dense urban, 15 dB for urban, 12 dB for suburban and 11 dB for rural areas
    float deepindoorloss=6.0; //6 dB deep indoor loss
    float antloss =5.0; //5 dBi antenna gain

    float dist_loss = pathloss_denseurban-antloss+penetrationloss+deepindoorloss;

    return dist_loss;
}



float mwTojoulePersec(float x)
{
    return x*0.001;
}

/** updates sinr of node n after each play*/
void updateSINR(int n)
{
    float sinr_den=log2lin(P.th_noise+P.NF);
    for(int n1=0;n1<N;n1++)
        if(n!=n1 && node[n1].isPlaying )
            sinr_den=sinr_den+node[n1].RSSI;
    node[n].SINR=node[n].RSSI/sinr_den;
}

/**finds if an intra-sf collision happen between nodes n1 and n2*/
int intraSF_collision(int n1, int n2)
{
    if(node[n1].ind_sf==node[n2].ind_sf)
        {if(absf(node[n1].RSSI-node[n2].RSSI) < C_sur_N_cible || node[n1].RSSI < node[n2].RSSI)
            return 1;//loss
        }
    return 0;
}

/**finds if an inter-sf collision happen between nodes n1 and n2*/
int interSF_collision(int n1, int n2)
{
    if(node[n1].ind_sf!=node[n2].ind_sf)
        if(node[n1].RSSI-node[n2].RSSI <= SINR_SF[node[n1].ind_sf] )
            return 1;//loss

    return 0;
}

/**return if the transmission of node n is successful or not*/
int Tr_success(int n)
{
    /**check for collisions*/
    for(int n1=0;n1<N;n1++)
       if (node[n1].RSSI >= log2lin(sensitivity_SF[node[n1].ind_sf]) && node[n].RSSI >= log2lin(sensitivity_SF[node[n].ind_sf]))
        if(n!=n1 && node[n1].isPlaying )
          {
              if((intraSF_collision(n,n1) || interSF_collision(n,n1) ) )
               {
                if(n>=N1 && n1>=N1)
                    {global_collisions++;
                    nColbyarm[node[n].arm]++;
                    inter_colArms[node[n].ind_sf][(int) node[n].tx/3]++;
                    }
                if(n>=N1 && n1<N1) exter_colArms[node[n].ind_sf][(int) node[n].tx/3]++;
                node[n].nb_collisions++;
                if(n>=N1 && n1>=N1)
                    {intrnalColl[dr]++;
                    }
                if(n>=N1 && n1<N1)
                    {externalColl[dr]++;
                    }
                return 0;
                }
              if(node[n].ind_sf==node[n1].ind_sf && node[n].tx==node[n1].tx ) {fprintf(testf,"sf= %d, tx=%f  \n",node[n1].ind_sf,node[n1].tx);
              }
          }

    if(node[n].RSSI >= log2lin(sensitivity_SF[node[n].ind_sf]))
        {node[n].nb_success++;
         return 1;
         }
    if(n>=N1)
        {global_loss++;
        nlossbyarm[node[n].arm]++;
        loss[dr]++;
        lossPerArm[node[n].arm]++;
        }
    node[n].nb_loss++;
    //printf("loss\n");
    return 0;
}

/**no one is playing at the end of the slot*/
void slotEnd()
{
    for(int n=0;n<N;n++)
        node[n].isPlaying=0;
}
