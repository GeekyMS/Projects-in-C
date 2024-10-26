#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];
bool visited[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool path(int index, int head);
void neighbours(int index, int queue[], int *back);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    for (int i = 0; i < MAX * (MAX - 1) / 2; i++)
    {
        visited[i] = false;
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        preferences[i][i] = 0;
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i; j < candidate_count; j++)
        {
            if ((preferences[i][j] - preferences[j][i]) > 0)
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if ((preferences[i][j] - preferences[j][i]) < 0)
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    int max = 0;
    int max_index;
    pair temp;
    for (int i = 0; i < pair_count; i++)
    {
        temp = pairs[i];
        max_index = i;
        for (int j = i; j < pair_count; j++)
        {
            if ((preferences[pairs[j].winner][pairs[j].loser] -
                 preferences[pairs[j].loser][pairs[j].winner]) >= max)
            {
                max = preferences[pairs[j].winner][pairs[j].loser] -
                      preferences[pairs[j].loser][pairs[j].winner];
                max_index = j;
            }
        }
        pairs[i] = pairs[max_index];
        pairs[max_index] = temp;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
    {
        if (!path(pairs[i].loser, pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        bool head = true;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                head = false;
                break;
            }
        }
        if (head)
        {
            printf("%s\n", candidates[i]);
        }
    }
}

bool path(int index, int head)
{
    for (int i = 0; i < MAX * (MAX - 1) / 2; i++)
    {
        visited[i] = false;
    }

    int node = index;
    int queue[candidate_count];
    queue[0] = node;
    int front = 0;
    int back = 1;
    while (front < back)
    {
        int current = queue[front++];

        neighbours(current, queue, &back);
    }

    return visited[head];
}

void neighbours(int index, int queue[], int *back)
{

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[index][i] == true)
        {
            if (visited[i] != true)
            {
                queue[*back] = i;
                (*back)++;
                visited[i] = true;
            }
        }
    }
}