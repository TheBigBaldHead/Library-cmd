#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

struct Time {
    int Year;
    int Month;
    int Day;
};

class Book;
class User;
class Admin;

User Find(long long int UserID, vector<User>& UserList);
Book Search(string Name, vector<Book>& BookList);
Book FindBook(string BookID, vector<Book>& BookList);
void RegisterAdmin(vector<Admin>& AdminList, vector<User>& UserList);
int LogIn(long long int UserID, vector<User>& UserList, vector<Admin>& AdminList);
// Right Format
bool BookIDValidation(string bookID);
bool UserIDValidation(long long int userID, bool admin, vector<Admin>& AdminList, vector<User>& UserList);
bool NameValidation(string Name);
bool WriterValidation(string Writer);
bool PasswordValidation(string Password);
// Time
string ConvertToString(Time t);
Time ReturningTime(Time t);
int TimeInDays(Time t1, Time t2);
// Admin
void AddBook(vector<Book>& BookList);
void RemoveBook(vector<Book>& BookList);
void LibraryStats(vector<Book> BookList, vector<User> UserList);
void EditBook(vector<Book>& BookList);
// User
void RegisterUser(vector<User>& UserList, vector<Admin>& AdminList);
bool SearchBook(string Name, vector<Book>& BookList);
void EditUser(long long int UserID, vector<User>& UserList);
void Borrow(long long int userID, vector<User>& UserList, vector<Book>& BookList);
void Enqueue(long long int userID, vector<User>& UserList, vector<Book>& BookList);
void ReturnBook(long long int userID, vector<User>& UserList, vector<Book>& BookList);
void ReturnTimeExtension(long long int userID, vector<User>& UserList, vector<Book>& BookList);
void Pay(long long int userID, vector<User> UserList, vector<Book> BookList);
void UserStats(long long int userID, vector<User> UserList, vector<Book> BookList);

void LoadFromFile(vector<Book>& BookList, vector<Admin>& AdminList, vector<User>& UserList);
void addBookToFile(Book book);
void addUserToFile(User user);
void addAdminToFile(Admin admin);
void updateBooks(vector<Book> BookList);
void updateUsers(vector<Admin> AdminList, vector<User> UserList);

