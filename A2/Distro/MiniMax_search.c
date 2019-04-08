/*
	CSC D84 - Unit 2 - MiniMax search and adversarial games

	This file contains stubs for implementing a MiniMax search
        procedure with alpha-beta pruning. Please read the assignment
	handout carefully - it describes the game, the data you will
	have to handle, and the search functions you must provide.

	Once you have read the handout carefully, implement your search
	code in the sections below marked with

	**************
	*** TO DO:
	**************

	Make sure to add it to your report.txt file - it will be marked!

	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E. Sep. 15
*/

#include "MiniMax_search.h"
 /*
   This function is the interface between your solution for the assignment and the driver code. The driver code
   in MiniMax_search_core_GL will call this function once per frame, and provide the following data
   
   Board and game layout:

	Exactly the same as for Assignment 1 - have a look at your code if you need a reminder of how the adjacency
	list and agent positions are stored.	

	Note that in this case, the path will contain a single move - at the top level, this function will provide
	an agent with the 'optimal' mini-max move given the game state.


   IMPORTANT NOTE: Mini-max is a recursive procedure. This function will need to fill-in the mini-max values for 
	 	   all game states down to the maximum search depth specified by the user. In order to do that,
		   the function needs to be called with the correct state at each specific node in the mini-max
		   search tree.

		   The game state is composed of:

			* Mouse, cat, and cheese positions (and number of cats and cheeses)
			
		   At the top level (when this function is called by the mini-max driver code), the game state
		   correspond to the current situation of the game. But once you start recursively calling
		   this function for lower levels of the search tree the positions of agents will have changed.
		   
		   Therefore, you will need to define local variables to keep the game state at each node of the
		   mini-max search tree, and you will need to update this state when calling recursively so that
		   the search does the right thing.

		   This function *must check* whether:
			* A candidate move results in a terminal configuration (cat eats mouse, mouse eats cheese)
			  at which point it calls the utility function to get a value
	 		* Maximum search depth has been reached (depth==maxDepth), at which point it will also call
			  the utility function to get a value
			* Otherwise, call recursively using the candidate configuration to find out what happens
			  deeper into the mini-max tree.

   Arguments:
		gr[graph_size][4]   		- This is an adjacency list for the maze
		path[1][2] 			- Your function will return the optimal mini-max move in this array.
		minmax_cost[size_X][size_Y]	- An array in which your code will store the
						  minimax value for maze locations expanded by
						  the search *when called for the mouse, not
						  for the cats!*

						  This array will be used to provide a visual 
						  display of minimax values during the game.

		cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
					 but there can be fewer). Only valid cat locations are 0 to (cats-1)
		cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
					     but possibly fewer). Valid locations are 0 to (cheeses-1)
		mouse_loc[1][2] - Mouse location - there can be only one!
		mode - Search mode selection:
					mode = 0 	- No alpha-beta pruning
					mode = 1	- Alpha-beta pruning

		(*utility)(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4]);
				- This is a pointer to the utility function which returns a value for a specific game configuration

				   NOTE: Unlike the search assignment, this utility function also gets access to the graph so you can do any processing
				   		 that requires knowledge of the maze for computing the utility values.

				  * How to call the utility function from within this function : *
					- Like any other function:
						u = utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
						
		agentId: Identifies which agent we are doing MiniMax for. agentId=0 for the mouse, agentId in [1, cats] for cats. Notice that recursive calls
                         to this function should increase the agentId to reflect the fact that the next level down corresponds to the next agent! For a game
                         with two cats and a mouse, the agentIds for the recursion should look like 0, 1, 2, 0, 1, 2, ...
	
		depth: Current search depth - whether this is a MIN or a MAX node depends both on depth and agentId.
		
		maxDepth: maximum desired search depth - once reached, your code should somehow return
			  a minimax utility value for this location.

		alpha. beta: alpha and beta values passed from the parent node to constrain search at this
			     level.

   Return values:
		Your search code will directly update data passed-in as arguments:
		
		- Mini-Max value	: Notice this function returns a double precision number. This is
					  the minimax value at this level of the tree. It will be used 
					  as the recursion backtracks filling-in the mini-max values back
					  from the leaves to the root of the search tree. 

		- path[1][2]		: Your MiniMax function will return the location for the agent's 
					  next location (i.e. the optimal move for the agent). 
		- minmax_cost[size_X][size_Y] 	:  Your search code will update this array to contain the
						   minimax value for locations that were expanded during
						   the search. This must be done *only* for the mouse.

						   Values in this array will be in the range returned by
						   your utility function.

		* Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
	     	  or the location of the mouse - if you try, the driver code will know it *
			
		That's that, now, implement your solution!
 */

 /********************************************************************************************************
 * 
 * TO DO:	Implement code to perform a MiniMax search. This will involve a limited-depth BFS-like
 *              expansion. Once nodes below return values, your function will propagate minimax utilities
 *		as per the minimax algorithm.
 *	
 *		Note that if alpha-beta pruning is specified, you must keep track of alphas and betas
 *		along the path.
 *
 *		You can use helper functions if it seems reasonable. Add them to the MiniMax_search.h
 *		file and explain in your code why they are needed and how they are used.
 *
 *		Recursion should appear somewhere.
 *
 *		MiniMax cost: If the agentId=0 (Mouse), then once you have a MiniMax value for a location
 *		in the maze, you must update minmax_cost[][] for that location.
 *
 *		How you design your solution is up to you. But:
 *
 *		- Document your implementation by adding concise and clear comments in this file
 *		- Document your design (how you implemented the solution, and why) in the report
 *
 ********************************************************************************************************/
