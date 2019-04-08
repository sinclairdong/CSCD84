/*
	CSC D84 - Unit 3 - Reinforcement Learning
	
	This file contains stubs for implementing the Q-Learning method
	for reinforcement learning as discussed in lecture. You have to
	complete two versions of Q-Learning.
	
	* Standard Q-Learning, based on a full-state representation and
	  a large Q-Table
	* Feature based Q-Learning to handle problems too big to allow
	  for a full-state representation
	    
	Read the assignment handout carefully, then implement the
	required functions below. Sections where you have to add code
	are marked

	**************
	*** TO DO:
	**************

	If you add any helper functions, make sure you document them
	properly and indicate in the report.txt file what you added.
	
	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E. Jan. 16
*/

#include "QLearn.h"

struct Node{
	int x;
	int y;
	int cost;
	struct Node *next; 
};

const double SMALL_NUMBER = -99999;
const double LARGE_NUMBER = 99999;

//insert a node into a priority queue implemented using linkedlist
struct Node* insert_P(int x, int y, int cost, Node * head){

	struct Node* result = (struct Node *) malloc(sizeof(struct Node));
	result->x = x;
	result->y = y;
	result->cost = cost;
	result->next = NULL;

    if (head == NULL){
         head = result;
		 return result;
	}

	if (head->cost > result->cost){
		result->next = head;
		return result;
	}
	
	struct Node* temp = head;

	while(temp->next != NULL && result->cost > temp->next->cost){
		temp = temp->next;
	}
	result->next = temp->next;
	temp->next = result;

	return head;
}

// remove a node from the linked list and return the pointer to the next node
struct Node* remove(Node * head){
	struct Node* result = head->next;
	free(head);
	return result;	
}


void QLearn_update(int s, int a, double r, int s_new, double *QTable)
{
 /*
   This function implementes the Q-Learning update as stated in Lecture. It 
   receives as input a <s,a,r,s'> tuple, and updates the Q-table accordingly.
   
   Your work here is to calculate the required update for the Q-table entry
   for state s, and apply it to the Q-table
     
   The update involves two constants, alpha and lambda, which are defined in QLearn.h - you should not 
   have to change their values. Use them as they are.
     
   Details on how states are used for indexing into the QTable are shown
   below, in the comments for QLearn_action. Be sure to read those as well!
 */
 
  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
   int i = 0;
   int max_states_to_explore = 4;
   int max_reward_from_new_state = 0;
   for(int i = 0; i < max_states_to_explore; i++)
   {
   	 int current_state_value = *(QTable+(4*s_new)+i);
   	 if(current_state_value > max_reward_from_new_state)
   	 {
   	 	max_reward_from_new_state = current_state_value;
   	 }
   }

   *(QTable+(4*s)+a) += alpha * (r + lambda * (max_reward_from_new_state -  *(QTable+(4*s)+a)));
} 

