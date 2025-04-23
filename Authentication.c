#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the Voter structure
typedef struct {
    char name[50];
    char id[20];      // Unique voter ID
    char pin[10];     // Encrypted PIN
    int has_voted;    // 0 = not voted, 1 = voted
} Voter;

// Function to authenticate a voter
int authenticate_voter() {
    char entered_id[20];
    char entered_pin[10];
    int attempt_count = 0;

    while (attempt_count < 3) {
        // Prompt for ID
        printf("Enter your ID: ");
        if (fgets(entered_id, sizeof(entered_id), stdin) == NULL) {
            printf("Error reading input.\n");
            return 0;
        }
        entered_id[strcspn(entered_id, "\n")] = 0; // Remove newline

        // Open voters.dat file for reading
        FILE *file = fopen("voters.dat", "rb");
        if (!file) {
            perror("Error opening voters database");
            return 0;
        }

        Voter voter;
        int found_id = 0;

        // Search for the voter in the file
        while (fread(&voter, sizeof(Voter), 1, file)) {
            if (strcmp(voter.id, entered_id) == 0) {
                found_id = 1;

                // Prompt for PIN only if ID is found
                printf("Enter your PIN: ");
                if (fgets(entered_pin, sizeof(entered_pin), stdin) == NULL) {
                    printf("Error reading input.\n");
                    fclose(file);
                    return 0;
                }
                entered_pin[strcspn(entered_pin, "\n")] = 0; // Remove newline

                // Validate PIN
                if (strcmp(voter.pin, entered_pin) == 0) {
                    fclose(file);

                    // Check if the voter has already voted
                    if (voter.has_voted) {
                        printf("Error: You have already voted.\n");
                        return 0;
                    }

                    // Successful authentication
                    printf("Authentication successful. Welcome, %s!\n", voter.name);
                    return 1;
                } else {
                    printf("Error: Incorrect PIN. Please try again.\n");
                    fclose(file);
                    attempt_count++;
                    break; // Go back to start (ask for ID again)
                }
            }
        }

        if (!found_id) {
            printf("Error: Incorrect ID. Please try again.\n");
            attempt_count++;
        }

        fclose(file);
    }

    printf("Error: Too many failed attempts. Authentication failed.\n");
    return 0;
}

int main() {
    printf("Welcome to the Voting System!\n");

    // Call the voter authentication function
    int auth_result = authenticate_voter();
    if (auth_result) {
        printf("Proceeding to the next step...\n");
    } else {
        printf("Exiting system.\n");
    }

    return 0;
}
