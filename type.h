#ifndef TYPE_H_
#define TYPE_H_

#endif /*TYPE_H_*/

//#define PI 3.141592653
#define e 2.718281828


//Base station structure
struct BS {
	int x;
	int y;
	int type;
	int connection;
	int azimuth;
	char *name;
	float pilot_power;
	float data_power;
	float height;

};

struct Param {
    //resolution
    float resolution;

    //channel characteristics
    float bandwidth; //kHz
    float th_noise; //thermal noise
    float NF; //Gateway noise figure dB
    float shadowing;

    //Uplink channel parameters
    float P0;
    float alpha;
};

struct Bandit {
	float mu; // average pdr
	float avg_reward; //average reward
	float stdd; //standard deviation
	int tk; // nombre de tirages du bras;
	double B; // Index UCB
	double beta;
	double U;
	double L;
	int notified;

	float UCB;
	float LCB;

	float var;
	float* reward;
};

struct node {
    struct Bandit *B;
	float pr;
	float p;

	float x;
	float y;

	float shadowing;
	float fading;
	float rayleigh;
	float epsilon;
	float eta;
	float delta;
	int k; // action courante du joueur
	int NbAction;
	int phase1;
	int stopCom;

    int ind_sf;
    int ch;
    float tx;
	float PL;
	float RSSI;
	float SINR;
	float SNR;
	float *last_sinr;
	int indx;
	int *nb_sf;
	int cons_coll;
	float energy;
	int init;
	int *A;
	int *nonOptimals;
	int *selected;
	int *discarded;
	int t;


    int estimationsSent;
    int finishEstimating;
    int *nbPlayArms;
    int T_k;

	float previous_emission_t;
	float current_emission_t;
	float end_emission_t;
	int nb_plays;
	int nb_pkts;
	int nb_success;
	int nb_collisions;
	int nb_loss;
	double sum_rewards;
	int isPlaying;
	int arm;


};

float* randn_nb;
struct BS *bs;
struct node *node;
struct Param P;
int netwDim;
float* timeOnAir;
float* frameduration;
float* DLframeduration;
float* sensitivity_SF;
float* SINR_SF;
int *nb_sf;
int *nb_tx;
int verbose; // =1 Active les affichages
int TimeDisplay; // Délai en itérations entre deux affichages
int Draws;	// Nombre de tirages
int K; // nombre d'actions
int Kch; //number of channels
int T; // Nombre max d'itérations
int T1;
int N; // Nombre de joueurs
int N1;
int M;
float p1;
float pN;
float col;
int explore;
int exploit;
float alpha;
int fool;
int backgroundN;
float delta; // probabilité totale de se tromper
float epsilon; // erreur d'approximation maximum
int Algorithm; //1 DME 0 ME
int Problem;
int reward_function;
int Kch; //number of channels
int counter;
float sum_Nstep;
float epsilon2;
float eta;
int retransmit;


int SNR_margin;
int R,i,time_slot_duration;
float C_sur_N_cible;
float* d;
float* margin_dec;
float* margin_inc;


int global_plays;
int global_success;
int global_loss;
double global_rewards;
int global_collisions;
double global_energy;
int *totalLossbyN;
float* energybyN;
int **arms;
float **rewardAvgs;
float** average_mu;
float* Rewards;
int *plays;
float** collRate;
int* loss;
float* muMin;
float* muMax;
float** playersMeans;
float* playersRewards;
int* M_best;
int* duration;

int dr;
double rw;
int nbPkts;
int *totally_lost;
int *total_sentPkts;
int *total_plays;
int *total_success;
int *total_collisions;
int *total_loss;
int *nPlaysbyarm;
int *nColbyarm;
int *nlossbyarm;
double *total_energy;
double **energyPerArm;
double *total_rewards;
double **pdrProgress;
double **total_pdrPerArm;
int *playsPerArm;
int **successPerArm;
int **lostPkts;
double *rewardPerArm;
int *lossPerArm;
double ***pdrProgressPerArm;
int **ex_playsPerArm;
double **ex_rewardsPerArm;
double **estimations;
double **finalEstimation;
int *sampleComplexity;
int *exploreTime;
int *commCost;
int *intrnalColl;
int *loss;
int *externalColl;
int **nbPlaysPerArm;
double *arm;
float *realAvg;
float sinrsum;
int **inter_colArms;
int **exter_colArms;
int **losArms;

