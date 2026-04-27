#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cfloat>
#include <cctype>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <string>
#include <cstdlib>
#include <set>
#include <sstream>
#include <sys/ioctl.h>
#include <unistd.h>
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
using namespace std;
inline string toLower(const string& s) {
    string res;
    res.reserve(s.size());  
    for (unsigned char c : s) {
        res += static_cast<char>(tolower(c));  
    }
    return res;
}
int monthToNumber(const string& month) {
    static const unordered_map<string, int> monthMap = {
        {"january", 1}, {"february", 2}, {"march", 3}, {"april", 4},
        {"may", 5}, {"june", 6}, {"july", 7}, {"august", 8},
        {"september", 9}, {"october", 10}, {"november", 11}, {"december", 12}
    };
    auto it = monthMap.find(toLower(month));  
    return it != monthMap.end() ? it->second : 0;  
}
struct ParsedDate {
    int year = 0;
    int month = 0;
    int day = 0;
};
ParsedDate parseDate(const string& dateStr) {
    ParsedDate parsed;
    string month;
    char comma = ',';
    stringstream ss(dateStr);  
    ss >> month >> parsed.day >> comma >> parsed.year;  
    parsed.month = monthToNumber(month);  
    return parsed;
}
unordered_map<string, string> locationGroup = {
    {"Main Gate", "Ground Floor"}, {"Clinic", "Ground Floor"}, {"Main Admission", "Ground Floor"},
    {"Gazebo", "Ground Floor"}, {"Gym", "Ground Floor"}, {"Back Gate", "Ground Floor"},
    {"Waiting Shed", "Ground Floor"}, {"TYK Stairs Left", "Ground Floor"}, {"TYK Elevator Left", "Ground Floor"},
    {"TYK Admission", "Ground Floor"}, {"Field", "Ground Floor"}, {"SAO", "Ground Floor"},
    {"LCT Stairs Mid", "Ground Floor"}, {"LCT Elevator Mid", "Ground Floor"}, {"EN Stairs Left", "Ground Floor"},
    {"RM103", "Ground Floor"}, {"EN Stairs Back Left", "Ground Floor"}, {"Parking", "Ground Floor"},
    {"TYK Stairs Mid", "Ground Floor"}, {"TYK Elevator Mid", "Ground Floor"}, {"Canteen", "Ground Floor"},
    {"TYK Stairs Right", "Ground Floor"}, {"HM Building", "Ground Floor"}, {"EN Stairs Right", "Ground Floor"},
    {"RM117", "Ground Floor"}, {"EN Stairs Back Right", "Ground Floor"}, {"RM100", "Ground Floor"},
    {"EN Admission", "Ground Floor"}, {"CpE Faculty", "Ground Floor"}, {"ME Lab", "Ground Floor"},
    {"TYK Stairs Left 2F", "Second Floor"}, {"TYK Elevator Left 2F", "Second Floor"}, {"RM200", "Second Floor"},
    {"RM201", "Second Floor"}, {"TYK Stairs Mid 2F", "Second Floor"}, {"TYK Elevator Mid 2F", "Second Floor"},
    {"RM203", "Second Floor"}, {"RM204", "Second Floor"}, {"TYK Stairs Right 2F", "Second Floor"},
    {"RM205", "Second Floor"}, {"RM206", "Second Floor"}, {"LCT Stairs Mid 2F", "Second Floor"},
    {"LCT Elevator Mid 2F", "Second Floor"}, {"RM208", "Second Floor"}, {"RM209", "Second Floor"},
    {"EN Stairs Left 2F", "Second Floor"}, {"Chemistry Tool Room", "Second Floor"}, {"EN Stairs Back Left 2F", "Second Floor"},
    {"CLR1", "Second Floor"}, {"CLR2", "Second Floor"}, {"ECE1", "Second Floor"}, {"EEPL1", "Second Floor"},
    {"EN Stairs Right 2F", "Second Floor"}, {"Physics Tool Room", "Second Floor"}, {"EN Stairs Back Right 2F", "Second Floor"},
    {"TYK Stairs Left 3F", "Third Floor"}, {"TYK Elevator Left 3F", "Third Floor"}, {"RM300", "Third Floor"},
    {"RM301", "Third Floor"}, {"TYK Stairs Mid 3F", "Third Floor"}, {"TYK Elevator Mid 3F", "Third Floor"},
    {"RM303", "Third Floor"}, {"RM304", "Third Floor"}, {"TYK Stairs Right 3F", "Third Floor"}, {"Bridge", "Third Floor"},
    {"RM305", "Third Floor"}, {"RM306", "Third Floor"}, {"LCT Stairs Mid 3F", "Third Floor"}, {"LCT Elevator Mid 3F", "Third Floor"},
    {"RM308", "Third Floor"}, {"RM309", "Third Floor"}, {"EN Stairs Left 3F", "Third Floor"}, {"Stock Room", "Third Floor"},
    {"EN Stairs Back Left 3F", "Third Floor"}, {"EN MPH", "Third Floor"}, {"EN Library", "Third Floor"}, {"RM314", "Third Floor"},
    {"EN Stairs Right 3F", "Third Floor"}, {"DAWN Room", "Third Floor"}, {"EN Stairs Back Right 3F", "Third Floor"},
    {"TYK Stairs Left 4F", "Fourth Floor"}, {"TYK Elevator Left 4F", "Fourth Floor"}, {"BGC LIBRARY", "Fourth Floor"},
    {"TYK Stairs Mid 4F", "Fourth Floor"}, {"TYK Elevator Mid 4F", "Fourth Floor"}, {"TYK Stairs Right 4F", "Fourth Floor"},
    {"LCT LIBRARY", "Fourth Floor"}, {"LCT Stairs Mid 4F", "Fourth Floor"}, {"LCT Elevator Mid 4F", "Fourth Floor"},
    {"EN Stairs Left 4F", "Fourth Floor"}, {"DR400", "Fourth Floor"}, {"EN Stairs Back Left 4F", "Fourth Floor"},
    {"RM400", "Fourth Floor"}, {"RM401", "Fourth Floor"}, {"RM402", "Fourth Floor"}, {"RM403", "Fourth Floor"},
    {"RM404", "Fourth Floor"}, {"EN Stairs Right 4F", "Fourth Floor"}, {"DR401", "Fourth Floor"}, {"EN Stairs Back Right 4F", "Fourth Floor"},
    {"TYK Stairs Left 5F", "Fifth Floor"}, {"TYK Elevator Left 5F", "Fifth Floor"}, {"TYK Stairs Mid 5F", "Fifth Floor"},
    {"TYK Elevator Mid 5F", "Fifth Floor"}, {"TYK MPH", "Fifth Floor"}, {"LCT Stairs Mid 5F", "Fifth Floor"},
    {"LCT Elevator Mid 5F", "Fifth Floor"}, {"LCT MPH", "Fifth Floor"}
};
struct Event {
    string name;          
    string location;      
    string date;          
    string status;        
    string statusColor;  
};
vector<Event> allEvents = {
    {"CPE Grand Ball", "TYK MPH", "May 3, 2025", "Upcoming", YELLOW},
    {"CPE Grand Ball", "TYK MPH", "March 30, 2023", "Ended", RED},
    {"CPE Grand Ball", "TYK MPH", "March 23, 2023", "Ended", RED},
    {"PASCOESS", "LCT MPH", "December 23, 2025", "Ended", RED},
    {"PASCOESS", "LCT MPH", "December 24, 2024", "Ended", RED},
    {"PASCOESS", "LCT MPH", "December 8, 2023", "Ended", RED},
    {"PASCOESS", "LCT MPH", "December 15, 2021", "Ended", RED},
    {"Quiz Bee", "LCT MPH", "September 25, 2023", "Ended", RED},
    {"Quiz Bee", "LCT MPH", "February 28, 2022", "Ended", RED},
    {"Quiz Bee", "LCT MPH", "September 24, 2021", "Ended", RED},
    {"Quiz Bee", "LCT MPH", "September 20, 2022", "Ended", RED},
    {"UE Week Booth", "EN MPH", "March 12, 2026", "Ended", RED},
    {"UE Week Booth", "EN MPH", "March 30, 2024", "Ended", RED},
    {"UE Week Booth", "EN MPH", "February 25, 2020", "Ended", RED},
    {"General Assembly", "EN MPH", "September 12, 2025", "Ended", RED},
    {"General Assembly", "EN MPH", "September 30, 2024", "Ended", RED},
    {"General Assembly", "EN MPH", "September 11, 2023", "Ended", RED},
    {"General Assembly", "EN MPH", "February 29, 2019", "Ended", RED}
};
struct Edge {
    int to;          
    float weight;    
};
class Graph {
private:
    unordered_map<string, int> nameToIndex;  
    vector<string> indexToName;              
    vector<vector<Edge>> adj;                
    vector<bool> isStairOrElevator;          
    int nodeCount = 0;                        
public:
    int addNode(const string& name) {
        string lname = toLower(name);
        auto it = nameToIndex.find(lname);
        if (it != nameToIndex.end()) return it->second;  
        int idx = nodeCount++;  
        nameToIndex[lname] = idx;  
        indexToName.push_back(name);  
        adj.push_back({});  
        isStairOrElevator.push_back(name.find("Stair") != string::npos || name.find("Elevator") != string::npos);
        return idx;
    }
    void addEdge(const string& from, const string& to, float weight) {
        int u = addNode(from);  
        int v = addNode(to);    
        adj[u].push_back({v, weight});  
        adj[v].push_back({u, weight});  
    }
    bool hasLocation(const string& name) const {
        return nameToIndex.find(toLower(name)) != nameToIndex.end();
    }
    void dijkstra(const string& start, const string& end) const {
        auto itStart = nameToIndex.find(toLower(start)), itEnd = nameToIndex.find(toLower(end));
        if (itStart == nameToIndex.end() || itEnd == nameToIndex.end()) return;  
        int src = itStart->second, dest = itEnd->second;  
        vector<float> dist(nodeCount, FLT_MAX);  
        vector<int> prev(nodeCount, -1);  
        priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> pq;
        dist[src] = 0.0f;  
        pq.push({0.0f, src});  
        while (!pq.empty()) {
            auto [d, u] = pq.top();  
            pq.pop();
            if (d > dist[u]) continue;  
            for (const auto& e : adj[u]) {
                if (dist[u] + e.weight < dist[e.to]) {
                    dist[e.to] = dist[u] + e.weight;  
                    prev[e.to] = u;  
                    pq.push({dist[e.to], e.to});  
                }
            }
        }
        if (dist[dest] == FLT_MAX) {
            cout << RED << "No path found." << RESET << "\n";
            return;
        }
        vector<int> path;
        for (int at = dest; at != -1; at = prev[at]) path.push_back(at);
        reverse(path.begin(), path.end());  
        cout << YELLOW << "Route to destination (Distance: " << fixed << setprecision(2) << dist[dest] << ")" << RESET << "\n";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << indexToName[path[i]] << (i + 1 < path.size() ? " -> " : "\n");
        }
    }
    void listLocations() const {
        static const vector<string> order = {"Ground Floor", "Second Floor", "Third Floor", "Fourth Floor", "Fifth Floor"};
        unordered_map<string, vector<string>> groups;  
        for (int i = 0; i < nodeCount; ++i) {
            if (isStairOrElevator[i]) continue;  
            groups[locationGroup.count(indexToName[i]) ? locationGroup.at(indexToName[i]) : "Other"].push_back(indexToName[i]);
        }
        for (const auto& group : order) {
            if (!groups.count(group)) continue;  
            cout << YELLOW << "\n[" << group << "]" << RESET << "\n";
            int count = 0;
            for (const auto& name : groups[group]) {
                cout << BLUE << left << setw(20) << name << RESET << (++count % 4 == 0 ? "\n" : "");
            }
            if (count % 4 != 0) cout << "\n";  
        }
    }
};
void initializeGraph(Graph& g) {
    g.addEdge("Main Gate", "Clinic", 38.25); g.addEdge("Clinic", "Main Admission", 64.41);
    g.addEdge("Clinic", "Waiting Shed", 39.48); g.addEdge("Main Admission", "Gazebo", 49.03);
    g.addEdge("Main Admission", "Waiting Shed", 85.62); g.addEdge("Gazebo", "Gym", 65.21);
    g.addEdge("Gazebo", "SAO", 124.00); g.addEdge("Gym", "Back Gate", 55.80);
    g.addEdge("Gym", "SAO", 152.48); g.addEdge("Waiting Shed", "TYK Stairs Left", 19.83);
    g.addEdge("TYK Stairs Left", "TYK Elevator Left", 5.09); g.addEdge("TYK Stairs Left", "TYK Stairs Left 2F", 19.81);
    g.addEdge("TYK Elevator Left", "TYK Admission", 35.59); g.addEdge("TYK Elevator Left", "TYK Elevator Left 2F", 19.81);
    g.addEdge("TYK Admission", "TYK Stairs Mid", 28.42); g.addEdge("TYK Admission", "Field", 28.42);
    g.addEdge("Field", "SAO", 84.08); g.addEdge("Field", "LCT Stairs Mid", 155.55);
    g.addEdge("Field", "TYK Stairs Mid", 95.95); g.addEdge("SAO", "LCT Stairs Mid", 97.75);
    g.addEdge("LCT Stairs Mid", "LCT Elevator Mid", 8.30); g.addEdge("LCT Stairs Mid", "LCT Stairs Mid 2F", 19.81);
    g.addEdge("LCT Elevator Mid", "EN Stairs Left", 92.09); g.addEdge("LCT Elevator Mid", "LCT Elevator Mid 2F", 19.81);
    g.addEdge("EN Stairs Left", "RM103", 23.74); g.addEdge("EN Stairs Left", "RM100", 53.71);
    g.addEdge("EN Stairs Left", "EN Stairs Left 2F", 19.05); g.addEdge("RM103", "EN Stairs Back Left", 9.99);
    g.addEdge("RM103", "RM100", 46.72); g.addEdge("EN Stairs Back Left", "Parking", 72.75);
    g.addEdge("EN Stairs Back Left", "EN Stairs Back Left 2F", 19.05); g.addEdge("Parking", "EN Stairs Back Right", 72.75);
    g.addEdge("TYK Stairs Mid", "TYK Elevator Mid", 20.24); g.addEdge("TYK Stairs Mid", "TYK Stairs Mid 2F", 19.81);
    g.addEdge("TYK Elevator Mid", "Canteen", 32.63); g.addEdge("TYK Elevator Mid", "TYK Elevator Mid 2F", 19.81);
    g.addEdge("Canteen", "TYK Stairs Right", 18.72); g.addEdge("TYK Stairs Right", "HM Building", 29.62);
    g.addEdge("TYK Stairs Right", "TYK Stairs Right 2F", 22.09); g.addEdge("HM Building", "EN Stairs Right", 55.31);
    g.addEdge("EN Stairs Right", "RM117", 23.74); g.addEdge("EN Stairs Right", "ME Lab", 53.71);
    g.addEdge("EN Stairs Right", "EN Stairs Right 2F", 19.05); g.addEdge("RM117", "EN Stairs Back Right", 9.99);
    g.addEdge("RM117", "ME Lab", 46.72); g.addEdge("EN Stairs Back Right", "EN Stairs Back Right 2F", 19.05);
    g.addEdge("RM100", "EN Admission", 29.34); g.addEdge("EN Admission", "CpE Faculty", 29.34);
    g.addEdge("CpE Faculty", "ME Lab", 29.34);
    g.addEdge("TYK Stairs Left 2F", "TYK Elevator Left 2F", 5.09); g.addEdge("TYK Stairs Left 2F", "TYK Stairs Left 3F", 19.81);
    g.addEdge("TYK Elevator Left 2F", "RM200", 21.63); g.addEdge("TYK Elevator Left 2F", "TYK Elevator Left 3F", 19.81);
    g.addEdge("RM200", "RM201", 21.63); g.addEdge("RM201", "TYK Stairs Mid 2F", 21.63);
    g.addEdge("TYK Stairs Mid 2F", "TYK Elevator Mid 2F", 20.24); g.addEdge("TYK Stairs Mid 2F", "TYK Stairs Mid 3F", 19.81);
    g.addEdge("TYK Elevator Mid 2F", "RM203", 14.43); g.addEdge("TYK Elevator Mid 2F", "TYK Elevator Mid 3F", 19.81);
    g.addEdge("RM203", "RM204", 14.43); g.addEdge("RM204", "TYK Stairs Right 2F", 14.43);
    g.addEdge("TYK Stairs Right 2F", "TYK Stairs Right 3F", 22.09); g.addEdge("RM205", "RM206", 14.54);
    g.addEdge("RM206", "LCT Stairs Mid 2F", 14.54); g.addEdge("LCT Stairs Mid 2F", "LCT Elevator Mid 2F", 8.30);
    g.addEdge("LCT Stairs Mid 2F", "LCT Stairs Mid 3F", 19.81); g.addEdge("LCT Elevator Mid 2F", "RM208", 26.94);
    g.addEdge("LCT Elevator Mid 2F", "LCT Elevator Mid 3F", 19.81); g.addEdge("RM208", "RM209", 26.94);
    g.addEdge("EN Stairs Left 2F", "Chemistry Tool Room", 23.74); g.addEdge("EN Stairs Left 2F", "CLR1", 53.71);
    g.addEdge("EN Stairs Left 2F", "EN Stairs Left 3F", 19.05); g.addEdge("Chemistry Tool Room", "EN Stairs Back Left 2F", 9.99);
    g.addEdge("Chemistry Tool Room", "CLR1", 46.72); g.addEdge("EN Stairs Back Left 2F", "EN Stairs Back Left 3F", 19.05);
    g.addEdge("CLR1", "CLR2", 29.34); g.addEdge("CLR2", "ECE1", 29.34); g.addEdge("ECE1", "EEPL1", 29.34);
    g.addEdge("EEPL1", "Physics Tool Room", 46.72); g.addEdge("EEPL1", "EN Stairs Right 2F", 53.71);
    g.addEdge("EN Stairs Right 2F", "Physics Tool Room", 23.74); g.addEdge("EN Stairs Right 2F", "EN Stairs Right 3F", 19.05);
    g.addEdge("Physics Tool Room", "EN Stairs Back Right 2F", 9.99); g.addEdge("EN Stairs Back Right 2F", "EN Stairs Back Right 3F", 19.05);
    g.addEdge("TYK Stairs Left 3F", "TYK Elevator Left 3F", 5.09); g.addEdge("TYK Stairs Left 3F", "TYK Stairs Left 4F", 19.81);
    g.addEdge("TYK Elevator Left 3F", "RM300", 21.63); g.addEdge("TYK Elevator Left 3F", "TYK Elevator Left 4F", 19.81);
    g.addEdge("RM300", "RM301", 21.63); g.addEdge("RM301", "TYK Stairs Mid 3F", 21.63);
    g.addEdge("TYK Stairs Mid 3F", "TYK Elevator Mid 3F", 20.24); g.addEdge("TYK Stairs Mid 3F", "TYK Stairs Mid 4F", 19.81);
    g.addEdge("TYK Elevator Mid 3F", "RM303", 14.43); g.addEdge("TYK Elevator Mid 3F", "TYK Elevator Mid 4F", 19.81);
    g.addEdge("RM303", "RM304", 14.43); g.addEdge("RM304", "TYK Stairs Right 3F", 14.43);
    g.addEdge("TYK Stairs Right 3F", "Bridge", 33.81); g.addEdge("TYK Stairs Right 3F", "TYK Stairs Right 4F", 22.09);
    g.addEdge("Bridge", "EN Stairs Right 3F", 43.16); g.addEdge("RM305", "RM306", 14.54);
    g.addEdge("RM306", "LCT Stairs Mid 3F", 14.54); g.addEdge("LCT Stairs Mid 3F", "LCT Elevator Mid 3F", 8.30);
    g.addEdge("LCT Stairs Mid 3F", "LCT Stairs Mid 4F", 19.81); g.addEdge("LCT Elevator Mid 3F", "RM308", 26.94);
    g.addEdge("LCT Elevator Mid 3F", "LCT Elevator Mid 4F", 19.81); g.addEdge("RM308", "RM309", 26.94);
    g.addEdge("EN Stairs Left 3F", "Stock Room", 23.74); g.addEdge("EN Stairs Left 3F", "EN MPH", 53.71);
    g.addEdge("EN Stairs Left 3F", "EN Stairs Left 4F", 19.05); g.addEdge("Stock Room", "EN Stairs Back Left 3F", 9.99);
    g.addEdge("Stock Room", "EN MPH", 46.72); g.addEdge("EN Stairs Back Left 3F", "EN Stairs Back Left 4F", 19.05);
    g.addEdge("EN MPH", "EN Library", 44.02); g.addEdge("EN Library", "RM314", 44.02);
    g.addEdge("RM314", "DAWN Room", 46.72); g.addEdge("RM314", "EN Stairs Right 3F", 53.71);
    g.addEdge("EN Stairs Right 3F", "DAWN Room", 23.74); g.addEdge("EN Stairs Right 3F", "EN Stairs Right 4F", 19.05);
    g.addEdge("DAWN Room", "EN Stairs Back Right 3F", 9.99); g.addEdge("EN Stairs Back Right 3F", "EN Stairs Back Right 4F", 19.05);
    g.addEdge("TYK Stairs Left 4F", "TYK Elevator Left 4F", 5.09); g.addEdge("TYK Stairs Left 4F", "TYK Stairs Left 5F", 19.81);
    g.addEdge("TYK Elevator Left 4F", "BGC LIBRARY", 32.45); g.addEdge("TYK Elevator Left 4F", "TYK Elevator Left 5F", 19.81);
    g.addEdge("BGC LIBRARY", "TYK Stairs Mid 4F", 32.45); g.addEdge("TYK Stairs Mid 4F", "TYK Elevator Mid 4F", 20.24);
    g.addEdge("TYK Stairs Mid 4F", "TYK Stairs Mid 5F", 19.81); g.addEdge("TYK Elevator Mid 4F", "TYK Stairs Right 4F", 43.41);
    g.addEdge("TYK Elevator Mid 4F", "TYK Elevator Mid 5F", 19.81); g.addEdge("LCT LIBRARY", "LCT Stairs Mid 4F", 14.54);
    g.addEdge("LCT Stairs Mid 4F", "LCT Elevator Mid 4F", 8.30); g.addEdge("LCT Stairs Mid 4F", "LCT Stairs Mid 5F", 19.81);
    g.addEdge("LCT Elevator Mid 4F", "LCT Elevator Mid 5F", 19.81); g.addEdge("EN Stairs Left 4F", "DR400", 23.74);
    g.addEdge("EN Stairs Left 4F", "RM400", 53.71); g.addEdge("DR400", "EN Stairs Back Left 4F", 9.99);
    g.addEdge("DR400", "RM400", 46.72); g.addEdge("RM400", "RM401", 22.01);
    g.addEdge("RM401", "RM402", 22.01); g.addEdge("RM402", "RM403", 22.01);
    g.addEdge("RM403", "RM404", 22.01); g.addEdge("RM404", "DR401", 46.72);
    g.addEdge("RM404", "EN Stairs Right 4F", 53.71); g.addEdge("EN Stairs Right 4F", "DR401", 23.74);
    g.addEdge("DR401", "EN Stairs Back Right 4F", 9.99);
    g.addEdge("TYK Stairs Left 5F", "TYK Elevator Left 5F", 5.09); g.addEdge("TYK Elevator Left 5F", "TYK Stairs Mid 5F", 64.90);
    g.addEdge("TYK Stairs Mid 5F", "TYK Elevator Mid 5F", 20.24); g.addEdge("TYK Elevator Mid 5F", "TYK MPH", 14.43);
    g.addEdge("LCT Stairs Mid 5F", "LCT Elevator Mid 5F", 8.30); g.addEdge("LCT Elevator Mid 5F", "LCT MPH", 26.94);  
}
int getTerminalWidth() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return (w.ws_col > 0) ? w.ws_col : 80;  
}
void printCentered(string text, string color = RESET) {
    int termWidth = getTerminalWidth();
    int padding = max(0, (termWidth - static_cast<int>(text.size())) / 2);  
    cout << color << string(padding, ' ') << text << RESET << "\n";  
}
void showHeader() {
    int termWidth = getTerminalWidth();
    cout << CYAN << string(termWidth, '=') << RESET << "\n";  
    printCentered("EEEEEEEE  NNN     NN   SSSS ", CYAN);
    printCentered("EEEEEEEE  NNNN    NN  SSSSSS", CYAN);
    printCentered("EE        NN NN   NN  SS    ", CYAN);
    printCentered("EEEEEE    NN NN   NN   SSSSS", CYAN);
    printCentered("EEEEEE    NN  NN  NN      SS", CYAN);
    printCentered("EE        NN  NN  NN      SS", CYAN);
    printCentered("EEEEEEEE  NN   NN NN  SSSSSS", CYAN);
    printCentered("EEEEEEEE  NN    NNNN   SSSS ", CYAN);
    cout << "\n";
    printCentered("[ EVENT NAVIGATION SYSTEM ]", YELLOW);
    printCentered("Discover Your Next Event.", RED);
    cout << CYAN << string(termWidth, '=') << RESET << "\n";  
}
vector<int> currentViewIndices;
bool statusPriority(const string& status) {
    return toLower(status) == "upcoming";  
}
bool compareEventIndices(int a, int b, int sortChoice) {
    const Event& left = allEvents[a];  
    const Event& right = allEvents[b];  
    ParsedDate leftDate = parseDate(left.date);  
    ParsedDate rightDate = parseDate(right.date);  
    switch (sortChoice) {
        case 1:  
            if (leftDate.year != rightDate.year) return leftDate.year > rightDate.year;
            if (leftDate.month != rightDate.month) return leftDate.month > rightDate.month;
            if (leftDate.day != rightDate.day) return leftDate.day > rightDate.day;
            return left.name < right.name;  
        case 2:  
            if (leftDate.month != rightDate.month) return leftDate.month > rightDate.month;
            if (leftDate.day != rightDate.day) return leftDate.day > rightDate.day;
            if (leftDate.year != rightDate.year) return leftDate.year > rightDate.year;
            return left.name < right.name;
        case 3:  
            if (statusPriority(left.status) != statusPriority(right.status)) {
                return statusPriority(left.status) > statusPriority(right.status);
            }
            if (leftDate.year != rightDate.year) return leftDate.year > rightDate.year;
            if (leftDate.month != rightDate.month) return leftDate.month > rightDate.month;
            if (leftDate.day != rightDate.day) return leftDate.day > rightDate.day;
            return left.name < right.name;
        default:  
            return left.name < right.name;
    }
}
void insertionSortEventIndices(vector<int>& indices, int sortChoice) {
    for (size_t i = 1; i < indices.size(); ++i) {
        int key = indices[i];  
        int j = static_cast<int>(i) - 1;
        while (j >= 0 && compareEventIndices(key, indices[j], sortChoice)) {
            indices[j + 1] = indices[j];
            --j;
        }
        indices[j + 1] = key;  
    }
}
int partitionEventIndices(vector<int>& indices, int low, int high, int sortChoice) {
    int pivot = indices[high];  
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (compareEventIndices(indices[j], pivot, sortChoice)) {
            ++i;
            swap(indices[i], indices[j]);
        }
    }
    swap(indices[i + 1], indices[high]);
    return i + 1;
}
void quickSortEventIndices(vector<int>& indices, int low, int high, int sortChoice) {
    if (low < high) {
        int pivotIndex = partitionEventIndices(indices, low, high, sortChoice);  
        quickSortEventIndices(indices, low, pivotIndex - 1, sortChoice);  
        quickSortEventIndices(indices, pivotIndex + 1, high, sortChoice);  
    }
}
void hybridSortEventIndices(vector<int>& indices, int sortChoice) {
    const size_t smallDataThreshold = 10;  
    if (indices.size() <= 1) return;  
    if (indices.size() <= smallDataThreshold) {
        insertionSortEventIndices(indices, sortChoice);
    } else {
        quickSortEventIndices(indices, 0, static_cast<int>(indices.size()) - 1, sortChoice);
    }
}
void insertionSortStrings(vector<string>& values) {
    for (size_t i = 1; i < values.size(); ++i) {
        string key = values[i];  
        int j = static_cast<int>(i) - 1;
        while (j >= 0 && toLower(key) < toLower(values[j])) {
            values[j + 1] = values[j];
            --j;
        }
        values[j + 1] = key;  
    }
}
int partitionStrings(vector<string>& values, int low, int high) {
    string pivot = values[high];  
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (toLower(values[j]) < toLower(pivot)) {
            ++i;
            swap(values[i], values[j]);
        }
    }
    swap(values[i + 1], values[high]);
    return i + 1;
}
void quickSortStrings(vector<string>& values, int low, int high) {
    if (low < high) {
        int pivotIndex = partitionStrings(values, low, high);
        quickSortStrings(values, low, pivotIndex - 1);  
        quickSortStrings(values, pivotIndex + 1, high);  
    }
}
void hybridSortStrings(vector<string>& values) {
    const size_t smallDataThreshold = 10;  
    if (values.size() <= 1) return;  
    if (values.size() <= smallDataThreshold) {
        insertionSortStrings(values);
    } else {
        quickSortStrings(values, 0, static_cast<int>(values.size()) - 1);
    }
}
bool compareEventNames(int a, int b) {
    string left = toLower(allEvents[a].name);  
    string right = toLower(allEvents[b].name);  
    if (left != right) return left < right;  
    ParsedDate leftDate = parseDate(allEvents[a].date);
    ParsedDate rightDate = parseDate(allEvents[b].date);
    if (leftDate.year != rightDate.year) return leftDate.year > rightDate.year;
    if (leftDate.month != rightDate.month) return leftDate.month > rightDate.month;
    return leftDate.day > rightDate.day;
}
void insertionSortNameIndices(vector<int>& indices) {
    for (size_t i = 1; i < indices.size(); ++i) {
        int key = indices[i];
        int j = static_cast<int>(i) - 1;
        while (j >= 0 && compareEventNames(key, indices[j])) {
            indices[j + 1] = indices[j];
            --j;
        }
        indices[j + 1] = key;
    }
}
int partitionNameIndices(vector<int>& indices, int low, int high) {
    int pivot = indices[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (compareEventNames(indices[j], pivot)) {
            ++i;
            swap(indices[i], indices[j]);
        }
    }
    swap(indices[i + 1], indices[high]);
    return i + 1;
}
void quickSortNameIndices(vector<int>& indices, int low, int high) {
    if (low < high) {
        int pivotIndex = partitionNameIndices(indices, low, high);
        quickSortNameIndices(indices, low, pivotIndex - 1);
        quickSortNameIndices(indices, pivotIndex + 1, high);
    }
}
void hybridSortNameIndices(vector<int>& indices) {
    const size_t smallDataThreshold = 10;
    if (indices.size() <= 1) return;
    if (indices.size() <= smallDataThreshold) {
        insertionSortNameIndices(indices);
    } else {
        quickSortNameIndices(indices, 0, static_cast<int>(indices.size()) - 1);
    }
}
vector<int> buildSortedIndicesByName() {
    vector<int> sortedIndices(allEvents.size());
    for (int i = 0; i < static_cast<int>(allEvents.size()); ++i) {
        sortedIndices[i] = i;
    }
    hybridSortNameIndices(sortedIndices);  
    return sortedIndices;
}
int binarySearchEventNameStart(const vector<int>& sortedIndices, const string& eventName) {
    int left = 0;
    int right = static_cast<int>(sortedIndices.size()) - 1;
    int result = -1;  
    string target = toLower(eventName);  
    while (left <= right) {
        int mid = left + (right - left) / 2;  
        string currentName = toLower(allEvents[sortedIndices[mid]].name);
        if (currentName == target) {
            result = mid;  
            right = mid - 1;  
        } else if (currentName < target) {
            left = mid + 1;  
        } else {
            right = mid - 1;  
        }
    }
    return result;  
}
void showEventEntries(const vector<int>& indices) {
    for (size_t i = 0; i < indices.size(); ++i) {
        const auto& e = allEvents[indices[i]];
        string eventLine = "[" + to_string(i + 1) + "] " + e.name;
        printCentered(eventLine, BLUE);
        string dateLine = "Date: " + string(e.date) + " | Loc: " + string(e.location);
        printCentered(dateLine, "");
        string statusLine = "Status: [" + string(e.status) + "]";
        printCentered(statusLine, e.statusColor);
        cout << "\n";
    }
}
void showEvents() {
    cout << "\n";
    int termWidth = getTerminalWidth();
    int dashCount = max(0, (termWidth - 12) / 2);  
    string headerLine = string(dashCount, '-') + " EVENT LIST " + string(dashCount, '-');
    printCentered(headerLine, MAGENTA);
    cout << "\n";
    currentViewIndices.clear();  
    set<string> seen;
    for (int i = 0; i < static_cast<int>(allEvents.size()); ++i) {
        if (seen.find(allEvents[i].name) == seen.end()) {
            seen.insert(allEvents[i].name);  
            currentViewIndices.push_back(i);  
        }
    }
    showEventEntries(currentViewIndices);  
    printCentered(RED + string("[S] Search for Event") + RESET, "");
    printCentered("[0] Exit", "");
}
vector<string> getUniqueEventNames() {
    vector<string> uniqueNames;
    set<string> seen;  
    for (const auto& event : allEvents) {
        if (!seen.count(event.name)) {
            seen.insert(event.name);
            uniqueNames.push_back(event.name);
        }
    }
    hybridSortStrings(uniqueNames);  
    return uniqueNames;
}
vector<int> getEventIndicesByName(const string& eventName) {
    vector<int> sortedIndices = buildSortedIndicesByName();  
    vector<int> indices;
    int start = binarySearchEventNameStart(sortedIndices, eventName);
    if (start == -1) return indices;  
    string target = toLower(eventName);
    for (int i = start; i < static_cast<int>(sortedIndices.size()); ++i) {
        if (toLower(allEvents[sortedIndices[i]].name) != target) break;  
        indices.push_back(sortedIndices[i]);
    }
    return indices;
}
void sortEventIndices(vector<int>& indices, int sortChoice) {
    hybridSortEventIndices(indices, sortChoice);
}
int searchForEvent() {
    vector<string> uniqueNames = getUniqueEventNames();
    while (true) {
        cout << "\033[H\033[2J\033[3J";
        showHeader();
        int termWidth = getTerminalWidth();
        int dashCount = max(0, (termWidth - 14) / 2);  
        string headerLine = string(dashCount, '-') + " SEARCH EVENT " + string(dashCount, '-');
        printCentered(headerLine, MAGENTA);
        cout << "\n";
        for (size_t i = 0; i < uniqueNames.size(); ++i) {
            string menuItem = "[" + to_string(i + 1) + "] " + uniqueNames[i];
            printCentered(menuItem, BLUE);
        }
        cout << "\n";
        printCentered(RED + string("[V] Search all events") + RESET, "");
        printCentered("[0] Back", "");
        string input;
        printCentered("Select event name: ", GREEN);
        cin >> input;
        if (toLower(input) == "v") {
            vector<int> allIndices;
            for (size_t i = 0; i < allEvents.size(); ++i) {
                allIndices.push_back(i);
            }
            while (true) {
                cout << "\033[H\033[2J\033[3J";
                showHeader();
                printCentered("All Events", BLUE);
                cout << "\n";
                printCentered("[1] Sort by Year", "");
                printCentered("[2] Sort by Month", "");
                printCentered("[3] Sort by Status", "");
                printCentered("[0] Back", "");
                cout << "\n";
                printCentered("Choose sorting option: ", GREEN);
                cin >> input;
                int sortChoice = 0;
                try {
                    sortChoice = stoi(input);
                } catch (...) {
                    continue;
                }
                if (sortChoice == 0) break;
                if (sortChoice < 1 || sortChoice > 3) continue;
                vector<int> sortedIndices = allIndices;
                sortEventIndices(sortedIndices, sortChoice);
                while (true) {
                    cout << "\033[H\033[2J\033[3J";
                    showHeader();
                    int termWidth = getTerminalWidth();
                    int dashCount = max(0, (termWidth - 14) / 2);
                    string sortedHeaderLine = string(dashCount, '-') + " SORTED EVENTS " + string(dashCount, '-');
                    printCentered(sortedHeaderLine, MAGENTA);
                    cout << "\n";
                    currentViewIndices = sortedIndices;
                    showEventEntries(currentViewIndices);
                    printCentered("[0] Back", "");
                    cout << "\n";
                    printCentered("Select event number: ", GREEN);
                    cin >> input;
                    int eventChoice = 0;
                    try {
                        eventChoice = stoi(input);
                    } catch (...) {
                        continue;
                    }
                    if (eventChoice == 0) break;
                    if (eventChoice > 0 && eventChoice <= static_cast<int>(currentViewIndices.size())) {
                        return currentViewIndices[eventChoice - 1];
                    }
                }
            }
            continue;
        }
        int eventNameChoice = 0;
        try {
            eventNameChoice = stoi(input);
        } catch (...) {
            continue;
        }
        if (eventNameChoice == 0) return -1;
        if (eventNameChoice < 1 || eventNameChoice > static_cast<int>(uniqueNames.size())) continue;
        vector<int> matchingIndices = getEventIndicesByName(uniqueNames[eventNameChoice - 1]);
        while (true) {
            cout << "\033[H\033[2J\033[3J";
            showHeader();
            cout << "\n" << BLUE << uniqueNames[eventNameChoice - 1] << RESET << "\n\n";
            cout << "[1] Sort by Year\n";
            cout << "[2] Sort by Month\n";
            cout << "[3] Sort by Status\n";
            cout << "[0] Back\n";
            cout << GREEN << "Choose sorting option: " << RESET;
            cin >> input;
            int sortChoice = 0;
            try {
                sortChoice = stoi(input);
            } catch (...) {
                continue;
            }
            if (sortChoice == 0) break;
            if (sortChoice < 1 || sortChoice > 3) continue;
            vector<int> sortedIndices = matchingIndices;
            sortEventIndices(sortedIndices, sortChoice);
            while (true) {
                cout << "\033[H\033[2J\033[3J";
                showHeader();
                cout << "\n" << MAGENTA << string(29, '-') << " SORTED EVENTS " << string(29, '-') << RESET << "\n\n";
                currentViewIndices = sortedIndices;
                showEventEntries(currentViewIndices);
                cout << "[0] Back\n";
                cout << GREEN << "Select event number: " << RESET;
                cin >> input;
                int eventChoice = 0;
                try {
                    eventChoice = stoi(input);
                } catch (...) {
                    continue;
                }
                if (eventChoice == 0) break;
                if (eventChoice > 0 && eventChoice <= static_cast<int>(currentViewIndices.size())) {
                    return currentViewIndices[eventChoice - 1];
                }
            }
        }
    }
}
int main() {
    Graph g;  
    initializeGraph(g);  
    while (true) {
        cout << "\033[H\033[2J\033[3J";  
        showHeader();  
        showEvents();  
        string input;
        cout << GREEN << "Select event number: " << RESET;
        cin >> input;  
        int selectedIndex = -1;  
        if (toLower(input) == "s") {
            selectedIndex = searchForEvent();  
            if (selectedIndex == -1) continue;  
        } else {
            int choice = 0;
            try {
                choice = stoi(input);  
            } catch (...) {
                continue;  
            }
            if (choice == 0) break;  
            if (choice > 0 && choice <= static_cast<int>(currentViewIndices.size())) {
                selectedIndex = currentViewIndices[choice - 1];
            } else {
                continue;  
            }
        }
        const Event& selected = allEvents[selectedIndex];  
        cout << "\n" << CYAN << string(79, '-') << RESET << "\n";
        cout << "Selected Event: " << BLUE << selected.name << RESET << "\n";
        cout << "Date: " << selected.date << " | Location: " << selected.location << "\n\n";
        g.listLocations();
        string myLoc;
        cout << "\n" << YELLOW << "Enter your current location: " << RESET;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  
        getline(cin, myLoc);  
        if (g.hasLocation(myLoc)) {
            cout << "\n";
            g.dijkstra(myLoc, selected.location);
        } else {
            cout << RED << "Invalid location." << RESET << "\n";
        }
        cout << "\nPress Enter to return to menu...";  
        cin.get();
    }
    cout << RED << "\nExiting system...\n" << RESET;
    return 0;
}
