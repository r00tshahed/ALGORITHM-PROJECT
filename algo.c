//Made by R00Tshahed

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_USERS 100
#define MAX_FRIENDS 10
#define INF INT_MAX
#define FILE_NAME "users_data.txt"

typedef struct {
    int userID;
    char name[50];
    int age;
    int friendList[MAX_FRIENDS];
    int friendCount;
    char post[255]; // Adding a field for posts
} User;

User users[MAX_USERS];
int userCount = 0;
int adjMatrix[MAX_USERS][MAX_USERS];

// Function prototypes
void clearScreen();
void addUser();
void loadUsers();
void saveUsers();
void viewUsers();
void addFriendship();
void displayFriendsNetwork(int userID);
void createPost();
void displayPost();
void findMutualFriends(int userID1, int userID2);
void searchByUserName();
void clearInputBuffer();

int main() {
    int choice, userID1, userID2;
    loadUsers(); // Load users from file at the start

    while (1) {
        clearScreen();
        printf("\033[1;35m===================================\033[0m\n");
        printf("\033[1;34m====== Social Media Platform ======\033[0m\n");
        printf("\033[1;35m===================================\033[0m\n");

        printf("\033[1;32m1. Add User\033[0m\n");
        printf("\033[1;33m2. View All Users\033[0m\n");
        printf("\033[1;36m3. Add Friendship\033[0m\n");
        printf("\033[1;32m4. Display Friend Network\033[0m\n");
        printf("\033[1;33m5. Create Post\033[0m\n");
        printf("\033[1;36m6. Display Post\033[0m\n");
        printf("\033[1;31m7. Find Mutual Friends\033[0m\n");
        printf("\033[1;32m8. Search by User Name\033[0m\n");
        printf("\033[1;31m9. Log out\033[0m\n");
        printf("\033[1;35m===================================\033[0m\n");
        printf("\033[1;36mEnter your choice: \033[0m");

        scanf("%d", &choice);
        clearScreen();

        switch (choice) {
            case 1:
                addUser();
                break;
            case 2:
                viewUsers();
                break;
            case 3:
                addFriendship();
                break;
            case 4:
                printf("Enter User ID for friend network: ");
                scanf("%d", &userID1);
                clearScreen();
                displayFriendsNetwork(userID1);
                break;
            case 5:
                createPost();
                break;
            case 6:
                displayPost();
                break;
            case 7:
                printf("Enter two User IDs to find mutual friends: ");
                scanf("%d %d", &userID1, &userID2);
                clearScreen();
                findMutualFriends(userID1, userID2);
                break;
            case 8:
                searchByUserName();
                break;
            case 9:
                printf("\033[1;31mLogging out... Thank you for using the platform!\033[0m\n");
                saveUsers(); // Save users to file before exiting
                exit(0);
            default:
                printf("\033[1;31mInvalid choice! Please try again.\033[0m\n");
        }
        printf("\n\033[1;36mPress Enter to continue...\033[0m");
        getchar();  // Consume extra newline character left by scanf
        getchar();  // Wait for the user to press Enter
    }
    return 0;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void addUser() {
    if (userCount >= MAX_USERS) {
        printf("Maximum number of users reached!\n");
        return;
    }
    printf("Enter User ID: ");
    scanf("%d", &users[userCount].userID);
    printf("Enter Name: ");
    getchar(); // Consume newline character
    fgets(users[userCount].name, sizeof(users[userCount].name), stdin);
    users[userCount].name[strcspn(users[userCount].name, "\n")] = 0; // Remove newline
    printf("Enter Age: ");
    scanf("%d", &users[userCount].age);

    // Initialize the user's friend list
    users[userCount].friendCount = 0;
    // Initialize post as empty string
    users[userCount].post[0] = '\0';

    for (int i = 0; i < MAX_USERS; i++) {
        adjMatrix[userCount][i] = INF;
        adjMatrix[i][userCount] = INF;
    }

    userCount++;
    printf("User added successfully!\n");
}

void loadUsers() {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        printf("No previous data found. Starting fresh.\n");
        return; // No users saved yet, return early
    }

    while (fscanf(file, "%d %49[^\n] %d\n", &users[userCount].userID, users[userCount].name, &users[userCount].age) == 3) {
        // Load the post
        fgets(users[userCount].post, sizeof(users[userCount].post), file);
        users[userCount].post[strcspn(users[userCount].post, "\n")] = 0;  // Remove newline

        // Load the number of friends
        fscanf(file, "%d", &users[userCount].friendCount);

        // Load the friends list
        for (int i = 0; i < users[userCount].friendCount; i++) {
            fscanf(file, "%d", &users[userCount].friendList[i]);
        }

        // Initialize the adjacency matrix for friendships
        for (int i = 0; i < MAX_USERS; i++) {
            adjMatrix[userCount][i] = INF;
            adjMatrix[i][userCount] = INF;
        }

        userCount++;
    }

    fclose(file);
    printf("User data loaded successfully!\n");
}

