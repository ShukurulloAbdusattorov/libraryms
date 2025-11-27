#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    int availableCopies;
    int totalCopies;

    Book(int _id, string _title, string _author, int _availableCopies, int _totalCopies) {
        id = _id;
        title = _title;
        author = _author;
        availableCopies = _availableCopies;
        totalCopies = _totalCopies;
    }
};

class Member {
public:
    int id;
    string name;
    string email;

    Member(int _id, string _name, string _email) {
        id = _id;
        name = _name;
        email = _email;
    }
};

vector<Book> books;
vector<Member> members;

void loadBooks() {
    ifstream file("books.txt");
    if (!file.is_open()) return;

    string line;
    int id, avail, total;
    string title, author;

    while (getline(file, line)) {

        if (line.rfind("Book ID:", 0) == 0) {
            id = stoi(line.substr(9));
            getline(file, line);
            title = line.substr(8);
            getline(file, line);
            author = line.substr(9);
            getline(file, line);
            avail = stoi(line.substr(18));
            getline(file, line);
            total = stoi(line.substr(15));
            getline(file, line);

            books.push_back(Book(id, title, author, avail, total));
        }
    }
    file.close();
}

void saveBooks(const vector<Book>& books) {
    ofstream file("books.txt", ios::out);

    if (!file.is_open()) {
        cout << "Error: Unable to open file for saving.\n";
        return;
    }

    for (const auto &b : books) {
        file << b.id << "\n"
             << b.title << "\n"
             << b.author << "\n"
             << b.availableCopies << "\n"
             << b.totalCopies << "\n"
             << "------------------------------" << "\n";
    }
    file.close();
    cout << "Books saved successfully.\n";
}

void loadMembers() {
    ifstream file("members.txt");
    if (!file.is_open()) return;

    string line;
    int id;
    string name, email;

    while (getline(file, line)) {
        if (line.empty()) continue;

        id = stoi(line);
        getline(file, name);
        getline(file, email);
        getline(file, line);

        members.push_back(Member(id, name, email));
    }
    file.close();
}

void saveMembers() {
    ofstream file("members.txt", ios::out);

    if (!file.is_open()) {
        cout << "Error: I am unable to open file for saving.\n";
        return;
    }
    for (const auto &m : members) {
        file << m.id << "\n"
             << m.name << "\n"
             << m.email << "\n"
             << "------------------------------" << "\n";
    }
    file.close();
    cout << "Members saved successfully.\n";
}

void addBook() {
    int id, copies;
    string title, author;

    cout << "Enter Book ID: ";
    cin >> id;
    cin.ignore();

    cout << "Enter Book Title: ";
    getline(cin, title);

    cout << "Enter Book Author: ";
    getline(cin, author);

    cout << "Enter Available Copies: ";
    cin >> copies;

    books.push_back(Book(id, title, author, copies, copies));
    saveBooks();

    cout << "Book added and saved successfully!\n";
}

void displayBooks() {
    cout << "\n------- List of Books -------\n";

    if (books.empty()) {
        cout << "No books available.\n";
        return;
    }

    for (auto &b : books) {
        cout << "ID: " << b.id
             << "Title: " << b.title
             << "Author: " << b.author
             << "Available: " << b.availableCopies
             << "Total: " << b.totalCopies << "\n";
    }
}

void issueBook() {
    int bookId, memberId;
    cout << "Enter Book ID to issue: ";
    cin >> bookId;
    cout << "Enter Member ID: ";
    cin >> memberId;

    for (auto &b : books) {
        if (b.id == bookId) {
            if (b.availableCopies > 0) {
                b.availableCopies--;
                saveBooks();
                cout << "Book issued successfully!\n";
                return;
            } else {
                cout << "No copies available!\n";
                return;
            }
        }
    }
    cout << "Book is not found!\n";
}

void returnBook() {
    int bookId;
    cout << "Enter Book ID to return: ";
    cin >> bookId;

    for (auto &b : books) {
        if (b.id == bookId) {

            if (b.availableCopies == b.totalCopies) {
                cout << "All copies are already in library! You cannot return.\n";
                return;
            }

            b.availableCopies++;
            saveBooks();
            cout << "Book is returned successfully!\n";
            return;
        }
    }
    cout << "Book is not found!\n";
}

void addMember() {
    int id;
    string name, email;

    cout << "Enter Member ID: ";
    cin >> id;
    cin.ignore();

    cout << "Enter Member Name: ";
    getline(cin, name);

    cout << "Enter Member Email: ";
    getline(cin, email);

    members.push_back(Member(id, name, email));
    saveMembers();

    cout << "Member added successfully!\n";
}

void displayMembers() {
    cout << "\n------- List of Members -------\n";

    if (members.empty()) {
        cout << "No members available.\n";
        return;
    }

    for (auto &m : members) {
        cout << "ID: " << m.id
             << "Name: " << m.name
             << "Email: " << m.email << "\n";
    }
}

void menu() {
    char choice;

    do {
        cout << "\n      LIBRARY MANAGEMENT SYSTEM      \n";
        cout << "1. Add Book\n";
        cout << "2. View Books\n";
        cout << "3. Add Member\n";
        cout << "4. View Members\n";
        cout << "5. Issue Book\n";
        cout << "6. Return Book\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case '1': addBook(); break;
            case '2': displayBooks(); break;
            case '3': addMember(); break;
            case '4': displayMembers(); break;
            case '5': issueBook(); break;
            case '6': returnBook(); break;
            case '7': cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n"; break;
        }
    } while (choice != '7');
}

int main() {
    loadBooks();
    loadMembers();

    menu();
    return 0;
}

