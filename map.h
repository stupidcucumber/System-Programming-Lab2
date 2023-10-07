typedef struct map
{
    char input;
    int* next_state; // -1 in the array means end of all states
    int ptr; // [1] [2] [3] [4] 
} map;

int get_hash(char, int);
int* get(map*, int, char);
void put(map*, int, char, int);