const double LARGE_NUMBER = 50000;

struct Node{
	int x;
	int y;
	int cost;
	struct Node *next; 
};
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



// function turn a number to xy cord by the fomula given by the hand out
int cord_to_number(int x_cord, int y_cord){
	return y_cord * size_X + x_cord;
}

double MiniMax(	
	double gr[graph_size][4], 
	int path[1][2], 
	double minmax_cost[size_X][size_Y], 
	int cat_loc[10][2],
	int cats, 
	int cheese_loc[10][2], 
	int cheeses, 
	int mouse_loc[1][2], 
	int mode, 
	double (*utility)(
		int cat_loc[10][2], 
		int cheese_loc[10][2], 
		int mouse_loc[1][2], 
		int cats, 
		int cheeses, 
		int depth, 
		double gr[graph_size][4]), 
	int agentId, 
	int depth, 
	int maxDepth, 
	double alpha, 
	double beta){
	

	// Stub so that the code compiles/runs - This will be removed and replaced by your code!
	
	// determine if this node is terminal if terminal return a verylarge number
	// WARNING!!!!! I modified the checkForTerminal function for better utility
	int terminality = checkForTerminal(mouse_loc, cat_loc, cheese_loc, cats, cheeses);
	
	if(terminality != 0){
		minmax_cost[mouse_loc[0][0]][mouse_loc[0][1]] = terminality * LARGE_NUMBER;
		return minmax_cost[mouse_loc[0][0]][mouse_loc[0][1]];
	}
	
	//check if we are at the leaf node of the search tree if at leaf node call utility
	if(depth == maxDepth){
		minmax_cost[mouse_loc[0][0]][mouse_loc[0][1]] = utility(cat_loc,cheese_loc,mouse_loc,cats,cheeses,depth,gr);
		return minmax_cost[mouse_loc[0][0]][mouse_loc[0][1]];
	}
	
	// otherwise recurse and find the max or min
	
	// set the path0 to current cell
	
	int pruning_flag = 0;
	int new_path[1][2];
	double value;
	// determine if this is a max node or min node
	if(agentId == 0){
		
		value = -1 * LARGE_NUMBER;
		// this is a max node the node is trying to look for maximum value
		// there are four things we can try here up down left right
		int graphlocation = cord_to_number(mouse_loc[0][0],mouse_loc[0][1]);

		// top
		// if we do not have a wall and we are not pruning
		if((gr[graphlocation][0] == 1) && (pruning_flag == 0)){
			int new_mouse_loc[1][2];
			new_mouse_loc[0][0] = mouse_loc[0][0]; 
			new_mouse_loc[0][1] = mouse_loc[0][1] - 1; 
			double new_value = MiniMax(gr, new_path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, new_mouse_loc,mode, utility, agentId + 1, depth+1, maxDepth,alpha,beta);
			if(new_value > value){
				value = new_value;
				path[0][0] = new_mouse_loc[0][0];
				path[0][1] = new_mouse_loc[0][1];
			}
			if(new_value > alpha){
				alpha = new_value;
			}
			//check if it is higher than beta
			// recall beta is the best solution for minimizer above thus if this value is greater than the best solution minimizer will choose the other one
			if(mode == 1){
				pruning_flag = value > beta ? 1 : 0;
			}
		}

		// right
		if((gr[graphlocation][1] == 1) && (pruning_flag == 0)){
			int new_mouse_loc[1][2];
			new_mouse_loc[0][0] = mouse_loc[0][0] + 1; 
			new_mouse_loc[0][1] = mouse_loc[0][1]; 
			double new_value = MiniMax(gr, new_path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, new_mouse_loc,mode, utility, agentId + 1, depth+1, maxDepth,alpha,beta);
			if(new_value > value){
				value = new_value;
				path[0][0] = new_mouse_loc[0][0];
				path[0][1] = new_mouse_loc[0][1];
			}
			if(new_value > alpha){
				alpha = new_value;
			}
			
			
			//check if it is higher than beta
			// recall beta is the best solution for minimizer above thus if this value is greater than the best solution minimizer will choose the other one
			if(mode == 1){
				pruning_flag = value > beta ? 1 : 0;
			}
		}

		
		// bottom
		if((gr[graphlocation][2] == 1) && (pruning_flag == 0)){
			int new_mouse_loc[1][2];
			new_mouse_loc[0][0] = mouse_loc[0][0]; 
			new_mouse_loc[0][1] = mouse_loc[0][1] + 1; 
			double new_value = MiniMax(gr, new_path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, new_mouse_loc,mode, utility, agentId + 1, depth+1, maxDepth,alpha,beta);
			if(new_value > value){
				value = new_value;
				path[0][0] = new_mouse_loc[0][0];
				path[0][1] = new_mouse_loc[0][1];
			}
			if(new_value > alpha){
				alpha = new_value;
			}
			
			//check if it is higher than beta
			// recall beta is the best solution for minimizer above thus if this value is greater than the best solution minimizer will choose the other one
			if(mode == 1){
				pruning_flag = value > beta ? 1 : 0;
			}
		}

		if((gr[graphlocation][3] == 1) && (pruning_flag == 0)){
			int new_mouse_loc[1][2];
			new_mouse_loc[0][0] = mouse_loc[0][0] - 1; 
			new_mouse_loc[0][1] = mouse_loc[0][1]; 
			double new_value = MiniMax(gr, new_path, minmax_cost, cat_loc, cats, cheese_loc, cheeses, new_mouse_loc,mode, utility, agentId + 1, depth+1, maxDepth,alpha,beta);
			if(new_value > value){
				value = new_value;
				path[0][0] = new_mouse_loc[0][0];
				path[0][1] = new_mouse_loc[0][1];
			}
			if(new_value > alpha){
				alpha = new_value;
			}
			
			//check if it is higher than beta
			// recall beta is the best solution for minimizer above thus if this value is greater than the best solution minimizer will choose the other one
			if(mode == 1){
				pruning_flag = value > beta ? 1 : 0;
			}
		}
/*		printf("before: x:%d    y:%d\n", path[0][0], path[0][1]);*/
/*		printf("after: x:%d    y:%d\n", path[0][0], path[0][1]);*/
		
	}else{
		//check which agent are we on and whitch depth the next level should be looking at
		int new_agent_id = (agentId + 1) % (cats+1);
		int new_depth = agentId == cats ? depth : depth +1;
		int graphlocation = cord_to_number(cat_loc[agentId - 1][0],cat_loc[agentId - 1][1]);
		value = LARGE_NUMBER;
		
		// not pruning and there is not a wal
		// top

		if((gr[graphlocation][0] == 1) && (pruning_flag == 0)){
		

			int new_cat_loc[cats][2];
			int i;
			for(i = 0; i < cats; i++){
				// make a new cat locations
				if(i == agentId - 1){
					new_cat_loc[i][0] = cat_loc[i][0];
					new_cat_loc[i][1] = cat_loc[i][1] - 1;
				}else{
					new_cat_loc[i][0] = cat_loc[i][0];
					new_cat_loc[i][1] = cat_loc[i][1];
				}
			}
			
			double new_value = MiniMax(gr, new_path, minmax_cost, new_cat_loc, cats, cheese_loc, cheeses, mouse_loc,mode, utility, new_agent_id, new_depth, maxDepth,alpha,beta);
			
			if(new_value < value){
				value = new_value;
			}
			if(new_value < beta){
				beta = new_value;
			}
			
			//check if it is higher than beta
			// recall beta is the best solution for minimizer above thus if this value is greater than the best solution minimizer will choose the other one
			if(mode == 1){
				pruning_flag = value < alpha ? 1 : 0;
			}
			
		} 
		
		

		// right
		if((gr[graphlocation][1] == 1) && (pruning_flag == 0)){
		

			int new_cat_loc[cats][2];
			int i;
			for(i = 0; i < cats; i++){
				// make a new cat locations
				if(i == agentId - 1){
					new_cat_loc[i][0] = cat_loc[i][0] + 1;
					new_cat_loc[i][1] = cat_loc[i][1];
				}else{
					new_cat_loc[i][0] = cat_loc[i][0];
					new_cat_loc[i][1] = cat_loc[i][1];
				}
			}
			
			double new_value = MiniMax(gr, new_path, minmax_cost, new_cat_loc, cats, cheese_loc, cheeses, mouse_loc,mode, utility, new_agent_id, new_depth, maxDepth,alpha,beta);
			
			if(new_value < value){
				value = new_value;
			}
			if(new_value < beta){
				beta = new_value;
			}
			
			//check if it is higher than beta
			// recall beta is the best solution for minimizer above thus if this value is greater than the best solution minimizer will choose the other one
			if(mode == 1){
				pruning_flag = value < alpha ? 1 : 0;
			}
			
		} 
		

		// bottom
		if((gr[graphlocation][2] == 1) && (pruning_flag == 0)){
		
		
			int new_cat_loc[cats][2];
			int i;
			for(i = 0; i < cats; i++){
				// make a new cat locations
				if(i == agentId - 1){
					new_cat_loc[i][0] = cat_loc[i][0];
					new_cat_loc[i][1] = cat_loc[i][1] + 1;
				}else{
					new_cat_loc[i][0] = cat_loc[i][0];
					new_cat_loc[i][1] = cat_loc[i][1];
				}
			}
			

			double new_value = MiniMax(gr, new_path, minmax_cost, new_cat_loc, cats, cheese_loc, cheeses, mouse_loc,mode, utility, new_agent_id, new_depth, maxDepth,alpha,beta);
			
			if(new_value < value){
				value = new_value;
			}
			if(new_value < beta){
				beta = new_value;
			}
			
			//check if it is higher than beta
			// recall beta is the best solution for minimizer above thus if this value is greater than the best solution minimizer will choose the other one
			if(mode == 1){
				pruning_flag = value < alpha ? 1 : 0;
			}
			
		} 

		// left
		if((gr[graphlocation][3] == 1) && (pruning_flag == 0)){
		

			int new_cat_loc[cats][2];
			int i;
			for(i = 0; i < cats; i++){
				// make a new cat locations
				if(i == agentId - 1){
					new_cat_loc[i][0] = cat_loc[i][0] - 1;
					new_cat_loc[i][1] = cat_loc[i][1];
				}else{
					new_cat_loc[i][0] = cat_loc[i][0];
					new_cat_loc[i][1] = cat_loc[i][1];
				}
			}
			
			double new_value = MiniMax(gr, new_path, minmax_cost, new_cat_loc, cats, cheese_loc, cheeses, mouse_loc,mode, utility, new_agent_id, new_depth, maxDepth,alpha,beta);
			
			if(new_value < value){
				value = new_value;
			}
			if(new_value < beta){
				alpha = new_value;
			}
			
			//check if it is higher than beta
			// recall beta is the best solution for minimizer above thus if this value is greater than the best solution minimizer will choose the other one
			if(mode == 1){
				pruning_flag = value < alpha ? 1 : 0;
			}
			
		} 
	
	}
	minmax_cost[mouse_loc[0][0]][mouse_loc[0][1]] = value;
	// printf("i am here at return\n");
 	return value;
}


double utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, int depth, double gr[graph_size][4])
{
 /*
	This function computes and returns the utility value for a given game configuration.
	As discussed in lecture, this should return a positive value for configurations that are 'good'
	for the mouse, and a negative value for locations that are 'bad' for the mouse.

	How to define 'good' and 'bad' is up to you. Note that you can write a utility function
	that favours your mouse or favours the cats, but that would be a bad idea... (why?)

	Input arguments:

		cat_loc - Cat locations
		cheese_loc - Cheese locations
		mouse_loc - Mouse location
		cats - # of cats
		cheeses - # of cheeses
		depth - current search depth
		gr - The graph's adjacency list for the maze

		These arguments are as described in A1. Do have a look at your solution!
 */
	// printf("i am here at util");
	double result = 0;
	int i;
	for(i = 0; i < cheeses; i++){
		result += 10 * (graph_size - distance(gr, mouse_loc[0], cheese_loc[i]));
	}
	
	for(i = 0; i < cats; i++){
		result -= graph_size - distance(gr, mouse_loc[0], cat_loc[i]);
	}
 	return result;   // <--- Obviously, this will be replaced by your computer utilities
}

int distance(double gr[graph_size][4], int locA[2], int locB[2]){
	
	// return abs(locA[1] - locB[1]) + abs(locA[0] - locB[0]);
	return a_star(gr,locA,locB);
}

