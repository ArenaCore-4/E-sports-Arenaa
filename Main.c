#include <stdio.h>
#include <string.h>
// defines used for global constants
#define MAX_PLAYERS 896
#define MAX_TEAMS 128
#define TEAM_SIZE 7
#define MAX_MATCHES 256
#define MAX_SLOTS 100

//structure used to group related variables
struct Player
{
    char id[20];
    char name[50];
};

struct Team
{
    char id[20];
    char name[50];
    char playerIDs[TEAM_SIZE][20];
    int activeTournament;
    int unresolvedMatches;
    int played;
	int wins;
	int draws;
	int losses;
	int points;
	int scoreFor;
	int scoreAgainst;
};
       
struct Tournament
{
    char id[20];
    char name[50];
    char game[30];
    float entryFee;
    int capacity;
    char registeredTeams[32][20];
    char waitingTeams[MAX_TEAMS][20];
    int registeredCount;
    int waitingCount;
};

struct Match
{
    char id[20];
    char tournamentID[20];
    char teamA[20];
    char teamB[20];
    int slot;
    int completed;
	int scoreA;
	int scoreB;
};
 
// Global arrays
struct Player players[MAX_PLAYERS];
struct Team teams[MAX_TEAMS];
struct Tournament tournaments[32];
struct Match matches[MAX_MATCHES];
int matchCount = 0;

int playerCount = 0;
int teamCount = 0;
int tournamentCount = 0;

// Match Scheduling (module 3)
void match_menu(void);

int find_match(char id[]);
int team_registered_in(int tournamentIndex, char teamID[]);
int slot_booked(int slot);
int team_busy_in_slot(char teamID[], int slot);

void schedule_match(void);
void search_match(void);
void cancel_match(void);
void view_all_matches(void);

// Main menu and Player & Team menu
int mainMenu(void);
void player_team_menu(void);

// Input helper functions
void read_string(char text[], int size);
int read_integer(void);

// Searching for players and teams
int find_player(char id[]);
int find_team(char id[]);
int player_assigned(char id[]);

// Player & Team Management (module 1)
void register_player(void);
void create_team(void);

void search_player(void);
void update_player(void);

void search_team(void);
void update_team(void);

void delete_team(void);

// Tournament Management (module 2)
void tournament_menu(void);

int find_tournament(char id[]);
int team_in_tournament(char id[]);

void create_tournament(void);
void register_team(void);
void withdraw_team(void);
void search_tournament(void);

// Results & Leaderboard (module 4)
int ask_team_index(const char label[], int otherIndex);
int find_match_by_teams(int a, int b);
void update_team_stats(int t, int scored, int conceded);
void enter_result(void);
int head_to_head(int a, int b);
int ranks_higher(int x, int y);
void view_leaderboard(void);
void result_menu(void);

// Reports & Statistics (module 5)
void reports_menu(void);
void tournament_standings(void);
void match_history(void);
void player_statistics(void);
void team_statistics(void);
void search_records(void);

// Save / Load (module 6)
void save_load_menu(void);
void save_data(void);
void load_data(void);

// input functions
void read_string(char text[], int size)
{
    int ch;
    fgets(text, size, stdin);

    if (text[strlen(text) - 1] == '\n')
    {
        text[strlen(text) - 1] = '\0';
    }
    else
    {
        while ((ch = getchar()) != '\n')
        {
        }
    }
}

int read_integer(void)
{
    char input[50];
    int number;
    read_string(input, 50);

    // sscanf converts the text to an integer; it returns 1 when the conversion works
    if (sscanf(input, "%d", &number) != 1)
    {
        return -999;
    }
    return number;
}

float read_float(void)
{
    char input[50];
    float number;
    read_string(input, 50);
    if (sscanf(input, "%f", &number) != 1)
    {
        return -999;
    }
    return number;
}

// find_player: searches players using an ID. 
int find_player(char id[], int index)
{
    if (index >= playerCount)
    {
        return -1;
    }

    if (strcmp(players[index].id, id) == 0)
    {
        return index;
    }

    return find_player(id, index + 1);
}

int find_player(char id[])
{
    return find_player(id, 0);
}

// find_team: searches teams using an ID. 
int find_team(char id[])
{
    int i;
    for (i = 0; i < teamCount; i++)
    {
        if (strcmp(teams[i].id, id) == 0)
        {
            return i;
        }
    }
    return -1;
}

