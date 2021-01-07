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
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool check_circle(int winner, int loser);

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
    for (int i = 0; i < candidate_count; i ++)
    {
        if (strcmp(name, candidates[i]) == 0)
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
    for (int i = 0; i < candidate_count; i ++)
    {
        for (int j = i + 1; j < candidate_count; j ++)
        {
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Pair count
    int count = 0;

    for (int i = 0; i < candidate_count; i ++)
    {
        for (int j = i + 1; j < candidate_count; j ++)
        {
            int winner, loser;

            // Decide who is winner or loser or they are in tie
            if (preferences[i][j] > preferences[j][i])
            {
                winner = i;
                loser = j;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                winner = j;
                loser = i;
            }
            else
            {
                continue;
            }


            // Update pairs and pair count
            pair new_pair;
            new_pair.winner = winner;
            new_pair.loser = loser;
            pairs[count] = new_pair;
            count += 1;
        }
    }

    // Update pair count
    pair_count = count;

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count - 1; i ++)
    {
        // Whether there are any swaps during following comparisons
        bool any_swap = false;

        for (int j = 0; j < pair_count - i - 1; j ++)
        {
            int preference_1 = preferences[pairs[j].winner][pairs[j].loser];
            int preference_2 = preferences[pairs[j + 1].winner][pairs[j + 1].loser];

            if (preference_1 < preference_2)
            {
                pair temp = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = temp;
                any_swap = true;
            }
        }

        // The array has already been sorted
        if (!any_swap)
        {
            return;
        }
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i ++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;

        // Lock the edge if no circle
        if (!check_circle(winner, loser))
        {
            locked[winner][loser] = true;
        }
    }

    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i ++)
    {
        bool is_winner = true;
        for (int j = 0; j < candidate_count; j ++)
        {
            if (locked[j][i])
            {
                is_winner = false;
                break;
            }
        }

        if (is_winner)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }

    return;
}

// Check whether there would be cirlce after adding edge between winner and loser
bool check_circle(int winner, int loser)
{
    if (winner == loser)
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i ++)
    {
        if (locked[loser][i])
        {
            if (check_circle(winner, i)) return true;
        }
    }

    return false;
}

