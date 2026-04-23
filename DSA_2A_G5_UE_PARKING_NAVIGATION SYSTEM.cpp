#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <climits>
#include <algorithm>
#include <ctime>
#include <limits>
#include <chrono>
#include <thread>
#include <cctype>
#include <string>

using namespace std;

// ================= UTIL =================
string toLower(string s) {
    for (char &c : s) c = tolower(c);
    return s;
}

string getCurrentTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    char buffer[30];
    sprintf(buffer, "%02d/%02d/%04d %02d:%02d:%02d",
            ltm->tm_mday,
            ltm->tm_mon + 1,
            1900 + ltm->tm_year,
            ltm->tm_hour,
            ltm->tm_min,
            ltm->tm_sec);

    return string(buffer);
}

// ================= SAFE INPUT =================
int safeInput(int minVal, int maxVal) {
    int choice, attempts = 0;

    while (attempts < 5) {
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "❌ Invalid input. Try again: ";
        }
        else if (choice < minVal || choice > maxVal) {
            cout << "❌ Incorrect choice. Try again: ";
        }
        else return choice;

        attempts++;
    }

    cout << "\n🚫 Too many invalid attempts. Program terminated.\n";
    exit(0);
}

// ================= SAFE ROLE INPUT =================
string safeRoleInput() {
    string role;
    int attempts = 0;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    while (attempts < 5) {
        cout << "Enter role: ";
        getline(cin, role);
        role = toLower(role);

        if (role == "student" || role == "faculty" || role == "dean" ||
            role == "president" || role == "chancellor" || role == "assocdean") {
            return role;
        }

        cout << "❌ Invalid role. Try again.\n";
        attempts++;
    }

    cout << "\n🚫 Too many invalid role attempts. Program terminated.\n";
    exit(0);
}

// ================= LOGS =================
struct ParkingLog {
    string role;
    string location;
    string timeIn;
    string timeOut;
};

vector<ParkingLog> logs;

// ================= PARKING STRUCT =================
struct ParkingInfo {
    int studentSlots;
    int facultySlots;
    int deanCBA, deanCOE, deanCAS, deanCFAD;
    int distanceFromGate;
};

// ================= PARKING DATA =================
unordered_map<string, ParkingInfo> parking = {
    {"Gymnasium", {32,0,0,0,0,0,80}},
    {"Lucio Tan", {15,0,0,0,0,0,430}},
    {"Front Engineering B1uilding", {32,0,1,1,1,1,400}},
    {"Tan Yan Kee Side Parking - Block 1", {0,0,1,0,1,0,130}},
    {"Tan Yan Kee Side Parking - Block 2", {0,0,0,1,0,1,130}},
    {"Engineering Side", {12,0,0,0,0,0,340}},
    {"Engineering Back", {12,0,0,0,0,0,470}}
};

vector<string> parkingList = {
    "Gymnasium",
    "Lucio Tan",
    "Front Engineering Building",
    "Tan Yan Kee Side Parking - Block 1",
    "Tan Yan Kee Side Parking - Block 2",
    "Engineering Side",
    "Engineering Back"
};

// ================= ROLE RULES =================
bool isAllowed(string role, string name) {
    if (role == "student")
        return name == "Gymnasium" || name == "Front Engineering Building" ||
               name == "Engineering Side" || name == "Engineering Back";

    if (role == "faculty")
        return name == "Lucio Tan" || name == "Front Engineering Building" ||
               name == "Engineering Back";

    if (role == "assocdean")
        return name == "Front Engineering Building" ||
               name == "Tan Yan Kee Side Parking - Block 1" ||
               name == "Tan Yan Kee Side Parking - Block 2";

    if (role == "president" || role == "chancellor")
        return name == "Gymnasium";

    return true;
}

// ================= SLOT CHECK =================
bool hasSlot(string role, ParkingInfo p) {
    if (role == "student") return p.studentSlots > 0;
    if (role == "faculty") return (p.studentSlots > 0 || p.facultySlots > 0);

    if (role == "dean_coe") return p.deanCOE > 0;
    if (role == "dean_cba") return p.deanCBA > 0;
    if (role == "dean_cas") return p.deanCAS > 0;
    if (role == "dean_cfad") return p.deanCFAD > 0;

    return true;
}

