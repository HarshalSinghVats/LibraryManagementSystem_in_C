#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structures for different purposes...
struct library {
    int bookNum;
    char bookName[150];
    char authName[100];
    int pages;
    int bookQuantity;
    int currentQuantity;
    int flag;
} book;

struct student {
    int stdId;
    char stdName[100];
    int class;
    char section;
    int booksIssued;
    int flag;
} std;

struct issue {
    int bookNum;
    int studId;
    char studName[150];
    char bookName[150];
    int bookRet; // value will be 1 while book is issued and 0 if the book has been returned...
} bookIssue;

// making file pointers...
FILE *fp;
FILE *student;
FILE *issuing;

// main-menu functions...
void bookSection();
void studentSection();
void transactionSection();

// books section functions...
void addBookRecord();
void viewBookRecord();
void deleteBookRecord();
void modifyStocksRecord();
void bookCount();
void totalBooks();
void viewAllBooksRecord();

// students section functions...
void addStudentsRecords();
void viewStudentsRecords();
void deleteStudentsRecords();
void booksIssued();

// transaction section functions...
void issueBook();
void returnBook();


int main() {
    // opening DAT files...
    fp = fopen("libraryRecords.DAT", "rb+");
    if (fp == NULL) {
        fp = fopen("libraryRecords.DAT", "wb+");
        if (fp == NULL) {
            perror("Error opening file libraryRecords.DAT");
            exit(1);
        }
    }

    student = fopen("libraryStudent.DAT", "rb+");
    if (student == NULL) {
        student = fopen("libraryStudent.DAT", "wb+");
        if (student == NULL) {
            perror("Error opening file libraryStudent.DAT");
            exit(1);
        }
    }

    issuing = fopen("libraryIssues.DAT", "rb+");
    if (issuing == NULL) {
        issuing = fopen("libraryIssues.DAT", "wb+");
        if (issuing == NULL) {
            perror("Error opening file libraryIssues.DAT");
            exit(1);
        }
    }

    // main part of interface...
    int choice = 0;
    while (1) {
        printf("\nWELCOME TO LIBRARY MANAGEMENT SYSTEM\n\n");
        printf("CHOOSE :\n1.BOOKS\n2.STUDENTS\n3.TRANSACTIONS\nENTER ANY OTHER KEY TO EXIT\n\n");
        scanf("%d", &choice);
        system("cls");

        switch (choice) {
            case 1:
                bookSection();
                break;
            case 2:
                studentSection();
                break;
            case 3:
                transactionSection();
                break;
            default:
                fclose(fp);
                fclose(student);
                fclose(issuing);
                return 0;
        }
    }
}

// book section functions...
void bookSection() {
    int bookChoice = 0;
    while (1) {
        printf("CHOOSE :\n1.ADD BOOKS\n2.VIEW INDIVIDUAL BOOKS\n3.DELETE BOOKS\n4.BOOKS PURCHASED\n5.VIEW BOOK STOCKS\n6.TOTAL BOOKS\n7.VIEW ALL BOOKS\nENTER ANY OTHER KEY TO MAIN MENU\n\n");
        scanf("%d", &bookChoice);
        system("cls");

        switch (bookChoice) {
            case 1:
                addBookRecord();
                break;
            case 2:
                viewBookRecord();
                break;
            case 3:
                deleteBookRecord();
                break;
            case 4:
                rewind(fp);
                modifyStocksRecord();
                break;
            case 5:
                bookCount();
                break;
            case 6:
                totalBooks();
                break;
            case 7:
                viewAllBooksRecord();
                break;
            default:
                printf("\nEXITING TO MAIN MENU\n\n");
                return;
        }
    }
}

// student section...
void studentSection() {
    int studChoice = 0;
    while (1) {
        printf("\nCHOOSE :\n1.ADD STUDENT RECORDS\n2.VIEW STUDENT RECORDS\n3.DELETE STUDENT RECORDS\n4.BOOKS ISSUED\nENTER ANY OTHER KEY TO RETURN TO MAIN MENU\n\n");
        scanf("%d", &studChoice);
        system("cls");

        switch (studChoice) {
            case 1:
                addStudentsRecords();
                break;
            case 2:
                viewStudentsRecords();
                break;
            case 3:
                deleteStudentsRecords();
                break;
            case 4:
                booksIssued();
                break;
            default:
                printf("\nEXITING TO MAIN MENU\n\n");
                return;
        }
    }
}

