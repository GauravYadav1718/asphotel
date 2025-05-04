#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <map>
#include <limits>
#include <algorithm>
#include <string>
#include <ctime>
#include <iomanip>
using namespace std;

// ------------------- ASCII DESIGN ELEMENTS -------------------
void printBanner(const string& title) {
    cout << "\n=====================================" << endl;
    cout << "     " << title << endl;
    cout << "=====================================\n" << endl;
}

void printLine() {
    cout << "-------------------------------------\n";
}

// ------------------- HOTEL BOOKING SYSTEM -------------------
class Guest {
public:
    string name;
    int id;
    int roomNumber;
    string checkInTime;
    Guest() {}
    Guest(string n, int i, int r, string time) : name(n), id(i), roomNumber(r), checkInTime(time) {}
};

unordered_map<int, Guest> guestMap;
unordered_map<int, bool> roomAvailability;
vector<int> allRooms = {101, 102, 103, 201, 202, 301, 302};

// ------------------- TIME HELPER -------------------
string getCurrentTime() {
    time_t now = time(0);
    char* dt = ctime(&now);
    return string(dt);
}

// ------------------- GREEDY ROOM ALLOCATION -------------------
int allocateRoom() {
    for (int room : allRooms) {
        if (!roomAvailability[room]) {
            roomAvailability[room] = true;
            return room;
        }
    }
    return -1;
}

// ------------------- BACKTRACKING FOR VIP UPGRADE -------------------
bool vipUpgrade(vector<int>& rooms, int index, int target, vector<int>& path) {
    if (target == 0) return true;
    if (index >= rooms.size() || target < 0) return false;
    path.push_back(rooms[index]);
    if (vipUpgrade(rooms, index + 1, target - rooms[index], path)) return true;
    path.pop_back();
    return vipUpgrade(rooms, index + 1, target, path);
}

// ------------------- DYNAMIC PROGRAMMING FOR DISCOUNT SCHEDULING -------------------
int dpSchedule(vector<int>& discounts) {
    int n = discounts.size();
    if (n == 0) return 0;
    if (n == 1) return discounts[0];
    vector<int> dp(n+1, 0);
    dp[1] = discounts[0];
    for (int i = 2; i <= n; ++i) {
        dp[i] = max(dp[i-1], dp[i-2] + discounts[i-1]);
    }
    return dp[n];
}

// ------------------- CHECK-IN -------------------
void checkIn() {
    printBanner("Guest Check-In");
    string name;
    int id;
    cout << "Enter guest name: ";
    cin >> name;
    cout << "Enter guest ID: ";
    cin >> id;

    if (guestMap.find(id) != guestMap.end()) {
        cout << "Guest with this ID already exists.\n";
        return;
    }

    int room = allocateRoom();
    if (room == -1) {
        cout << "No rooms available for " << name << endl;
        return;
    }

    string checkInTime = getCurrentTime();
    guestMap[id] = Guest(name, id, room, checkInTime);
    cout << name << " checked into room " << room << " at " << checkInTime << endl;
    printLine();
}

// ------------------- CHECK-OUT -------------------
void checkOut() {
    printBanner("Guest Check-Out");
    int id;
    cout << "Enter guest ID to check out: ";
    cin >> id;

    if (guestMap.find(id) == guestMap.end()) {
        cout << "Guest not found.\n";
        return;
    }
    int room = guestMap[id].roomNumber;
    roomAvailability[room] = false;
    cout << guestMap[id].name << " checked out from room " << room << endl;
    guestMap.erase(id);
    printLine();
}

// ------------------- DISPLAY GUEST LIST -------------------
void displayGuests() {
    printBanner("Current Guest List");
    cout << left << setw(10) << "ID" << setw(15) << "Name" << setw(10) << "Room" << "Check-in Time" << endl;
    printLine();
    for (auto& entry : guestMap) {
        cout << left << setw(10) << entry.first << setw(15) << entry.second.name << setw(10) << entry.second.roomNumber << entry.second.checkInTime;
    }
    printLine();
}

// ------------------- DISPLAY ROOM STATUS -------------------
void displayRooms() {
    printBanner("Room Availability");
    for (int room : allRooms) {
        cout << "Room " << room << ": " << (roomAvailability[room] ? "Occupied" : "Available") << endl;
    }
    printLine();
}

// ------------------- DISPLAY HELP -------------------
void displayHelp() {
    printBanner("Help Menu");
    cout << "1: Book a room for a new guest.\n";
    cout << "2: Check out a guest using their ID.\n";
    cout << "3: View list of currently checked-in guests.\n";
    cout << "4: View room availability status.\n";
    cout << "5: Try VIP room upgrades.\n";
    cout << "6: Calculate best combination of discounts.\n";
    cout << "0: Exit the system.\n";
    printLine();
}

// ------------------- MAIN -------------------
int main() {
    int choice;
    printBanner(" Hotel Booking ");

    do {
        cout << "\n1. Book a Room\n2. Check Out\n3. Display Guest List\n4. Display Room Status\n5. VIP Upgrade\n6. Calculate Discounts\n7. Help\n0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                checkIn();
                break;
            case 2:
                checkOut();
                break;
            case 3:
                displayGuests();
                break;
            case 4:
                displayRooms();
                break;
            case 5: {
                vector<int> prices = {50, 100, 150};
                vector<int> upgradePath;
                cout << "\nVIP Upgrade for 150: ";
                if (vipUpgrade(prices, 0, 150, upgradePath)) {
                    for (int p : upgradePath) cout << p << " ";
                    cout << endl;
                } else cout << "No upgrade path found\n";
                break;
            }
            case 6: {
                vector<int> discountOptions = {10, 20, 30, 40};
                int maxDiscount = dpSchedule(discountOptions);
                cout << "\nMaximum Discount Value: " << maxDiscount << endl;
                break;
            }
            case 7:
                displayHelp();
                break;
            case 0:
                printBanner("Exiting system. Goodbye!");
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 0);

    return 0;
}