int main() {
    vector<Book> BookList;
    vector<Admin> AdminList;
    vector<User> UserList;
    LoadFromFile(BookList, AdminList, UserList);
    bool LoggedIn = false;
    bool Admin = false;
    long long int USERID = 0;
    while (true) {
        updateBooks(BookList);
        updateUsers(AdminList, UserList);
        if (LoggedIn) {
            if (Admin) { // Admin Menu
                cout << "\n --------- Admin Menu ---------" << endl;
                cout << "enter '1' to add a book" << endl;
                cout << "enter '2' to remove a book" << endl;
                cout << "enter '3' to show the stats of the library" << endl;
                cout << "enter '4' to edit the stats of a book" << endl;
                cout << "enter '5' to log out" << endl;
                char admininput;
                cin >> admininput;
                switch (admininput) {
                    case '1':
                        AddBook(BookList);
                        break;
                    case '2':
                        RemoveBook(BookList);
                        break;
                    case '3':
                        LibraryStats(BookList, UserList);
                        break;
                    case '4':
                        EditBook(BookList);
                        break;
                    case '5':
                        LoggedIn = false;
                        Admin = false;
                        USERID = 0;
                        break;
                    default:
                        cout << "Invalid input" << endl;
                        break;
                }
            }
            else { // User Menu
                cout << "\n --------- User Menu ---------" << endl;
                cout << "enter '1' to edit your information" << endl;
                cout << "enter '2' to search for a book" << endl;
                cout << "enter '3' to borrow a book" << endl;
                cout << "enter '4' to enqueue for a book" << endl;
                cout << "enter '5' to return a book" << endl;
                cout << "enter '6' to extend the returning time of a book" << endl;
                cout << "enter '7' to pay your debt" << endl;
                cout << "enter '8' to show the stats of the books you have borrowed" << endl;
                cout << "enter '9' to log out" << endl;
                char userinput;
                cin >> userinput;

                if (userinput == '1') {
                    EditUser(USERID, UserList);
                }
                else if (userinput == '2') {
                    string Name;
                    cout << "Name: ";
                    cin.ignore();
                    getline(cin, Name);
                    if (SearchBook(Name, BookList)) {
                        cout << "This book exists in the library" << endl;
                    }
                    else {
                        cout << "Book not found" << endl;
                    }
                }
                else if (userinput == '3') {
                    Borrow(USERID, UserList, BookList);
                }
                else if (userinput == '4') {
                    Enqueue(USERID, UserList, BookList);
                }
                else if (userinput == '5') {
                    ReturnBook(USERID, UserList, BookList);
                }
                else if (userinput == '6') {
                    ReturnTimeExtension(USERID, UserList, BookList);
                }
                else if (userinput == '7') {
                    Pay(USERID, UserList, BookList);
                }
                else if (userinput == '8') {
                    UserStats(USERID, UserList, BookList);
                }
                else if (userinput == '9') {
                    LoggedIn = false;
                    Admin = false;
                    USERID = 0;
                    cout << "Logged out!" << endl;
                }
                else {
                    cout << "Invalid input" << endl;
                }
            }
        }
        else { // Not logged in yet
            cout << "\nenter '1' to Register User" << endl;
            cout << "enter '2' to Register Admin" << endl;
            cout << "enter '3' to Log in" << endl;
            char input;
            cin >> input;
            if (input == '1') {
                RegisterUser(UserList, AdminList);
            }
            else if (input == '2') {
                RegisterAdmin(AdminList, UserList);
            }
            else if (input == '3') {
                long long int UserID;
                cout << "User ID: ";
                cin >> UserID;
                if (UserID / 10000000000 != 0 || UserID / 1000000000 == 0) {
                    cout << "Invalid User ID" << endl;
                    continue;
                }
                int result = LogIn(UserID, UserList, AdminList);
                if (result > 0) {
                    LoggedIn = true;
                    if (result == 2) {
                        cout << "Logged in as admin with user ID " << UserID << endl;
                        Admin = true;
                    }
                    else {
                        cout << "Logged in as user with user ID " << UserID << endl;
                    }
                    USERID = UserID;
                }
                else if (result == -1) {
                    cout << "Wrong Password" << endl;
                }
                else {
                    cout << "User ID not found" << endl;
                }
            }
            else {
                cout << "Invalid input" << endl;
            }
        }
    }
}

class Book {
    public:
        string BookID;
        string Name;
        int DailyPenalty;
        long long int UserID = -1;
        bool Available = true;
        vector<User> Queue; 
        Time ReturnTime;
        Book(string bookID, string name, int dailyPenalty) {
            BookID = bookID;
            Name = name;
            DailyPenalty = dailyPenalty;
            Time time;
            time.Year = 0;
            time.Month = 0;
            time.Day = 0;
            ReturnTime = time;
        }
};

class User {
    public:
        long long int UserID;
        string Name;
        string Password;
        string Phone;
        int debt = 0;
        User(long long int userID, string name, string password, string phone) {
            UserID = userID;
            Name = name;
            Password = password;
            Phone = phone;
        }
        vector<string> BorrowedBookIDs;
};

class Admin {
    public:
        long long int UserID;
        string Name;
        string Password;
        string EducationDegree;
        string Specialty;
        Admin(long long int userid, string name, string password, string educationDegree, string specialty) {
            UserID = userid;
            Name = name;
            Password = password;
            EducationDegree = educationDegree;
            Specialty = specialty;
        }
};

User Find(long long int UserID, vector<User>& UserList) {
    for (unsigned int i = 0; i < UserList.size(); i++) {
        if (UserList[i].UserID == UserID) {
            return UserList[i];
        }
    }
}

Book Search(string Name, vector<Book>& BookList) {
    for (unsigned int i = 0; i < BookList.size(); i++) {
        if (BookList[i].Name == Name) {
            return BookList[i];
        }
    }
}

bool SearchBook(string Name, vector<Book>& BookList) {
    for (unsigned int i = 0; i < BookList.size(); i++) {
        if (BookList[i].Name == Name) {
            return true;
        }
    }
    return false;
}

Book FindBook(string BookID, vector<Book>& BookList) {
    for (unsigned int i = 0; i < BookList.size(); i++) {
        if (BookList[i].BookID == BookID) {
            return BookList[i];
        }
    }
}

