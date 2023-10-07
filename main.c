#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stddef.h>
#include <stdbool.h>


typedef struct state
{
    int begin_state;
    char input;
    int end_state;
} state;


int next_integer(FILE* file_ptr)
{
    int result = 0;
    char c;

    while (!isdigit(c = fgetc(file_ptr)));

    do
    {
        result *= 10;
        result += (int) (c - 48);
    } while (isdigit(c = fgetc(file_ptr)));

    return result;
}


char next_input(FILE* file_ptr)
{
    char c;
    while (!isalpha(c = fgetc(file_ptr)));
    
    return c;
}


state* next_state(FILE* file_ptr)
{
    state* result = (state*) malloc(sizeof(state));

    result->begin_state = next_integer(file_ptr);
    result->input = next_input(file_ptr);
    result->end_state = next_integer(file_ptr);

    return result;
}

typedef struct
{
    map** states;
    int* final_states;
    int initial_state;
    int final_states_size;
    int states_size;
} automata;


automata* build_graph(FILE* file_ptr)
{
    int meta_data[4]; // A_size, S_size, initial_state, F_size

    for (int i = 0; i < 4; i++)
    {
        meta_data[i] = next_integer(file_ptr);
    }
    
    int* final_states = (int*) malloc(sizeof(int) * meta_data[3]);
    for (int i = 0; i < meta_data[3]; i++)
    {
        final_states[i] = next_integer(file_ptr);
    }
    
    automata* result = (automata*) malloc(sizeof(automata));

    map** states = (map**) malloc(sizeof(map*) * meta_data[1]);

    for (int i = 0; i < meta_data[1]; i++)
    {
        map* temp = (map*) malloc(sizeof(map) * meta_data[1]);
        for (int j = 0; j < meta_data[1]; j++)
        {
            temp[j].next_state = (int*) malloc(sizeof(int) * meta_data[1]);
            temp[j].ptr = 0;

            for (int k = 0; k < meta_data[1]; k++)
            {
                temp[j].next_state[k] = -1;
            }
        }

        states[i] = temp;
    }

    while (!feof(file_ptr))
    {
        state* temp = next_state(file_ptr);

        put(states[temp->begin_state], meta_data[1], temp->input, temp->end_state);
    }

    result->states = states;
    result->initial_state = meta_data[2];
    result->states_size = meta_data[1];
    result->final_states_size = meta_data[3];
    result->final_states = final_states;

    return result;
}


bool execute(char* sequence, map** states, int states_size, int* final_states, int final_states_size, int state_index)
{
    /*
        This is the recursive algorithm.
    */
    printf("Current symbol is: %c, state with which it will be processed: %d\n", sequence[0], state_index);
    if (sequence[0] == '\0')
    {
        for (int i = 0; i < final_states_size; i++)
        {
            if (final_states[i] == state_index)
            {
                return true;
            }
        }

        return false;
    }

    bool result = false;
    map* state = states[state_index];
    int* next_states = get(state, states_size, sequence[0]);

    for (int i = 0; next_states[i] != -1; i++)
    {
        result = result || execute(sequence + 1, states, states_size, final_states, final_states_size, next_states[i]);
    }


    return result;
}


int main(int count, char* args[])
{
    if (count < 2)
    {
        printf("Please, provide a filename of a text file with automata!\n");
    }

    const char* filename = args[1];
    char* sequence = args[2];

    FILE* file_ptr = fopen(filename, "r");

    automata* result = build_graph(file_ptr);

    bool is_accepted = execute(sequence, result->states, result->states_size, result->final_states, result->final_states_size, result->initial_state);

    if (is_accepted)
    {
        printf("Sequence is accepted!\n");
    }
    else
    {
        printf("Sequence is rejected!\n");
    }

    return 0;
}