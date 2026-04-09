#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "students.dat"
#define SUBJECTS 5

char subjectNames[SUBJECTS][20] = {"OS", "C Programming", "Data Science", "AI", "Maths"};



struct Student {
    int  roll;
    char name[50];
    float marks[SUBJECTS];          
    int  totalClasses[SUBJECTS];    
    int  attendedClasses[SUBJECTS]; 
};


void addStudent();
void searchStudent();
void deleteStudent();
void addMarks();
void addAttendance();
void showReport();
void displayMenu();
float calculateCGPA(float marks[]);
float getGradePoint(float mark);


int main() {
    int choice;

    printf("\n========================================\n");
    printf("      STUDENT MANAGEMENT SYSTEM\n");
    printf("========================================\n");

    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent();    break;
            case 2: searchStudent(); break;
            case 3: deleteStudent(); break;
            case 4: addMarks();      break;
            case 5: addAttendance(); break;
            case 6: showReport();    break;
            case 7: printf("\nExiting... Goodbye!\n"); break;
            default: printf("\nInvalid choice! Try again.\n");
        }

    } while (choice != 7);

    return 0;
}


void displayMenu() {
    printf("\n--- Student Management System ---\n");
    printf("1. Add Student\n");
    printf("2. Search Student\n");
    printf("3. Delete Student\n");
    printf("4. Add / Update Marks\n");
    printf("5. Add / Update Attendance\n");
    printf("6. Show Full Report Of Student\n");
    printf("7. Exit\n");
}


void addStudent() {
    struct Student s;
    FILE *fp;
    int i;

    fp = fopen(FILENAME, "ab");
    if (fp == NULL) { printf("Error opening file!\n"); return; }

    printf("\nEnter Roll Number : ");
    scanf("%d", &s.roll);
    printf("Enter Name        : ");
    scanf(" %[^\n]", s.name);

    
    for (i = 0; i < SUBJECTS; i++) {
        s.marks[i]          = 0;
        s.totalClasses[i]   = 0;
        s.attendedClasses[i] = 0;
    }

    fwrite(&s, sizeof(struct Student), 1, fp);
    fclose(fp);

    printf("Student Added Successfully!\n");
}


void searchStudent() {
    struct Student s;
    FILE *fp;
    int roll, found = 0;

    printf("\nEnter Roll Number to search: ");
    scanf("%d", &roll);

    fp = fopen(FILENAME, "rb");
    if (fp == NULL) { printf("No records found!\n"); return; }

    while (fread(&s, sizeof(struct Student), 1, fp) == 1) {
        if (s.roll == roll) {
            printf("\n--- Student Found ---\n");
            printf("Roll No : %d\n", s.roll);
            printf("Name    : %s\n", s.name);
            found = 1;
            break;
        }
    }

    fclose(fp);
    if (!found) printf("Roll No %d not found!\n", roll);
}