// ================= GRAPH (Simplified - only for distance) =================
struct Edge { int to, weight; };

class Graph {
public:
    unordered_map<string,int> nameToIndex;
    vector<string> indexToName;
    vector<vector<Edge>> adj;
    int nodeCount = 0;

    int addNode(string name) {
        string l = toLower(name);
        if (nameToIndex.count(l)) return nameToIndex[l];

        nameToIndex[l] = nodeCount++;
        indexToName.push_back(name);
        adj.push_back({});
        return nodeCount - 1;
    }

    void addEdge(string u, string v, int w) {
        int a = addNode(u);
        int b = addNode(v);
        adj[a].push_back({b, w});
        adj[b].push_back({a, w});
    }

    // ================= SIMPLIFIED DISTANCE ONLY =================
    int getShortestDistance(string start, string dest) {
        vector<int> dist(nodeCount, INT_MAX);
        int src = nameToIndex[toLower(start)];
        int target = nameToIndex[toLower(dest)];

        dist[src] = 0;
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
        pq.push({0, src});

        while (!pq.empty()) {
            auto [cd, u] = pq.top(); pq.pop();
            if (cd > dist[u]) continue;

            for (auto &e : adj[u]) {
                if (dist[e.to] > dist[u] + e.weight) {
                    dist[e.to] = dist[u] + e.weight;
                    pq.push({dist[e.to], e.to});
                }
            }
        }
        return dist[target];
    }

    // ================= FIXED NAVIGATION GUIDE =================
    void showDistance(string dest) {
        int distance = getShortestDistance("Main Gate", dest);
        cout << "\n📏 Distance = " << distance << " meters\n";
    }

    // ✅ COMPLETE FIX FOR ALL DEAN ROLES
    void guide(string dest) {
        cout << "\n===== NAVIGATION GUIDE =====\nStart: Main Gate\n";

        if (dest == "Gymnasium") {
            cout << "→ Gymnasium (80m)\n";
        }
        else if (dest.find("Tan Yan Kee Side Parking") != string::npos) {
            cout << "→ Tyk Front → Tyk Side → " << dest << " (130m)\n";
        }
        else if (dest == "Front Engineering Building") {
            cout << "→ Tyk Front → Tyk Side → Front Engineering Building (400m)\n";
        }
        else if (dest == "Engineering Side") {
            cout << "→ Tyk Front → Tyk Side → Front Engineering Building → Engineering Side (340m)\n";
        }
        else if (dest == "Engineering Back") {
            cout << "→ Tyk Front → Tyk Side → Front Engineering Building → Engineering Side → Engineering Back (470m)\n";
        }
        else if (dest == "Lucio Tan") {
            cout << "→ Main Road → Lucio Tan Building (430m)\n";
        }
        else {
            cout << "→ Direct route to " << dest << "\n";
        }
    }

    // ================= SORTING & SEARCHING =================
    int getDistance(string name) {
        return parking[name].distanceFromGate;
    }

    // 🔍 NEW: LINEAR SEARCH
    bool linearSearch(const vector<string>& arr, string target) {
        cout << "\n🔍 Performing Linear Search...\n";
        for (int i = 0; i < arr.size(); i++) {
            cout << "Checking index " << (i+1) << ": " << arr[i] << "... ";
            this_thread::sleep_for(chrono::milliseconds(300));
            
            if (toLower(arr[i]) == toLower(target)) {
                cout << "✅ FOUND!\n";
                return true;
            }
            cout << "❌ Not found\n";
        }
        cout << "🔍 Search completed. Target '" << target << "' not found.\n";
        return false;
    }

    int partition(vector<string>& arr, int low, int high) {
        int pivot = getDistance(arr[high]);
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (getDistance(arr[j]) < pivot)
                swap(arr[++i], arr[j]);
        }

