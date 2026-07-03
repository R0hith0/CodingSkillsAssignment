#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100

struct Student {
    char roll[30];
    char name[30];
    int group;
};

struct Subject {
    char name[50];
    char time[20];
};

struct Subject groupA[] = {
    {"Data Structures", "09:00 - 10:00"},
    {"DBMS", "10:00 - 11:00"},
    {"OS", "11:00 - 12:00"},
    {"CN", "12:00 - 01:00"}
};

struct Subject groupB[] = {
    {"C Programming", "09:00 - 10:00"},
    {"Python", "10:00 - 11:00"},
    {"AI", "11:00 - 12:00"},
    {"ML", "12:00 - 01:00"}
};

struct Subject groupC[] = {
    {"Web Dev", "09:00 - 10:00"},
    {"DBMS", "10:00 - 11:00"},
    {"Cloud", "11:00 - 12:00"},
    {"Security", "12:00 - 01:00"}
};

char* names[] = {
    "Arjun", "Rohit", "Kiran", "Sneha", "Rahul",
    "Ananya", "Vikram", "Divya", "Aman", "Ishita"
};

int getGroup(char roll[]) {
    int sum = 0;
    for(int i = 0; roll[i] != '\0'; i++) {
        if(roll[i] >= '0' && roll[i] <= '9')
            sum += roll[i] - '0';
    }
    return sum % 3;
}

char* getName() {
    return names[rand() % 10];
}

void printSchedule(int group) {

    struct Subject *t;

    if(group == 0) t = groupA;
    else if(group == 1) t = groupB;
    else t = groupC;

    printf("\nTIMETABLE:\n");
    for(int i = 0; i < 4; i++) {
        printf("%s | %s\n", t[i].name, t[i].time);
    }
}

int main() {

    srand(time(0));

    int n;
    struct Student s[MAX];

    FILE *fp = fopen("students.txt", "r");
    if(!fp) {
        printf("File not found\n");
        return 0;
    }

    fscanf(fp, "%d", &n);

    for(int i = 0; i < n; i++) {
        fscanf(fp, "%s", s[i].roll);
        strcpy(s[i].name, getName());
        s[i].group = getGroup(s[i].roll);
    }

    fclose(fp);

    char input[30];
    printf("Enter Roll Number: ");
    scanf("%s", input);

    int found = 0;

    for(int i = 0; i < n; i++) {
        if(strcmp(s[i].roll, input) == 0) {

            printf("\nName   : %s\n", s[i].name);
            printf("Roll   : %s\n", s[i].roll);
            printf("Group  : %d\n", s[i].group);

            printSchedule(s[i].group);

            found = 1;
            break;
        }
    }

    if(!found) {
        printf("Student not found\n");
    }

    return 0;
}