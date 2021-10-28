#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "type.h"
#define PI 3.14159265

/*****************************************_______________________ADR1__________________________***************************************/


//refines the power and sf depending on max sinr
/**finds the maximum SINR value among the last 20 transmissions of node n*/
float maxSINR(int n)
{
    float max=-9999;
    for(int i=0;i<20;i++)
       {if((lin2log(node[n].last_sinr[i])> max))
            {max=lin2log(node[n].last_sinr[i]);
            }
        }
    return max;
}

/**finds the sin margin in ADR */
float SINRmargin(int n)
{
    float sinr_max=maxSINR(n);
    float sinr_margin=sinr_max-margin_inc[node[n].ind_sf]-SNR_margin;
    return sinr_margin;
}



/**finds the new Tpower and SF in ADR algo*/
void ADR(int n)
{
    counter++;
    float sinr_margin=SINRmargin(n);
    int Nstep=floor(sinr_margin/3);
    int SFmin = 0;
    float PTmin=2.0;


    if (Nstep > 0)
        {while (Nstep > 0 && node[n].ind_sf > SFmin)
            {node[n].ind_sf = node[n].ind_sf-1;
            Nstep = Nstep-1;
            }
        while (Nstep > 0 && node[n].tx> PTmin)
           {node[n].tx = node[n].tx - 3.0;
           Nstep = Nstep - 1;
           }
        }
    else
        while (Nstep < 0 && node[n].tx < 14.0)
            {node[n].tx = node[n].tx + 3.0;
            Nstep = Nstep + 1;
            }

    if(node[n].indx==20)
        {node[n].indx=0;
         for(int i=0;i<20;i++)
             node[n].last_sinr[i]=-1;
         }

}