int a_star(double gr[graph_size][4], int locA[2], int locB[2]){
	// if on top of hceese stay where you are
	// initialize a queue in a form of a linklist
	struct Node* head;
	// add current location on queue
	head = insert_P(locA[0],locA[1], 0, NULL);

	// see how many nodes had been expanded
	int order = 0;
	// have somting to keep track of the predecessor and the final chees position
	// initialize the x value to be -1 to indicate that it hasn't been visited yet
	int history[graph_size][2];
/*			printf("before_for_loop  %d\n", cord_to_number(head->x,head->y));*/
/*			printf("x:%d      y:%d    \n",locA[0],locA[1]);*/
	history[cord_to_number(head->x,head->y)][1]=0;
	int goal[2];
	goal[0] = -1;
	goal[1] = -1;
	int i;
	
	
	for(i = 0; i < graph_size; i++){
		history[i][0] = -1;
	}
	
	// now keep looping untill we found the chess or the stack is empty
	while (head != NULL){
		// find the graph number converting from cord
		int graph_location = cord_to_number(head->x,head->y);
		
		// add 4 new node and I can't figure out a way to use a loop to do this
		// only add it when it has't been added before and it is a connection
		int new_node_loc;
		int cost;
		
		// top
		new_node_loc = cord_to_number(head->x,head->y-1);
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
		new_node_loc = cord_to_number(head->x+1,head->y);
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
		new_node_loc = cord_to_number(head->x,head->y+1);
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
		new_node_loc = cord_to_number(head->x-1,head->y);
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
    int reverse[graph_size];
    int initial_location = cord_to_number(locA[0],locA[1]);
    int counter = 0;
    reverse[0] = cord_to_number(goal[0],goal[1]);
    while(reverse[counter] != initial_location){
    	reverse[counter+1] = history[reverse[counter]][0];
    	counter++;
    }

	return counter;

}


int checkForTerminal(int mouse_loc[1][2],int cat_loc[10][2],int cheese_loc[10][2],int cats,int cheeses)
{
 /* 
   This function determines whether a given configuration constitutes a terminal node.
   Terminal nodes are those for which:
     - A cat eats the mouse
     or
     - The mouse eats a cheese
   
   If the node is a terminal, the function returns 1 on mouse having lunch
   	return -1 if cat is having lunch, else it returns 0
 */

 // Check for cats having lunch
 for (int i=0; i<cats; i++)
  if (mouse_loc[0][0]==cat_loc[i][0]&&mouse_loc[0][1]==cat_loc[i][1]) return(-1);

 // Check for mouse having lunch
 for (int i=0; i<cheeses; i++)
  if (mouse_loc[0][0]==cheese_loc[i][0]&&mouse_loc[0][1]==cheese_loc[i][1]) return(1);

 return(0);

}