// player_assigned: returns 1 if the player is already in any team, else 0.
int player_assigned(char id[])
{
    int i, j;
    for (i = 0; i < teamCount; i++)
    {
        for (j = 0; j < TEAM_SIZE; j++)
        {
            if (strcmp(teams[i].playerIDs[j], id) == 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

// register_player: adds a new player after checking: storage not full,
void register_player(void)
{
    int i;
    if (playerCount >= MAX_PLAYERS)
    {
        printf("\nPlayer storage is full!\n");
        return;
    }
    printf("\nEnter Player ID: ");
    read_string(players[playerCount].id, 20);

    if (strlen(players[playerCount].id) == 0)
    {
        printf("\nPlayer ID cannot be empty!\n");
        return;
    }

    // Reject a duplicate ID
    if (find_player(players[playerCount].id) != -1)
    {
        printf("\nPlayer ID already exists!\n");
        return;
    }

    printf("Enter Player Name: ");
    read_string(players[playerCount].name, 50);

    // Reject an empty name
    if (strlen(players[playerCount].name) == 0)
    {
        printf("\nPlayer name cannot be empty!\n");
        return;
    }

    // Reject a duplicate name (compare with all registered players)
    for (i = 0; i < playerCount; i++)
    {
        if (strcmp(players[i].name,
                   players[playerCount].name) == 0)
        {
            printf("\nPlayer name already exists!\n");
            return;
        }
    }

    playerCount++;
    printf("\nPlayer registered successfully!\n");
}

// create_team: creates a team with an ID, a name of TEAM_SIZE (7) players.
// Every player must already exist, must not be in another team and must not be repeated.
void create_team(void)
{
    int i, j;
    char id[20];

    if (teamCount >= MAX_TEAMS)
    {
        printf("\nTeam storage is full!\n");
        return;
    }
    printf("\nEnter Team ID: ");
    read_string(teams[teamCount].id, 20);

    if (strlen(teams[teamCount].id) == 0)
    {
        printf("\nTeam ID cannot be empty!\n");
        return;
    }

    if (find_team(teams[teamCount].id) != -1)
    {
        printf("\nTeam ID already exists!\n");
        return;
    }

    printf("Enter Team Name: ");
    read_string(teams[teamCount].name, 50);

    if (strlen(teams[teamCount].name) == 0)
    {
        printf("\nTeam name cannot be empty!\n");
        return;
    }

    for (i = 0; i < teamCount; i++)
    {
        if (strcmp(teams[i].name,
                   teams[teamCount].name) == 0)
        {
            printf("\nTeam name already exists!\n");
            return;
        }
    }

    for (i = 0; i < TEAM_SIZE; i++)
    {
        printf("\nEnter Player %d ID: ", i + 1);
        read_string(id, 20);

        if (find_player(id) == -1)
        {
            printf("\nPlayer does not exist! Register first.\n");
            return;
        }

        if (player_assigned(id) == 1)
        {
            printf("\nPlayer already belongs to another team!\n");
            return;
        }

        for (j = 0; j < i; j++)
        {
            if (strcmp(teams[teamCount].playerIDs[j], id) == 0)
            {
                printf("\nDuplicate player in team!\n");
                return;
            }
        }

        // Save the player ID in the team
        strcpy(teams[teamCount].playerIDs[i], id);
    }

    // A new team has no active tournament and no pending matches
    teams[teamCount].activeTournament = 0;
    teams[teamCount].unresolvedMatches = 0;

    teams[teamCount].played = 0;
    teams[teamCount].wins = 0;
    teams[teamCount].draws = 0;
    teams[teamCount].losses = 0;
    teams[teamCount].points = 0;
    teams[teamCount].scoreFor = 0;
    teams[teamCount].scoreAgainst = 0;

    teamCount++;

    printf("\nTeam created successfully!\n");
}

// search_player: finds a player by exact ID or exact name and shows the details.
void search_player(void)
{
    char search[50];
    int i, found = 0;

    printf("\nEnter Player ID or Name: ");
    read_string(search, 50);

    for (i = 0; i < playerCount; i++)
    {
        if (strcmp(players[i].id, search) == 0 ||
            strcmp(players[i].name, search) == 0)
        {
            printf("\nPlayer Found!\n");
            printf("Player ID: %s\n", players[i].id);
            printf("Player Name: %s\n", players[i].name);

            found = 1;
            break;
        }
    }

    if (found == 0)
    {
        printf("\nPlayer not found!\n");
    }
}

// update_player: changes the name of an existing player.
void update_player(void)
{
    char id[20];
    char newName[50];
    int i, index;

    printf("\nEnter Player ID: ");
    read_string(id, 20);

    index = find_player(id);

    if (index == -1)
    {
        printf("\nPlayer not found!\n");
        return;
    }

    printf("Enter New Player Name: ");
    read_string(newName, 50);

    if (strlen(newName) == 0)
    {
        printf("\nName cannot be empty!\n");
        return;
    }

    // The new name must not already belong to a different player (i != index)
    for (i = 0; i < playerCount; i++)
    {
        if (i != index &&
            strcmp(players[i].name, newName) == 0)
        {
            printf("\nThis name already exists!\n");
            return;
        }
    }

    strcpy(players[index].name, newName);
    printf("\nPlayer updated successfully!\n");
}

// search_team: finds a team by exact ID or exact name and shows its members.
void search_team(void)
{
    char search[50];
    int i, j, found = 0;
    int playerIndex;

    printf("\nEnter Team ID or Name: ");
    read_string(search, 50);

    for (i = 0; i < teamCount; i++)
    {
        if (strcmp(teams[i].id, search) == 0 ||
            strcmp(teams[i].name, search) == 0)
        {
            printf("\nTeam Found!\n");
            printf("Team ID: %s\n", teams[i].id);
            printf("Team Name: %s\n", teams[i].name);

            // Show the roster
            printf("\nTeam Members:\n");

            for (j = 0; j < TEAM_SIZE; j++)
            {
                playerIndex = find_player(teams[i].playerIDs[j]);
                if (playerIndex != -1)
                {
                    printf("%d. %s\n",
                           j + 1,
                           players[playerIndex].name);
                }
            }

            found = 1;
            break;
        }
    }

    if (found == 0)
    {
        printf("\nTeam not found!\n");
    }
}

// update_team: changes the name of an existing team (the ID cannot be changed).
void update_team(void)
{
    char id[20];
    char newName[50];
    int i, index;

    printf("\nEnter Team ID: ");
    read_string(id, 20);

    index = find_team(id);

    if (index == -1)
    {
        printf("\nTeam not found!\n");
        return;
    }

    printf("Enter New Team Name: ");
    read_string(newName, 50);

    if (strlen(newName) == 0)
    {
        printf("\nTeam name cannot be empty!\n");
        return;
    }

    // The new name must not already belong to a different team
    for (i = 0; i < teamCount; i++)
    {
        if (i != index &&
            strcmp(teams[i].name, newName) == 0)
        {
            printf("\nTeam name already exists!\n");
            return;
        }
    }
    strcpy(teams[index].name, newName);
    printf("\nTeam updated successfully!\n");
}

// deleting team using ID
void delete_team(void)
{
    char id[20];
    int i, index;

    printf("\nEnter Team ID to Delete: ");
    read_string(id, 20);

    index = find_team(id);

    if (index == -1)
    {
        printf("\nTeam not found!\n");
        return;
    }

    if (teams[index].activeTournament == 1)
    {
        printf("\nCannot delete! Team is in an active tournament.\n");
        return;
    }

    if (teams[index].unresolvedMatches > 0)
    {
        printf("\nCannot delete! Team has unresolved matches.\n");
        return;
    }

    for (i = index; i < teamCount - 1; i++)
    {
        teams[i] = teams[i + 1];
    }

    teamCount--;
    printf("\nTeam deleted successfully!\n");
}

// find_tournament: searches tournaments[] for an ID. 
int find_tournament(char id[])
{
    int i;
    for (i = 0; i < tournamentCount; i++)
    {
        if (strcmp(tournaments[i].id, id) == 0)
        {
            return i;
        }
    }
    return -1;
}

// team_in_tournament: returns 1 if the team is registered OR waiting in any tournament.
int team_in_tournament(char id[])
{
    int i, j;

    for (i = 0; i < tournamentCount; i++)
    {
        for (j = 0; j < tournaments[i].registeredCount; j++)
        {
            if (strcmp(tournaments[i].registeredTeams[j], id) == 0)
            {
                return 1;
            }
        }
        for (j = 0; j < tournaments[i].waitingCount; j++)
        {
            if (strcmp(tournaments[i].waitingTeams[j], id) == 0)
            {
                return 1;
            }
        }
    }

    return 0;
}

// create_tournament: creates a tournament after checking every input
void create_tournament(void)
{
    int i, capacity;
    float fee;
    char id[20], name[50], game[30];

    // Maximum 32 tournaments
    if (tournamentCount >= 32)
    {
        printf("\nTournament limit reached.\n");
        return;
    }

    printf("\nEnter Tournament ID: ");
    read_string(id, 20);

    if (strlen(id) == 0 || find_tournament(id) != -1)
    {
        printf("\nInvalid or duplicate Tournament ID.\n");
        return;
    }

    printf("Enter Tournament Name: ");
    read_string(name, 50);

    if (strlen(name) == 0)
    {
        printf("\nTournament name cannot be empty.\n");
        return;
    }

    for (i = 0; i < tournamentCount; i++)
    {
        if (strcmp(tournaments[i].name, name) == 0)
        {
            printf("\nTournament name already exists.\n");
            return;
        }
    }

    printf("Enter Game Name: ");
    read_string(game, 30);

    if (strlen(game) == 0)
    {
        printf("\nGame name cannot be empty.\n");
        return;
    }

    printf("Enter Entry Fee: ");
    fee = read_float();

    if (fee < 0 || fee == -999)
    {
        printf("\nInvalid entry fee.\n");
        return;
    }

    // Only these three capacities are allowed
    printf("Select Tournament Capacity (8, 16, or 32): ");
    capacity = read_integer();

    if (capacity != 8 && capacity != 16 && capacity != 32)
    {
        printf("\nInvalid tournament capacity.\n");
        return;
    }

    // All inputs are valid: store the tournament at the next free place
    strcpy(tournaments[tournamentCount].id, id);
    strcpy(tournaments[tournamentCount].name, name);
    strcpy(tournaments[tournamentCount].game, game);

    tournaments[tournamentCount].entryFee = fee;
    tournaments[tournamentCount].capacity = capacity;

    // A new tournament starts with no registered and no waiting teams
    tournaments[tournamentCount].registeredCount = 0;
    tournaments[tournamentCount].waitingCount = 0;

    tournamentCount++;
    printf("\nTournament created successfully!\n");
}

// register_team: registers a team in a tournament.
void register_team(void)
{
    char tournamentID[20], teamID[20];
    int tournamentIndex, teamIndex;

    printf("\nEnter Tournament ID: ");
    read_string(tournamentID, 20);

    tournamentIndex = find_tournament(tournamentID);

    if (tournamentIndex == -1)
    {
        printf("\nTournament not found.\n");
        return;
    }

    printf("Enter Team ID: ");
    read_string(teamID, 20);

    teamIndex = find_team(teamID);
    if (teamIndex == -1)
    {
        printf("\nTeam not found.\n");
        return;
    }

    // A team can be in only one tournament (registered or waiting)
    if (team_in_tournament(teamID))
    {
        printf("\nTeam is already registered or waiting in another tournament.\n");
        return;
    }

    if (tournaments[tournamentIndex].registeredCount <
        tournaments[tournamentIndex].capacity)
    {
        strcpy(
            tournaments[tournamentIndex].registeredTeams[
                tournaments[tournamentIndex].registeredCount],
            teamID
        );

        tournaments[tournamentIndex].registeredCount++;
        teams[teamIndex].activeTournament = 1;

        printf("\nTeam registered successfully!\n");
    }
    else
    {
        // Tournament is full: the waiting list also has a limit
        if (tournaments[tournamentIndex].waitingCount >= MAX_TEAMS)
        {
            printf("\nWaiting list is full.\n");
            return;
        }

        strcpy(
            tournaments[tournamentIndex].waitingTeams[
                tournaments[tournamentIndex].waitingCount],
            teamID
        );

        tournaments[tournamentIndex].waitingCount++;

        printf("\nTournament is full. Team added to waiting list.\n");
    }
}

// withdraw_team: removes a team from a tournament.
// If a registered team leaves, the first team on the waiting list takes its place.
void withdraw_team(void)
{
    char tournamentID[20], teamID[20];
    int tournamentIndex, teamIndex;
    int i, found = 0;

    printf("\nEnter Tournament ID: ");
    read_string(tournamentID, 20);

    tournamentIndex = find_tournament(tournamentID);

    if (tournamentIndex == -1)
    {
        printf("\nTournament not found.\n");
        return;
    }

    printf("Enter Team ID: ");
    read_string(teamID, 20);

    // First look for the team in the registered teams
    for (i = 0; i < tournaments[tournamentIndex].registeredCount; i++)
    {
        if (strcmp(tournaments[tournamentIndex].registeredTeams[i], teamID) == 0)
        {
            teamIndex = find_team(teamID);

            if (teamIndex != -1 && teams[teamIndex].unresolvedMatches > 0)
            {
                printf("\nCannot withdraw! Team has unresolved matches. Cancel them first.\n");
                return;
            }

            found = 1;

            for (int j = i; j < tournaments[tournamentIndex].registeredCount - 1; j++)
            {
                strcpy(
                    tournaments[tournamentIndex].registeredTeams[j],
                    tournaments[tournamentIndex].registeredTeams[j + 1]
                );
            }

            tournaments[tournamentIndex].registeredCount--;

            // The team is no longer active in a tournament
            teamIndex = find_team(teamID);

            if (teamIndex != -1)
            {
                teams[teamIndex].activeTournament = 0;
            }

            printf("\nTeam withdrawn successfully.\n");

            // If some team is waiting, promote the first waiting team into the free place
            if (tournaments[tournamentIndex].waitingCount > 0)
            {
                strcpy(
                    tournaments[tournamentIndex].registeredTeams[
                        tournaments[tournamentIndex].registeredCount],
                    tournaments[tournamentIndex].waitingTeams[0]
                );

                teamIndex = find_team(tournaments[tournamentIndex].waitingTeams[0]);

                if (teamIndex != -1)
                {
                    teams[teamIndex].activeTournament = 1;
                }

                tournaments[tournamentIndex].registeredCount++;

                for (int j = 0; j < tournaments[tournamentIndex].waitingCount - 1; j++)
                {
                    strcpy(
                        tournaments[tournamentIndex].waitingTeams[j],
                        tournaments[tournamentIndex].waitingTeams[j + 1]
                    );
                }

                tournaments[tournamentIndex].waitingCount--;
                printf("First waiting team promoted successfully.\n");
            }

            break;
        }
    }

    // Not found in the registered teams: search the waiting list
    if (found == 0)
    {
        for (i = 0; i < tournaments[tournamentIndex].waitingCount; i++)
        {
            if (strcmp(tournaments[tournamentIndex].waitingTeams[i], teamID) == 0)
            {
                found = 1;
                for (int j = i; j < tournaments[tournamentIndex].waitingCount - 1; j++)
                {
                    strcpy(
                        tournaments[tournamentIndex].waitingTeams[j],
                        tournaments[tournamentIndex].waitingTeams[j + 1]
                    );
                }

                tournaments[tournamentIndex].waitingCount--;

                printf("\nTeam removed from waiting list successfully.\n");
                break;
            }
        }
    }

    if (found == 0)
    {
        printf("\nTeam is not registered or waiting in this tournament.\n");
    }
}

// search_tournament: finds a tournament by ID or by name and shows its details,
void search_tournament(void)
{
    char search[50];
    int i, j, found = 0, choice, index;

    printf("\n1. Search by Tournament ID\n");
    printf("2. Search by Tournament Name\n");
    printf("Enter choice: ");
    choice = read_integer();

    if (choice != 1 && choice != 2)
    {
        printf("\nInvalid choice.\n");
        return;
    }

    printf("Enter search value: ");
    read_string(search, 50);

    // Show full details of the matching tournament
    for (i = 0; i < tournamentCount; i++)
    {
        if ((choice == 1 && strcmp(tournaments[i].id, search) == 0) ||
            (choice == 2 && strcmp(tournaments[i].name, search) == 0))
        {
            found = 1;

            printf("\nTournament ID: %s\n", tournaments[i].id);
            printf("Tournament Name: %s\n", tournaments[i].name);
            printf("Game: %s\n", tournaments[i].game);
            printf("Entry Fee: %.2f\n", tournaments[i].entryFee);
            printf("Capacity: %d\n", tournaments[i].capacity);
            printf("Registered Teams: %d\n", tournaments[i].registeredCount);
            printf("Waiting Teams: %d\n", tournaments[i].waitingCount);

            printf("\n--- Registered Teams ---\n");

            for (j = 0; j < tournaments[i].registeredCount; j++)
            {
                index = find_team(tournaments[i].registeredTeams[j]);

                if (index != -1)
                {
                    printf("%s - %s\n", teams[index].id, teams[index].name);
                }
            }

            printf("\n--- Waiting List ---\n");

            for (j = 0; j < tournaments[i].waitingCount; j++)
            {
                index = find_team(tournaments[i].waitingTeams[j]);

                if (index != -1)
                {
                    printf("%s - %s\n", teams[index].id, teams[index].name);
                }
            }

            break;
        }
    }

    if (found == 0)
    {
        printf("\nTournament not found.\n");
    }
}

void tournament_menu(void)
{
    int choice;
    do
    {
        printf("\n========== TOURNAMENT MANAGEMENT ==========\n");
        printf("1. Create Tournament\n");
        printf("2. Register Team\n");
        printf("3. Withdraw Team\n");
        printf("4. Search Tournament\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");

        choice = read_integer();
        switch (choice)
        {
            case 1:
                create_tournament();
                break;

            case 2:
                register_team();
                break;

            case 3:
                withdraw_team();
                break;

            case 4:
                search_tournament();
                break;

            case 0:
                printf("\nReturning to Main Menu...\n");
                break;

            default:
                printf("\nInvalid choice. Try again.\n");
        }

    } while (choice != 0);
}
// find_match: searches matches using ID. 
int find_match(char id[])
{
    int i;

    for (i = 0; i < matchCount; i++)
    {
        if (strcmp(matches[i].id, id) == 0)
        {
            return i;
        }
    }

    return -1;
}

int team_registered_in(int tournamentIndex, char teamID[])
{
    int i;

    for (i = 0; i < tournaments[tournamentIndex].registeredCount; i++)
    {
        if (strcmp(tournaments[tournamentIndex].registeredTeams[i], teamID) == 0)
        {
            return 1;
        }
    }

    return 0;
}

// slot_booked: returns 1 if ANY match already uses this slot.
int slot_booked(int slot)
{
    int i;
    for (i = 0; i < matchCount; i++)
    {
        if (matches[i].slot == slot)
        {
            return 1;
        }
    }

    return 0;
}

// team_busy_in_slot: returns 1 if the team already plays (as Team A or Team B) in this slot.
int team_busy_in_slot(char teamID[], int slot)
{
    int i;

    for (i = 0; i < matchCount; i++)
    {
        if (matches[i].slot == slot &&
            (strcmp(matches[i].teamA, teamID) == 0 ||
             strcmp(matches[i].teamB, teamID) == 0))
        {
            return 1;
        }
    }
    return 0;
}

// schedule_match: books a new match.
void schedule_match(void)
{
    char matchID[20], tournamentID[20], teamA[20], teamB[20];
    int tournamentIndex, teamAIndex, teamBIndex, slot;

    if (matchCount >= MAX_MATCHES)
    {
        printf("\nMatch storage is full!\n");
        return;
    }

    printf("\nEnter Match ID: ");
    read_string(matchID, 20);

    if (strlen(matchID) == 0 || find_match(matchID) != -1)
    {
        printf("\nInvalid or duplicate Match ID!\n");
        return;
    }

    printf("Enter Tournament ID: ");
    read_string(tournamentID, 20);

    tournamentIndex = find_tournament(tournamentID);

    if (tournamentIndex == -1)
    {
        printf("\nTournament not found!\n");
        return;
    }

    printf("Enter Team A ID: ");
    read_string(teamA, 20);

    teamAIndex = find_team(teamA);

    if (teamAIndex == -1)
    {
        printf("\nTeam A not found!\n");
        return;
    }

    printf("Enter Team B ID: ");
    read_string(teamB, 20);
    teamBIndex = find_team(teamB);

    if (teamBIndex == -1)
    {
        printf("\nTeam B not found!\n");
        return;
    }

    if (strcmp(teamA, teamB) == 0)
    {
        printf("\nBoth teams cannot be the same!\n");
        return;
    }

    if (team_registered_in(tournamentIndex, teamA) == 0 ||
        team_registered_in(tournamentIndex, teamB) == 0)
    {
        printf("\nBoth teams must be registered in this tournament!\n");
        return;
    }

    printf("Enter Slot Number (1-%d): ", MAX_SLOTS);
    slot = read_integer();

    if (slot < 1 || slot > MAX_SLOTS)
    {
        printf("\nInvalid slot number!\n");
        return;
    }

    if (slot_booked(slot))
    {
        printf("\nSlot is already booked!\n");
        return;
    }

    if (team_busy_in_slot(teamA, slot) ||
        team_busy_in_slot(teamB, slot))
    {
        printf("\nOne of the teams is already playing in this slot!\n");
        return;
    }

    strcpy(matches[matchCount].id, matchID);
    strcpy(matches[matchCount].tournamentID, tournamentID);
    strcpy(matches[matchCount].teamA, teamA);
    strcpy(matches[matchCount].teamB, teamB);
    matches[matchCount].slot = slot;

    matches[matchCount].completed = 0;
    matches[matchCount].scoreA = 0;
    matches[matchCount].scoreB = 0;

    teams[teamAIndex].unresolvedMatches++;
    teams[teamBIndex].unresolvedMatches++;

    matchCount++;

    printf("\nMatch scheduled successfully!\n");
}

// search_match: finds a match by its ID and shows its details.
void search_match(void)
{
    char id[20];
    int index, a, b;

    printf("\nEnter Match ID: ");
    read_string(id, 20);

    index = find_match(id);

    if (index == -1)
    {
        printf("\nMatch not found!\n");
        return;
    }

    a = find_team(matches[index].teamA);
    b = find_team(matches[index].teamB);

    printf("\nMatch Found!\n");
    printf("Match ID: %s\n", matches[index].id);
    printf("Tournament ID: %s\n", matches[index].tournamentID);

    if (a != -1)
    {
        printf("Team A: %s - %s\n", teams[a].id, teams[a].name);
    }

    if (b != -1)
    {
        printf("Team B: %s - %s\n", teams[b].id, teams[b].name);
    }

    printf("Slot: %d\n", matches[index].slot);
}

// view_all_matches: lists every scheduled match in one line each.
void view_all_matches(void)
{
    int i, a, b;

    if (matchCount == 0)
    {
        printf("\nNo matches scheduled.\n");
        return;
    }

    printf("\n--- Scheduled Matches ---\n");

    for (i = 0; i < matchCount; i++)
    {
        a = find_team(matches[i].teamA);
        b = find_team(matches[i].teamB);

        printf("%s | Tournament: %s | %s vs %s | Slot: %d\n",
               matches[i].id,
               matches[i].tournamentID,
               (a != -1) ? teams[a].name : matches[i].teamA,
               (b != -1) ? teams[b].name : matches[i].teamB,
               matches[i].slot);
    }
}

// cancel_match: removes a scheduled match and fixes the teams' unresolved match counters.
void cancel_match(void)
{
    char id[20];
    int i, index, a, b;

    printf("\nEnter Match ID to Cancel: ");
    read_string(id, 20);

    index = find_match(id);

    if (index == -1)
    {
        printf("\nMatch not found!\n");
        return;
    }

    a = find_team(matches[index].teamA);
    b = find_team(matches[index].teamB);

    if (matches[index].completed == 1)
    {
        printf("\nCannot cancel! The result of this match is already entered.\n");
        return;
    }

    if (a != -1 && teams[a].unresolvedMatches > 0)
    {
        teams[a].unresolvedMatches--;
    }

    if (b != -1 && teams[b].unresolvedMatches > 0)
    {
        teams[b].unresolvedMatches--;
    }

    // Shift all later matches to the left to remove the cancelled match
    for (i = index; i < matchCount - 1; i++)
    {
        matches[i] = matches[i + 1];
    }

    matchCount--;

    printf("\nMatch cancelled successfully!\n");
}

void match_menu(void)
{
    int choice;

    do
    {
        printf("\n========== MATCH SCHEDULING ==========\n");
        printf("1. Schedule Match\n");
        printf("2. Search Match\n");
        printf("3. View All Matches\n");
        printf("4. Cancel Match\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");

        choice = read_integer();

        switch (choice)
        {
            case 1:
                schedule_match();
                break;

            case 2:
                search_match();
                break;

            case 3:
                view_all_matches();
                break;

            case 4:
                cancel_match();
                break;

            case 0:
                printf("\nReturning to Main Menu...\n");
                break;

            default:
                printf("\nInvalid choice. Try again.\n");
        }

    } while (choice != 0);
}

#define WIN_POINTS 3
#define DRAW_POINTS 1
#define LOSS_POINTS 0

int ask_team_index(const char label[], int otherIndex);
int find_match_by_teams(int a, int b);
void update_team_stats(int t, int scored, int conceded);
void enter_result(void);
int head_to_head(int a, int b);
int ranks_higher(int x, int y);
void view_leaderboard(void);
void result_menu(void);

// ask_team_index: keeps asking for a team ID until a valid team is entered.
int ask_team_index(const char label[], int otherIndex)
{
    char id[20];
    int index;

    while (1)
    {
        printf("Enter %s ID: ", label);
        read_string(id, 20);

        index = find_team(id);

        if (index == -1)
        {
            printf("Team not found!\n");
        }
        else if (index == otherIndex)
        {
            printf("A team cannot play against itself!\n");
        }
        else
        {
            return index;
        }
    }
}

// find_match_by_teams: finds the match where team a is Team A and team b is Team B.
int find_match_by_teams(int a, int b)
{
    int k, firstFound = -1;

    for (k = 0; k < matchCount; k++)
    {
        if (strcmp(matches[k].teamA, teams[a].id) == 0 &&
            strcmp(matches[k].teamB, teams[b].id) == 0)
        {
            if (matches[k].completed == 0)
            {
                return k;
            }

            if (firstFound == -1)
            {
                firstFound = k;
            }
        }
    }

    return firstFound;
}

// update_team_stats: updates the statistics of ONE team after a match.
// t = team index, scored = score the team made, conceded = score it received.
void update_team_stats(int t, int scored, int conceded)
{
    teams[t].played++;
    teams[t].scoreFor += scored;
    teams[t].scoreAgainst += conceded;

    if (scored > conceded)
    {
        teams[t].wins++;
        teams[t].points += WIN_POINTS;
    }
    else if (scored == conceded)
    {
        teams[t].draws++;
        teams[t].points += DRAW_POINTS;
    }
    else
    {
        teams[t].losses++;
        teams[t].points += LOSS_POINTS;
    }
}

// enter_result: enters the scores of a scheduled match and updates both teams' statistics.
void enter_result(void)
{
    int a, b, k, scoreA, scoreB, temp;

    if (teamCount < 2)
    {
        printf("\nAt least 2 teams are needed to enter a result!\n");
        return;
    }

    if (matchCount == 0)
    {
        printf("\nNo matches have been scheduled yet!\n");
        return;
    }

    // Ask for both teams (Team B must be different from Team A)
    a = ask_team_index("Team A", -1);
    b = ask_team_index("Team B", a);

    // Find the scheduled match between these two teams
    k = find_match_by_teams(a, b);

    // if no match was found, try the teams in the opposite order
    if (k == -1)
    {
        k = find_match_by_teams(b, a);

        if (k != -1)
        {
            temp = a;
            a = b;
            b = temp;
        }
    }

    if (k == -1)
    {
        printf("\nNo such scheduled match!\n");
        return;
    }

    if (matches[k].completed == 1)
    {
        printf("\nResult already entered!\n");
        return;
    }
    printf("Enter score of %s: ", teams[a].name);
    scoreA = read_integer();

    if (scoreA < 0)
    {
        printf("\nInvalid score! Nothing was changed.\n");
        return;
    }

    printf("Enter score of %s: ", teams[b].name);
    scoreB = read_integer();

    if (scoreB < 0)
    {
        printf("\nInvalid score! Nothing was changed.\n");
        return;
    }

    matches[k].scoreA = scoreA;
    matches[k].scoreB = scoreB;
    matches[k].completed = 1;

    update_team_stats(a, scoreA, scoreB);
    update_team_stats(b, scoreB, scoreA);

    if (teams[a].unresolvedMatches > 0)
    {
        teams[a].unresolvedMatches--;
    }

    if (teams[b].unresolvedMatches > 0)
    {
        teams[b].unresolvedMatches--;
    }
    printf("\nResult saved: %s %d - %d %s\n",
           teams[a].name, scoreA, scoreB, teams[b].name);
    if (scoreA == scoreB)
    {
        printf("The match was a draw.\n");
    }
}

// head_to_head: compares two teams using only the completed matches between them.
// Returns a positive number if team a earned more points, negative if team b did, 0 if equal.
int head_to_head(int a, int b)
{
    int k, sA, sB, ptsA = 0, ptsB = 0;

    for (k = 0; k < matchCount; k++)
    {
        if (matches[k].completed == 0)
        {
            continue;
        }

        if (strcmp(matches[k].teamA, teams[a].id) == 0 &&
            strcmp(matches[k].teamB, teams[b].id) == 0)
        {
            sA = matches[k].scoreA;
            sB = matches[k].scoreB;
        }
        else if (strcmp(matches[k].teamA, teams[b].id) == 0 &&
                 strcmp(matches[k].teamB, teams[a].id) == 0)
        {
            sA = matches[k].scoreB;
            sB = matches[k].scoreA;
        }
        else
        {
            // This match is between other teams: skip it
            continue;
        }

        if (sA > sB)
        {
            ptsA += WIN_POINTS;
            ptsB += LOSS_POINTS;
        }
        else if (sA == sB)
        {
            ptsA += DRAW_POINTS;
            ptsB += DRAW_POINTS;
        }
        else
        {
            ptsA += LOSS_POINTS;
            ptsB += WIN_POINTS;
        }
    }
    return ptsA - ptsB;
}

// ranks_higher: returns 1 if team x must be ranked above team y.
int ranks_higher(int x, int y)
{
    int h, diffX, diffY;

    if (teams[x].points != teams[y].points)
    {
        return teams[x].points > teams[y].points;
    }

    h = head_to_head(x, y);

    if (h != 0)
    {
        return h > 0;
    }
    diffX = teams[x].scoreFor - teams[x].scoreAgainst;
    diffY = teams[y].scoreFor - teams[y].scoreAgainst;
    if (diffX != diffY)
    {
        return diffX > diffY;
    }
    return strcmp(teams[x].id, teams[y].id) < 0;
}

// view_leaderboard: sorts all teams with bubble sort and prints the leaderboard.
void view_leaderboard(void)
{
    // rank holds team indexes in ranking order (we sort the indexes, not the teams)
    int rank[MAX_TEAMS];
    int i, j, temp, t, doneCount = 0;

    if (teamCount == 0)
    {
        printf("\nNo teams registered yet.\n");
        return;
    }
    for (i = 0; i < matchCount; i++)
    {
        if (matches[i].completed == 1)
        {
            doneCount++;
        }
    }
    for (i = 0; i < teamCount; i++)
    {
        rank[i] = i;
    }
    // Bubble sort
    for (i = 0; i < teamCount - 1; i++)
    {
        for (j = 0; j < teamCount - 1 - i; j++)
        {
            if (ranks_higher(rank[j + 1], rank[j]))
            {
                temp = rank[j];
                rank[j] = rank[j + 1];
                rank[j + 1] = temp;
            }
        }
    }
    // Print the table heading (P = played, W = wins, D = draws, L = losses,
    // GF = goals/score for, GA = score against, GD = difference, Pts = points)
    printf("\n==================== LEADERBOARD ====================\n");
    printf("%-4s %-10s %-20s %3s %3s %3s %3s %4s %4s %4s %4s\n",
           "Rank", "Team ID", "Team Name", "P", "W", "D", "L",
           "GF", "GA", "GD", "Pts");
    for (i = 0; i < teamCount; i++)
    {
        t = rank[i];
        printf("%-4d %-10.10s %-20.20s %3d %3d %3d %3d %4d %4d %+4d %4d\n",
               i + 1,
               teams[t].id,
               teams[t].name,
               teams[t].played,
               teams[t].wins,
               teams[t].draws,
               teams[t].losses,
               teams[t].scoreFor,
               teams[t].scoreAgainst,
               teams[t].scoreFor - teams[t].scoreAgainst,
               teams[t].points);
    }

    printf("\nWin = %d pts, Draw = %d pt each, Loss = %d pts.\n",
           WIN_POINTS, DRAW_POINTS, LOSS_POINTS);
    printf("Ranking: points, head-to-head, score difference, team ID.\n");
    if (doneCount == 0)
    {
        printf("(No results have been entered yet.)\n");
    }
}
void result_menu(void)
{
    int choice;
    do
    {
        printf("\n========== RESULTS & LEADERBOARD ==========\n");
        printf("1. Enter Match Result\n");
        printf("2. View Leaderboard\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");

        choice = read_integer();

        switch (choice)
        {
            case 1:
                enter_result();
                break;

            case 2:
                view_leaderboard();
                break;

            case 0:
                printf("\nReturning to Main Menu...\n");
                break;

            default:
                printf("\nInvalid choice. Try again.\n");
        }

    } while (choice != 0);
}

// tournament_standings: shows the standings of ONE tournament.
void tournament_standings(void)
{
    char id[20];
    int t, i, j, a, b, temp;
    int rank[32];
    int played[32], wins[32], draws[32], losses[32];
    int points[32], scoreFor[32], scoreAgainst[32];
    int headToHead[32][32];

    printf("\nEnter Tournament ID: ");
    read_string(id, 20);

    t = find_tournament(id);

    if (t == -1)
    {
        printf("\nTournament not found!\n");
        return;
    }
    if (tournaments[t].registeredCount == 0)
    {
        printf("\nNo registered teams in this tournament.\n");
        return;
    }
    for (i = 0; i < tournaments[t].registeredCount; i++)
    {
        rank[i] = i;
        played[i] = 0;
        wins[i] = 0;
        draws[i] = 0;
        losses[i] = 0;
        points[i] = 0;
        scoreFor[i] = 0;
        scoreAgainst[i] = 0;
        for (j = 0; j < tournaments[t].registeredCount; j++)
        {
            headToHead[i][j] = 0;
        }
    }
    for (i = 0; i < matchCount; i++)
    {
        // Skip matches that are pending or belong to another tournament
        if (matches[i].completed == 0 ||
            strcmp(matches[i].tournamentID, id) != 0)
        {
            continue;
        }

        // Find the positions of Team A and Team B in the registered list
        a = -1;
        b = -1;
        for (j = 0; j < tournaments[t].registeredCount; j++)
        {
            if (strcmp(tournaments[t].registeredTeams[j],
                       matches[i].teamA) == 0)
            {
                a = j;
            }
            if (strcmp(tournaments[t].registeredTeams[j],
                       matches[i].teamB) == 0)
            {
                b = j;
            }
        }
        if (a == -1 || b == -1)
        {
            continue;
        }
        played[a]++;
        played[b]++;

        scoreFor[a] += matches[i].scoreA;
        scoreAgainst[a] += matches[i].scoreB;

        scoreFor[b] += matches[i].scoreB;
        scoreAgainst[b] += matches[i].scoreA;

        // Team A won
        if (matches[i].scoreA > matches[i].scoreB)
        {
            wins[a]++;
            losses[b]++;
            points[a] += WIN_POINTS;
            points[b] += LOSS_POINTS;
            headToHead[a][b] += WIN_POINTS;
        }
        else if (matches[i].scoreA < matches[i].scoreB)
        {
            // Team B won
            wins[b]++;
            losses[a]++;
            points[b] += WIN_POINTS;
            points[a] += LOSS_POINTS;
            headToHead[b][a] += WIN_POINTS;
        }
        else
        {
            // Draw
            draws[a]++;
            draws[b]++;
            points[a] += DRAW_POINTS;
            points[b] += DRAW_POINTS;
            headToHead[a][b] += DRAW_POINTS;
            headToHead[b][a] += DRAW_POINTS;
        }
    }

    // Bubble sort using the same rules
    for (i = 0; i < tournaments[t].registeredCount - 1; i++)
    {
        for (j = 0; j < tournaments[t].registeredCount - 1 - i; j++)
        {
            a = rank[j];
            b = rank[j + 1];

            if (points[b] > points[a] ||
                (points[b] == points[a] &&
                 headToHead[b][a] > headToHead[a][b]) ||
                (points[b] == points[a] &&
                 headToHead[b][a] == headToHead[a][b] &&
                 scoreFor[b] - scoreAgainst[b] >
                 scoreFor[a] - scoreAgainst[a]) ||
                (points[b] == points[a] &&
                 headToHead[b][a] == headToHead[a][b] &&
                 scoreFor[b] - scoreAgainst[b] ==
                 scoreFor[a] - scoreAgainst[a] &&
                 strcmp(tournaments[t].registeredTeams[b],
                        tournaments[t].registeredTeams[a]) < 0))
            {
                temp = rank[j];
                rank[j] = rank[j + 1];
                rank[j + 1] = temp;
            }
        }
    }
    printf("\n========== TOURNAMENT STANDINGS ==========\n");
    printf("Tournament: %s\n", tournaments[t].name);
    printf("\n%-5s %-12s %4s %4s %4s %4s %5s %5s %5s %5s\n",
           "Rank", "Team ID", "P", "W", "D", "L",
           "GF", "GA", "GD", "Pts");

    for (i = 0; i < tournaments[t].registeredCount; i++)
    {
        j = rank[i];
        printf("%-5d %-12s %4d %4d %4d %4d %5d %5d %+5d %5d\n",
               i + 1,
               tournaments[t].registeredTeams[j],
               played[j],
               wins[j],
               draws[j],
               losses[j],
               scoreFor[j],
               scoreAgainst[j],
               scoreFor[j] - scoreAgainst[j],
               points[j]);
    }
}
void match_history(void)
{
    int i, a, b, t;

    if (matchCount == 0)
    {
        printf("\nNo match history available.\n");
        return;
    }

    printf("\n========== MATCH HISTORY ==========\n");

    for (i = 0; i < matchCount; i++)
    {
        a = find_team(matches[i].teamA);
        b = find_team(matches[i].teamB);
        t = find_tournament(matches[i].tournamentID);

        printf("\nMatch ID: %s\n", matches[i].id);

        if (t != -1)
        {
            printf("Tournament: %s\n", tournaments[t].name);
        }
        else
        {
            printf("Tournament ID: %s\n", matches[i].tournamentID);
        }

        printf("Team A: %s\n",
               (a != -1) ? teams[a].name : matches[i].teamA);

        printf("Team B: %s\n",
               (b != -1) ? teams[b].name : matches[i].teamB);

        printf("Slot: %d\n", matches[i].slot);

        if (matches[i].completed == 1)
        {
            printf("Status: Completed\n");
            printf("Score: %d - %d\n",
                   matches[i].scoreA, matches[i].scoreB);
        }
        else
        {
            printf("Status: Pending\n");
        }

        printf("-----------------------------------\n");
    }
}

// player_statistics: shows a player's details and the performance of the player's team.
void player_statistics(void)
{
    char id[20];
    int index, i, j, found = 0;

    printf("\nEnter Player ID: ");
    read_string(id, 20);

    index = find_player(id);

    if (index == -1)
    {
        printf("\nPlayer not found!\n");
        return;
    }

    printf("\n========== PLAYER STATISTICS ==========\n");
    printf("Player ID: %s\n", players[index].id);
    printf("Player Name: %s\n", players[index].name);

    for (i = 0; i < teamCount; i++)
    {
        for (j = 0; j < TEAM_SIZE; j++)
        {
            if (strcmp(teams[i].playerIDs[j], id) == 0)
            {
                printf("\nTeam ID: %s\n", teams[i].id);
                printf("Team Name: %s\n", teams[i].name);

                printf("\nTeam Performance:\n");
                printf("Matches Played: %d\n", teams[i].played);
                printf("Wins: %d\n", teams[i].wins);
                printf("Draws: %d\n", teams[i].draws);
                printf("Losses: %d\n", teams[i].losses);
                printf("Points: %d\n", teams[i].points);

                found = 1;
                break;
            }
        }

        if (found == 1)
        {
            break;
        }
    }

    if (found == 0)
    {
        printf("\nThis player is not assigned to a team.\n");
    }
}

// team_statistics: shows the full statistics of one team.
void team_statistics(void)
{
    char id[20];
    int index;

    printf("\nEnter Team ID: ");
    read_string(id, 20);

    index = find_team(id);

    if (index == -1)
    {
        printf("\nTeam not found!\n");
        return;
    }

    printf("\n========== TEAM STATISTICS ==========\n");
    printf("Team ID: %s\n", teams[index].id);
    printf("Team Name: %s\n", teams[index].name);

    printf("\nMatches Played: %d\n", teams[index].played);
    printf("Wins: %d\n", teams[index].wins);
    printf("Draws: %d\n", teams[index].draws);
    printf("Losses: %d\n", teams[index].losses);

    printf("Points: %d\n", teams[index].points);
    printf("Score For: %d\n", teams[index].scoreFor);
    printf("Score Against: %d\n", teams[index].scoreAgainst);

    printf("Score Difference: %d\n",
           teams[index].scoreFor - teams[index].scoreAgainst);
}

// search_records: small menu to search a player or a team.
void search_records(void)
{
    int choice;

    printf("\n========== SEARCH RECORDS ==========\n");
    printf("1. Search Player\n");
    printf("2. Search Team\n");
    printf("0. Back\n");
    printf("Enter choice: ");

    choice = read_integer();

    switch (choice)
    {
        case 1:
            search_player();
            break;

        case 2:
            search_team();
            break;

        case 0:
            break;

        default:
            printf("\nInvalid choice!\n");
    }
}
void reports_menu(void)
{
    int choice;

    do
    {
        printf("\n========== REPORTS & STATISTICS ==========\n");
        printf("1. Tournament Standings\n");
        printf("2. Match History\n");
        printf("3. Player Statistics\n");
        printf("4. Team Statistics\n");
        printf("5. Search Records\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");

        choice = read_integer();

        switch (choice)
        {
            case 1:
                tournament_standings();
                break;

            case 2:
                match_history();
                break;

            case 3:
                player_statistics();
                break;

            case 4:
                team_statistics();
                break;

            case 5:
                search_records();
                break;

            case 0:
                printf("\nReturning to Main Menu...\n");
                break;

            default:
                printf("\nInvalid choice. Try again.\n");
        }

    } while (choice != 0);
}
void player_team_menu(void)
{
    int choice;

    do
    {
        printf("\n========== PLAYER & TEAM MANAGEMENT ==========\n");

        printf("1. Register Player\n");
        printf("2. Create Team\n");
        printf("3. Search Player\n");
        printf("4. Update Player\n");
        printf("5. Search Team\n");
        printf("6. Update Team\n");
        printf("7. Delete Team\n");
        printf("0. Back to Main Menu\n");

        printf("\nEnter Choice: ");
        choice = read_integer();

        switch (choice)
        {
            case 1:
                register_player();
                break;

            case 2:
                create_team();
                break;

            case 3:
                search_player();
                break;

            case 4:
                update_player();
                break;

            case 5:
                search_team();
                break;

            case 6:
                update_team();
                break;

            case 7:
                delete_team();
                break;

            case 0:
                printf("\nReturning to Main Menu...\n");
                break;

            default:
                printf("\nInvalid Choice!\n");
        }

    } while (choice != 0);
}

// save_data: writes the four counters and then the used part of every array
void save_data(void)
{
    FILE *fp;
    fp = fopen("ArenaCore.dat", "wb");
    if (fp == NULL)
    {
        printf("\nUnable to open file for saving!\n");
        return;
    }

    if (fwrite(&playerCount, sizeof(int), 1, fp) != 1 ||
        fwrite(&teamCount, sizeof(int), 1, fp) != 1 ||
        fwrite(&tournamentCount, sizeof(int), 1, fp) != 1 ||
        fwrite(&matchCount, sizeof(int), 1, fp) != 1 ||
        fwrite(players, sizeof(struct Player), playerCount, fp) != playerCount ||
        fwrite(teams, sizeof(struct Team), teamCount, fp) != teamCount ||
        fwrite(tournaments, sizeof(struct Tournament), tournamentCount, fp) != tournamentCount ||
        fwrite(matches, sizeof(struct Match), matchCount, fp) != matchCount)
    {
        fclose(fp);
        printf("\nError while saving data!\n");
        return;
    }
    fclose(fp);

    printf("\nAll data saved successfully!\n");
}

// load_data: reads the data 
void load_data(void)
{
    FILE *fp;
    int savedPlayerCount;
    int savedTeamCount;
    int savedTournamentCount;
    int savedMatchCount;
    int i;
    fp = fopen("ArenaCore.dat", "rb");
    if (fp == NULL)
    {
        printf("\nNo saved data found!\n");
        return;
    }
    if (fread(&savedPlayerCount, sizeof(int), 1, fp) != 1 ||
        fread(&savedTeamCount, sizeof(int), 1, fp) != 1 ||
        fread(&savedTournamentCount, sizeof(int), 1, fp) != 1 ||
        fread(&savedMatchCount, sizeof(int), 1, fp) != 1)
    {
        fclose(fp);
        playerCount = 0;
        teamCount = 0;
        tournamentCount = 0;
        matchCount = 0;
        printf("\nSaved file is damaged!\n");
        return;
    }
    if (savedPlayerCount < 0 || savedPlayerCount > MAX_PLAYERS ||
        savedTeamCount < 0 || savedTeamCount > MAX_TEAMS ||
        savedTournamentCount < 0 || savedTournamentCount > 32 ||
        savedMatchCount < 0 || savedMatchCount > MAX_MATCHES)
    {
        fclose(fp);
        playerCount = 0;
        teamCount = 0;
        tournamentCount = 0;
        matchCount = 0;
        printf("\nInvalid data in saved file!\n");
        return;
    }
    if (fread(players, sizeof(struct Player), savedPlayerCount, fp) != savedPlayerCount ||
        fread(teams, sizeof(struct Team), savedTeamCount, fp) != savedTeamCount ||
        fread(tournaments, sizeof(struct Tournament), savedTournamentCount, fp) != savedTournamentCount ||
        fread(matches, sizeof(struct Match), savedMatchCount, fp) != savedMatchCount)
    {
        fclose(fp);
        playerCount = 0;
        teamCount = 0;
        tournamentCount = 0;
        matchCount = 0;
        printf("\nSaved file is incomplete or damaged!\n");
        return;
    }
    fclose(fp);
    for (i = 0; i < savedTournamentCount; i++)
    {
        if (tournaments[i].capacity != 8 &&
            tournaments[i].capacity != 16 &&
            tournaments[i].capacity != 32)
        {
            playerCount = 0;
            teamCount = 0;
            tournamentCount = 0;
            matchCount = 0;
            printf("\nInvalid tournament data!\n");
            return;
        }

        if (tournaments[i].registeredCount < 0 ||
            tournaments[i].registeredCount > tournaments[i].capacity ||
            tournaments[i].waitingCount < 0 ||
            tournaments[i].waitingCount > MAX_TEAMS)
        {
            playerCount = 0;
            teamCount = 0;
            tournamentCount = 0;
            matchCount = 0;
            printf("\nInvalid tournament records!\n");
            return;
        }
    }
    playerCount = savedPlayerCount;
    teamCount = savedTeamCount;
    tournamentCount = savedTournamentCount;
    matchCount = savedMatchCount;

    printf("\nAll data loaded successfully!\n");
}
void save_load_menu(void)
{
    int choice;
    do
    {
        printf("\n========== SAVE / LOAD DATA ==========\n");
        printf("1. Save Data\n");
        printf("2. Load Data\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        choice = read_integer();
        switch (choice)
        {
            case 1:
                save_data();
                break;

            case 2:
                load_data();
                break;

            case 0:
                printf("\nReturning to Main Menu...\n");
                break;

            default:
                printf("\nInvalid choice. Try again.\n");
        }

    } while (choice != 0);
}
// mainMenu: shows the main menu and opens the module chosen by the user.
int mainMenu(void){	
	int menuchoice;
	do{
	printf("Main Menu\n______________________________________________\n");
	printf("1. Player & Team Management\n2. Tournament Management\n3. Match Scheduling\n4. Results & Leaderboard\n5.Reports & Statistics\n6. Save/Load data\n0. Exit\n\n");
	printf("Enter choice (1-6)\nExit program: 0\nChoice: ");                                                                                                               
	menuchoice = read_integer();
	switch (menuchoice){
		case 1:
			player_team_menu();
			break;
		case 2:
		    tournament_menu();
		    break;
		case 3:
		    match_menu();
		    break;
		case 4:
		    result_menu();
		    break;
		case 5:
		    reports_menu();
		    break;
		case 6:
		    save_load_menu();
		    break;
		case 0:
			printf("\nProgram Closed\n______________________________________________\n");
			break;
		default:
			printf("\nInvalid Input\n______________________________________________\n");
	}
} while (menuchoice !=0);
return 0;
}
int main(){
		load_data();
	mainMenu();
	save_data();
	return 0;
}