void deleteStudent() {
    struct Student s;
    FILE *fp, *temp;
    int roll, found = 0;

    printf("\nEnter Roll Number to delete: ");
    scanf("%d", &roll);

    fp   = fopen(FILENAME, "rb");
    temp = fopen("temp.dat", "wb");

    if (fp == NULL) { printf("No records found!\n"); return; }

    while (fread(&s, sizeof(struct Student), 1, fp) == 1) {
        if (s.roll == roll) found = 1;
        else fwrite(&s, sizeof(struct Student), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(FILENAME);
        rename("temp.dat", FILENAME);
        printf("Student deleted successfully!\n");
    } else {
        remove("temp.dat");
        printf("Roll No %d not found!\n", roll);
    }
}



void addMarks() {
    struct Student s;
    FILE *fp;
    int roll, found = 0, i;
    long pos;

    printf("\nEnter Roll Number: ");
    scanf("%d", &roll);

    fp = fopen(FILENAME, "r+b");
    if (fp == NULL) { printf("No records found!\n"); return; }

    while (fread(&s, sizeof(struct Student), 1, fp) == 1) {
        if (s.roll == roll) {
            found = 1;
            printf("\nEnter marks (out of 100) for each subject:\n");
            for (i = 0; i < SUBJECTS; i++) {
                printf("  %-15s : ", subjectNames[i]);
                scanf("%f", &s.marks[i]);
            }
           
            pos = ftell(fp) - sizeof(struct Student);
            fseek(fp, pos, SEEK_SET);
            fwrite(&s, sizeof(struct Student), 1, fp);
            printf("Marks updated successfully!\n");
            break;
        }
    }

    fclose(fp);
    if (!found) printf("Roll No %d not found!\n", roll);
}


void addAttendance() {
    struct Student s;
    FILE *fp;
    int roll, found = 0, i;
    long pos;

    printf("\nEnter Roll Number: ");
    scanf("%d", &roll);

    fp = fopen(FILENAME, "r+b");
    if (fp == NULL) { printf("No records found!\n"); return; }

    while (fread(&s, sizeof(struct Student), 1, fp) == 1) {
        if (s.roll == roll) {
            found = 1;
            printf("\nEnter attendance for each subject:\n");
            for (i = 0; i < SUBJECTS; i++) {
                printf("\n  Subject: %s\n", subjectNames[i]);
                printf("  Total Classes Held    : ");
                scanf("%d", &s.totalClasses[i]);
                printf("  Classes Attended      : ");
                scanf("%d", &s.attendedClasses[i]);

                
                if (s.attendedClasses[i] > s.totalClasses[i]) {
                    printf("  Attended > Total! Setting attended = total.\n");
                    s.attendedClasses[i] = s.totalClasses[i];
                }
            }
            pos = ftell(fp) - sizeof(struct Student);
            fseek(fp, pos, SEEK_SET);
            fwrite(&s, sizeof(struct Student), 1, fp);
            printf("\nAttendance updated successfully!\n");
            break;
        }
    }

    fclose(fp);
    if (!found) printf("Roll No %d not found!\n", roll);
}


float getGradePoint(float mark) {
    if (mark >= 90) return 10.0;
    else if (mark >= 80) return 9.0;
    else if (mark >= 70) return 8.0;
    else if (mark >= 60) return 7.0;
    else if (mark >= 50) return 6.0;
    else if (mark >= 40) return 5.0;
    else return 0.0;  // Fail
}


float calculateCGPA(float marks[]) {
    float total = 0;
    int i;
    for (i = 0; i < SUBJECTS; i++)
        total += getGradePoint(marks[i]);
    return total / SUBJECTS;
}


void showReport() {
    struct Student s;
    FILE *fp;
    int roll, found = 0, i;
    float attPct, cgpa, gp;

    printf("\nEnter Roll Number: ");
    scanf("%d", &roll);

    fp = fopen(FILENAME, "rb");
    if (fp == NULL) { printf("No records found!\n"); return; }

    while (fread(&s, sizeof(struct Student), 1, fp) == 1) {
        if (s.roll == roll) {
            found = 1;

            printf("\n========================================\n");
            printf("         STUDENT REPORT CARD\n");
            printf("========================================\n");
            printf("Roll No : %d\n", s.roll);
            printf("Name    : %s\n", s.name);

           
            printf("\n--- Marks & Grade Points ---\n");
            printf("%-18s %-10s %-12s %s\n", "Subject", "Marks", "Grade Pt", "Grade");
            printf("----------------------------------------------------\n");

            for (i = 0; i < SUBJECTS; i++) {
                gp = getGradePoint(s.marks[i]);
                char grade[3];
                if      (gp == 10) strcpy(grade, "O");
                else if (gp == 9)  strcpy(grade, "A+");
                else if (gp == 8)  strcpy(grade, "A");
                else if (gp == 7)  strcpy(grade, "B+");
                else if (gp == 6)  strcpy(grade, "B");
                else if (gp == 5)  strcpy(grade, "C");
                else               strcpy(grade, "F");

                printf("%-18s %-10.2f %-12.1f %s\n",
                       subjectNames[i], s.marks[i], gp, grade);
            }

            cgpa = calculateCGPA(s.marks);
            printf("----------------------------------------------------\n");
            printf("CGPA : %.2f / 10.0\n", cgpa);

            
            printf("\n--- Attendance ---\n");
            printf("%-18s %-10s %-10s %s\n", "Subject", "Total", "Attended", "Percentage");
            printf("----------------------------------------------------\n");

            for (i = 0; i < SUBJECTS; i++) {
                if (s.totalClasses[i] > 0)
                    attPct = ((float)s.attendedClasses[i] / s.totalClasses[i]) * 100;
                else
                    attPct = 0;

                printf("%-18s %-10d %-10d %.1f%%",
                       subjectNames[i], s.totalClasses[i],
                       s.attendedClasses[i], attPct);

                if (attPct < 75)
                    printf("  *** LOW ATTENDANCE ***");
                printf("\n");
            }

            printf("========================================\n");
            break;
        }
    }

    fclose(fp);
    if (!found) printf("Roll No %d not found!\n", roll);
}