bool SearchBookID(string BookID, vector<Book>& BookList) {
    for (unsigned int i = 0; i < BookList.size(); i++) {
        if (BookList[i].BookID == BookID) {
            return true;
        }
    }
    return false;
}
// Admin
void AddBook(vector<Book>& BookList) {
    string BookID;
    cout << "Book ID: ";
    cin >> BookID;
    if (!BookIDValidation(BookID)) {
        cout << "Invalid Book ID" << endl;
        return;
    }
    string Name;
    cout << "Book Name: ";
    cin.ignore();
    getline(cin, Name);
    if (!NameValidation(Name)) {
        cout << "Invalid Book Name" << endl;
        return;
    }
    int DailyPenalty;
    cout << "Daily Penalty: ";
    cin >> DailyPenalty;
    if (DailyPenalty <= 0) {
        cout << "Invalid Daily Penalty" << endl;
        return;
    }
    Book book = Book(BookID, Name, DailyPenalty);
    BookList.push_back(book);
    cout << "Book added!" << endl;
    addBookToFile(book);
}

void RemoveBook(vector<Book>& BookList) {
    string Name;
    cout << "Book name: ";
    cin.ignore();
    getline(cin, Name);
    if (!NameValidation(Name)) {
        cout << "Invalid name" << endl;
        return;
    }
    
    for (unsigned int i = 0; i < BookList.size(); i++) {
        if (BookList[i].Name == Name) {
            if (BookList[i].Available && BookList[i].Queue.size() == 0) {
                BookList.erase(BookList.begin() + i);
                cout << "Book removed!" << endl;
                return;
            }
            else {
                cout << "Book is not removeable now!" << endl;
            }
            return;
        }
    }
}

void LibraryStats(vector<Book> BookList, vector<User> UserList) {
    for (unsigned int i = 0; i < BookList.size(); i++) {
        Book book = BookList[i];
        cout << "Book name: " << book.Name << " / Book ID: " << book.BookID << " / Daily Penalty: " << book.DailyPenalty;
        if (book.Available) {
            cout << " / Available" << endl;
        }
        else {
            User user = Find(book.UserID, UserList);
            cout << " / Unavailable (taken by " << user.Name << ")" << endl;
        }
    }
}

void EditBook(vector<Book>& BookList) {
    string Name;
    cout << "Book name: ";
    cin.ignore();
    getline(cin, Name);
    if (!NameValidation(Name)) {
        cout << "Invalid name" << endl;
        return;
    }
    if (!SearchBook(Name, BookList)) {
        cout << "Book not found" << endl;
        return;
    }
    string BookID;
    cout << "Book ID: ";
    cin >> BookID;
    if (!BookIDValidation(BookID)) {
        cout << "Invalid Book ID" << endl;
        return;
    }
    int dailyPenalty;
    cout << "Daily Penalty: ";
    cin >> dailyPenalty;
    if (dailyPenalty <= 0) {
        cout << "Invalid Daily Penalty" << endl;
        return;
    }
    for (unsigned int i = 0; i < BookList.size(); i++) {
        if (BookList[i].Name == Name) {
            BookList[i].BookID = BookID;
            BookList[i].DailyPenalty = dailyPenalty;
            cout << "Book information editted!" << endl;
            return;
        }
    }
}
// User
void RegisterUser(vector<User>& UserList, vector<Admin>& AdminList) {
    long long int UserID;
    cout << "User ID: ";
    cin >> UserID;
    if (!UserIDValidation(UserID, false, AdminList, UserList)) {
        cout << "Invalid user ID!" << endl;
        return;
    }
    string Name;
    cout << "Name: ";
    cin.ignore();
    getline(cin, Name);
    if (!NameValidation(Name)) {
        cout << "Invalid name!" << endl;
        return;
    }
    string Password;
    cout << "Password: ";
    cin >> Password;
    if (!PasswordValidation(Password)) {
        cout << "Password!" << endl;
        return;
    }
    string Phone;
    cout << "Phone: ";
    cin >> Phone;

    User user = User(UserID, Name, Password, Phone);
    UserList.push_back(user);
    cout << "User added!" << endl;
    addUserToFile(user);
}

