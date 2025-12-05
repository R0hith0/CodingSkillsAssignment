#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CREDENTIALFILE "credentials.txt"
#define STUDENTFILE "students.txt"

char currentUser[64];
char currentRole[64];

/* ========= FUNCTION PROTOTYPES ========= */
int loginSystem();
void mainMenu();
void adminMenu();
void userMenu();

int addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

void readLine(char *buf, int size);
/* ======================================= */
void readLine(char *buf, int size) {
    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    size_t ln = strcspn(buf, "\n");
    buf[ln] = '\0';
}

/*---------------Login System---------------*/
int loginSystem() {
    char username[64], password[64];
    char fileUser[64], filePass[64], fileRole[64];

    printf("========== Login Screen ==========\n");
    printf("Username: ");
    readLine(username, sizeof(username));
    printf("Password: ");
    readLine(password, sizeof(password));

    FILE *fp = fopen(CREDENTIALFILE, "r");
    if (!fp) {
        printf("Error: credentials.txt not found!\n");
        return 0;
    }

    int logged = 0;
    while (fscanf(fp, "%63s %63s %63s", fileUser, filePass, fileRole) == 3) {
        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            strncpy(currentRole, fileRole, sizeof(currentRole) - 1);
            currentRole[sizeof(currentRole) - 1] = '\0';
            strncpy(currentUser, fileUser, sizeof(currentUser) - 1);
            currentUser[sizeof(currentUser) - 1] = '\0';
            logged = 1;
            break;
        }
    }

    fclose(fp);
    return logged;
}