int QLearn_action(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, double *QTable, int size_X, int graph_size)
{
  /*
     This function decides the action the mouse will take. It receives as inputs
     - The graph - so you can check for walls! The mouse must never move through walls
     - The mouse position
     - The cat position
     - The chees position
     - A 'pct' value in [0,1] indicating the amount of time the mouse uses the QTable to decide its action,
       for example, if pct=.25, then 25% of the time the mouse uses the QTable to choose its action,
       the remaining 75% of the time it chooses randomly among the available actions.
       
     Remember that the training process involves random exploration initially, but as training
     proceeds we use the QTable more and more, in order to improve our QTable values around promising
     actions.
     
     The value of pct is controlled by QLearn_core_GL, and increases with each round of training.
     
     This function *must return* an action index in [0,3] where
        0 - move up
        1 - move right
        2 - move down
        3 - move left
        
     QLearn_core_GL will print a warning if your action makes the mouse cross a wall, or if it makes
     the mouse leave the map - this should not happen. If you see a warning, fix the code in this
     function!
     
   The Q-table has been pre-allocated and initialized to 0. The Q-table has
   a size of
   
        graph_size^3 x 4
        
   This is because the table requires one entry for each possible state, and
   the state is comprised of the position of the mouse, cat, and cheese. 
   Since each of these agents can be in one of graph_size positions, all
   possible combinations yield graph_size^3 states.
   
   Now, for each state, the mouse has up to 4 possible moves (up, right,
   down, and left). We ignore here the fact that some moves are not possible
   from some states (due to walls) - it is up to the QLearn_action() function
   to make sure the mouse never crosses a wall. 
   
   So all in all, you have a big table.
        
   For example, on an 8x8 maze, the Q-table will have a size of
   
       64^3 x 4  entries
       
       with 
       
       size_X = 8		<--- size of one side of the maze
       graph_size = 64		<--- Total number of nodes in the graph
       
   Indexing within the Q-table works as follows:
   
     say the mouse is at   i,j
         the cat is at     k,l
         the cheese is at  m,n
         
     state = (i+(j*size_X)) + ((k+(l*size_X))*graph_size) + ((m+(n*size_X))*graph_size*graph_size)
     ** Make sure you undestand the state encoding above!
     
     Entries in the Q-table for this state are

     *(QTable+(4*state)+a)      <-- here a is the action in [0,3]
     
     (yes, it's a linear array, no shorcuts with brackets!)
     
     NOTE: There is only one cat and once cheese, so you only need to use cats[0][:] and cheeses[0][:]
   */
  
  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/  
   int moveIndex = -1;
  time_t t;

  // generates a number between 0 to 100
  int qPercentage = rand() % 100;

  int mouseX = mouse_pos[0][0];
  int mouseY = mouse_pos[0][1];

  int catX = cats[0][0];
  int catY = cats[0][1];
  
  int cheeseX = cheeses[0][0];
  int cheeseY = cheeses[0][1];

  int state = (mouseX + (mouseY * size_X)) + ((catX + (catY * size_X)) * graph_size) + ((cheeseX + (cheeseY * size_X)) * graph_size * graph_size);
  if(qPercentage < (pct * 100))
    { 
    //printf("Using Q table");
    int i = 0;
    int maxStatesToExplore = 4;
  	for(i = 0; i < maxStatesToExplore; i++)
   	{
      int graphIndex = mouseX + (mouseY * size_X);
        if(gr[graphIndex][i] == 1){
            if(moveIndex < 0){
                  moveIndex = i;
            }else{
                moveIndex = (*(QTable + (4 * state) + i) > *(QTable + (4 * state) + moveIndex)) ? i : moveIndex;
            }
        }
  	}
    
    // return the chosen move according to Q table.
  	return(moveIndex);
  }
  else
  {
     // get the graph index 
     int graphIndex =  mouseX + (mouseY * size_X);
     // choose a random direction.
     int randomMove = rand() % 4;

     while(gr[graphIndex][randomMove] == 0)
     {
     	randomMove = rand() % 4;
     }

     return(randomMove);
  }
}


int calculateManhattanDistance(int entity1[1][2] , int entity2[1][2])
{
	int Xdifference = abs(entity1[0][0] - entity2[0][1]);
	int Ydifference = abs(entity2[0][0] - entity2[0][1]);
	return (Xdifference + Ydifference);
}

