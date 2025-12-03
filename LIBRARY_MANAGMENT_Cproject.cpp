#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Book {
    int id;
    char bookName[50];
    char authorName[50];
    char date[12];
};

struct Student {
    int bookId;          
    char bookName[50];   
    char sName[50];      
    char sClass[50];     
    int sRoll;           
    char date[12];       
};

void addBook();
void listBooks();
void deleteBook();
void issueBook();
void listIssuedBooks();
void clearBuffer();

int main() {
    int choice;

    while(1) {
        system("cls"); // works on windows

        printf("\n===== LIBRARY MANAGEMENT SYSTEM =====\n");
        printf("1. Add Book\n");
        printf("2. List All Books\n");
        printf("3. Delete Book\n");
        printf("4. Issue Book\n");
        printf("5. Issued Books List\n");
        printf("0. Exit\n");
        printf("-------------------------------------\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        clearBuffer();

        switch(choice) {
            case 1: addBook(); break;
            case 2: listBooks(); break;
            case 3: deleteBook(); break;
            case 4: issueBook(); break;
            case 5: listIssuedBooks(); break;
            case 0: exit(0);
            default: printf("Invalid choice!\n");
        }

        printf("\nPress ENTER to continue...");
        getchar();
    }

    return 0;
}

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void addBook() {
    struct Book b;
    char currentDate[12];

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(currentDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    strcpy(b.date, currentDate);

    printf("Enter Book ID: ");
    scanf("%d", &b.id);
    clearBuffer();

    printf("Enter Book Name: ");
    fgets(b.bookName, sizeof(b.bookName), stdin);
    b.bookName[strcspn(b.bookName, "\n")] = '\0';

    printf("Enter Author Name: ");
    fgets(b.authorName, sizeof(b.authorName), stdin);
    b.authorName[strcspn(b.authorName, "\n")] = '\0';

    FILE *fp = fopen("books.bin", "ab");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);

    printf("Book added successfully!\n");
}

void listBooks() {
    struct Book b;
    FILE *fp = fopen("books.bin", "rb");

    if (!fp) {
        printf("No books available!\n");
        return;
    }

    printf("\n===== AVAILABLE BOOKS =====\n");
    printf("%-10s %-30s %-20s %s\n",
           "Book ID", "Book Name", "Author", "Added Date");

    while (fread(&b, sizeof(b), 1, fp)) {
        printf("%-10d %-30s %-20s %s\n",
               b.id, b.bookName, b.authorName, b.date);
    }

    fclose(fp);
}

void deleteBook() {
    int deleteId, found = 0;
    struct Book b;

    FILE *fp = fopen("books.bin", "rb");
    if (!fp) {
        printf("No books available!\n");
        return;
    }

    FILE *temp = fopen("temp.bin", "wb");

    printf("Enter Book ID to delete: ");
    scanf("%d", &deleteId);

    while (fread(&b, sizeof(b), 1, fp)) {
        if (b.id == deleteId) {
            found = 1;
        } else {
            fwrite(&b, sizeof(b), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("books.bin");
    rename("temp.bin", "books.bin");

    if (found)
        printf("Book deleted successfully.\n");
    else
        printf("Book ID not found.\n");
}

void issueBook() {
    int bookId, found = 0;
    struct Book b;
    struct Student s;

    FILE *fp = fopen("books.bin", "rb");
    if (!fp) {
        printf("No books available!\n");
        return;
    }

    printf("Enter Book ID to issue: ");
    scanf("%d", &bookId);
    clearBuffer();

    while (fread(&b, sizeof(b), 1, fp)) {
        if (b.id == bookId) {
            found = 1;
            strcpy(s.bookName, b.bookName);
            s.bookId = bookId;
            break;
        }
    }
    fclose(fp);

    if (!found) {
        printf("Book ID not found!\n");
        return;
    }

    printf("Enter Student Name: ");
    fgets(s.sName, sizeof(s.sName), stdin);
    s.sName[strcspn(s.sName, "\n")] = '\0';

    printf("Enter Student Class: ");
    fgets(s.sClass, sizeof(s.sClass), stdin);
    s.sClass[strcspn(s.sClass, "\n")] = '\0';

    printf("Enter Student Roll: ");
    scanf("%d", &s.sRoll);

    char date[12];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    strcpy(s.date, date);

    fp = fopen("issued.bin", "ab");
    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);

    printf("Book issued successfully!\n");
}

void listIssuedBooks() {
    struct Student s;
    FILE *fp = fopen("issued.bin", "rb");

    if (!fp) {
        printf("No books have been issued.\n");
        return;
    }

    printf("\n===== ISSUED BOOKS =====\n");
    printf("%-10s %-30s %-15s %-10s %-30s %s\n",
           "BookID", "Student Name", "Class", "Roll", "Book Name", "Date");

    while (fread(&s, sizeof(s), 1, fp)) {
        printf("%-10d %-30s %-15s %-10d %-30s %s\n",
               s.bookId, s.sName, s.sClass, s.sRoll, s.bookName, s.date);
    }

    fclose(fp);
}