void saveUsers() {
    FILE *file = fopen(FILE_NAME, "w");
    if (file == NULL) {
        printf("Error saving user data!\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        // Save basic user data
        fprintf(file, "%d %s %d\n", users[i].userID, users[i].name, users[i].age);

        // Save the post (if any)
        fprintf(file, "%s\n", users[i].post);

        // Save friends
        fprintf(file, "%d ", users[i].friendCount); // Save friend count first
        for (int j = 0; j < users[i].friendCount; j++) {
            fprintf(file, "%d ", users[i].friendList[j]);
        }
        fprintf(file, "\n"); // End of friend's line
    }

    fclose(file);
    printf("User data saved successfully!\n");
}

void viewUsers() {
    if (userCount == 0) {
        printf("No users available!\n");
        return;
    }

    // Bubble sort the users by User ID
    for (int i = 0; i < userCount - 1; i++) {
        for (int j = 0; j < userCount - i - 1; j++) {
            if (users[j].userID > users[j + 1].userID) {
                // Swap the user details
                User temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
            }
        }
    }

    printf("====== User List (Sorted by User ID) ======\n");
    for (int i = 0; i < userCount; i++) {
        printf("User ID: %d, Name: %s, Age: %d\n",
               users[i].userID, users[i].name, users[i].age);
    }
}

void addFriendship() {
    int id1, id2;
    printf("Enter first User ID: ");
    scanf("%d", &id1);
    printf("Enter second User ID: ");
    scanf("%d", &id2);

    int idx1 = -1, idx2 = -1;
    for (int i = 0; i < userCount; i++) {
        if (users[i].userID == id1) idx1 = i;
        if (users[i].userID == id2) idx2 = i;
    }

    if (idx1 == -1 || idx2 == -1) {
        printf("One or both users do not exist!\n");
        return;
    }

    adjMatrix[idx1][idx2] = 1;
    adjMatrix[idx2][idx1] = 1;
    users[idx1].friendList[users[idx1].friendCount++] = id2;
    users[idx2].friendList[users[idx2].friendCount++] = id1;

    printf("Friendship added successfully!\n");
}

void displayFriendsNetwork(int userID) {
    int idx = -1;
    for (int i = 0; i < userCount; i++) {
        if (users[i].userID == userID) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("User not found!\n");
        return;
    }

    printf("Friend Network of %s (ID: %d):\n", users[idx].name, userID);
    if (users[idx].friendCount == 0) {
        printf("No existing friends found.\n");
        return;
    }

    for (int i = 0; i < users[idx].friendCount; i++) {
        int friendID = users[idx].friendList[i];
        printf("Friend ID: %d\n", friendID);
    }
}

void createPost() {
    int userID;
    printf("Enter User ID to create a post: ");
    scanf("%d", &userID);

    int idx = -1;
    for (int i = 0; i < userCount; i++) {
        if (users[i].userID == userID) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("User not found!\n");
        return;
    }

    printf("Enter your post: ");
    clearInputBuffer();
    fgets(users[idx].post, sizeof(users[idx].post), stdin);
    users[idx].post[strcspn(users[idx].post, "\n")] = 0;  // Remove newline
    printf("Post created successfully!\n");
}

void displayPost() {
    int userID;
    printf("Enter User ID to display the post: ");
    scanf("%d", &userID);

    int idx = -1;
    for (int i = 0; i < userCount; i++) {
        if (users[i].userID == userID) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("User not found!\n");
        return;
    }

    if (strlen(users[idx].post) == 0) {
        printf("No post available for %s (ID: %d)\n", users[idx].name, userID);
    } else {
        printf("Post by %s (ID: %d):\n%s\n", users[idx].name, userID, users[idx].post);
    }
}

void findMutualFriends(int userID1, int userID2) {
    int idx1 = -1, idx2 = -1;
    for (int i = 0; i < userCount; i++) {
        if (users[i].userID == userID1) idx1 = i;
        if (users[i].userID == userID2) idx2 = i;
    }

    if (idx1 == -1 || idx2 == -1) {
        printf("One or both users not found!\n");
        return;
    }

    printf("Mutual friends between %d and %d:\n", userID1, userID2);
    int found = 0;
    for (int i = 0; i < users[idx1].friendCount; i++) {
        for (int j = 0; j < users[idx2].friendCount; j++) {
            if (users[idx1].friendList[i] == users[idx2].friendList[j]) {
                printf("Friend ID: %d\n", users[idx1].friendList[i]);
                found = 1;
            }
        }
    }
    if (!found) {
        printf("No mutual friends found!\n");
    }
}

void searchByUserName() {
    char name[50];
    printf("Enter name to search: ");
    clearInputBuffer();
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;  // Remove newline

    int found = 0;
    for (int i = 0; i < userCount; i++) {
        if (strstr(users[i].name, name)) {
            printf("User ID: %d, Name: %s, Age: %d\n", users[i].userID, users[i].name, users[i].age);
            found = 1;
        }
    }

    if (!found) {
        printf("No users found with the name \"%s\"!\n", name);
    }
}

void clearInputBuffer() {
    while (getchar() != '\n');  // Clear the input buffer
}