        swap(arr[i + 1], arr[high]);
        return i + 1;
    }

    void quickSort(vector<string>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }

    void insertionSort(vector<string>& arr) {
        for (int i = 1; i < arr.size(); i++) {
            string key = arr[i];
            int j = i - 1;

            while (j >= 0 && getDistance(arr[j]) > getDistance(key)) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }

    // 🔍 NEW: ENHANCED SHOW SORTED PARKING WITH LINEAR SEARCH
    void showSortedParking() {
        vector<string> temp = parkingList;

        cout << "\n🎯 SORTING & SEARCH DEMO\n";
        cout << "1. 🔥 Quick Sort\n";
        cout << "2. 🐌 Insertion Sort\n";
        cout << "3. 🔍 Linear Search\n";
        cout << "Choice: ";
        int c = safeInput(1, 3);

        if (c == 1) {
            cout << "\n🔥 Quick Sort (by distance):\n";
            quickSort(temp, 0, temp.size() - 1);
            cout << "✅ Quick Sort completed!\n";
        } 
        else if (c == 2) {
            cout << "\n🐌 Insertion Sort (by distance):\n";
            insertionSort(temp);
            cout << "✅ Insertion Sort completed!\n";
        } 
        else { // c == 3 - Linear Search
            cout << "\nEnter parking name to search: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            string target;
            getline(cin, target);
            
            bool found = linearSearch(parkingList, target);
            
            if (found) {
                cout << "\n🎉 Parking '" << target << "' exists!\n";
                cout << "📏 Distance: " << getDistance(target) << "m\n";
            } else {
                cout << "\n❌ Parking '" << target << "' not found in system.\n";
            }
            return;
        }

        // Display sorted results
        cout << "\n📋 SORTED RESULTS (by distance from gate):\n";
        cout << "----------------------------------------\n";
        for (auto &n : temp)
            cout << "📍 " << n << " | " << getDistance(n) << "m\n";
    }

    // ================= PARKING LOGIC =================
    void showLive(string role) {
        cout << "\n===== LIVE PARKING STATUS =====\n";

        int idx = 1;
        for (auto &name : parkingList) {
            if (!isAllowed(role, name)) continue;

            ParkingInfo p = parking[name];
            int avail = p.studentSlots;
            if (role == "faculty") avail += p.facultySlots;

            cout << idx++ << ". " << name << " | Slots: " << avail << "\n";
        }
    }

    string autoSelect(string role) {
        if (role == "president" || role == "chancellor")
            return "Gymnasium";

        string best = "";
        int bestDist = INT_MAX;

        for (auto &name : parkingList) {
            if (!isAllowed(role, name)) continue;

            if (parking[name].distanceFromGate < bestDist &&
                hasSlot(role, parking[name])) {
                bestDist = parking[name].distanceFromGate;
                best = name;
            }
        }

        return best;
    }

    void updateSlots(string role, string dest) {
        ParkingInfo &p = parking[dest];

        if (role == "student") p.studentSlots--;
        else if (role == "faculty") {
            if (p.facultySlots > 0) p.facultySlots--;
            else p.studentSlots--;
        }
        else if (role == "dean_coe") p.deanCOE--;
        else if (role == "dean_cba") p.deanCBA--;
        else if (role == "dean_cas") p.deanCAS--;
        else if (role == "dean_cfad") p.deanCFAD--;
    }

    void chooseParking(string role) {
        showLive(role);

        string dest;

        if (role == "dean") {
            cout << "\nSelect College:\n1. CBA\n2. COE\n3. CAS\n4. CFAD\nChoice: ";
            int c = safeInput(1, 4);

            if (c == 1) role = "dean_cba";
            else if (c == 2) role = "dean_coe";
            else if (c == 3) role = "dean_cas";
            else role = "dean_cfad";

            dest = autoSelect(role);
            cout << "\n📍 Assigned Parking: " << dest << "\n";
        }
        else if (role == "student") {
            cout << "\nWhere do you want to park?\n";
            cout << "1. Gymnasium\n2. Front Engineering Building\n3. Engineering Side\n4. Engineering Back\nChoice: ";

            int p = safeInput(1, 4);

            vector<string> opts = {
                "Gymnasium",
                "Front Engineering Building",
                "Engineering Side",
                "Engineering Back"
            };

            dest = opts[p - 1];
        }
        else {
            cout << "\n1. Auto\n2. Manual\nChoice: ";
            int c = safeInput(1, 2);

            if (c == 1) {
                dest = autoSelect(role);
                cout << "\n🤖 Auto-selected: " << dest << "\n";
            }
            else {
                cout << "\nSelect parking:\n";
                int idx = 1;
                vector<string> allowed;
                for (auto &name : parkingList) {
                    if (isAllowed(role, name)) {
                        cout << idx << ". " << name << "\n";
                        allowed.push_back(name);
                        idx++;
                    }
                }
                cout << "Choice: ";
                int choice = safeInput(1, allowed.size());
                dest = allowed[choice - 1];
            }
        }

        if (dest.empty()) {
            cout << "\n❌ No parking location selected.\n";
            return;
        }

        if (!hasSlot(role, parking[dest])) {
            cout << "\n❌ No available slots at " << dest << ".\n";
            return;
        }

        guide(dest);           
        showDistance(dest);    
        updateSlots(role, dest);

        logs.push_back({role, dest, getCurrentTime(), "N/A"});
    }

    void showLogs() {
        cout << "\n===== PARKING LOGS =====\n";
        if (logs.empty()) {
            cout << "No parking records yet.\n";
            return;
        }
        for (auto &l : logs) {
            cout << "👤 " << l.role << " | 📍 " << l.location << "\n";
            cout << "⏰ IN: " << l.timeIn << "\n";
            cout << "⏰ OUT: " << l.timeOut << "\n\n";
        }
    }

    void exitParking() {
        for (int i = logs.size() - 1; i >= 0; i--) {
            if (logs[i].timeOut == "N/A") {
                logs[i].timeOut = getCurrentTime();
                cout << "✅ Exit time recorded for " << logs[i].location << "\n";
                return;
            }
        }
        cout << "❌ No active parking session found\n";
    }
};

