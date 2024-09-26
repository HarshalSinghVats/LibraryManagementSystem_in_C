#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// structures for different purposes...
struct library
{
    int bookNum;
    char bookName[150];
    char authName[100];
    int pages;
    int bookQuantity;
    int currentQuantity;
    int flag;
} book;

struct student
{
    int stdId;
    char stdName[100];
    int class;
    char section;
    int booksIssued;
    int flag;
} std;

struct issue
{
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


int main()
{

    // opening DAT files...
    fp = fopen("libraryRecords.DAT", "rb+");
    if (fp == NULL)
    {
        fp = fopen("libraryRecords.DAT", "wb+");

        if (fp == NULL)
            exit(1);
    }

    student = fopen("libraryStudent.DAT", "rb+");
    if (student == NULL)
    {
        student = fopen("libraryStudent.DAT", "wb+");
        if (student == NULL)
            exit(1);
    }

    issuing = fopen("libraryIssues.DAT", "rb+");
    if (issuing == NULL)
        {
        issuing = fopen("libraryIssues.DAT", "wb+");
        if (issuing == NULL)
            exit(1);
    }

    // main part of interface...
    int choice = 0;
    while (choice < 5)
    {

        printf("\nWELCOME TO LIBRARY MANAGEMENT SYSTEM\n\n");
        printf("CHOOSE :\n1.BOOKS\n2.STUDENTS\n3.TRANSACTIONS\nWNTER ANY OTHER KEY TO EXIT\n\n");
        scanf("%d", &choice);
        system("cls");
        switch (choice)
        {
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
            return 0;
        }
    }
    fclose(fp);
    fclose(student);
    fclose(issuing);
    return 0;
}

void bookSection()
{
    int bookChoice = 0;

    while (bookChoice < 8)
    {

        printf("CHOOSE :\n1.ADD BOOKS\n2.VIEW INDIVIDUAL BOOKS\n3.DELETE BOOKS\n4.BOOKS PURCHASED\n5.VIEW BOOK STOCKS\n6.TOTAL BOOKS\n7.VIEW ALL BOOKS\nENTER ANY OTHER KEY TO MAIN MENU\n\n");
        scanf("%d", &bookChoice);
        system("cls");

        switch (bookChoice)
        {
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
            printf("\nEXITING\n\n");
            return;
        }
    }
    printf("\n\n");
}

// student section...
void studentSection()
{
    int studChoice = 0;
    while (studChoice < 6)
    {

        printf("\nCHOOSE :\n1.ADD STUDENT RECORDS\n2.VIEW STUDENT RECORDS\n3.DELETE STUDENT RECORDS\n4.BOOKS ISSUED\nENTER ANY OTHER KEY TO RETURN TO MAIN MENU\n\n");
        scanf("%d", &studChoice);
        system("cls");

        switch (studChoice)
        {
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
            printf("\nEXITING\n\n");
            break;
        }
    }
}

// transaction section...
void transactionSection()
{
    int transactChoice = 0;
    while (transactChoice < 4)
    {
        printf("CHOOSE :\n1.ISSUE BOOK\n2.RETURN BOOK\n4.RETURN TO MAIN MENU\n\n");
        scanf("%d", &transactChoice);
        system("cls");
        switch (transactChoice)
        {
        case 1:
            issueBook();
            break;
        case 2:
            returnBook();
            break;
        default:
            printf("EXITING\n");
            break;
        }
    }
}

// book section functions...
void addBookRecord()
{
    system("cls");
    fseek(fp, 0, SEEK_END);

    printf("\nADD DETAILS :\n");

    printf("Enter book number : ");
    scanf("%d", &book.bookNum);

    printf("Enter book name : ");
    fflush(stdin);
    gets(book.bookName);

    printf("Enter author name : ");
    fflush(stdin);
    gets(book.authName);

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

void viewBookRecord()
{

    int x;
    printf("Enter book number : ");
    scanf("%d", &x);
    system("cls");
    int count = 0;
    rewind(fp);
    while (fread(&book, sizeof(book), 1, fp) == 1)
    {
        if (x == book.bookNum && book.flag == 0)
        {
            printf("\nBook number : %d\nBook name : %s\nAuthor name : %s\nPages : %d\n\n", book.bookNum, book.bookName, book.authName, book.pages);
            break;
            count = 1;
        }
    }
    printf("\n");
    return;
}

void deleteBookRecord()
{
    rewind(fp);

    int bookCheck;
    printf("Enter book number to delete : ");
    scanf("%d", &bookCheck);

    while (fread(&book, sizeof(book), 1, fp) == 1)
    {
        if (bookCheck == book.bookNum)
        {
            book.flag = 1;
            fseek(fp, -sizeof(book), SEEK_CUR);
            fwrite(&book, sizeof(book), 1, fp);
            break;
        }
    }
    return;
}

void modifyStocksRecord()
{
    system("cls");
    int bookCheck;
    printf("Enter book number : ");
    scanf("%d", &bookCheck);
    int bookCount;
    while (fread(&book, sizeof(book), 1, fp) == 1)
    {
        if (bookCheck == book.bookNum && book.flag == 0)
        {
            printf("\nEnter book quantity : ");
            fflush(stdin);
            scanf("%d", &bookCount);
            book.bookQuantity += bookCount;
            book.currentQuantity += book.bookQuantity;
            fseek(fp, -sizeof(book), SEEK_CUR);
            fwrite(&book, sizeof(book), 1, fp);
            fflush(stdin);
            printf("\n");
            break;
        }
    }
    return;
}

void bookCount()
{
    system("cls");

    int k;
    printf("Enter book number : ");
    scanf("%d", &k);
    rewind(fp);

    while (fread(&book, sizeof(book), 1, fp) == 1)
    {
        if (k == book.bookNum && book.flag == 0)
        {
            printf("\nTotal number of books available : %d\n", book.bookQuantity);
            printf("\nNumber of books available : %d\n\n", book.currentQuantity);
            break;
        }
    }
    return;
}

void totalBooks()
{
    system("cls");
    int bookCount = 0;
    while (fread(&book, sizeof(book), 1, fp) == 1)
    {
        if (book.flag != 1)
            bookCount += book.bookQuantity;
        else
            continue;
    }
    printf("\nNumber of books in library : %d\n\n", bookCount);
    return;
}

void viewAllBooksRecord()
{
    system("cls");
    rewind(fp);

    while (fread(&book, sizeof(book), 1, fp) == 1)
    {
        if (book.flag != 1)
            printf("\nBook number : %d\nBook name : %s\nAuthor name : %s\nPages : %d\n\n", book.bookNum, book.bookName, book.authName, book.pages);
    }
    return;
}

// student section functions...

void addStudentsRecords()
{

    fseek(student, 0, SEEK_END);

    fflush(stdin);
    printf("ADD DETAILS :\n");

    printf("Enter student id : ");
    scanf("%d", &std.stdId);

    printf("Enter student name : ");
    fflush(stdin);
    gets(std.stdName);

    printf("Enter class : ");
    scanf("%d", &std.class);

    printf("Enter section : ");
    fflush(stdin);
    scanf("%c", &std.section);

    std.booksIssued = 0;

    fwrite(&std, sizeof(std), 1, student);
    fflush(stdin);
    return;
}

void viewStudentsRecords(){
    int x;
    printf("Enter student id : ");
    scanf("%d", &x);
    while (fread(&std, sizeof(std), 1, student) == 1)
    {
        if (x == std.stdId)
        {
            printf("\n\nStudent id : %d\nStudent name : %s\nClass : %d\nSection : %c\nBooks Issued : %d\n\n", std.stdId, std.stdName, std.class, std.section, std.booksIssued);
            break;
        }
    }
    return;
}

void deleteStudentsRecords()
{
    rewind(student);
    int studCheck;
    printf("Enter book number to delete : ");
    scanf("%d", &studCheck);

    while (fread(&std, sizeof(std), 1, student) == 1)
    {
        if (studCheck == book.bookNum)
        {
            std.flag = 1;
            fseek(student, -sizeof(std), SEEK_CUR);
            fwrite(&std, sizeof(std), 1, student);
            break;
        }
    }
    return;
}

void booksIssued()
{
    rewind(issuing);
    int stdCheck;
    printf("Enter student id : ");
    scanf("%d", &stdCheck);
    while (fread(&bookIssue, sizeof(bookIssue), 1, issuing) == 1)
    {
        if (stdCheck == bookIssue.studId)
        {
            printf("\nBook number - %d\nBook name - %s\n\n", bookIssue.bookNum, bookIssue.bookName);
        }
    }
    return;
}

void finesIssued() {}

// transaction section functions...
void issueBook()
{
    rewind(fp);
    rewind(student);
    fseek(issuing, 0, SEEK_END);
    int num;
    int count = 0, stdCount = 0;
    printf("Enter book number : ");
    scanf("%d", &num);

    while (fread(&book, sizeof(book), 1, fp) == 1)
    {
        if (book.bookNum == num && book.flag == 0 && book.currentQuantity != 0)
        {
            count = 1;
            break;
        }
        else if (book.bookNum == num && book.flag == 0 && book.currentQuantity == 0)
        {
            count = 2;
            break;
        }
    }

    // checking for book availability...
    if (count == 1)
    {
        int stdCheck;
        printf("Enter student id : ");
        scanf("%d", &stdCheck);

        while (fread(&std, sizeof(std), 1, student) == 1)
        {
            if (std.stdId == stdCheck && std.booksIssued < 3)
            {
                stdCount = 1;
                break;
            }
            else if (std.stdId == stdCheck && std.booksIssued > 3)
            {
                printf("\nStudent cannot issue more books unless previously issued books are returned.\n");
                break;
            }
            else
                stdCount = 3;
        }
    }

    else if (count == 2)
    {
        printf("\nBook is not currently available in the library.\n");
    }
    else
    {
        printf("\nBook not found\n");
    }

    if (stdCount == 1)
    {
        bookIssue.bookNum = book.bookNum;
        bookIssue.studId = std.stdId;
        bookIssue.bookRet = 1;
        strcpy(bookIssue.bookName, book.bookName);
        strcpy(bookIssue.studName, std.stdName);

        printf("\nDetails of issuing :");
        printf("\nBook number : %d", bookIssue.bookNum);
        printf("\nBook name : %s", bookIssue.bookName);
        printf("\nStudent id : %d", bookIssue.studId);
        printf("\nStudent number : %s", bookIssue.studName);

        std.booksIssued++;
        book.currentQuantity--;

        fseek(fp, -(long)sizeof(book), SEEK_CUR);
        fwrite(&book, sizeof(book), 1, fp);

        fseek(student, -(long)sizeof(std), SEEK_CUR);
        fwrite(&std, sizeof(std), 1, student);

        fwrite(&bookIssue, sizeof(bookIssue), 1, issuing);
        fflush(stdin);
    }
    printf("\n\n");
    return;
}

void returnBook()
{
    rewind(fp);
    rewind(student);
    rewind(issuing);

    int stdNm, bookNm;
    int flag = 0;
    printf("Enter student id : ");
    scanf("%d", &stdNm);
    printf("Enter book number : ");
    scanf("%d", &bookNm);

    while (fread(&bookIssue, sizeof(bookIssue), 1, issuing) == 1)
    {
        if (bookIssue.bookNum == bookNm)
        {
            bookIssue.bookRet = 0;
            flag = 1;
            break;
        }
    }

    if (flag == 1)
    {
        printf("\n\nBook is not issued or does not exist.\n");
        exit(2);
    }

    while (fread(&std, sizeof(std), 1, student) == 1)
    {
        if (std.stdId == stdNm)
        {
            std.booksIssued--;
            break;
        }
    }
    while (fread(&book, sizeof(book), 1, fp) == 1)
    {
        if (book.bookNum == bookNm)
        {
            book.currentQuantity++;
            break;
        }
    }

    fseek(fp, -(long)sizeof(book), SEEK_CUR);
    fwrite(&book, sizeof(book), 1, fp);

    fseek(student, -(long)sizeof(std), SEEK_CUR);
    fwrite(&std, sizeof(std), 1, student);

    fseek(issuing, -(long)sizeof(bookIssue), SEEK_CUR);
    fwrite(&bookIssue, sizeof(bookIssue), 1, issuing);
    fflush(stdin);

    return;
}
