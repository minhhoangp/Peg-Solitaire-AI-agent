#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "ai.h"
#include "utils.h"
#include "hashtable.h"
#include "stack.h"


void copy_state(state_t* dst, state_t* src){
	
	//Copy field
	memcpy( dst->field, src->field, SIZE*SIZE*sizeof(int8_t) );

	dst->cursor = src->cursor;
	dst->selected = src->selected;
}

/**
 * Saves the path up to the node as the best solution found so far
*/
void save_solution( node_t* solution_node ){
	node_t* n = solution_node;
	while( n->parent != NULL ){
		copy_state( &(solution[n->depth]), &(n->state) );
		solution_moves[n->depth-1] = n->move;

		n = n->parent;
	}
	solution_size = solution_node->depth;
}


node_t* create_init_node( state_t* init_state ){
	node_t * new_n = (node_t *) malloc(sizeof(node_t));
	new_n->parent = NULL;
	new_n->depth = 0;
	copy_state(&(new_n->state), init_state);
	return new_n;
}

/**
 * Apply an action to node n and return a new node resulting 
 * from executing the action
*/
node_t* applyAction(node_t* n, position_s* selected_peg, move_t action ){

	node_t* new_node = (node_t *) malloc(sizeof(node_t));

	/* point to the parent node, update the state with the action chosen,
    update the depth of the node and updates the action used to create 
    the node*/ 
	new_node->parent = n;   
	new_node->move = action;
	copy_state(&(new_node->state), &(n->state));       
	new_node->depth = n->depth + 1;
	execute_move_t( &(new_node->state), selected_peg, action );  

	return new_node;

}


/**
 * Find a solution path as per algorithm description in the handout
 */

void find_solution( state_t* init_state ){

	HashTable table;

	// Choose initial capacity of PRIME NUMBER 
	// Specify the size of the keys and values you want to store once 
	ht_setup( &table, sizeof(int8_t) * SIZE * SIZE, sizeof(int8_t) * SIZE * SIZE, 
			 16769023);

	// Initialize Stack
	initialize_stack();

	//Add the initial node
	node_t* initial_node = create_init_node( init_state );

	//linked list that stores addresses of all generated nodes
	list_t* linked_list = make_empty_list();

	//insert the intitial node to linked list
	insert_at_foot(linked_list, initial_node);

	//push the intitial node into stack
	stack_push(initial_node);

	//store the number of remaining pegs of the first state
	int remaining_pegs = num_pegs(init_state);

	//check if the stack is not empty 
	while (!is_stack_empty(stack)){

			//extract the last node that was inserted into node_t* n
			node_t* n = stack_top();

			//remove the last node that was inserted 
			stack_pop();

			//increase the number of explored nodes by 1
			expanded_nodes = expanded_nodes + 1;

			/*save the solution if the number of pegs left in this stage
			is smaller than the number of remaining pegs */
			if (num_pegs(&(n->state)) < remaining_pegs){
				save_solution(n);
				remaining_pegs = num_pegs(&(n->state));
			}

			//iterate all cells in the board in each state
			int8_t x;
			int8_t y;
			position_s chosen_peg_alloc;
			position_s* chosen_peg = &chosen_peg_alloc;
			for (x=0;x<SIZE;x++) {
				for (y=0;y<SIZE;y++) {
					chosen_peg->x = x;
					chosen_peg->y = y; 
					move_t jump;

					// iterate all jump actions of the cell
					for (int z=0;z<4;z++){
						if (z==0){ jump = left;}
						else if (z==1){ jump = right;}
						else if (z==2){ jump = up;}
						else if (z==3){ jump = down;}

						// handle the duplicated nodes, DFS and win condition
						peg_graph(&table , n ,&(n->state), chosen_peg, 
								  jump, remaining_pegs, linked_list);
					}
				}
			}

			// check if budget exhausted
			if (expanded_nodes >= budget){
				printf("explored nodes are : %d\n", expanded_nodes);

				//free stack and new nodes generated 
				ht_destroy(&table);
				free_list(linked_list);
				return;
			}
	}

	//free stack and new nodes generated 
	ht_destroy(&table);
	free_list(linked_list);
}

/**
 * Handle the duplicated nodes, DFS and win condition
 */
void peg_graph(HashTable* table, node_t* n, state_t* n_state,
			   position_s* chosen_peg, move_t jump, 
			   int remaining_pegs, list_t* linked_list){

	// check if it is a legal jump action
	if (can_apply(n_state, chosen_peg, jump)){

		// create a new node that executed the jump action
		node_t* new_node = applyAction(n, chosen_peg, jump);

		// increase the number of generated nodes by 1
		generated_nodes = generated_nodes + 1;

		/* check if it is win condition to save the solution and update
		the number of remaining pegs */
		if (won(&(new_node->state))){
			save_solution(new_node);
			remaining_pegs = num_pegs(&(new_node->state));

			// insert the node created into the linked list and exit
			insert_at_foot(linked_list, new_node);
			return;
		}

		// check if hashtable does not contain the similar node 
		if (!ht_contains(table, &(new_node->state))){

			//insert the node created into the linked list
			insert_at_foot(linked_list, new_node);

			//push the node into stack
			stack_push(new_node);

			//insert the node into hashtable
			int insert = ht_insert(table, &(new_node->state), new_node);
		} else {
			free(new_node);
		}
	}
}