// transaction section...
void transactionSection() {
    int transactChoice = 0;
    while (1) {
        printf("CHOOSE :\n1.ISSUE BOOK\n2.RETURN BOOK\nENTER ANY OTHER KEY TO RETURN TO MAIN MENU\n\n");
        scanf("%d", &transactChoice);
        system("cls");

        switch (transactChoice) {
            case 1:
                issueBook();
                break;
            case 2:
                returnBook();
                break;
            default:
                printf("EXITING TO MAIN MENU\n");
                return;
        }
    }
}

// book section functions...
void addBookRecord() {
    system("cls");
    fseek(fp, 0, SEEK_END);

    printf("\nADD DETAILS :\n");
    printf("Enter book number : ");
    scanf("%d", &book.bookNum);

    printf("Enter book name : ");
    fflush(stdin);
    fgets(book.bookName, sizeof(book.bookName), stdin);

    printf("Enter author name : ");
    fflush(stdin);
    fgets(book.authName, sizeof(book.authName), stdin);

    printf("Enter number of pages : ");
    scanf("%d", &book.pages);

    book.flag = 0;
    book.currentQuantity = 0;
    book.bookQuantity = 0;

    fwrite(&book, sizeof(book), 1, fp);
    fflush(stdin);
    printf("\n");
    return;
}

void viewBookRecord() {
    int x;
    printf("Enter book number : ");
    scanf("%d", &x);
    system("cls");
    int count = 0;
    rewind(fp);
    while (fread(&book, sizeof(book), 1, fp) == 1) {
        if (x == book.bookNum && book.flag == 0) {
            printf("\nBook number : %d\nBook name : %s\nAuthor name : %s\nPages : %d\n\n", book.bookNum, book.bookName, book.authName, book.pages);
            count = 1;
            break;
        }
    }
    if (!count) {
        printf("Book not found.\n");
    }
    printf("\n");
    return;
}

void deleteBookRecord() {
    rewind(fp);
    int bookCheck;
    printf("Enter book number to delete : ");
    scanf("%d", &bookCheck);

    while (fread(&book, sizeof(book), 1, fp) == 1) {
        if (bookCheck == book.bookNum) {
            book.flag = 1;
            fseek(fp, -sizeof(book), SEEK_CUR);
            fwrite(&book, sizeof(book), 1, fp);
            printf("Book deleted successfully.\n");
            break;
        }
    }
    return;
}

void modifyStocksRecord() {
    system("cls");
    int bookCheck;
    printf("Enter book number : ");
    scanf("%d", &bookCheck);
    int bookCount;
    while (fread(&book, sizeof(book), 1, fp) == 1) {
        if (bookCheck == book.bookNum && book.flag == 0) {
            printf("\nEnter book quantity : ");
            fflush(stdin);
            scanf("%d", &bookCount);
            book.bookQuantity += bookCount;
            book.currentQuantity += bookCount;
            fseek(fp, -sizeof(book), SEEK_CUR);
            fwrite(&book, sizeof(book), 1, fp);
            fflush(stdin);
            printf("\n");
            break;
        }
    }
    return;
}

void bookCount() {
    system("cls");
    int k;
    printf("Enter book number : ");
    scanf("%d", &k);
    rewind(fp);

    while (fread(&book, sizeof(book), 1, fp) == 1) {
        if (k == book.bookNum && book.flag == 0) {
            printf("\nTotal number of books available : %d\n", book.bookQuantity);
            printf("\nNumber of books available : %d\n\n", book.currentQuantity);
            break;
        }
    }
    return;
}

void totalBooks() {
    system("cls");
    int bookCount = 0;
    rewind(fp);
    while (fread(&book, sizeof(book), 1, fp) == 1) {
        if (book.flag != 1)
            bookCount += book.bookQuantity;
    }
    printf("\nNumber of books in library : %d\n\n", bookCount);
    return;
}

void viewAllBooksRecord() {
    system("cls");
    rewind(fp);

    while (fread(&book, sizeof(book), 1, fp) == 1) {
        if (book.flag != 1)
            printf("\nBook number : %d\nBook name : %s\nAuthor name : %s\nPages : %d\n\n", book.bookNum, book.bookName, book.authName, book.pages);
    }
    return;
}

// student section functions...
void addStudentsRecords() {
    fseek(student, 0, SEEK_END);
    printf("Enter student ID : ");
    scanf("%d", &std.stdId);

    printf("Enter student name : ");
    fflush(stdin);
    fgets(std.stdName, sizeof(std.stdName), stdin);

    printf("Enter student class : ");
    scanf("%d", &std.class);

    printf("Enter section : ");
    fflush(stdin);
    scanf("%c", &std.section);

    std.flag = 0;
    std.booksIssued = 0;

    fwrite(&std, sizeof(std), 1, student);
    fflush(stdin);
    printf("\n");
}

