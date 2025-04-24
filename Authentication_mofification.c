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

// Function to authenticate voter
int authenticate_voter(char *voter_id) {
    char entered_id[20];
    char entered_pin[10];

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
                printf("Authentication successful.\n");
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

int main() {
    char voter_id[20];
    if (authenticate_voter(voter_id)) {
        printf("You can now vote.\n");
    }
    return 0;
}