void RegisterAdmin(vector<Admin>& AdminList, vector<User>& UserList) {
    long long int UserID;
    cout << "User ID: ";
    cin >> UserID;
    if (!UserIDValidation(UserID, true, AdminList, UserList)) {
        cout << "Invalid user ID!" << endl;
        return;
    }
    string Name;
    cout << "Name: ";
    cin.ignore();
    getline(cin, Name);
    if (!NameValidation(Name)) {
        cout << "Invalid name!" << endl;
        return;
    }
    string Password;
    cout << "Password: ";
    cin >> Password;
    if (!PasswordValidation(Password)) {
        cout << "Invalid Password!" << endl;
        return;
    }
    string EducationDegree;
    cout << "Education degree: ";
    cin.ignore();
    getline(cin, EducationDegree);

    string Specialty;
    cout << "Specialty: ";
    cin >> Specialty;
    Admin admin = Admin(UserID, Name, Password, EducationDegree, Specialty);
    AdminList.push_back(admin);
    cout << "Admin added!" << endl;
    addAdminToFile(admin);
}

void EditUser(long long int UserID, vector<User>& UserList) {
    string Name;
    cout << "Name: ";
    cin.ignore();
    getline(cin, Name);
    if (!NameValidation(Name)) {
        cout << "Invalid name!" << endl;
        return;
    }
    string Password;
    cout << "Password: ";
    cin >> Password;
    if (!PasswordValidation(Password)) {
        cout << "Password!" << endl;
        return;
    }
    string Phone;
    cout << "Phone: ";
    cin >> Phone;

    for (unsigned int i = 0; i < UserList.size(); i++) {
        if (UserList[i].UserID == UserID) {
            UserList[i].Name = Name;
            UserList[i].Password = Password;
            UserList[i].Phone = Phone;
            cout << "User information editted!" << endl;
            return;
        }
    }
}

void Borrow(long long int userID, vector<User>& UserList, vector<Book>& BookList) {
    string Name;
    cout << "Name of the book: ";
    cin.ignore();
    getline(cin, Name);

    if (!SearchBook(Name, BookList)) {
        cout << "Book not found";
        return;
    }
    for (unsigned int j = 0; j < UserList.size(); j++) {
        if (UserList[j].UserID == userID) {
            for (unsigned int i = 0; i < BookList.size(); i++) {
                if (BookList[i].Name == Name) {
                    if (BookList[i].Available) {
                        if (BookList[i].Queue.size() != 0) {
                            if (BookList[i].Queue[0].Name != UserList[j].Name) {
                                cout << "It's time for " << BookList[i].Queue[0].Name << " to borrow the book, stay in queue until your turn" << endl;
                                return;
                            }
                            else {
                                BookList[i].Queue.erase(BookList[i].Queue.begin());
                            }
                        }
                        Time time;
                        cout << "Enter The current time" << endl;
                        cout << "Year: ";
                        cin >> time.Year;
                        cout << "Month: ";
                        cin >> time.Month;
                        cout << "Day: ";
                        cin >> time.Day;
                        BookList[i].UserID = UserList[i].UserID;
                        BookList[i].ReturnTime = ReturningTime(time);
                        BookList[i].Available = false;
                        cout << "You should return the book until: " << ConvertToString(BookList[i].ReturnTime) << endl; 
                        UserList[j].BorrowedBookIDs.push_back(BookList[i].BookID);
                    }
                    else {
                        if (BookList[i].UserID == userID) {
                            cout << "You have already borrowed this book" << endl;
                        }
                        else {
                            cout << "This book is borrowed by someone else" << endl;
                        }
                    }
                    return;
                }
            }
        }
    }
}

void Enqueue(long long int userID, vector<User>& UserList, vector<Book>& BookList) {
    string Name;
    cout << "Book Name: ";
    cin.ignore();
    getline(cin, Name);
    if (!NameValidation(Name)) {
        cout << "Invalid Name" << endl;
        return;
    }
    User user = Find(userID, UserList);
    if (!SearchBook(Name, BookList)) {
        cout << "Book not found" << endl;
        return;
    }
    for (unsigned int i = 0; i < BookList.size(); i++) {
        if (BookList[i].Name == Name) {
            if (BookList[i].Available && BookList[i].Queue.size() == 0) {
                cout << "You can borrow the book now!" << endl;
            }
            else {
                BookList[i].Queue.push_back(user);
                cout << "You got in queue for this book" << endl;
            }
        }
    }
}