// ================= MAIN =================
int main() {
    Graph g;

    // Graph edges (kept for distance calculation)
    g.addEdge("Main Gate", "Gymnasium", 80);
    g.addEdge("Gymnasium", "Tyk Front", 60);
    g.addEdge("Tyk Front", "Tyk Side", 50);
    g.addEdge("Tyk Side", "Front Engineering Building", 40);
    g.addEdge("Front Engineering Building", "Engineering Side", 30);
    g.addEdge("Engineering Side", "Engineering Back", 20);

    cout << "\033[1;31m"; 
    cout << R"(
 _     _____   ____  ____  ____  _  __ _  _      _____   
/ \ /\/  __/  /  __\/  _ \/  __\/ |/ // \/ \  /|/  __/   
| | |||  \    |  \/|| / \||  \/||   / | || |\ ||| |  __  
| \_/||  /_   |  __/| |-|||    /|   \ | || | \||| |_/ /   
\____/\____\  \_/   \_/ \|\_/\_\\_|\_\\_/\_/  \|\____/  

 
 _      ____  _     _  _____ ____ _____ _  ____  _       
/ \  /|/  _ \/ \ |\/ \/  __//  _ Y__ __Y \/  _ \/ \  /|  
| |\ ||| / \|| | //| || |  _| / \| / \ | || / \|| |\ ||  
| | \||| |-||| \// | || |_//| |-|| | | | || \_/|| | \||  
\_/  \|\_/ \|\__/  \_/\____\\_/ \| \_/ \_/\____/\_/  \|  

 ____ ___  _ ____ _____ _____ _                          
/ ___\\  \/// ___Y__ __Y  __// \__/|                     
|    \ \  / |    \ / \ |  \  | |\/||                     
\___ | / /  \___ | | | |  /_ | |  ||                     
\____//_/   \____/ \_/ \____\\_/  \|                     
)" << endl;

    cout << "\033[0m"; 

    while (true) {
        cout << "\n🏪 UE PARKING NAVIGATION SYSTEM\n";
        cout << "1. 🚗 Enter Parking\n2. 🚪 Exit Parking\n3. 📋 View Logs\n4. 📊 Sort Parking\n5. ❌ Quit\nChoice: ";

        int ch = safeInput(1, 5);

        if (ch == 1) {
            string role = safeRoleInput();
            cout << "\n🔄 Processing your request...\n";
            this_thread::sleep_for(chrono::seconds(2));
            g.chooseParking(role);
        }
        else if (ch == 2) g.exitParking();
        else if (ch == 3) g.showLogs();
        else if (ch == 4) g.showSortedParking();
        else {
            cout << "\n\n🎉 THANK YOU FOR USING UE PARKING NAVIGATION SYSTEM!!!\n";
            cout << "🚗 Safe travels!\n";
            break;
        }
    }

    return 0;
}