void viewStudentsRecords() {
    rewind(student);
    int idCheck;
    printf("Enter student ID : ");
    scanf("%d", &idCheck);

    while (fread(&std, sizeof(std), 1, student) == 1) {
        if (idCheck == std.stdId && std.flag == 0) {
            printf("\nStudent ID : %d\nStudent Name : %s\nClass : %d\nSection : %c\nBooks Issued : %d\n", std.stdId, std.stdName, std.class, std.section, std.booksIssued);
            break;
        }
    }
    return;
}

void deleteStudentsRecords() {
    rewind(student);
    int studCheck;
    printf("Enter student ID to delete : ");
    scanf("%d", &studCheck);

    while (fread(&std, sizeof(std), 1, student) == 1) {
        if (studCheck == std.stdId) {
            std.flag = 1;
            fseek(student, -sizeof(std), SEEK_CUR);
            fwrite(&std, sizeof(std), 1, student);
            printf("Student deleted successfully.\n");
            break;
        }
    }
    return;
}

void booksIssued() {
    rewind(student);
    int studentCheck;
    printf("Enter student ID : ");
    scanf("%d", &studentCheck);
    while (fread(&std, sizeof(std), 1, student) == 1) {
        if (studentCheck == std.stdId) {
            printf("\nBooks issued by student are : %d\n\n", std.booksIssued);
            break;
        }
    }
    return;
}

// transaction section functions...
void issueBook() {
    rewind(fp);
    rewind(student);
    rewind(issuing);
    int studCheck;
    printf("Enter student ID : ");
    scanf("%d", &studCheck);

    while (fread(&std, sizeof(std), 1, student) == 1) {
        if (studCheck == std.stdId && std.flag == 0) {
            if (std.booksIssued >= 3) {
                printf("Student cannot issue more books unless previously issued books are returned.\n");
                break;
            }

            printf("Enter book number to issue : ");
            int bookCheck;
            scanf("%d", &bookCheck);

            while (fread(&book, sizeof(book), 1, fp) == 1) {
                if (bookCheck == book.bookNum && book.flag == 0 && book.currentQuantity > 0) {
                    fseek(issuing, 0, SEEK_END);
                    strcpy(bookIssue.studName, std.stdName);
                    bookIssue.bookNum = book.bookNum;
                    strcpy(bookIssue.bookName, book.bookName);
                    bookIssue.studId = std.stdId;
                    bookIssue.bookRet = 1;

                    std.booksIssued++;
                    book.currentQuantity--;

                    fwrite(&bookIssue, sizeof(bookIssue), 1, issuing);
                    fseek(student, -sizeof(std), SEEK_CUR);
                    fwrite(&std, sizeof(std), 1, student);
                    fseek(fp, -sizeof(book), SEEK_CUR);
                    fwrite(&book, sizeof(book), 1, fp);

                    printf("\nBook Issued Successfully.\n");
                    break;
                }
            }
        }
    }
    return;
}

void returnBook() {
    rewind(fp);
    rewind(student);
    rewind(issuing);
    int studCheck;
    printf("Enter student ID : ");
    scanf("%d", &studCheck);
    int bookCheck;
    printf("Enter book number to return : ");
    scanf("%d", &bookCheck);

    int flag = 0;
    while (fread(&bookIssue, sizeof(bookIssue), 1, issuing) == 1) {
        if (studCheck == bookIssue.studId && bookCheck == bookIssue.bookNum && bookIssue.bookRet == 1) {
            flag = 1;
            bookIssue.bookRet = 0;
            break;
        }
    }
    if (!flag) {
        printf("\n\nBook is not issued or does not exist.\n");
        return;
    }

    while (fread(&std, sizeof(std), 1, student) == 1) {
        if (studCheck == std.stdId && std.flag == 0) {
            std.booksIssued--;
            fseek(student, -sizeof(std), SEEK_CUR);
            fwrite(&std, sizeof(std), 1, student);
            break;
        }
    }

    while (fread(&book, sizeof(book), 1, fp) == 1) {
        if (bookCheck == book.bookNum && book.flag == 0) {
            book.currentQuantity++;
            fseek(fp, -sizeof(book), SEEK_CUR);
            fwrite(&book, sizeof(book), 1, fp);
            printf("\nBook Returned Successfully.\n");
            break;
        }
    }

    return;
}
