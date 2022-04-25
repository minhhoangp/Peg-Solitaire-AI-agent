#ifndef __AI__
#define __AI__

#include <stdint.h>
#include <unistd.h>
#include "utils.h"
#include "hashtable.h"

void initialize_ai();

void find_solution( state_t* init_state );
void free_memory(unsigned expanded_nodes);
void peg_graph(HashTable* table, node_t* n, state_t* n_state, 
               position_s* chosen_peg, move_t jump, 
               int remaining_pegs, list_t* linked_list);

#endif
