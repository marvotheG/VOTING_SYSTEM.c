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

// Function to register a voter
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

int main() {
    register_voter();
    return 0;
}