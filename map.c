#include "map.h"
#include "stdio.h"

int get_hash(char value, int size)
{
    int result;
    result = (int) value % size;

    return result;
}

int* get(map* state, int map_size, char key)
{
    int hash = get_hash(key, map_size);

    return state[hash].next_state;
}

void put(map* state, int map_size, char key, int value)
{
    int hash = get_hash(key, map_size);
    int pointer = state[hash].ptr;
    state[hash].next_state[pointer] = value;
    state[hash].ptr++;
}