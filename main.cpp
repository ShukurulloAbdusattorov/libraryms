#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>

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

int lastBookID = 100000;
int lastMemberID = 100000;

int getValidInt(const string &prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (value > 0) {
                return value;
            } else {
                cout << "Number must be greater than 0!" << "\n";
            }
        } else {
            cout << "Invalid input! Please enter a number." << "\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void loadBooks() {
    ifstream file("books.txt");
    if (!file.is_open()) return;

    string line;
    int id, avail, total;
    string title, author;

    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line.rfind("ID: ", 0) == 0) {
            id = stoi(line.substr(4));
            if (id > lastBookID) lastBookID = id;

            getline(file, line);
            title = line.substr(7);

            getline(file, line);
            author = line.substr(8);

            getline(file, line);
            avail = stoi(line.substr(11));

            getline(file, line);
            total = stoi(line.substr(7));

            getline(file, line);
            books.push_back(Book(id, title, author, avail, total));
        }
    }
    file.close();
}

void saveBooks() {
    ofstream file("books.txt", ios::out);
    if (!file.is_open()) {
        cout << "Error: Unable to open books.txt for saving." << "\n";
        return;
    }

    for (const auto &b : books) {
        file << "ID: " << b.id << "\n" << "Title: " << b.title << "\n" << "Author: " << b.author << "\n" << "Available: " << b.availableCopies << "\n" << "Total: " << b.totalCopies << "\n" << "--------------------------------------------------\n";
    }
    file.close();
}

void loadMembers() {
    ifstream file("members.txt");
    if (!file.is_open()) return;

    string line;
    int id;
    string name, email;

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (line.rfind("ID: ", 0) == 0) {
            id = stoi(line.substr(4));
            if (id > lastMemberID) lastMemberID = id;

            getline(file, line);
            name = line.substr(6);

            getline(file, line);
            email = line.substr(7);

            getline(file, line);
            members.push_back(Member(id, name, email));
        }
    }
    file.close();
}

void saveMembers() {
    ofstream file("members.txt", ios::out);
    if (!file.is_open()) {
        cout << "Error: Unable to open members.txt for saving." << "\n";
        return;
    }

    for (const auto &m : members) {
        file << "ID: " << m.id << "\n" << "Name: " << m.name << "\n" << "Email: " << m.email << "\n" << "------------------------------\n";
    }
    file.close();
}

void addBook() {
    string title, author;
    cout << "Enter Book Title: ";
    getline(cin, title);

    cout << "Enter Book Author: ";
    getline(cin, author);

    int copies = getValidInt("Enter Available Copies: ");

    int newID = lastBookID + 1;
    lastBookID = newID;

    books.push_back(Book(newID, title, author, copies, copies));
    saveBooks();
    cout << "Book is added with ID " << newID << " successfully!" << "\n";
}

void displayBooks() {
    cout << "\n" << "-------- List of Books --------" << "\n";
    if (books.empty()) {
        cout << "No books available." << "\n";
        return;
    }

    for (auto &b : books) {
        cout << "ID: " << b.id << ", Title: " << b.title << ", Author: " << b.author << ", Available: " << b.availableCopies << ", Total: " << b.totalCopies << "\n";
    }
}

void issueBook() {
    int bookId = getValidInt("Enter Book ID to issue: ");
    int memberId = getValidInt("Enter Member ID: ");

    for (auto &b : books) {
        if (b.id == bookId) {
            if (b.availableCopies > 0) {
                b.availableCopies--;
                saveBooks();
                cout << "Book is issued successfully!" << "\n";
                return;
            } else {
                cout << "No copies available!" << "\n";
                return;
            }
        }
    }
    cout << "Book is not found!" << "\n";
}

void returnBook() {
    int bookId = getValidInt("Enter Book ID to return: ");

    for (auto &b : books) {
        if (b.id == bookId) {
            if (b.availableCopies == b.totalCopies) {
                cout << "All copies are already in library! You cannot return." << "\n";
                return;
            }
            b.availableCopies++;
            saveBooks();
            cout << "Book is returned successfully!" << "\n";
            return;
        }
    }
    cout << "Book is not found!" << "\n";
}

void addMember() {
    string name, email;
    cout << "Enter Member Name: ";
    getline(cin, name);

    while (true) {
        cout << "Enter Member Email: ";
        getline(cin, email);

        size_t atPos = email.find('@');
        size_t lastAtPos = email.rfind('@');
        size_t dotPos = email.rfind(".com");

        if (atPos != string::npos && atPos == lastAtPos && dotPos != string::npos && atPos < dotPos && dotPos == email.length() - 4) {
            break;
            } else {
                cout << "Invalid email! Must be in the format something@domain.com\n";
            }
    }

    int newID = lastMemberID + 1;
    lastMemberID = newID;

    members.push_back(Member(newID, name, email));
    saveMembers();
    cout << "Member added with ID " << newID << " successfully!" << "\n";
}

void displayMembers() {
    cout << "\n" << "------- List of Members -------" << "\n";
    if (members.empty()) {
        cout << "No members available." << "\n";
        return;
    }

    for (auto &m : members) {
        cout << "ID: " << m.id << ", Name: " << m.name << ", Email: " << m.email << "\n";
    }
}

void menu() {
    string input;

    do {
        cout << "\n      LIBRARY MANAGEMENT SYSTEM      \n";
        cout << "1. Add Book" << "\n";
        cout << "2. View Books" << "\n";
        cout << "3. Add Member" << "\n";
        cout << "4. View Members" << "\n";
        cout << "5. Issue Book" << "\n";
        cout << "6. Return Book" << "\n";
        cout << "7. Exit" << "\n";
        cout << "Enter choice: ";
        getline(cin, input);

        if (input.length() != 1 || input[0] < '1' || input[0] > '7') {
            cout << "Invalid choice!" << "\n";
            continue;
        }

        char choice = input[0];

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
    } while (input[0] != '7');
}

int main() {
    loadBooks();
    loadMembers();
    menu();
    return 0;
}
