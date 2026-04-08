#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "students.dat"

// Structure to store student data
struct Student {
    int roll;
    char name[50];
    float marks;
};

// ─── Function Prototypes ───────────────────────────────────────
void addStudent();
void displayStudents();
void searchStudent();
void deleteStudent();
void displayMenu();

// ──────────────────────────────────────────────────────────────
//  MAIN
// ──────────────────────────────────────────────────────────────
int main() {
    int choice;

    printf("\n========================================\n");
    printf("     STUDENT MANAGEMENT SYSTEM\n");
    printf("========================================\n");

    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent();      break;
            case 2: displayStudents(); break;
            case 3: searchStudent();   break;
            case 4: deleteStudent();   break;
            case 5:
                printf("\nExiting program. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }

    } while (choice != 5);

    return 0;
}

// ──────────────────────────────────────────────────────────────
//  DISPLAY MENU
// ──────────────────────────────────────────────────────────────
void displayMenu() {
    printf("\n--- Student Management System ---\n");
    printf("1. Add Student\n");
    printf("2. Display Students\n");
    printf("3. Search Student\n");
    printf("4. Delete Student\n");
    printf("5. Exit\n");
}

// ──────────────────────────────────────────────────────────────
//  ADD STUDENT  →  Append record to binary file
// ──────────────────────────────────────────────────────────────
void addStudent() {
    struct Student s;
    FILE *fp;

    fp = fopen(FILENAME, "ab");   // ab = append binary
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nEnter Roll Number : ");
    scanf("%d", &s.roll);
    printf("Enter Name        : ");
    scanf(" %[^\n]", s.name);     // reads full name with spaces
    printf("Enter CGPA       : ");
    scanf("%f", &s.marks);

    fwrite(&s, sizeof(struct Student), 1, fp);
    fclose(fp);

    printf("Student Added Successfully!\n");
}

// ──────────────────────────────────────────────────────────────
//  DISPLAY ALL STUDENTS  →  Read and print every record
// ──────────────────────────────────────────────────────────────
void displayStudents() {
    struct Student s;
    FILE *fp;
    int count = 0;

    fp = fopen(FILENAME, "rb");   // rb = read binary
    if (fp == NULL) {
        printf("\nNo records found! File does not exist.\n");
        return;
    }

    printf("\n========================================\n");
    printf("%-10s %-20s %-10s\n", "Roll No", "Name", "Marks");
    printf("========================================\n");

    while (fread(&s, sizeof(struct Student), 1, fp) == 1) {
        printf("%-10d %-20s %-10.2f\n", s.roll, s.name, s.marks);
        count++;
    }

    fclose(fp);

    if (count == 0)
        printf("No student records found.\n");
    else
        printf("========================================\n");
        printf("Total Students: %d\n", count);
}

// ──────────────────────────────────────────────────────────────
//  SEARCH STUDENT  →  Find by roll number
// ──────────────────────────────────────────────────────────────
void searchStudent() {
    struct Student s;
    FILE *fp;
    int roll, found = 0;

    printf("\nEnter Roll Number to search: ");
    scanf("%d", &roll);

    fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    while (fread(&s, sizeof(struct Student), 1, fp) == 1) {
        if (s.roll == roll) {
            printf("\n--- Student Found ---\n");
            printf("Roll No : %d\n", s.roll);
            printf("Name    : %s\n", s.name);
            printf("Marks   : %.2f\n", s.marks);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found)
        printf("Student with Roll No %d not found!\n", roll);
}

// ──────────────────────────────────────────────────────────────
//  DELETE STUDENT  →  Copy all except target → overwrite
// ──────────────────────────────────────────────────────────────
void deleteStudent() {
    struct Student s;
    FILE *fp, *temp;
    int roll, found = 0;

    printf("\nEnter Roll Number to delete: ");
    scanf("%d", &roll);

    fp   = fopen(FILENAME, "rb");
    temp = fopen("temp.dat", "wb");

    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    // Copy all records EXCEPT the one to delete
    while (fread(&s, sizeof(struct Student), 1, fp) == 1) {
        if (s.roll == roll) {
            found = 1;           // skip this record
        } else {
            fwrite(&s, sizeof(struct Student), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(FILENAME);           // delete original
        rename("temp.dat", FILENAME); // rename temp → original
        printf("Student deleted successfully!\n");
    } else {
        remove("temp.dat");
        printf("Student with Roll No %d not found!\n", roll);
    }
}