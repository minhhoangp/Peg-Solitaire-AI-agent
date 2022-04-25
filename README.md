# Peg-Solitaire-AI-agent
A practice of Depth-first search (DFS) algorithms. 

## Introduciton
In this project, I will build a AI agent using Depth-first Search(DFS) strategy to play Peg Solitaire (https://www.pegsolitaire.org/) game and try to achieve the highest score.
![image](https://user-images.githubusercontent.com/88297894/165080359-aa50b317-5f90-422f-b949-5bd51fc2036a.png)

## Implementation
Look at the file peg solitaire.c (main function) to know which function is called to call the algorithm.
The structure of a node, the state, a stack and a hashtable implementation will be used to check for
duplicate states efficiently (line 21 in the algorithm). Look into the utils.* files to know about the
functions can be called to apply an action to update a game state.

## AI agent
The algorithm pseudocode:

![image](https://user-images.githubusercontent.com/88297894/165076480-357f1b1e-1a64-46bc-8ac0-a1eaac089391.png)

When the AI solver is called (Algorithm 1), it should explore all possible paths (sequence of jump
actions) following a Depth First Search (DFS) strategy, until consuming the budget or until a path
solving the game is found. Note that we do not include duplicate states in the search. If a state was
already generated, we will not include it again in the stack (line 21). The algorithm should return the
best solution found, the path leading to the least number of remaining pegs. This path will then
be executed by the game engine.


## Note:
This project is a university assignment from Algorithms & Data Structure subject in The University of Melbourne, Department of Computing  and Software System, Semester 2 20.
