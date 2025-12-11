#include <stdio.h>
#include <string.h>

struct Member {
    int id;
    char name[50];
    int age;
    char gender[10];
    char phone[15];
    char expiry[15];  // DD/MM/YYYY
};

struct Workout {
    int memberId;
    char date[20];
    char exercise[30];
    int sets;
    int reps;
    float weight;
    float onerm;
};

// Global arrays
struct Member members[100];
struct Workout workouts[200];

int memberCount = 0;
int workoutCount = 0;

// ----------------------------------------------------
// Helper function to clear input buffer
// ----------------------------------------------------
void clearBuffer() {
    while (getchar() != '\n');
}

// ----------------------------------------------------
// Add Member
// ----------------------------------------------------
void addMember() {
    struct Member m;

    printf("\nEnter ID: ");
    while (scanf("%d", &m.id) != 1) {
        printf("Invalid ID! Enter a number: ");
        clearBuffer();
    }
    clearBuffer();

    printf("Enter Name: ");
    scanf("%49[^\n]", m.name);
    clearBuffer();

    printf("Enter Age: ");
    while (scanf("%d", &m.age) != 1) {
        printf("Invalid Age! Enter a number: ");
        clearBuffer();
    }
    clearBuffer();

    printf("Enter Gender: ");
    scanf("%9s", m.gender);
    clearBuffer();

    printf("Enter Phone: ");
    scanf("%14s", m.phone);
    clearBuffer();

    printf("Enter Expiry Date (DD/MM/YYYY): ");
    scanf("%14s", m.expiry);
    clearBuffer();

    members[memberCount++] = m;

    printf("\nMember added successfully!\n");
}

// ----------------------------------------------------
// View All Members
// ----------------------------------------------------
void viewMembers() {
    if(memberCount == 0) {
        printf("\nNo members added yet!\n");
        return;
    }

    printf("\n--- Member List ---\n");
    for(int i = 0; i < memberCount; i++) {
        printf("\nID: %d\nName: %s\nAge: %d\nGender: %s\nPhone: %s\nExpiry: %s\n",
            members[i].id, members[i].name, members[i].age,
            members[i].gender, members[i].phone, members[i].expiry);
    }
}

// ----------------------------------------------------
// Check Expiry Function
// returns 1 = expired, 0 = active
// ----------------------------------------------------
int isExpired(char expiry[], int cd, int cm, int cy) {
    int d, m, y;
    sscanf(expiry, "%d/%d/%d", &d, &m, &y);

    if (y < cy) return 1;
    if (y == cy && m < cm) return 1;
    if (y == cy && m == cm && d < cd) return 1;

    return 0;
}

// ----------------------------------------------------
// Add Workout (BLOCK IF EXPIRED)
// ----------------------------------------------------
void addWorkout() {
    int id;
    int cd, cm, cy;

    printf("\nEnter today's date (DD MM YYYY): ");
    while (scanf("%d %d %d", &cd, &cm, &cy) != 3) {
        printf("Invalid input! Enter date as DD MM YYYY: ");
        clearBuffer();
    }
    clearBuffer();

    printf("Enter Member ID: ");
    while (scanf("%d", &id) != 1) {
        printf("Invalid ID! Enter a number: ");
        clearBuffer();
    }
    clearBuffer();

    // Find member
    int found = 0, idx = -1;

    for(int i = 0; i < memberCount; i++) {
        if(members[i].id == id) {
            found = 1;
            idx = i;
            break;
        }
    }

    if(!found) {
        printf("\n❌ Member not found!\n");
        return;
    }

    // Check expiry
    if(isExpired(members[idx].expiry, cd, cm, cy)) {
        printf("\n❌ Subscription Expired for %s!\n", members[idx].name);
        printf("❌ Cannot add workout. Please renew subscription.\n");
        return;
    }

    // If active → allow workout logging
    struct Workout w;
    w.memberId = id;

    printf("Enter Date (DD/MM/YYYY): ");
    scanf("%19s", w.date);
    clearBuffer();

    printf("Enter Exercise Name: ");
    scanf("%29[^\n]", w.exercise);
    clearBuffer();

    printf("Enter Sets: ");
    while (scanf("%d", &w.sets) != 1) {
        printf("Invalid input! Enter number: ");
        clearBuffer();
    }
    clearBuffer();

    printf("Enter Reps: ");
    while (scanf("%d", &w.reps) != 1) {
        printf("Invalid input! Enter number: ");
        clearBuffer();
    }
    clearBuffer();

    printf("Enter Weight (kg): ");
    while (scanf("%f", &w.weight) != 1) {
        printf("Invalid input! Enter weight: ");
        clearBuffer();
    }
    clearBuffer();

    w.onerm = w.weight * (1 + (float)w.reps / 30);
    workouts[workoutCount++] = w;

    printf("\nWorkout Added Successfully!\n");
    printf("Estimated 1RM = %.2f kg\n", w.onerm);
}

// ----------------------------------------------------
// View Progress
// ----------------------------------------------------
void viewProgress() {
    int id;
    int cd, cm, cy;

    printf("\nEnter today's date (DD MM YYYY): ");
    while (scanf("%d %d %d", &cd, &cm, &cy) != 3) {
        printf("Invalid input! Enter date as DD MM YYYY: ");
        clearBuffer();
    }
    clearBuffer();

    printf("Enter Member ID to view progress: ");
    while (scanf("%d", &id) != 1) {
        printf("Invalid input! Enter number: ");
        clearBuffer();
    }
    clearBuffer();

    int found = 0, idx = -1;

    for(int i = 0; i < memberCount; i++) {
        if(members[i].id == id) {
            found = 1;
            idx = i;
            break;
        }
    }

    if(!found) {
        printf("\n❌ Member not found!\n");
        return;
    }

    printf("\n--- Progress for %s (ID %d) ---\n",
           members[idx].name, members[idx].id);

    if(isExpired(members[idx].expiry, cd, cm, cy))
        printf("❌ Subscription Status: EXPIRED\n");
    else
        printf("✅ Subscription Status: ACTIVE\n");

    int hasWorkout = 0;

    for(int i = 0; i < workoutCount; i++) {
        if(workouts[i].memberId == id) {
            printf("\nDate: %s\nExercise: %s\nSets: %d  Reps: %d  Weight: %.2f\n1RM: %.2f\n",
                   workouts[i].date, workouts[i].exercise,
                   workouts[i].sets, workouts[i].reps,
                   workouts[i].weight, workouts[i].onerm);
            hasWorkout = 1;
        }
    }

    if(!hasWorkout) {
        printf("\nNo workout logs found for this member.\n");
    }
}

// ----------------------------------------------------
// MAIN MENU
// ----------------------------------------------------
int main() {
    int choice;

    while(1) {
        printf("\n=============================\n");
        printf("      Tejas Gym Project      \n");
        printf("=============================\n");
        printf("1. Add Member\n");
        printf("2. View Members\n");
        printf("3. Add Workout Log\n");
        printf("4. View Member Progress\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        while (scanf("%d", &choice) != 1) {
            printf("Invalid input! Enter a number: ");
            clearBuffer();
        }
        clearBuffer();

        switch(choice) {
            case 1: addMember(); break;
            case 2: viewMembers(); break;
            case 3: addWorkout(); break;
            case 4: viewProgress(); break;
            case 5: printf("Exiting...\n"); return 0;
            default: printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}