double QLearn_reward(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
    This function computes and returns a reward for the state represented by the input mouse, cat, and
    cheese position. 
    
    You can make this function as simple or as complex as you like. But it should return positive values
    for states that are favorable to the mouse, and negative values for states that are bad for the 
    mouse.
    
    I am providing you with the graph, in case you want to do some processing on the maze in order to
    decide the reward. 
        
    This function should return a maximim/minimum reward when the mouse eats/gets eaten respectively.      
   */
    int distanceFromMouseToCat = (int) distance(gr, mouse_pos[0], cats[0], size_X);
    
    int mouseToCheese = (int) distance(gr, mouse_pos[0], cheeses[0], size_X);

    int catToCheese = (int) distance(gr, cats[0], cheeses[0], size_X);
    int mouse_index = mouse_pos[0][0] + (mouse_pos[0][1]*size_X);
    
    int movability = -10;
    int found_cat = 0;

    for (int i = 0; i < 4; i++){
        if (gr[mouse_index][i] == 1){
            movability +=2;
        }
        if ((mouseToCheese == 0) && (gr[mouse_index][i] == 1)){
            //printf("CHEESE\n"); 
            return LARGE_NUMBER;
        }

        if ((distanceFromMouseToCat == 0) && (gr[mouse_index][i] == 1)){
        //printf("CAT\n");
            found_cat = 1;
        }
    }

    //If Either one is right beside it, return a large value right away, favoring
    // cheese first
    if (found_cat == 1){
        return SMALL_NUMBER;
    } 
    

    //Otherwise calc some reward other wise
    double reward = (distanceFromMouseToCat - mouseToCheese)+ (catToCheese - mouseToCheese) + movability;

    
  return(reward);		// <--- of course, you will change this as well!     
}

void feat_QLearn_update(double gr[max_graph_size][4],double weights[25], double reward, int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
    This function performs the Q-learning adjustment to all the weights associated with your
    features. Unlike standard Q-learning, you don't receive a <s,a,r,s'> tuple, instead,
    you receive the current state (mouse, cats, and cheese potisions), and the reward 
    associated with this action (this is called immediately after the mouse makes a move,
    so implicit in this is the mouse having selected some action)
    
    Your code must then evaluate the update and apply it to the weights in the weight array.    
   */
  
   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/ 
   // printf("Update start\n");
	int maxA = -1;

	double features[25], qsa;
	double maxU = 0; 



	for (int i = 0; i < 25; i++) {
		features[i] = 0;
	}
    // printf("after for\n");
	maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, &maxU, &maxA);
	evaluateFeatures(gr, features, mouse_pos, cats, cheeses, size_X, graph_size);
	qsa = Qsa(weights, features);
		// printf("reward : %f\n", reward);
		// printf("maxu : %f\n", maxU);
		// printf("qsa : %f\n", qsa); 
	for (int n = 0; n < numFeatures; n++) {
		// weight update
      
		weights[n] += alpha*(reward + (lambda*(maxU)) - qsa)*(features[n]);
		// printf("weight%d: %f, feature:%f\n", n, weights[n], features[n]); 
	}
	// printf("update finish\n");
}