int **Lambda; // Lambda[n*NbBin][k]=1 lorsque le joueur n*NbBin a demandé la suppression de l'action k
int **omiga;
float **Logs; // Regret par temps
int *SampleC; // Complexité d'échantillonnage
int *collisions;
int *plays;
int* nb_plays;
float* global_mu;
int *Action; // Tableau des actions
int *globalSelected;
int *globalDiscarded;
float *Arm;
float *mu;
int *phase1;
int **armsPlayed;

float** wEXP;
float** pEXP;
float gamma;

double *fairness;

char filename1[256], filename2[256],filename3[256],filename4[256];
FILE *file1;
FILE *file2;
FILE *file3;
FILE *file4;
FILE *testf;
FILE *interF;
FILE *intraF;
FILE *intraF2;
FILE *lossF;
FILE *timeF;
FILE *pdrPerarmF;

int slot;
/********************________________Functions________________*************************/

void Initialise(char *filename1,char *filename2);
void initParam();
void BSread(char *nom_fichier);
void initialize_problem();
void alloc();
void initNodes();
void initializeActiveRates();
void InitBandit(struct Bandit *B);
int InitNetwork (char *filename);
int locateNodes();
void init();
void run();
void perc_sf();
double getFainess();

float RAND_R(float x);
float log2lin(float login);
float lin2log(float linin);
float randn();
float* sort(float* array, int size);
float absf(float x);
int count(int* vector,int element, int size);
int* find(int* vector,int element, int size);
int count_(int* vector,int element, int size);
int* find_(int* vector,int element, int size);
float pathLoss(int n);
void slotEnd();

void updateEmissionTime(int n,int slot);
/*****************_________________Algos__________________******************/
double calcTimeonAir(int SF, int BW, int CR, int H, int n_preamble, int PL );
float SINRmargin(int n);
float maxSINR(int n);
float avgSINR(int n);
void ADR1(int n);
void ADR2(int n);
void background();
void dorg();
void dofg();
void exploitGreedy();
void exploitUCB();
void exploitADR2();
void exploreArms();

void Alg1();
void Alg2();
void Alg3();
void Alg31();
void Alg32();
void Alg4();
void Alg5();
void Alg6();
void Alg7();

//UCB1
int allArmsPlayedOnce(int n);
int play_roundRobin(int n);
int playUniform(int K_arms);
int recommendArm_UCB (int n);
void updateUCB1 (int n);

/*****************________________FUNCTIONS________________*******************/
int interSF_collision(int n1, int n2);
int intraSF_collision(int n1, int n2);
int Tr_success(int n);
void updateSINR(int n);
void playArm(int n, int k);
int selectArm(int n);

//cost
float mwTojoulePersec(float x);
float energy(int n);
float energy_N(int n, int nbRetransm);
float cost(int n, int fail, int nbRetransm);
float cost_N(int n, int fail, int nbRetransm);
double getReward(int n, int fail, int nbRetransm);
float energyRew_N(int n, int nbRetransm);

void collectStats();
void statsPerdraw();

int slowPl();
int nodeFinishEstimating(int n);
int enoughEstimations();
int finishEstimating();
int testFinishRequiredNb();
float collisionRate(int n);
void findFinalEstimation();
void initialize_T_k();
void find_epsilon2();
int* sortNodes();

/********************LUCB******************************/

float beta(int t, int n , int k);
float confInt(int t, int n, int k);
float confLvl(int t, int n, int k);
int allArmsPlayed(int **armsPlayed,int n);
float * sort2Arrays( float *max,int* maxArms,int M);
int* findOptimals(int n);
int* findNonOptimals(int n);
float findBestNonOptimal(int n);
float findWorstOptimal(int n);
int stop(int n);
int stop_reset(int n);
int RoundRobinn(int k, int **A, int n);
int chooseNotPlayedArm(int **A, int n);
void LUCB();
int Card(int * AD);
int* RemainingActions(int * AD);
int RoundRobin(int k, int *A);
int RandomChoice(int *A, int n);
int played(int k);
void InitAction();
void InitPlayers();
int sameVector(int *A, int *B, int n);
void InitServer();
void CallServer();
void ServerCall();
int * sortArr( float *max);
int isOftheBest(int n, float* max, int index, int size);
int allPlayersFinish();
void findM_best();
int playM_bestUniformly();

/*************************exp3************************/
void p_exp3(int n);
int drawArm_exp3(int n);
void update_wEXP(int n ,float y);
void exploitEXP3();
