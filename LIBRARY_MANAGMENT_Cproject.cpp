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
    int id;
    char sName[50];
    char sClass[50];
    int sRoll;
    char bookName[50];
    char date[12];
};

void addBook();
void booksList();
void del();
void issueBook();
void issueList();
void clearInputBuffer();

int main() {
    int ch;

    while(1){
        system("cls"); 
        printf("<== Library Management System ==>\n");
        printf("1. Add Book\n");
        printf("2. Books List\n");
        printf("3. Remove Book\n");
        printf("4. Issue Book\n");
        printf("5. Issued Book List\n");
        printf("0. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);
        clearInputBuffer(); 

        switch(ch){
            case 0: exit(0);
            case 1: addBook(); break;
            case 2: booksList(); break;
            case 3: del(); break;
            case 4: issueBook(); break;
            case 5: issueList(); break;
            default: printf("Invalid Choice...\n");
        }
        printf("\nPress Enter to continue...");
        getchar(); 
    }

    return 0;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void addBook() {
    struct Book b;
    char myDate[12];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(myDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year + 1900);
    strcpy(b.date, myDate);

    FILE *fp = fopen("books.txt", "ab");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter book id: ");
    scanf("%d", &b.id);
    clearInputBuffer();

    printf("Enter book name: ");
    fgets(b.bookName, sizeof(b.bookName), stdin);
    b.bookName[strcspn(b.bookName, "\n")] = 0;

    printf("Enter author name: ");
    fgets(b.authorName, sizeof(b.authorName), stdin);
    b.authorName[strcspn(b.authorName, "\n")] = 0;

    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);

    printf("Book Added Successfully!\n");
}

void booksList() {
    struct Book b;
    FILE *fp = fopen("books.txt", "rb");
    if (!fp) {
        printf("No books available.\n");
        return;
    }

    system("cls");
    printf("<== Available Books ==>\n\n");
    printf("%-10s %-30s %-20s %s\n\n", "Book id", "Book Name", "Author", "Date");

    while(fread(&b, sizeof(b), 1, fp) == 1) {
        printf("%-10d %-30s %-20s %s\n", b.id, b.bookName, b.authorName, b.date);
    }

    fclose(fp);
}

void del() {
    int id, f = 0;
    struct Book b;
    FILE *fp = fopen("books.txt", "rb");
    if (!fp) {
        printf("No books available.\n");
        return;
    }

    FILE *ft = fopen("temp.txt", "wb");
    if (!ft) {
        fclose(fp);
        printf("Error opening temp file!\n");
        return;
    }

    printf("Enter Book id to remove: ");
    scanf("%d", &id);

    while(fread(&b, sizeof(b), 1, fp) == 1) {
        if (b.id == id) {
            f = 1;
        } else {
            fwrite(&b, sizeof(b), 1, ft);
        }
    }

    fclose(fp);
    fclose(ft);

    remove("books.txt");
    rename("temp.txt", "books.txt");

    if(f) {
        printf("Deleted Successfully.\n");
    } else {
        printf("Record Not Found!\n");
    }
}

void issueBook() {
    struct Book b;
    struct Student s;
    char myDate[12];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(myDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year + 1900);
    strcpy(s.date, myDate);

    int f = 0;
    FILE *fp = fopen("books.txt", "rb");
    if (!fp) {
        printf("No books available.\n");
        return;
    }

    printf("Enter Book id to issue: ");
    scanf("%d", &s.id);
    clearInputBuffer();

    while(fread(&b, sizeof(b), 1, fp) == 1) {
        if(b.id == s.id) {
            strcpy(s.bookName, b.bookName);
            f = 1;
            break;
        }
    }
    fclose(fp);

    if(f == 0) {
        printf("No book found with this id.\n");
        return;
    }

    printf("Enter Student Name: ");
    fgets(s.sName, sizeof(s.sName), stdin);
    s.sName[strcspn(s.sName, "\n")] = 0;

    printf("Enter Student Class: ");
    fgets(s.sClass, sizeof(s.sClass), stdin);
    s.sClass[strcspn(s.sClass, "\n")] = 0;

    printf("Enter Student Roll: ");
    scanf("%d", &s.sRoll);

    fp = fopen("issue.txt", "ab");
    if (!fp) {
        printf("Error opening issue file!\n");
        return;
    }

    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);

    printf("Book Issued Successfully!\n");
}

void issueList() {
    struct Student s;
    FILE *fp = fopen("issue.txt", "rb");
    if (!fp) {
        printf("No issued books.\n");
        return;
    }

    system("cls");
    printf("<== Book Issue List ==>\n\n");
    printf("%-10s %-30s %-20s %-10s %-30s %s\n\n", "S.id", "Name", "Class", "Roll", "Book Name", "Date");

    while(fread(&s, sizeof(s), 1, fp) == 1) {
        printf("%-10d %-30s %-20s %-10d %-30s %s\n", s.id, s.sName, s.sClass, s.sRoll, s.bookName, s.date);
    }

    fclose(fp);
}