int feat_QLearn_action(double gr[max_graph_size][4],double weights[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, int size_X, int graph_size)
{
  /*
    Similar to its counterpart for standard Q-learning, this function returns the index of the next
    action to be taken by the mouse.
    
    Once more, the 'pct' value controls the percent of time that the function chooses an optimal
    action given the current policy.
    
    E.g. if 'pct' is .15, then 15% of the time the function uses the current weights and chooses
    the optimal action. The remaining 85% of the time, a random action is chosen.
    
    As before, the mouse must never select an action that causes it to walk through walls or leave
    the maze.    
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/        
    // printf("action start\n");
	// test the random
	int result;
	int random = rand() % 100; // get a random number between 0 -100 and compare with pct

	int maxA;
	double maxU;
	// int location = cord_to_number(mouse_pos[0][0], mouse_pos[0][1], size_X);
	int location = mouse_pos[0][0] + mouse_pos[0][1] * size_X;
	// printf("size_x : %d mosex: %d    %d\n", size_X, mouse_pos[0][0], mouse_pos[0][1]);
	// printf("location: %d\n", location); 
	if(random > pct * 100){
		// make a random move
		// printf("random\n");
		do{
			result = rand() % 4;
			// printf("random result: %d\n", result);
		}while(gr[location][result] == 0);
	}else{
	    // printf("not random\n");
		maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, &maxU, &maxA);
		// printf("no plz no, %d\n", maxA);
    	result = maxA;
    	// printf("%d\n", maxA);
    	// printf("OMFG\n");
    	// printf("result: %d\n", result);
	}
    // printf("action finish\n");

  return result;		// <--- replace this while you're at it!

}

void evaluateFeatures(double gr[max_graph_size][4],double features[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
   This function evaluates all the features you defined for the game configuration given by the input
   mouse, cats, and cheese positions. You are free to define up to 25 features. This function will
   evaluate each, and return all the feature values in the features[] array.
   
   Take some time to think about what features would be useful to have, the better your features, the
   smarter your mouse!
   
   Note that instead of passing down the number of cats and the number of cheese chunks (too many parms!)
   the arrays themselves will tell you what are valid cat/cheese locations.
   
   You can have up to 5 cats and up to 5 cheese chunks, and array entries for the remaining cats/cheese
   will have a value of -1 - check this when evaluating your features!
  */

   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/      
    // printf("eva start\n");
   	int i ;
   	
    for(i = 0; i < 25; i++){
        features[i] = 0;
    }
    
	total_closest_furthest_average_distance(&features[0], &features[1], &features[2], &features[3], gr, mouse_pos[0], cats, size_X);
	total_closest_furthest_average_distance(&features[4], &features[5], &features[6], &features[7], gr, mouse_pos[0], cheeses, size_X);

   		int location = mouse_pos[0][0] + mouse_pos[0][1] * size_X;
   	// the walls
   	for(i = 0; i < 4; i++){
   		features[8] += gr[location][i];
  	}
  	
  	features[8] = sigmoid(features[8]);
     // printf("eva finish\n");
}

double Qsa(double weights[25], double features[25])
{
  /*
    Compute and return the Qsa value given the input features and current weights
   */

  /***********************************************************************************************
  * TO DO: Complete this function
  ***********************************************************************************************/  
// printf("qsa start\n");
  double result = 0;
  
  for(int i = 0; i < numFeatures; i++){
  	result += weights[i] * features[i];
  }
 // printf("qsa finish\n");
  return result;	// <--- stub! compute and return the Qsa value

}

void maxQsa(double gr[max_graph_size][4],double weights[25],int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size, double *maxU, int *maxA)
{
 /*
   Given the state represented by the input positions for mouse, cats, and cheese, this function evaluates
   the Q-value at all possible neighbour states and returns the max. The maximum value is returned in maxU
   and the index of the action corresponding to this value is returned in maxA.
   
   You should make sure the function does not evaluate moves that would make the mouse walk through a
   wall. 
  */
 
   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/  
 // printf("Max start\n");
 		int location = mouse_pos[0][0] + mouse_pos[0][1] * size_X;
    
 	int temp_pos[1][2];// next move
 	double temp_Qsa; // store the qsa value
 	double features[25];
 	double maxU_temp = SMALL_NUMBER * 1000;
 	int maxA_temp = -1;

  	//check top
  	if(gr[location][0] == 1){
	 	temp_pos[0][0] = mouse_pos[0][0];
	 	temp_pos[0][1] = mouse_pos[0][1] - 1;
	 	
	 	evaluateFeatures(gr, features, temp_pos, cats, cheeses, size_X, graph_size);
	 	temp_Qsa = Qsa(weights, features);
	 	if(temp_Qsa > maxU_temp){
	 		maxU_temp = temp_Qsa;
	 		maxA_temp = 0;
	 	}
	}
    
	// check right
	if(gr[location][1] == 1){
	// printf("checking right\n");
	 	temp_pos[0][0] = mouse_pos[0][0] + 1;
	 	temp_pos[0][1] = mouse_pos[0][1];
	 	
	 	evaluateFeatures(gr, features, temp_pos, cats, cheeses, size_X, graph_size);
	 	temp_Qsa = Qsa(weights, features);
	 	if(temp_Qsa > maxU_temp){
	 		maxU_temp = temp_Qsa;
	 		maxA_temp = 1;
	 	}
	}

	// check down
	if(gr[location][2] == 1){
	// printf("checking down\n");
	 	temp_pos[0][0] = mouse_pos[0][0];
	 	temp_pos[0][1] = mouse_pos[0][1] + 1;
	 	
	 	evaluateFeatures(gr, features, temp_pos, cats, cheeses, size_X, graph_size);
	 	temp_Qsa = Qsa(weights, features);
	 	if(temp_Qsa > maxU_temp){
	 		maxU_temp = temp_Qsa;
	 		maxA_temp = 2;
	 	}
	}
 	

 		// check left
	if(gr[location][3] == 1){
	// printf("checking left\n");
	 	temp_pos[0][0] = mouse_pos[0][0] - 1;
	 	temp_pos[0][1] = mouse_pos[0][1];
	 	
	 	evaluateFeatures(gr, features, temp_pos, cats, cheeses, size_X, graph_size);
	 	temp_Qsa = Qsa(weights, features);
	 	if(temp_Qsa > maxU_temp){
	 		maxU_temp = temp_Qsa;
	 		maxA_temp = 3;
	 	}
	}
	*maxA = maxA_temp;
	*maxU = maxU_temp;
	if(maxA_temp == -1){
	    printf("wrong falg");
	}
     // printf("max finish %d  %f\n",*maxA,*maxU);
  // return;
   
}


/***************************************************************************************************
 *  Add any functions needed to compute your features below 
 *                 ---->  THIS BOX <-----
 * *************************************************************************************************/
// function turn a number to xy cord by the fomula given by the hand out
int cord_to_number(int x_cord, int y_cord, int size_X){
  return  x_cord + (y_cord * size_X);
}

double sigmoid(double x){

    return 1 / (1 + x);
}
void total_closest_furthest_average_distance(double *total, double *closest, double *furthest, double *average, double gr[max_graph_size][4], int locA[2], int locB[5][2], int size_X){
	int counter = 1;
	int temp = distance(gr, locA, locB[0], size_X);
	*total = temp;
	*closest = temp;
	*furthest = temp;
	while(locB[counter][0] != -1 && counter < 5){
		temp = distance(gr, locA, locB[counter], size_X);
		*total += temp;
		*closest = *closest < temp ? *closest : temp;
		*furthest = *furthest > temp ? *furthest : temp;
		counter++;
	}
	
	*average = *total / counter;
	
	*average = sigmoid(*average);
	*total = sigmoid(*total);
	*closest = sigmoid(*closest);
	*furthest = sigmoid(*furthest);
}

int distance(double gr[max_graph_size][4], int locA[2], int locB[2], int size_X)
{
	
	// return abs(locA[1] - locB[1]) + abs(locA[0] - locB[0]);
	return a_star(gr,locA,locB, size_X);
}


int a_star(double gr[max_graph_size][4], int locA[2], int locB[2], int size_X){
	// if on top of hceese stay where you are
	// initialize a queue in a form of a linklist
	struct Node* head;
	// add current location on queue
	head = insert_P(locA[0],locA[1], 0, NULL);

	// see how many nodes had been expanded
	int order = 0;
	// have somting to keep track of the predecessor and the final chees position
	// initialize the x value to be -1 to indicate that it hasn't been visited yet
	int history[max_graph_size][2];
/*			printf("before_for_loop  %d\n", cord_to_number(head->x,head->y));*/
/*			printf("x:%d      y:%d    \n",locA[0],locA[1]);*/
	history[cord_to_number(head->x,head->y, size_X)][1]=0;
	int goal[2];
	goal[0] = -1;
	goal[1] = -1;
	int i;
	
	
	for(i = 0; i < max_graph_size; i++){
		history[i][0] = -1;
	}
	
	// now keep looping untill we found the chess or the stack is empty
	while (head != NULL){
		// find the graph number converting from cord
		int graph_location = cord_to_number(head->x,head->y, size_X);
		
		// add 4 new node and I can't figure out a way to use a loop to do this
		// only add it when it has't been added before and it is a connection
		int new_node_loc;
		int cost;
		
		// top
		new_node_loc = cord_to_number(head->x,head->y-1, size_X);
		if((gr[graph_location][0] == 1) && (history[new_node_loc][0] == -1 )){
		
			history[new_node_loc][0] = graph_location;
			history[new_node_loc][1] = history[graph_location][1] + 1;
			cost = history[new_node_loc][1] + sqrt((locB[0] - head->x) * (locB[0] - head->x) + (locB[1] - head->y - 1) * (locB[1] - head->y - 1));
			head->next = insert_P(head->x,head->y-1,cost,head->next);
			
			if(locB[0] == head->x && locB[1] == head->y - 1){
				goal[0] = head->x;
				goal[1] = head->y-1;
				break;
			}
		}
		
		// right
		new_node_loc = cord_to_number(head->x+1,head->y, size_X);
		if((gr[graph_location][1] == 1) && (history[new_node_loc][0] == -1)){
			
			history[new_node_loc][0] = graph_location;
			history[new_node_loc][1] = history[graph_location][1] + 1;
			cost = history[new_node_loc][1] + sqrt((locB[0] - head->x + 1) * (locB[0] - head->x + 1) + (locB[1] - head->y) * (locB[1] - head->y));
			head->next = insert_P(head->x+1,head->y,cost, head->next);
			
			if(locB[0] == head->x+1 && locB[1] == head->y){
				goal[0] = head->x+1;
				goal[1] = head->y;
				break;
			}
		}	
		
		// bottom
		new_node_loc = cord_to_number(head->x,head->y+1, size_X);
		if((gr[graph_location][2] == 1) && (history[new_node_loc][0] == -1 )){
		
			history[new_node_loc][0] = graph_location;
			history[new_node_loc][1] = history[graph_location][1] + 1;
			cost = history[new_node_loc][1] + sqrt((locB[0] - head->x) * (locB[0] - head->x) + (locB[1] - head->y + 1) * (locB[1] - head->y + 1));
			head->next = insert_P(head->x,head->y+1,cost, head->next);

			if(locB[0] == head->x && locB[1] == head->y + 1){
				goal[0] = head->x;
				goal[1] = head->y+1;
				break;
			}
		}	
		
		// left
		new_node_loc = cord_to_number(head->x-1,head->y, size_X);
		if((gr[graph_location][3] == 1) && (history[new_node_loc][0] == -1)){
		
			history[new_node_loc][0] = graph_location;
			history[new_node_loc][1] = history[graph_location][1] + 1;
			head->next = insert_P(head->x-1,head->y,cost, head->next);
			cost = history[new_node_loc][1] + sqrt((locB[0] - head->x - 1) * (locB[0] - head->x - 1) + (locB[1] - head->y) * (locB[1] - head->y));

			if(locB[0] == head->x-1 && locB[1] == head->y){
				goal[0] = head->x-1;
				goal[1] = head->y;
				break;
			}
		}
		// remove the node from the queue ready for next iteration
		head = remove(head);
	}
	// printf("X:%dY:%d\n",goal[0],goal[1]);
	
	// free any node that left behind in the stack in case if there is an early exit
	struct Node* tmp;
	while (head != NULL)
    {
       tmp = head;
       head = head->next;
       free(tmp);
    }
    // to do- with the goal location use history to trace back all path then update the path then return
    // find the backward path first
    int reverse[max_graph_size];
    int initial_location = cord_to_number(locA[0],locA[1], size_X);
    int counter = 0;
    reverse[0] = cord_to_number(goal[0],goal[1], size_X);
    while(reverse[counter] != initial_location){
    	reverse[counter+1] = history[reverse[counter]][0];
    	counter++;
    }

	return counter;

}
