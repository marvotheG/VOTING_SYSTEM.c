#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define Voter structure
typedef struct {
    char name[50];
    char id[20];
    char pin[10];
    int has_voted;
} Voter;

// Candidate positions and names
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

// Vote storage
int votes[4][2] = {0};

// Function to check if ID is unique
int is_unique_id(const char *id_to_check) {
    FILE *file = fopen("voters.dat", "rb");
    if (!file) return 1;

    Voter voter;
    while (fread(&voter, sizeof(Voter), 1, file)) {
        if (strcmp(voter.id, id_to_check) == 0) {
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return 1;
}

// Function to register voter
void register_voter() {
    Voter new_voter;

    printf("Enter your name: ");
    fgets(new_voter.name, sizeof(new_voter.name), stdin);
    new_voter.name[strcspn(new_voter.name, "\n")] = 0;

    printf("Enter your ID: ");
    fgets(new_voter.id, sizeof(new_voter.id), stdin);
    new_voter.id[strcspn(new_voter.id, "\n")] = 0;

    if (!is_unique_id(new_voter.id)) {
        printf("Error: ID already registered.\n");
        return;
    }

    printf("Enter your PIN: ");
    fgets(new_voter.pin, sizeof(new_voter.pin), stdin);
    new_voter.pin[strcspn(new_voter.pin, "\n")] = 0;

    new_voter.has_voted = 0;

    FILE *file = fopen("voters.dat", "ab");
    if (!file) {
        printf("Error saving voter.\n");
        return;
    }

    fwrite(&new_voter, sizeof(Voter), 1, file);
    fclose(file);

    printf("Registration successful!\n");
}

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

// Authenticate voter and mark as voted
int authenticate_voter(char *voter_id) {
    char entered_id[20], entered_pin[10];

    printf("Enter your ID: ");
    fgets(entered_id, sizeof(entered_id), stdin);
    entered_id[strcspn(entered_id, "\n")] = 0;

    FILE *file = fopen("voters.dat", "rb+");
    if (!file) {
        printf("Could not open voter database.\n");
        return 0;
    }

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
            } else {
                printf("Incorrect PIN.\n");
                fclose(file);
                return 0;
            }
        }
    }

    fclose(file);
    printf("Voter ID not found.\n");
    return 0;
}

// Voting function
void vote(const char *voter_id) {
    load_votes();

    for (int i = 0; i < 4; i++) {
        printf("\n%s:\n", positions[i]);
        printf("1. %s\n2. %s\n", candidates[i][0], candidates[i][1]);

        int choice;
        while (1) {
            printf("Enter your choice (1 or 2): ");
            scanf("%d", &choice);
            getchar();

            if (choice == 1 || choice == 2) {
                votes[i][choice - 1]++;
                break;
            } else {
                printf("Invalid candidate choice. Please select 1 or 2.\n");
            }
        }
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

// Main program loop
int main() {
    int choice;
    char voter_id[20];

    while (1) {
        printf("\nOnline Voting System\n");
        printf("1. Register Voter\n");
        printf("2. Vote\n");
        printf("3. View Results\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clear newline

        switch (choice) {
            case 1:
                register_voter();
                break;
            case 2:
                if (authenticate_voter(voter_id)) {
                    vote(voter_id);
                }
                break;
            case 3:
                display_results();
                break;
            case 4:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid option. Try again.\n");
        }
    }

    return 0;
}