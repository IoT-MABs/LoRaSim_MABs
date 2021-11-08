This project is a LoRa simulator with applied MAB algorithms.

To run the simulator, use the file "param.txt" to enter the simulation settings:

	- "Algorithm" corresponds to the algorithm used for LoRa communications, it can take the following values:
		-- 1: DORG preceded with the collaborative exploration algorithm
		-- 2: DOFG preceded with the collaborative exploration algorithm
		-- 3: CBAIMPB with LUCB followed by uniform policy for exploitaion
		-- 4: selfish UCB1
		-- 5: The classical ADR algorithm
		-- 6: The adversarial algorithm EXP3

	- "Problem" corresponds to the set of arms available, it can be "1" with the arms are the set of 7 spreading factors, 
  	or "2" with the arms are the set of 30 pairs (SF, transmitting power). Any other problems can be easily defines in 
  	"MAB.c"

	- "reward_function" corresponds to the reward function used in MAB algirothms. The available functions can be found 
  	in "MAB.c"

	- "alpha" is a parameter of the reward function

	- "K" is the number of arms 

	- "M" is the number of optimal arms we look for in CBAIMPB

	- "T" is the total number of packets to be sent during the simulation

	- "T1" is the number of time slots during which the external nodes send packets before the network starts 

	- "R" is the radius of the hexagone the nodes are located in

	- "TimeDisplay" is a parameter corresponding to display data on screen

	- "delta", "epsilon", and "eta" are exploration parameters

	- "gamma" is an EXP3 parameter 

	- "Draws" is the number of the times the experiment is run

	- "N" and "N1" are the number of internal and external nodes respectively

	- "background" corresponds to the policy the extrenal nodes are following referring to "MAB.c"

	- "retransmit" is the maximum number of possible retransmissions


After the simulation ends, text files will be created inclusing many performnace metrics such as: packet delivery rate,
energy consumption, total rewards, communication cost, sample complexity, etc..

 