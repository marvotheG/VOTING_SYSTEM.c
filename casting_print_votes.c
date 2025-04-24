#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Candidate names for each position
const char *positions[] = {
    "Congress Person DCIT",
    "General Delegate DCIT",
    "Male Delegate DCIT",
    "Female Delegate DCIT"
};

const char *candidates[][2] = {
    {"Peter Mutai", "Megan James"},
    {"Mercy Atieno", "Benson Williams"},
    {"Sylvia Njeri", "Wilson Maina"},
    {"Wangeci Jane", "Diddy Mwendwa"}
};

int votes[4][2] = {0};

// Load saved votes
void load_votes() {
    FILE *file = fopen("votes.dat", "rb");
    if (file) {
        fread(votes, sizeof(votes), 1, file);
        fclose(file);
    }
}

// Save votes to file
void save_votes() {
    FILE *file = fopen("votes.dat", "wb");
    if (file) {
        fwrite(votes, sizeof(votes), 1, file);
        fclose(file);
    }
}

// Include voter authentication here if this is a standalone file
int authenticate_voter(char *voter_id) {
    char entered_id[20], entered_pin[10];

    printf("Enter your ID: ");
    fgets(entered_id, sizeof(entered_id), stdin);
    entered_id[strcspn(entered_id, "\n")] = 0;

    FILE *file = fopen("voters.dat", "rb+");
    if (!file) return 0;

    Voter voter;
    while (fread(&voter, sizeof(Voter), 1, file)) {
        if (strcmp(voter.id, entered_id) == 0) {
            printf("Enter your PIN: ");
            fgets(entered_pin, sizeof(entered_pin), stdin);
            entered_pin[strcspn(entered_pin, "\n")] = 0;

            if (strcmp(voter.pin, entered_pin) == 0) {
                if (voter.has_voted) {
                    printf("You have already voted.\n");
                    fclose(file);
                    return 0;
                }
                strcpy(voter_id, voter.id);
                fseek(file, -sizeof(Voter), SEEK_CUR);
                voter.has_voted = 1;
                fwrite(&voter, sizeof(Voter), 1, file);
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}

// Voting function
void vote(const char *voter_id) {
    load_votes();

    for (int i = 0; i < 4; i++) {
        printf("\n%s:\n", positions[i]);
        printf("1. %s\n2. %s\n", candidates[i][0], candidates[i][1]);

        int choice;
        printf("Enter your choice (1 or 2): ");
        scanf("%d", &choice);
        getchar();

        if (choice != 1 && choice != 2) {
            printf("Invalid choice. Please select 1 or 2.\n");
            i--;
            continue;
        }

        votes[i][choice - 1]++;
    }

    save_votes();
    printf("Vote submitted successfully!\n");
}

// Display voting results
void display_results() {
    load_votes();

    printf("\nVoting Results:\n");
    for (int i = 0; i < 4; i++) {
        printf("\n%s\n", positions[i]);
        printf("%s: %d votes\n", candidates[i][0], votes[i][0]);
        printf("%s: %d votes\n", candidates[i][1], votes[i][1]);

        if (votes[i][0] > votes[i][1]) {
            printf("Winner: %s\n", candidates[i][0]);
        } else if (votes[i][1] > votes[i][0]) {
            printf("Winner: %s\n", candidates[i][1]);
        } else {
            printf("Result: Tie\n");
        }
    }
}

typedef struct {
    char name[50];
    char id[20];
    char pin[10];
    int has_voted;
} Voter;

int main() {
    char voter_id[20];

    if (authenticate_voter(voter_id)) {
        vote(voter_id);
        display_results();
    } else {
        printf("Authentication failed.\n");
    }

    return 0;
}