void ReturnBook(long long int userID, vector<User>& UserList, vector<Book>& BookList) {
    string Name;
    cout << "Book Name: ";
    cin.ignore();
    getline(cin, Name);
    if (!NameValidation(Name)) {
        cout << "Invalid Name" << endl;
        return;
    }
    if (!SearchBook(Name, BookList)) {
        cout << "Book not found" << endl;
        return;
    }
    for (unsigned int i = 0; i < BookList.size(); i++) {
        if (BookList[i].Name == Name) {
            bool found = false;
            for (unsigned int j = 0; j < UserList.size(); j++) {
                if (UserList[j].UserID == userID) {
                    for (unsigned int k = 0; k < UserList[j].BorrowedBookIDs.size(); k++) {
                        if (UserList[j].BorrowedBookIDs[k] == BookList[i].BookID) {
                            UserList[j].BorrowedBookIDs.erase(UserList[j].BorrowedBookIDs.begin() + k);
                            found = true;
                        }
                    }
                    if (!found) {
                        cout << "You haven't borrowed this book yet" << endl;
                        return;
                    }
                    Time time;
                    cout << "Enter The current time" << endl;
                    cout << "Year: ";
                    cin >> time.Year;
                    cout << "Month: ";
                    cin >> time.Month;
                    cout << "Day: ";
                    cin >> time.Day;
                    BookList[i].UserID = -1;
                    BookList[i].Available = true;
                    if (BookList[i].Queue.size() != 0) {
                        cout << "User " << BookList[i].Queue[0].Name << " can now borrow the book " << BookList[i].Name << endl;
                    }
                    int days = TimeInDays(time, BookList[i].ReturnTime);
                    int newDebt;
                    if (days > 0) {
                        newDebt = days * BookList[i].DailyPenalty ;
                    }
                    else {
                        newDebt = 0;
                    }
                    if (newDebt > 0) {
                        cout << "You returned the book late. your debt was " << UserList[j].debt << " and now it is ";
                        UserList[j].debt += newDebt;
                        cout << UserList[j].debt << endl;
                    }
                    else {
                        cout << "Book returned" << endl;
                    }
                }
            }
        }
    }
}
    
void ReturnTimeExtension(long long int userID, vector<User>& UserList, vector<Book>& BookList) {
    string BookID;
    cout << "Book ID: ";
    cin >> BookID;
    if (!BookIDValidation(BookID)) {
        cout << "Invalid Book ID" << endl;
        return;
    }
    User user = Find(userID, UserList);
    bool found = false;
    for (unsigned int i = 0; i < user.BorrowedBookIDs.size(); i++) {
        if (user.BorrowedBookIDs[i] == BookID) {
            found = true;
        }
    }
    if (!SearchBookID(BookID, BookList)) {
        cout << "Book not found" << endl;
        return;
    }
    if (!found) {
        cout << "You haven't borrowed this book yet" << endl;
        return;
    }
    for (unsigned int i = 0; i < BookList.size(); i++) {
        if (BookList[i].BookID == BookID) {
            if (BookList[i].Queue.size() == 0) {
                BookList[i].ReturnTime = ReturningTime(BookList[i].ReturnTime);
                cout << "You should return the book until: " << ConvertToString(BookList[i].ReturnTime) << endl; 
            }
            else {
                cout << "You can't extend the returning time because there are " <<  BookList[i].Queue.size() << " people in the queue!" << endl;
            }
            return;
        }
    }
}

void Pay(long long int userID, vector<User> UserList, vector<Book> BookList) {
    User user = Find(userID, UserList);
    Time time;
    cout << "Enter The current time" << endl;
    cout << "Year: ";
    cin >> time.Year;
    cout << "Month: ";
    cin >> time.Month;
    cout << "Day: ";
    cin >> time.Day;
    int sum = user.debt;
    for (unsigned int i = 0; i < user.BorrowedBookIDs.size(); i++) {
        Book book = FindBook(user.BorrowedBookIDs[i], BookList);
        int days = TimeInDays(time, book.ReturnTime);
        if (days != -1) {
            sum += book.DailyPenalty * days;
        }
    }
    if (sum == 0) {
        cout << "You have no debt" << endl;
    }
    else {
        cout << "You must pay " << sum << endl;
    }
}