/*---------------Main Menu---------------*/
void mainMenu() {
    if (strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else
        userMenu();
}

/*---------------Admin Menu---------------*/
void adminMenu() {
    int choice;
    do {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. Add New Student\n");
        printf("2. Display All Records\n");
        printf("3. Search Records\n");
        printf("4. Update Student Details\n");
        printf("5. Delete Student Records\n");
        printf("6. Logout\n");
        printf("Enter Choice: ");

        char buf[16];
        readLine(buf, sizeof(buf));
        choice = atoi(buf);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Logging out...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 6);
}

/*---------------User Menu---------------*/
void userMenu() {
    int choice;
    do {
        printf("\n===== USER MENU =====\n");
        printf("1. View All Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter Choice: ");

        char buf[16];
        readLine(buf, sizeof(buf));
        choice = atoi(buf);

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: printf("Logging out...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 3);
}

/*---------------Add Student---------------*/
int addStudent() {
    char id[64], name[128], course[128], ageBuf[16];
    int age;

    printf("\nEnter Student ID: ");
    readLine(id, sizeof(id));
    if (strlen(id) == 0) {
        printf("ID cannot be empty.\n");
        return 0;
    }

    printf("Enter Student Name: ");
    readLine(name, sizeof(name));

    printf("Enter Age: ");
    readLine(ageBuf, sizeof(ageBuf));
    age = atoi(ageBuf);

    printf("Enter Course: ");
    readLine(course, sizeof(course));

    FILE *fp = fopen(STUDENTFILE, "a");
    if (!fp) {
        printf("Error opening file for writing!\n");
        return 0;
    }

    fprintf(fp, "%s|%s|%d|%s\n", id, name, age, course);
    fclose(fp);

    printf("\nStudent Added Successfully!\n");
    return 1;
}

/*---------------Display Students---------------*/
void displayStudents() {
    FILE *fp = fopen(STUDENTFILE, "r");
    if (!fp) {
        printf("\nNo student records found!\n");
        return;
    }

    char line[512];
    printf("\n===== ALL STUDENT RECORDS =====\n");

    while (fgets(line, sizeof(line), fp)) {
        /* remove trailing newline */
        line[strcspn(line, "\n")] = '\0';

        char *id = strtok(line, "|");
        char *name = strtok(NULL, "|");
        char *ageStr = strtok(NULL, "|");
        char *course = strtok(NULL, "|");

        if (!id || !name || !ageStr || !course) continue; 

        printf("ID: %s\nName: %s\nAge: %s\nCourse: %s\n\n", id, name, ageStr, course);
    }

    fclose(fp);
}

/*---------------Search Student---------------*/
void searchStudent() {
    char searchID[64];
    printf("\nEnter Student ID to Search: ");
    readLine(searchID, sizeof(searchID));

    FILE *fp = fopen(STUDENTFILE, "r");
    if (!fp) {
        printf("No records found!\n");
        return;
    }

    char line[512];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        char copy[512];
        strncpy(copy, line, sizeof(copy)-1);
        copy[sizeof(copy)-1] = '\0';

        char *id = strtok(copy, "|");
        char *name = strtok(NULL, "|");
        char *ageStr = strtok(NULL, "|");
        char *course = strtok(NULL, "|");

        if (!id) continue;
        if (strcmp(id, searchID) == 0) {
            printf("\nRecord Found!\n");
            printf("ID: %s\nName: %s\nAge: %s\nCourse: %s\n", id, name ? name : "", ageStr ? ageStr : "", course ? course : "");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nStudent Not Found!\n");

    fclose(fp);
}

/*---------------Update Student---------------*/
void updateStudent() {
    char searchID[64];
    printf("\nEnter Student ID to Update: ");
    readLine(searchID, sizeof(searchID));

    FILE *fp = fopen(STUDENTFILE, "r");
    FILE *temp = fopen("temp_students.txt", "w");

    if (!fp || !temp) {
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        printf("File error!\n");
        return;
    }

    char line[512];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        char copy[512];
        strncpy(copy, line, sizeof(copy)-1);
        copy[sizeof(copy)-1] = '\0';
        copy[strcspn(copy, "\n")] = '\0';

        char *id = strtok(copy, "|");
        char *name = strtok(NULL, "|");
        char *ageStr = strtok(NULL, "|");
        char *course = strtok(NULL, "|");

        if (id && strcmp(id, searchID) == 0) {
            found = 1;
            char newID[64], newName[128], newCourse[128], ageBuf[16];
            int newAge;

            printf("\nRecord Found! Enter New Details:\n");

            printf("New ID: ");
            readLine(newID, sizeof(newID));
            if (strlen(newID) == 0) strncpy(newID, id, sizeof(newID)-1);

            printf("New Name: ");
            readLine(newName, sizeof(newName));
            if (strlen(newName) == 0 && name) strncpy(newName, name, sizeof(newName)-1);

            printf("New Age: ");
            readLine(ageBuf, sizeof(ageBuf));
            if (strlen(ageBuf) == 0 && ageStr) newAge = atoi(ageStr);
            else newAge = atoi(ageBuf);

            printf("New Course: ");
            readLine(newCourse, sizeof(newCourse));
            if (strlen(newCourse) == 0 && course) strncpy(newCourse, course, sizeof(newCourse)-1);

            fprintf(temp, "%s|%s|%d|%s\n", newID, newName, newAge, newCourse);
        } else {
            fputs(line, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENTFILE);
    rename("temp_students.txt", STUDENTFILE);

    if (found)
        printf("\nRecord Updated Successfully!\n");
    else
        printf("\nStudent Not Found!\n");
}

/*---------------Delete Student---------------*/
void deleteStudent() {
    char searchID[64];
    printf("\nEnter Student ID to Delete: ");
    readLine(searchID, sizeof(searchID));

    FILE *fp = fopen(STUDENTFILE, "r");
    FILE *temp = fopen("temp_students.txt", "w");

    if (!fp || !temp) {
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        printf("File error!\n");
        return;
    }

    char line[512];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        char copy[512];
        strncpy(copy, line, sizeof(copy)-1);
        copy[sizeof(copy)-1] = '\0';
        copy[strcspn(copy, "\n")] = '\0';

        char *id = strtok(copy, "|");
        if (id && strcmp(id, searchID) == 0) {
            found = 1;
        } else {
            fputs(line, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENTFILE);
    rename("temp_students.txt", STUDENTFILE);

    if (found)
        printf("\nRecord Deleted Successfully!\n");
    else
        printf("\nStudent Not Found!\n");
}

/*---------------Main---------------*/
int main() {
    currentRole[0] = '\0';
    currentUser[0] = '\0';

    if (loginSystem()) {
        mainMenu();
    } else {
        printf("\nLogin Failed. Exiting...\n");
    }
    return 0;
}

