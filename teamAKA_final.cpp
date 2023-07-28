/*
SUMMER COURSE: OBJECT ORIENTED PROGRAMMING WITH C++ 
MINI PROJECT CODE
CONFERENCE ROOM BOOKING SYSTEM
TEAM NAME: AKA
TEAM MEMBERS:
Anamika Dey(PES2UG20CS042)
Anurag Mummadi(PES2UG20CS058)
Khushi Satish Hiremath(PES2UG20CS162)
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

const int NUM_FLOORS = 4;
const int ROOMS_PER_FLOOR = 5;
const string DATABASE_FILE = "conference_rooms.csv";

class ConferenceRoom {
private:
    static const int MAX_ROOMS = 10;

protected:
    string roomName;
    bool isAvailable;
    string bookedBy;
    string bookedFor;

public:
    ConferenceRoom(const string& name) : roomName(name), isAvailable(true) {}

    virtual ~ConferenceRoom() = default;

    virtual void bookRoom(const string& user, const string& purpose, const string& time) {
        if (isAvailable) {
            isAvailable = false;
            bookedBy = user;
            bookedFor = purpose + " at " + time;
            cout << roomName << " has been booked by " << bookedBy << " for " << bookedFor << endl;
        } else {
            cout << "Sorry, " << roomName << " is already booked." << endl;
        }
    }

    void cancelBooking() {
        if (!isAvailable) {
            isAvailable = true;
            bookedBy = "";
            bookedFor = "";
            cout << "Booking for " << roomName << " has been canceled." << endl;
        } else {
            cout << roomName << " is not booked." << endl;
        }
    }

    bool isRoomAvailable() const {
        return isAvailable;
    }

    string getRoomName() const {
        return roomName;
    }

    static int getMaxRooms() {
        return MAX_ROOMS;
    }
};

class SmallConferenceRoom : public ConferenceRoom {
public:
    SmallConferenceRoom(const string& name) : ConferenceRoom(name) {}

    // Override base class function
    void bookRoom(const string& user, const string& purpose, const string& time) override {
        // Additional logic specific to small conference rooms (if any)
        ConferenceRoom::bookRoom(user, purpose, time);
    }
};

class LargeConferenceRoom : public ConferenceRoom {
public:
    LargeConferenceRoom(const string& name) : ConferenceRoom(name) {}

    // Override base class function
    void bookRoom(const string& user, const string& purpose, const string& time) override {
        // Additional logic specific to large conference rooms (if any)
        ConferenceRoom::bookRoom(user, purpose, time);
    }
};

struct Booking {
    string name;
    string employeeID;
    string numOfPeople;
    string date;
    string startTime;
    string duration;
    string roomNumber;
};

void readDatabase( std::vector<Booking>& bookings) {
    
    // string filename="conference_bookings.csv";  

// =========================================================================================

    // code for reading:
    const char* env_var = getenv("Paratha");
    string filename;
    if(env_var == NULL){
        filename = "conference_bookings.csv";
    }
    else{
        filename = env_var;
        filename = filename + "\\conference_bookings.csv";
    }

// =========================================================================================
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string field;
        Booking booking;

        // Read the fields from the CSV line
        std::getline(ss, field, ',');
        booking.name = field;

        std::getline(ss, field, ',');
        booking.employeeID = field;

        std::getline(ss, field, ',');
        booking.numOfPeople = field;

        std::getline(ss, field, ',');
        booking.date = field;

        std::getline(ss, field, ',');
        booking.startTime = field;

        std::getline(ss, field, ',');
        booking.duration = field;

        std::getline(ss, field, ',');
        booking.roomNumber = field;

        // Add the Booking object to the vector
        bookings.push_back(booking);
        
    }

    

}


void writeDatabase(const vector<Booking>& bookings) {
    std::ofstream file(DATABASE_FILE);
    if (!file.is_open()) {
        cerr << "Error saving data to the database.\n";
        return;
    }

    for (const auto& booking : bookings) {
        file << booking.name << " " << booking.employeeID << " "
            << booking.numOfPeople << " " << booking.date << " "
            << booking.startTime << " " << booking.duration << " "
            << booking.roomNumber << "\n";
    }
    file.close();
}

void displayAvailableRooms(const std::vector<Booking>& bookings) {
    vector<bool> rooms(NUM_FLOORS * ROOMS_PER_FLOOR, true);

    for (const auto& booking : bookings) {
        rooms[std::stoi(booking.roomNumber) - 1] = false;
    }

    cout << "Available Rooms:\n";
    cout << "----------------\n";
    cout << "Room No. | Floor\n";
    cout << "---------|------\n";
    for (int floor = 1; floor <= NUM_FLOORS; ++floor) {
        for (int room = 1; room <= ROOMS_PER_FLOOR; ++room) {
            int roomNumber = (floor - 1) * ROOMS_PER_FLOOR + room;
            if (rooms[roomNumber - 1]) {
                cout << std::setw(8) << roomNumber << " | " << floor << "\n";
            }
        }
    }
    cout << "----------------\n";
}

void bookRoom(vector<Booking>& bookings) {
    Booking newBooking;
    cout << "Enter your name: ";
    getline(cin >> ws, newBooking.name);

    cout << "Enter your employee ID: ";
    getline(cin >> ws, newBooking.employeeID);

    cout << "Number of people attending the conference: ";
    cin >> newBooking.numOfPeople;

    cout << "Date of the conference (YYYY-MM-DD): ";
    cin >> newBooking.date;

    cout << "Starting time of the conference (HH:MM): ";
    cin >> newBooking.startTime;

    cout << "Duration of the conference (in hours): ";
    cin >> newBooking.duration;

    displayAvailableRooms(bookings);

    cout << "Enter the room number you want to book: ";
    cin >> newBooking.roomNumber;

    cin.ignore(); // Clear the newline character from the previous input
    bookings.push_back(newBooking);
    writeDatabase(bookings);

    cout << "Booking confirmed. Room #" << newBooking.roomNumber
        << " is booked for " << newBooking.duration << " hours on "
        << newBooking.date << " at " << newBooking.startTime << ".\n";
}

void displayBookings(const std::vector<Booking>& bookings, const std::string& employeeID) {
    cout << "Your Bookings:\n";
    cout << "-------------------------------------------\n";
    cout << "Room No. | Date       | Start Time | Duration\n";
    cout << "---------|------------|------------|---------\n";

    for (const auto& booking : bookings) {
        if (booking.employeeID == employeeID) {
            cout << setw(8) << booking.roomNumber << " | " << booking.date << " | "
                << booking.startTime << "   | " << std::setw(7) << booking.duration << "\n";
        }
    }

    cout << "-------------------------------------------\n";
}

void cancelBooking(vector<Booking>& bookings, const string& employeeID) {
    displayBookings(bookings, employeeID);

    cout << "Enter the room number you want to cancel: ";
    int roomNumber;
    cin >> roomNumber;

    cin.ignore(); // Clear the newline character from the previous input
    bool foundBooking = false;
    auto it = bookings.begin();
    while (it != bookings.end()) {
        if (std::stoi(it->roomNumber) == roomNumber && it->employeeID == employeeID) {
            it = bookings.erase(it);
            foundBooking = true;
        } else {
            ++it;
        }
    }

    if (foundBooking) {
        writeDatabase(bookings);
        cout << "Booking for Room #" << roomNumber << " has been canceled.\n";
    } else {
        cout << "Booking not found for Room #" << roomNumber << ".\n";
    }
}

int main() {
    vector<Booking> bookings;
    readDatabase(bookings);

    int choice;
    string employeeID;

    while (true) {
        cout << "Conference Room Booking System\n";
        cout << "----------------------------\n";
        cout << "1. Book a room\n";
        cout << "2. View your bookings\n";
        cout << "3. Cancel a booking\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
    
    if(choice==1 || choice==2|| choice==3|| choice==4){
        switch (choice) {
        case 1:
            bookRoom(bookings);
            break;
        case 2:
            cout << "Enter your employee ID: ";
            cin.ignore(); // Clear the newline character from the previous input
            getline(cin >> ws, employeeID);
            displayBookings(bookings, employeeID);
            break;
        case 3:
            cout << "Enter your employee ID: ";
            cin.ignore(); // Clear the newline character from the previous input
            getline(cin >> ws, employeeID);
            cancelBooking(bookings, employeeID);
            break;
        case 4:
            cout << "Exiting the program.\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
    else
    {
        cout<<"Invalid choice. Please try again.\n";
        return 0;

    }

    return 0;
}}