void UserStats(long long int userID, vector<User> UserList, vector<Book> BookList) {
    User user = Find(userID, UserList);
    if (user.BorrowedBookIDs.size() == 0) {
        cout << "you haven't borrowed any book" << endl;
    }
    for (unsigned int i = 0; i < user.BorrowedBookIDs.size(); i++) {
        Book book = FindBook(user.BorrowedBookIDs[i], BookList);
        
        cout << "Book name: " << book.Name << " / Book ID: " << book.BookID << " / Returning Time: " << ConvertToString(book.ReturnTime) << endl;
    }
}

bool BookIDValidation(string bookID) {
    if (bookID.length() == 10) {
        for (int i = 0; i < 10; i++) {
            if (!isalnum(bookID[i])) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool UserIDValidation(long long int userID, bool admin, vector<Admin>& AdminList, vector<User>& UserList) {
    if (userID / 10000000000 == 0) {
        if (admin) {
            if (userID / 1000000000 == 1) {
                for (unsigned int i = 0; i < AdminList.size(); i++) {
                    if (AdminList[i].UserID == userID) {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }
        else if (!admin) {
            if (userID / 1000000000 == 2) {
                for (unsigned int i = 0; i < UserList.size(); i++) {
                    if (UserList[i].UserID == userID) {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }
    }
    return false;
}

bool NameValidation(string Name) {
    if (Name.length() <= 100) {
        return true;
    }
    return false;
}

bool WriterValidation(string Writer) {
    if (Writer.length() <= 50) {
        return true;
    }
    return false;
}

bool PasswordValidation(string Password) {
    if (Password.length() <= 10) {
        return true;
    }
    return false;
}

int LogIn(long long int UserID, vector<User>& UserList, vector<Admin>& AdminList) {
    for (unsigned int i = 0; i < UserList.size(); i++) {
        if (UserList[i].UserID == UserID) {
            string Password;
            cout << "Password: ";
            cin >> Password;
            if (UserList[i].Password == Password) {
                return 1; // user is logged in
            }
            return -1; // Wrong password
        }
    }
    for (unsigned int i = 0; i < AdminList.size(); i++) {
        if (AdminList[i].UserID == UserID) {
            string Password;
            cout << "Password: ";
            cin >> Password;
            if (AdminList[i].Password == Password) {
                return 2; // admin is logged in
            }
            return -1; // Wrong password
        }
    }
    return 0; // no user or admin found with this UserID
}

string ConvertToString(Time t) {
    return to_string(t.Year) + "/" + to_string(t.Month) + "/" + to_string(t.Day);
}

Time ReturningTime(Time t) { // returning time is 14 days
	t.Day += 14;
	if (t.Day > 30) {
		t.Month++;
		t.Day -= 30;
		if (t.Month > 12) {
			t.Year++;
			t.Month -= 12;
		}
	}
	return t;
}

int TimeInDays(Time Now, Time t) {
	int Year = Now.Year - t.Year;
	int Month = Now.Month - t.Month;
	int Day = Now.Day - t.Day;
    if (Year * 365 + Month * 30 + Day >= 0) {
        return Year * 360 + Month * 30 + Day;
    }   
	return -1;
}

void LoadFromFile(vector<Book>& BookList, vector<Admin>& AdminList, vector<User>& UserList) {
    fstream userFile, BookFile;
    userFile.open("Users.txt", ios::in);
    string line;
    while (getline(userFile, line)) {
        if (line == "user") {
            string Name, Password, Phone, userID, Debt, BookIDsCount;
            getline(userFile, userID);
            long long int UserID;
            if (userID != "") {
                UserID = stoll(userID);
            }
            getline(userFile, Name);
            getline(userFile, Password);
            getline(userFile, Phone);
            getline(userFile, Debt);
            getline(userFile, BookIDsCount);
            vector<string> BookIDs;
            string BookID;
            int count = 0;
            if (BookIDsCount != "") {
                count = stoi(BookIDsCount);
            }
            for (int i = 0; i < count; i++) {
                getline(userFile, BookID);
                BookIDs.push_back(BookID);
            }
            User user = User(UserID, Name, Password, Phone);
            if (Debt != "") {
                user.debt = stoi(Debt);
            }
            user.BorrowedBookIDs = BookIDs;
            
            UserList.push_back(user);
        }
        else if (line == "admin") {
            string Name, Password, userID, EducationDegree, Specialty;
            getline(userFile, userID);
            long long int UserID;
            if (userID != "") {
                UserID = stoll(userID);
            }
            getline(userFile, Name);
            getline(userFile, Password);
            getline(userFile, EducationDegree);
            getline(userFile, Specialty);
            
            Admin admin = Admin(UserID, Name, Password, EducationDegree, Specialty);
            AdminList.push_back(admin);
        }
    }
    userFile.close();
    string line1;
    BookFile.open("Books.txt", ios::in);
    while (getline(BookFile, line1)) {
        if (line1 == "book") {
            string BookID, Name, dailyPentalty, userID, year, month, day, availablity, queue;
            getline(BookFile, BookID);
            getline(BookFile, Name);
            getline(BookFile, dailyPentalty);
            int DailyPentalty = 0;
            if (dailyPentalty != "") {
                DailyPentalty = stoi(dailyPentalty);
            }
            getline(BookFile, userID);
            long long int UserID = 0;
            if (userID != "") {
                UserID = stoll(userID);
            }
            getline(BookFile, year);
            int Year = 0;
            if (year != "") {
                Year = stoi(year);
            }
            getline(BookFile, month);
            int Month = 0;
            if (month != "") {
                Month = stoi(month);
            }
            getline(BookFile, day);
            int Day = 0;
            if (day != "") {
                Day = stoi(day);
            }
            getline(BookFile, availablity);

            Book book = Book(BookID, Name, DailyPentalty);
            book.Available = availablity == "a" ? true : false;
            Time time;
            time.Year = Year;
            time.Month = Month;
            time.Day = Day;
            book.ReturnTime = time;
            book.UserID = UserID;
            vector<User> Queue;
            string queueSize;
            getline(BookFile, queueSize);
            unsigned int Size = 0;
            if (queueSize != "") {
                Size = stoi(queueSize);
            }
            string userId;
            for (unsigned int i = 0; i < Size; i++) {
                getline(BookFile, userId);
                long long int USERID = 0;
                if (userId != "") {
                    USERID = stoll(userId);
                }
                User user = Find(USERID, UserList);
                Queue.push_back(user);
            }
            book.Queue = Queue;
            BookList.push_back(book);
        }
        else {
            return;
        }
    }
    BookFile.close();
}

void addBookToFile(Book book) {
    fstream file;
    file.open("Books.txt", ios::app);
    file << "book" << endl;
    file << book.BookID << endl;
    file << book.Name << endl;
    file << book.DailyPenalty << endl;
    file << book.UserID << endl;
    file << book.ReturnTime.Year << endl;
    file << book.ReturnTime.Month << endl;
    file << book.ReturnTime.Day << endl;
    if (book.Available) {
        file << "a" << endl;
    }
    else {
        file << "u" << endl;
    }
    file << book.Queue.size() << endl;
    for (unsigned int i = 0; i < book.Queue.size(); i++) {
        file << book.Queue[i].UserID << " ";
    }
    file << endl;
    file.close();
}

void addUserToFile(User user) {
    fstream file;
    file.open("Users.txt", ios::app);
    file << "user" << endl;
    file << user.UserID << endl;
    file << user.Name << endl;
    file << user.Password << endl;
    file << user.Phone << endl;
    file << user.debt << endl;
    file << user.BorrowedBookIDs.size() << endl;
    for (unsigned int i = 0; i < user.BorrowedBookIDs.size(); i++) {
        file << user.BorrowedBookIDs[i] << endl;
    }
    file.close();
}

void addAdminToFile(Admin admin) {
    fstream file;
    file.open("Users.txt", ios::app);
    file << "admin" << endl;
    file << admin.UserID << endl;
    file << admin.Name << endl;
    file << admin.Password << endl;
    file << admin.EducationDegree << endl;
    file << admin.Specialty << endl;
    file.close();
}

void updateBooks(vector<Book> BookList) {
    fstream file;
    file.open("Books.txt", ios::out);
    file << "";
    for (unsigned int i = 0; i < BookList.size(); i++) {
        addBookToFile(BookList[i]);
    }
    file.close();
}

void updateUsers(vector<Admin> AdminList, vector<User> UserList) {
    fstream file;
    file.open("Users.txt", ios::out);
    file << "";
    for (unsigned int i = 0; i < AdminList.size(); i++) {
        addAdminToFile(AdminList[i]);
    }
    for (unsigned int i = 0; i < UserList.size(); i++) {
        addUserToFile(UserList[i]);
    }
    file.close();
}