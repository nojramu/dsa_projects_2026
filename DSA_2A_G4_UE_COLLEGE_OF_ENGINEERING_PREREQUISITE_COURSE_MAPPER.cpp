#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <queue>
#include <algorithm>
#include <set>
#include <limits>

using namespace std;


struct Course {
    string code;
    string name;
    int units;
    string program;
    int year;
    int semester;
    vector<string> prerequisites;
    vector<string> substitutes;
};

// Other Data Structures
unordered_map<string, Course> courseTable;
unordered_map<string, vector<string>> prereqGraph;
vector<Course> courseList;
set<string> courseCodes;

void pauseScreen() {
    cout << "\nPress Enter to continue...";

    cin.clear(); 
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    cin.get(); // wait for Enter
}


void addCourse(const Course& c) {
    courseTable[c.code + "_" + c.program] = c;
    prereqGraph[c.code] = c.prerequisites;
    courseList.push_back(c);
    courseCodes.insert(c.code);
}

// For Displaying

void displayCoursesByProgram(const string& program) {
    map<pair<int, int>, vector<Course>> coursesByYearSem;     
    for (const auto& c : courseList) {
        if (c.program == program) {
            coursesByYearSem[{c.year, c.semester}].push_back(c); 
        }
    }
   

    for (const auto& yearSem : coursesByYearSem) {               
        int year = yearSem.first.first;                        
        int semester = yearSem.first.second;                   
        const auto& courses = yearSem.second;                  
        string semesterLabel = (semester == 3) ? "SUMMER" : "SEMESTER " + to_string(semester); 
        cout << "\n========== YEAR " << year << " - " << semesterLabel << " ==========\n"; 
        int totalUnits = 0;                                   
       
        for (const auto& c : courses) {
            cout << "  " << c.code << " - " << c.name
                 << " (" << c.units << " units)\n";
            totalUnits += c.units;
        }
       
        cout << "  Total Units: " << totalUnits << "\n";     
    }
}


// Search algos

void YearSemSearch(int year, int sem, const string& program) {
    cout << "\n========== YEAR " << year << " - ";         
    if (sem == 3) {                                         
        cout << "SUMMER ==========\n";                         
    } else {
        cout << "SEMESTER " << sem << " ==========\n";         
    }
   
    bool found = false;                                         
    for (const auto& c : courseList) {
        if (c.year == year && c.semester == sem && c.program == program) {
            cout << "  " << c.code << " - " << c.name << " (" << c.units << " units)\n";
            found = true;                                       
        }
    }
   
    if (!found) {                                               
        cout << "  No courses found for this year and semester.\n";
    }
}


void bubbleSortByName() {
    for (size_t i = 0; i < courseList.size(); i++) {
        for (size_t j = 0; j < courseList.size() - i - 1; j++) {
            if (courseList[j].name > courseList[j + 1].name) {
                swap(courseList[j], courseList[j + 1]);
            }
        }
    }
}


bool compareByCode(const Course& a, const Course& b) {
    return a.code < b.code;
}



void searchCourse(string code, string program) {
string key = code + "_" + program;

if (courseTable.find(key) != courseTable.end()) {
const Course& c = courseTable[key];

cout << "Found:\n";
cout << c.code << " - " << c.name
<< " (" << c.units << " units)"
<< " | Year " << c.year
<< " Sem " << c.semester
<< " | " << c.program << endl;
} else {
cout << "Course not found.\n";
}
}



void merge(vector<Course>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<Course> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i].units <= R[j].units) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<Course>& arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}


// Depth First Search for prereqs

void dfs(string course, set<string>& visited) {
    for (const auto& pre : prereqGraph[course]) {
        if (!visited.count(pre)) {
            visited.insert(pre);
            dfs(pre, visited);
        }
    }
}

void showPrerequisites(string code, string program) {
    set<string> visited;
    dfs(code, visited);

    cout << "Prerequisites for " << code << ":\n";
    for (const auto& c : visited) {
        cout << c << endl;
    }
}

// Computer
void computeUnits(int year, int sem, const string& program) {
    int total = 0;
    cout << "\nYear " << year << " - ";                      
    if (sem == 3) {                                             
        cout << "SUMMER:\n";                                
    } else {
        cout << "Semester " << sem << ":\n";               
    }
    
    for (const auto& c : courseList) {
        if (c.year == year && c.semester == sem && c.program == program) {
            cout << "  " << c.code << " - " << c.name << " (" << c.units << " units)\n"; 
            total += c.units;
        }
    }
    cout << "Total Units: " << total << endl;                  
}



bool isSubstitute(string target, string candidate) {
    for (const auto& sub : courseTable[target].substitutes) {
        if (sub == candidate) return true;
    }
    return false;
}


void showDependentCourses(string code, const string& program) {
    cout << "Courses that require " << code << ":\n";

    bool found = false;

    for (const auto& c : courseList) {
        if (c.program != program) continue;

        for (const auto& pre : c.prerequisites) {
            if (pre == code) {
                cout << c.code << " - " << c.name << endl;
                found = true;
                break;
            }
        }
    }

    if (!found) {
        cout << "No courses depend on this subject.\n";
    }
}

    //All Sample Data 
    void loadSampleData() {
    
    //EE
    // ==========================================
    // FIRST YEAR - FIRST SEMESTER
    // ==========================================
    addCourse({"MLC1101", "Literacy/Civic Welfare/Military Science 1", 3, "EE", 1, 1, {"STC1101", "STL1101", "STM1101"}, {}});
    addCourse({"NCH1101", "Chemistry for Engineers Lecture", 3, "EE", 1, 1, {}, {}});
    addCourse({"NCH1102", "Chemistry for Engineers Laboratory", 1, "EE", 1, 1, {"NCH1101"}, {}});
    addCourse({"NEE1107", "Environmental Science and Engineering", 3, "EE", 1, 1, {}, {}});
    addCourse({"NES1100", "Pre-Calculus for Engineers", 4, "EE", 1, 1, {}, {}});
    addCourse({"NES1101", "Engineering Drawing", 1, "EE", 1, 1, {"NCE1201", "NME1102"}, {}});
    addCourse({"NMA1101", "Calculus 1", 3, "EE", 1, 1, {}, {}});
    addCourse({"PPF1101", "PATHFIT 1 (Movement Education)", 2, "EE", 1, 1, {}, {}});
    addCourse({"ZGE1104", "Mathematics in the Modern World", 3, "EE", 1, 1, {}, {}});
    addCourse({"ZPD1101", "Personality Development", 1, "EE", 1, 1, {}, {}});

    // ==========================================
    // FIRST YEAR - SECOND SEMESTER
    // ==========================================
    addCourse({"MLC1102", "Literacy/Civic Welfare/Military Science 2", 3, "EE", 1, 2, {"MLC1101"}, {"STC1102", "STL1102", "STM1102"}});
    addCourse({"NEE1103", "Computer Programming", 1, "EE", 1, 2, {}, {}});
    addCourse({"NEE1104", "Engineering Data Analysis", 3, "EE", 1, 2, {"NMA1101"}, {"NEC1201"}});
    addCourse({"NEE1201", "Computer-Aided Drafting", 1, "EE", 1, 2, {"NES1101"}, {"NEC1204"}});
    addCourse({"NMA1201", "Calculus 2", 3, "EE", 1, 2, {"NMA1101"}, {}});
    addCourse({"NPH1201", "Physics for Engineers Lecture", 3, "EE", 1, 2, {"NMA1201"}, {}});
    addCourse({"NPH1202", "Physics for Engineers Laboratory", 1, "EE", 1, 2, {"NPH1201"}, {}});
    addCourse({"PPF1201", "PATHFIT 2 (Physical Fitness)", 2, "EE", 1, 2, {"PPF1101"}, {}});
    addCourse({"ZGE1105", "Purposive Communication", 3, "EE", 1, 2, {}, {}});
    addCourse({"ZGE1106", "Readings in Philippine History", 3, "EE", 1, 2, {}, {}});

    // ==========================================
    // SECOND YEAR - FIRST SEMESTER
    // ==========================================
    addCourse({"NEE1105", "Differential Equations", 3, "EE", 2, 1, {"NMA1201"}, {}});
    addCourse({"NEE1106", "Engineering Mechanics", 3, "EE", 2, 1, {"NPH1201"}, {}});
    addCourse({"NEE1202", "Engineering Economics", 3, "EE", 2, 1, {"NEE1104"}, {}});
    addCourse({"NEE2101", "Circuits 1 Lecture", 3, "EE", 2, 1, {"NMA1201", "NPH1201"}, {}});
    addCourse({"NEE2102", "Circuits 1 Laboratory", 1, "EE", 2, 1, {"NEE2101"}, {}});
    addCourse({"NME3108", "Basic Thermodynamics", 3, "EE", 2, 1, {"NPH1201"}, {}});
    addCourse({"PPF2101", "PATHFIT 3", 2, "EE", 2, 1, {"PPF1201"}, {}});
    addCourse({"ZGE1101", "Art Appreciation", 3, "EE", 2, 1, {}, {}});
    addCourse({"ZGE1102", "The Contemporary World", 3, "EE", 2, 1, {}, {}});

    // ==========================================
    // SECOND YEAR - SECOND SEMESTER
    // ==========================================
    addCourse({"NEC2201", "Electronic Circuits: Devices and Analysis Lecture", 3, "EE", 2, 2, {"NEE2101"}, {}});
    addCourse({"NEC2202", "Electronic Circuits: Devices and Analysis Lab", 1, "EE", 2, 2, {"NEC2201"}, {}});
    addCourse({"NEE1203", "Fundamentals of Deformable Bodies", 2, "EE", 2, 2, {"NEE1106"}, {}});
    addCourse({"NEE1207", "Fluid Mechanics", 2, "EE", 2, 2, {"NPH1201"}, {}});
    addCourse({"NEE2201", "Circuits 2 Lecture", 3, "EE", 2, 2, {"NEE2101"}, {}});
    addCourse({"NEE2202", "Circuits 2 Laboratory", 1, "EE", 2, 2, {"NEE2201"}, {}});
    addCourse({"NEE2203", "Engineering Mathematics for EE", 3, "EE", 2, 2, {"NEE1105"}, {}});
    addCourse({"NEE2204", "Electromagnetics", 3, "EE", 2, 2, {"NEE1105", "NPH1201"}, {}});
    addCourse({"NEE2205", "Management of Engineering Projects", 2, "EE", 2, 2, {"NEE1202"}, {}});
    addCourse({"PPF2201", "PATHFIT 4", 2, "EE", 2, 2, {"PPF2101"}, {}});

    // ==========================================
    // THIRD YEAR - FIRST SEMESTER
    // ==========================================
    addCourse({"NEE3101", "Electrical Systems Design 1", 1, "EE", 3, 1, {"NEE2201"}, {}});
    addCourse({"NEE3102", "Electrical Machines 1 Lecture", 3, "EE", 3, 1, {"NEE2201", "NEE2204"}, {}});
    addCourse({"NEE3103", "Industrial Electronics Lecture", 3, "EE", 3, 1, {"NEC2201"}, {}});
    addCourse({"NEE3104", "Industrial Electronics Laboratory", 1, "EE", 3, 1, {"NEE3103"}, {}});
    addCourse({"NEE3105", "Electrical Machines 1 Lab", 3, "EE", 3, 1, {"NEE2201", "NEE2204"}, {}});
    addCourse({"NEE3106", "Feedback Control Systems", 2, "EE", 3, 1, {"NEE2201", "NEE2203"}, {}});
    addCourse({"NEE3107", "EE Law, Codes, and Professional Ethics", 2, "EE", 3, 1, {"ZGE1103"}, {}});
    addCourse({"NEE3108", "Logic Circuits and Switching Theory", 3, "EE", 3, 1, {"NEC2201"}, {}});

    // ==========================================
    // THIRD YEAR - SECOND SEMESTER
    // ==========================================
    addCourse({"NEE3201", "Electrical Apparatus and Devices Lecture", 3, "EE", 3, 2, {"NEE2201"}, {}});
    addCourse({"NEE3202", "Electrical Apparatus and Devices Lab", 1, "EE", 3, 2, {"NEE3201"}, {}});
    addCourse({"NEE3203", "Electrical Machines 2 Lecture", 3, "EE", 3, 2, {"NEE3105"}, {}});
    addCourse({"NEE3204", "Electrical Machines 2 Lab", 1, "EE", 3, 2, {"NEE3203"}, {}});
    addCourse({"NEE3205", "Microprocessor Systems", 3, "EE", 3, 2, {"NEE3108"}, {}});
    addCourse({"NEE3206", "Electrical Standards and Practices", 1, "EE", 3, 2, {"NEE3107"}, {}});
    addCourse({"NEE3207", "Basic Occupational Safety and Health", 3, "EE", 3, 2, {}, {}});
    addCourse({"NEE3208", "Integration Course 1", 1, "EE", 3, 2, {"NEE1203", "NEE3101"}, {}});
    addCourse({"NEE3209", "Research Methods", 1, "EE", 3, 2, {"NEE1104"}, {}});
    addCourse({"NEE4980", "Research Project 1", 1, "EE", 3, 2, {"NEE3209"}, {}});
    addCourse({"ZGEEL01", "GE Elective 1", 3, "EE", 3, 2, {}, {"ZGE4301", "ZGE4305"}});

    // ==========================================
    // THIRD YEAR - SUMMER / SPECIAL TERM
    // ==========================================
    addCourse({"NEE4970", "On-the-Job Training", 3, "EE", 3, 3, {"NEE3203"}, {}});

    // ==========================================
    // FOURTH YEAR - FIRST SEMESTER
    // ==========================================
    addCourse({"NEE4101", "Instrumentation and Control", 3, "EE", 4, 1, {"NEE3106"}, {}});
    addCourse({"NEE4102", "Power System Analysis", 5, "EE", 4, 1, {"NEE3102", "NEE3206"}, {}});
    addCourse({"NEE4103", "Electrical Systems Design", 3, "EE", 4, 1, {"NEE3203"}, {}});
    addCourse({"NEE4104", "Illumination Engineering Design", 2, "EE", 4, 1, {"NEE3203"}, {}});
    addCourse({"NEE4990", "Research Project 2", 1, "EE", 4, 1, {"NEE4980"}, {}});
    addCourse({"NEEL01", "EE Elective 1", 3, "EE", 4, 1, {}, {}});
    addCourse({"ZGE1108", "Understanding the Self", 3, "EE", 4, 1, {}, {}});
    addCourse({"ZGEEL02", "GE Elective 2", 3, "EE", 4, 1, {}, {}});

    // ==========================================
    // FOURTH YEAR - SECOND SEMESTER
    // ==========================================
    addCourse({"NEE4201", "Field Trip, Seminars, and Colloquia", 2, "EE", 4, 2, {}, {}});
    addCourse({"NEE4202", "Power Plant Engineering Design", 1, "EE", 4, 2, {"NEE4102"}, {}});
    addCourse({"NEE4203", "Technopreneurship", 3, "EE", 4, 2, {}, {}});
    addCourse({"NEE4204", "Distribution Systems and Substation Design", 3, "EE", 4, 2, {"NEE4102"}, {}});
    addCourse({"NEE4208", "EE Integration Course 2", 1, "EE", 4, 2, {"NEE4102"}, {}});
    addCourse({"NEEL02", "EE Elective 2", 3, "EE", 4, 2, {"NEEL01"}, {}});
    addCourse({"ZGE1107", "Science, Technology, and Society", 3, "EE", 4, 2, {}, {}});
    addCourse({"ZGE1109", "Life and Works of Rizal", 3, "EE", 4, 2, {}, {}});
    addCourse({"ZGEEL03", "GE Elective 3", 3, "EE", 4, 2, {}, {}});


    //CPE ON TOP
    // ==========================================
    // FIRST YEAR - FIRST SEMESTER
    // ==========================================
    addCourse({"MLC1101", "Literacy/Civic Welfare/Military Science 1", 3, "CpE", 1, 1, {}, {"STC1101", "STL1101", "STM1101"}});
    addCourse({"NCH1101", "Chemistry for Engineers Lecture", 3, "CpE", 1, 1, {}, {}});
    addCourse({"NCH1102", "Chemistry for Engineers Laboratory", 1, "CpE", 1, 1, {"NCH1101"}, {}});
    addCourse({"NCP1101", "Computer Engineering as a Discipline", 1, "CpE", 1, 1, {}, {}});
    addCourse({"NCP1102", "Programming Logic and Design", 2, "CpE", 1, 1, {}, {}});
    addCourse({"NMA1101", "Calculus 1", 3, "CpE", 1, 1, {}, {}});
    addCourse({"PPF1101", "PATHFIT 1 (Movement Education)", 2, "CpE", 1, 1, {}, {}});
    addCourse({"ZGE1108", "Understanding the Self", 3, "CpE", 1, 1, {}, {}});

    // ==========================================
    // FIRST YEAR - SECOND SEMESTER
    // ==========================================
    addCourse({"MLC1102", "Literacy/Civic Welfare/Military Science 2", 3, "CpE", 1, 2, {"MLC1101"}, {"STC1102", "STL1102", "STM1102"}});
    addCourse({"NCP1103", "Discrete Mathematics", 3, "CpE", 1, 2, {}, {}});
    addCourse({"NCP1202", "Database Management System", 1, "CpE", 1, 2, {}, {}});
    addCourse({"NCP1203", "Data Structures and Algorithms", 2, "CpE", 1, 2, {"NCP1102"}, {}});
    addCourse({"NEE2103", "Fundamentals of Electrical Circuits Lecture", 3, "CpE", 1, 2, {"NPH1201"}, {}});
    addCourse({"NEE2104", "Fundamentals of Electrical Circuits Laboratory", 1, "CpE", 1, 2, {"NEE2103"}, {}});
    addCourse({"NMA1201", "Calculus 2", 3, "CpE", 1, 2, {"NMA1101"}, {}});
    addCourse({"NPH1201", "Physics for Engineers Lecture", 3, "CpE", 1, 2, {"NMA1101"}, {}});
    addCourse({"NPH1202", "Physics for Engineers Laboratory", 1, "CpE", 1, 2, {"NPH1201"}, {}});
    addCourse({"PPF1201", "PATHFIT 2 (Physical Fitness)", 2, "CpE", 1, 2, {"PPF1101"}, {}});
    addCourse({"ZGE1104", "Mathematics in the Modern World", 3, "CpE", 1, 2, {}, {}});

    // ==========================================
    // SECOND YEAR - FIRST SEMESTER
    // ==========================================
    addCourse({"NCP2101", "Logic Circuits and Design Lecture", 3, "CpE", 2, 1, {}, {}});
    addCourse({"NCP2102", "Logic Circuits and Design Laboratory", 1, "CpE", 2, 1, {"NCP2101"}, {}});
    addCourse({"NCP2103", "Object Oriented Programming", 2, "CpE", 2, 1, {"NCP1203"}, {}});
    addCourse({"NCP2104", "Differential Equations", 3, "CpE", 2, 1, {"NMA1201"}, {}});
    addCourse({"NCP2105", "Engineering Data Analysis", 3, "CpE", 2, 1, {}, {}});
    addCourse({"NEC1205", "Fundamentals of Electronic Circuits Lecture", 3, "CpE", 2, 1, {"NMA1101"}, {}});
    addCourse({"NEC1206", "Fundamentals of Electronic Circuits Laboratory", 1, "CpE", 2, 1, {"NEC1205"}, {}});
    addCourse({"PPF2101", "PATHFIT 3", 2, "CpE", 2, 1, {"PPF1201"}, {}});
    addCourse({"ZGE1107", "Science, Technology, and Society", 3, "CpE", 2, 1, {}, {}});
    addCourse({"ZPD1101", "Personality Development", 1, "CpE", 2, 1, {}, {}});

    // ==========================================
    // SECOND YEAR - SECOND SEMESTER
    // ==========================================
    addCourse({"NCP1201", "PC Troubleshooting and Maintenance", 1, "CpE", 2, 2, {}, {}});
    addCourse({"NCP2201", "Computer Architecture and Organization Lecture", 3, "CpE", 2, 2, {"NCP2101"}, {}});
    addCourse({"NCP2202", "Computer Architecture and Organization Lab", 1, "CpE", 2, 2, {"NCP2201"}, {}});
    addCourse({"NCP2204", "Microprocessors Lecture", 3, "CpE", 2, 2, {"NCP2101"}, {}});
    addCourse({"NCP2205", "Microprocessors Laboratory", 1, "CpE", 2, 2, {"NCP2204"}, {}});
    addCourse({"NCP2206", "Data and Digital Communications Lecture", 3, "CpE", 2, 2, {}, {}});
    addCourse({"NCP2207", "Data and Digital Communications Laboratory", 1, "CpE", 2, 2, {"NCP2206"}, {}});
    addCourse({"NCP2208", "Operating Systems Lecture", 3, "CpE", 2, 2, {}, {}});
    addCourse({"NCP2209", "Operating Systems Laboratory", 1, "CpE", 2, 2, {"NCP2208"}, {}});
    addCourse({"PPF2201", "PATHFIT 4", 2, "CpE", 2, 2, {"PPF2101"}, {}});
    addCourse({"ZGE1109", "Life and Works of Rizal", 3, "CpE", 2, 2, {}, {}});

    // ==========================================
    // THIRD YEAR - FIRST SEMESTER
    // ==========================================
    addCourse({"NCP2203", "Fundamentals of Mixed Signals and Sensors", 3, "CpE", 3, 1, {}, {}});
    addCourse({"NCP3101", "Embedded Systems Lecture", 3, "CpE", 3, 1, {"NCP2204"}, {}});
    addCourse({"NCP3102", "Embedded Systems Laboratory", 1, "CpE", 3, 1, {"NCP3101"}, {}});
    addCourse({"NCP3103", "Computer Networks and Security Lecture", 3, "CpE", 3, 1, {"NCP2206"}, {}});
    addCourse({"NCP3104", "Computer Networks and Security Laboratory", 1, "CpE", 3, 1, {"NCP3103"}, {}});
    addCourse({"NCP3105", "Software Design Lecture", 3, "CpE", 3, 1, {"NCP2103"}, {}});

    // ==========================================
    // THIRD YEAR - SECOND SEMESTER
    // ==========================================
    addCourse({"NCP3202", "Feedback and Control Systems", 3, "CpE", 3, 2, {}, {}});
    addCourse({"NCP3203", "Computer Network Design Application Lecture", 2, "CpE", 3, 2, {"NCP3103"}, {}});
    addCourse({"NCP3204", "Computer Network Design Application Lab", 1, "CpE", 3, 2, {"NCP3203"}, {}});
    addCourse({"NCP3205", "Methods of Research", 2, "CpE", 3, 2, {"NCP2105", "NCP3101", "NCP3103", "NCP3105"}, {}});
    addCourse({"NCP3206", "Computer-Aided Drafting", 1, "CpE", 3, 2, {}, {}});
    addCourse({"NCP4104", "Project Management Lecture", 2, "CpE", 3, 2, {"NCP3105"}, {}});
    addCourse({"NCP4105", "Project Management Laboratory", 1, "CpE", 3, 2, {"NCP4104"}, {}});
    addCourse({"ZGE1106", "Readings in Philippine History", 3, "CpE", 3, 2, {}, {}});

    // ==========================================
    // FOURTH YEAR - FIRST SEMESTER
    // ==========================================
    addCourse({"NCP3201", "Introduction to HDL", 1, "CpE", 4, 1, {}, {}});
    addCourse({"NCP4101", "Computer Engineering Drafting and Design", 1, "CpE", 4, 1, {"NCP3206"}, {}});
    addCourse({"NCP4102", "Digital Signal Processing Lecture", 3, "CpE", 4, 1, {"NCP3202"}, {}});
    addCourse({"NCP4103", "Digital Signal Processing Laboratory", 1, "CpE", 4, 1, {"NCP4102"}, {}});
    addCourse({"NCP4980", "CpE Practice and Design 1", 1, "CpE", 4, 1, {"NCP3205"}, {}});
    addCourse({"ZGE1101", "Art Appreciation", 3, "CpE", 4, 1, {}, {}});
    addCourse({"ZGE1102", "The Contemporary World", 3, "CpE", 4, 1, {}, {}});
    addCourse({"ZGE1103", "Ethics", 3, "CpE", 4, 1, {}, {}});

    // ==========================================
    // FOURTH YEAR - SECOND SEMESTER
    // ==========================================
    addCourse({"NCP3107", "Technopreneurship", 3, "CpE", 4, 2, {}, {}});
    addCourse({"NCP3207", "Basic Occupational Health and Safety", 3, "CpE", 4, 2, {}, {}});
    addCourse({"NCP4106", "Engineering Economics", 3, "CpE", 4, 2, {}, {}});
    addCourse({"NCP4990", "CpE Practice and Design 2", 2, "CpE", 4, 2, {"NCP4980"}, {}});

    //ME
    // ===== FIRST YEAR - FIRST SEM =====
    addCourse({"MLC1101","Literacy/Civic Welfare/Military Science 1",3,"ME",1,1,{},{"STC1101","STL1101","STM1101"}});
    addCourse({"NCH1101","Chemistry for Engineers Lecture",3,"ME",1,1,{},{}});
    addCourse({"NCH1102","Chemistry for Engineers Laboratory",1,"ME",1,1,{"NCH1101"},{}});
    addCourse({"NMA1101","Calculus 1",3,"ME",1,1,{},{}});
    addCourse({"NME1101","Mechanical Engineering Orientation",1,"ME",1,1,{},{}});
    addCourse({"NME1102","Engineering Drawing",1,"ME",1,1,{},{"NEC1201","NES1101"}});
    addCourse({"PPF1101","PATHFIT 1",2,"ME",1,1,{},{}});
    addCourse({"ZGE1104","Mathematics in the Modern World",3,"ME",1,1,{},{}});
    addCourse({"ZGE1105","Purposive Communication",3,"ME",1,1,{},{}});

    // ===== FIRST YEAR - SECOND SEM =====
    addCourse({"MLC1102","Literacy/Civic Welfare/Military Science 2",3,"ME",1,2,{"MLC1101"},{"STC1102","STL1102","STM1102"}});
    addCourse({"NMA1201","Calculus 2",3,"ME",1,2,{"NMA1101"},{}});
    addCourse({"NME1201","Workshop Theory and Practice",2,"ME",1,2,{},{}});
    addCourse({"NME1202","Mechanical Drafting",1,"ME",1,2,{"NME1102"},{}});
    addCourse({"NPH1201","Physics for Engineers Lecture",3,"ME",1,2,{"NMA1201"},{}});
    addCourse({"NPH1202","Physics for Engineers Lab",1,"ME",1,2,{"NPH1201"},{}});
    addCourse({"PPF1201","PATHFIT 2",2,"ME",1,2,{"PPF1101"},{}});
    addCourse({"ZGE1101","Art Appreciation",3,"ME",1,2,{},{}});
    addCourse({"ZGE1106","Readings in Philippine History",3,"ME",1,2,{},{}});
    addCourse({"ZGE1109","Life and Works of Rizal",3,"ME",1,2,{},{}});

    // ===== SECOND YEAR - FIRST SEM =====
    addCourse({"NEE1101","Basic Electrical Engineering Lecture",2,"ME",2,1,{"NMA1201","NPH1201"},{}});
    addCourse({"NEE1102","Basic Electrical Engineering Lab",1,"ME",2,1,{"NEE1101"},{}});
    addCourse({"NME2101","Thermodynamics 1",3,"ME",2,1,{"NMA1201","NPH1201"},{}});
    addCourse({"NME2102","Statics of Rigid Bodies",3,"ME",2,1,{"NPH1201"},{}});
    addCourse({"NME2103","Computer-Aided Drafting",1,"ME",2,1,{"NME1202"},{}});
    addCourse({"NME2104","Differential Equations",3,"ME",2,1,{"NMA1201"},{}});
    addCourse({"NME2105","Machine Shop Theory and Practice",2,"ME",2,1,{},{}});
    addCourse({"NME2106","Engineering Management",2,"ME",2,1,{},{}});
    addCourse({"PPF2101","PATHFIT 3",2,"ME",2,1,{},{}});
    addCourse({"ZGE1102","The Contemporary World",3,"ME",2,1,{},{}});
    addCourse({"ZGE1107","Science, Technology, and Society",3,"ME",2,1,{},{}});

    // ===== SECOND YEAR - SECOND SEM =====
    addCourse({"NCP2210","Computer Fundamentals and Programming",1,"ME",2,2,{},{}});
    addCourse({"NEC2211","Basic Electronics Lecture",2,"ME",2,2,{"NPH1201"},{}});
    addCourse({"NEC2212","Basic Electronics Lab",1,"ME",2,2,{"NEC2211"},{}});
    addCourse({"NME2201","Thermodynamics 2",3,"ME",2,2,{"NME2101"},{}});
    addCourse({"NME2202","Engineering Data Analysis",3,"ME",2,2,{"NMA1201"},{}});
    addCourse({"NME2203","Dynamics of Rigid Bodies",2,"ME",2,2,{"NPH1201"},{}});
    addCourse({"NME2204","Machine Elements 1",3,"ME",2,2,{"NPH1201"},{}});
    addCourse({"NME2205","Advanced Math for ME",3,"ME",2,2,{"NME2104"},{}});
    addCourse({"NME2206","Occupational Safety and Health",3,"ME",2,2,{},{}});
    addCourse({"PPF2201","PATHFIT 4",2,"ME",2,2,{},{}});
    addCourse({"ZGE1103","Ethics",3,"ME",2,2,{},{}});

    // ===== THIRD YEAR - FIRST SEM =====
    addCourse({"NME3101","Mechanics of Deformable Bodies",3,"ME",3,1,{"NME2102"},{}});
    addCourse({"NME3102","Engineering Economics",3,"ME",3,1,{"NMA1201"},{}});
    addCourse({"NME3103","Heat Transfer",2,"ME",3,1,{"NME2201"},{}});
    addCourse({"NME3104","Fluid Mechanics",3,"ME",3,1,{"NME2101"},{}});
    addCourse({"NME3105","Machine Elements 2",2,"ME",3,1,{"NME2204"},{}});
    addCourse({"NME3106","Materials Science for ME",3,"ME",3,1,{"NME3101","NCH1101"},{}});
    addCourse({"NME3107","Technopreneurship",3,"ME",3,1,{"NME2106"},{}});
    addCourse({"ZGEEL01","GE Elective 1",3,"ME",3,1,{},{"ZGE4301","ZGE4305","ZGE4309"}});
    addCourse({"ZPD1101","Personality Development",1,"ME",3,1,{},{}});

    // ===== THIRD YEAR - SECOND SEM =====
    addCourse({"NME3201","Research Methods",1,"ME",3,2,{},{}});
    addCourse({"NME3202","Refrigeration Systems",4,"ME",3,2,{"NME3103"},{}});
    addCourse({"NME3203","Fluid Machinery",4,"ME",3,2,{"NME3104"},{}});
    addCourse({"NME3204","Combustion Engineering",3,"ME",3,2,{"NME2201"},{}});
    addCourse({"NME3205","Machine Design 1",4,"ME",3,2,{"NME3101"},{}});
    addCourse({"NME3206","ME Laboratory 1",2,"ME",3,2,{"NME2201"},{}});
    addCourse({"NME3207","Vibration Engineering",2,"ME",3,2,{"NME2104"},{}});
    addCourse({"ZGEEL02","GE Elective 2",3,"ME",3,2,{},{"ZGE4301","ZGE4304","ZGE4305"}});
    addCourse({"ZGEEL03","GE Elective 3",3,"ME",3,2,{},{"ZGE4301","ZGE4305"}});

    // ===== FOURTH YEAR - FIRST SEM =====
    addCourse({"NME4101","Integrated ME Course 1",1,"ME",4,1,{"NME2102","NME2201","NME2203","NME3101","NME3102"},{}});
    addCourse({"NME4102","Air Conditioning Systems",4,"ME",4,1,{"NME3202"},{}});
    addCourse({"NME4103","Instrumentation and Control",3,"ME",4,1,{"NEC2211"},{}});
    addCourse({"NME4104","Machine Design 2",3,"ME",4,1,{"NME3205"},{}});
    addCourse({"NME4105","Computer Applications for ME",2,"ME",4,1,{},{}});
    addCourse({"NME4106","ME Laboratory 2",2,"ME",4,1,{"NME3206"},{}});
    addCourse({"NME4980","Project Study 1",1,"ME",4,1,{"NME3201"},{}});
    addCourse({"NMEEL01","ME Elective 1",2,"ME",4,1,{"NME3202"},{"NME4301","NME4401"}});
    addCourse({"ZGE1108","Understanding the Self",3,"ME",4,1,{},{}});

    // ===== FOURTH YEAR - SECOND SEM =====
    addCourse({"NME4201","Integrated ME Course 2",2,"ME",4,2,{"NME4202","NME4205","NME3205","NME4102","NME4104"},{}});
    addCourse({"NME4202","Industrial Plant Engineering",4,"ME",4,2,{"NME4102"},{}});
    addCourse({"NME4203","ME Laws and Ethics",3,"ME",4,2,{},{}});
    addCourse({"NME4204","Manufacturing Processes",2,"ME",4,2,{"NME4202"},{}});
    addCourse({"NME4205","Power Plant Design",5,"ME",4,2,{"NME3204"},{}});
    addCourse({"NME4206","ME Laboratory 3",2,"ME",4,2,{"NME4106"},{}});
    addCourse({"NME4990","Project Study 2",1,"ME",4,2,{"NME4980"},{}});
    addCourse({"NMEEL02","ME Elective 2",2,"ME",4,2,{},{}});

    //ECE
    // ==========================================
    // FIRST YEAR - FIRST SEMESTER
    // ==========================================
    addCourse({"MLC1101", "Literacy/Civic Welfare/Military Science 1", 3, "ECE", 1, 1, {}, {}});
    addCourse({"NCH1101", "Chemistry for Engineers Lecture", 3, "ECE", 1, 1, {}, {}});
    addCourse({"NCH1102", "Chemistry for Engineers Laboratory", 1, "ECE", 1, 1, {"NCH1101"}, {}});
    addCourse({"NEC1101", "Discrete Mathematics", 2, "ECE", 1, 1, {}, {}});
    addCourse({"NES1101", "Engineering Drawing", 1, "ECE", 1, 1, {}, {}});
    addCourse({"NMA1101", "Calculus 1", 3, "ECE", 1, 1, {}, {}});
    addCourse({"PPE1101", "Physical Education 1", 2, "ECE", 1, 1, {}, {}});
    addCourse({"ZGE1105", "Purposive Communication", 3, "ECE", 1, 1, {}, {}});
    addCourse({"ZPD1101", "Personality Development", 1, "ECE", 1, 1, {}, {}});
    addCourse({"NES1100", "Pre-Calculus for Engineers", 4, "ECE", 1, 1, {}, {}}); // Bridging course

    // ==========================================
    // FIRST YEAR - SECOND SEMESTER
    // ==========================================
    addCourse({"MLC1102", "Literacy/Civic Welfare/Military Science 2", 3, "ECE", 1, 2, {"MLC1101"}, {}});
    addCourse({"NEC1201", "Engineering Data Analysis", 3, "ECE", 1, 2, {"NMA1101"}, {}});
    addCourse({"NEC1202", "Material Science and Engineering for ECE", 3, "ECE", 1, 2, {"NCH1101"}, {}});
    addCourse({"NEC1203", "Environmental Science and Engineering", 3, "ECE", 1, 2, {}, {}});
    addCourse({"NEC1204", "Computer-Aided Drafting", 1, "ECE", 1, 2, {"NES1101"}, {}});
    addCourse({"NMA1201", "Calculus 2", 3, "ECE", 1, 2, {"NMA1101"}, {}});
    addCourse({"NPH1201", "Physics for Engineers Lecture", 3, "ECE", 1, 2, {"NMA1201"}, {}});
    addCourse({"NPH1202", "Physics for Engineers Laboratory", 1, "ECE", 1, 2, {"NPH1201"}, {}});
    addCourse({"PPE1102", "Physical Education 2", 2, "ECE", 1, 2, {"PPE1101"}, {}});
    addCourse({"ZGE1106", "Readings in Philippine History", 3, "ECE", 1, 2, {}, {}});

    // ==========================================
    // SECOND YEAR - FIRST SEMESTER
    // ==========================================
    addCourse({"NEC1102", "Computer Programming with Hardware Interfacing", 2, "ECE", 2, 1, {}, {}});
    addCourse({"NEC2101", "Physics for Engineers 2 Lecture", 3, "ECE", 2, 1, {"NPH1201"}, {}});
    addCourse({"NEC2102", "Physics for Engineers 2 Laboratory", 1, "ECE", 2, 1, {"NEC2101"}, {}});
    addCourse({"NEC2103", "Differential Equations", 3, "ECE", 2, 1, {"NMA1201"}, {}});
    addCourse({"NEC2104", "Electronics 1 Lecture", 3, "ECE", 2, 1, {"NEC1202"}, {}});
    addCourse({"NEC2105", "Electronics 1 Laboratory", 1, "ECE", 2, 1, {"NEC2104"}, {}});
    addCourse({"NEC2106", "Mechanics 1: Statics and Dynamics", 2, "ECE", 2, 1, {"NPH1201"}, {}});
    addCourse({"NEC2107", "Computer-Aided Electronic Circuit and PCB Design", 1, "ECE", 2, 1, {"NEC1204"}, {}});
    addCourse({"NEE2101", "Circuits 1 Lecture", 3, "ECE", 2, 1, {"NMA1201"}, {}});
    addCourse({"NEE2102", "Circuits 1 Laboratory", 1, "ECE", 2, 1, {"NEE2101"}, {}});
    addCourse({"PPE1103", "Physical Education 3", 2, "ECE", 2, 1, {"PPE1102"}, {}});
    addCourse({"ZGE1104", "Mathematics in the Modern World", 3, "ECE", 2, 1, {}, {}});

    // ==========================================
    // SECOND YEAR - SECOND SEMESTER
    // ==========================================
    addCourse({"NEC2203", "Mechanics 2: Strength of Materials", 2, "ECE", 2, 2, {"NEC2106"}, {}});
    addCourse({"NEC2204", "Adv. Engineering Mathematics for ECE Lecture", 3, "ECE", 2, 2, {"NEC2103"}, {}});
    addCourse({"NEC2205", "Adv. Engineering Mathematics for ECE Lab", 1, "ECE", 2, 2, {"NEC2204"}, {}});
    addCourse({"NEC2206", "Electronics 2 Lecture", 3, "ECE", 2, 2, {"NEC2104"}, {}});
    addCourse({"NEC2207", "Electronics 2 Laboratory", 1, "ECE", 2, 2, {"NEC2206"}, {}});
    addCourse({"NEC2208", "Digital Electronics 1 Lecture", 3, "ECE", 2, 2, {"NEC2104"}, {}});
    addCourse({"NEC2209", "Digital Electronics 1 Laboratory", 1, "ECE", 2, 2, {"NEC2208"}, {}});
    addCourse({"NEC2210", "Electromagnetics", 4, "ECE", 2, 2, {"NEC2101", "NEC2103"}, {}});
    addCourse({"NEE2201", "Circuits 2 Lecture", 3, "ECE", 2, 2, {"NEE2101"}, {}});
    addCourse({"NEE2202", "Circuits 2 Laboratory", 1, "ECE", 2, 2, {"NEE2201"}, {}});
    addCourse({"PPE1104", "Physical Education 4", 2, "ECE", 2, 2, {"PPE1103"}, {}});

    // ==========================================
    // THIRD YEAR - FIRST SEMESTER
    // ==========================================
    addCourse({"NEC3101", "Electronics 3 Lecture", 3, "ECE", 3, 1, {"NEC2206"}, {}});
    addCourse({"NEC3102", "Electronics 3 Laboratory", 1, "ECE", 3, 1, {"NEC3101"}, {}});
    addCourse({"NEC3103", "Digital Electronics 2 Lecture", 3, "ECE", 3, 1, {"NEC2208"}, {}});
    addCourse({"NEC3104", "Digital Electronics 2 Laboratory", 1, "ECE", 3, 1, {"NEC3103"}, {}});
    addCourse({"NEC3105", "Feedback and Control Systems Lecture", 3, "ECE", 3, 1, {"NEC2204"}, {}});
    addCourse({"NEC3106", "Feedback and Control Systems Lab", 1, "ECE", 3, 1, {"NEC3105"}, {}});
    addCourse({"NEC3107", "Methods of Research", 3, "ECE", 3, 1, {"NEC1201"}, {}});
    addCourse({"NEC3108", "Communications 1: Principles of Comm Systems Lec", 3, "ECE", 3, 1, {"NEC2204", "NEC2206"}, {}});
    addCourse({"NEC3109", "Communications 1: Principles of Comm Systems Lab", 1, "ECE", 3, 1, {"NEC3108"}, {}});
    addCourse({"ZGE1109", "Life and Works of Rizal", 3, "ECE", 3, 1, {}, {}});
    addCourse({"ZGEEL01", "GE Elective 1", 3, "ECE", 3, 1, {}, {}});

    // ==========================================
    // THIRD YEAR - SECOND SEMESTER
    // ==========================================
    addCourse({"NEC3201", "Communications 2 Lecture", 3, "ECE", 3, 2, {"NEC3108"}, {}});
    addCourse({"NEC3202", "Communications 2 Laboratory", 1, "ECE", 3, 2, {"NEC3201"}, {}});
    addCourse({"NEC3203", "Communications 3 Lecture", 3, "ECE", 3, 2, {"NEC3108", "NEC2210"}, {}});
    addCourse({"NEC3204", "Communications 3 Laboratory", 1, "ECE", 3, 2, {"NEC3203"}, {}});
    addCourse({"NEC3205", "Engineering Economics", 3, "ECE", 3, 2, {}, {}});
    addCourse({"NEC4980", "Design 1 / Capstone Project 1", 1, "ECE", 3, 2, {"NEC3107"}, {}});
    addCourse({"NECEL01", "ECE Elective 1 Lecture", 3, "ECE", 3, 2, {}, {}});
    addCourse({"NECEL02", "ECE Elective 1 Laboratory", 1, "ECE", 3, 2, {"NECEL01"}, {}});
    addCourse({"ZGE1102", "The Contemporary World", 3, "ECE", 3, 2, {}, {}});
    addCourse({"ZGE1103", "Ethics", 3, "ECE", 3, 2, {}, {}});
    addCourse({"ZGEEL02", "GE Elective 2", 3, "ECE", 3, 2, {}, {}});

    // ==========================================
    // THIRD YEAR - SUMMER
    // ==========================================
    addCourse({"NEC4970", "On-the-Job Training", 3, "ECE", 3, 3, {}, {}});

    // ==========================================
    // FOURTH YEAR - FIRST SEMESTER
    // ==========================================
    addCourse({"NEC4101", "Integrated Course 1", 2, "ECE", 4, 1, {}, {}});
    addCourse({"NEC4102", "Signals, Spectra, and Signal Processing Lec", 3, "ECE", 4, 1, {"NEC3108"}, {}});
    addCourse({"NEC4103", "Signals, Spectra, and Signal Processing Lab", 1, "ECE", 4, 1, {"NEC4102"}, {}});
    addCourse({"NEC4104", "Advanced Telecommunication Networks Lec", 2, "ECE", 4, 1, {"NEC3201"}, {}});
    addCourse({"NEC4105", "Advanced Telecommunication Networks Lab", 1, "ECE", 4, 1, {"NEC4104"}, {}});
    addCourse({"NEC4106", "Communications 4 (Data Communications) Lec", 3, "ECE", 4, 1, {"NEC3201"}, {}});
    addCourse({"NEC4107", "Communications 4 (Data Communications) Lab", 1, "ECE", 4, 1, {"NEC4106"}, {}});
    addCourse({"NEC4990", "Design 2 / Capstone Project 2", 1, "ECE", 4, 1, {"NEC4980"}, {}});
    addCourse({"ZGE1101", "Art Appreciation", 3, "ECE", 4, 1, {}, {}});
    addCourse({"ZGE1107", "Science, Technology, and Society", 3, "ECE", 4, 1, {}, {}});
    addCourse({"ZGE1108", "Understanding the Self", 3, "ECE", 4, 1, {}, {}});
    addCourse({"ZGEEL03", "GE Elective 3", 3, "ECE", 4, 1, {}, {}});

    // ==========================================
    // FOURTH YEAR - SECOND SEMESTER
    // ==========================================
    addCourse({"NEC3206", "ECE Laws, Contracts, Ethics, and Safety", 3, "ECE", 4, 2, {}, {}});
    addCourse({"NEC4201", "Integrated Course 2", 2, "ECE", 4, 2, {"NEC4101"}, {}});
    addCourse({"NEC4202", "Adv. Comm System and Design Lecture", 2, "ECE", 4, 2, {"NEC3201", "NEC3203"}, {}});
    addCourse({"NEC4203", "Adv. Comm System and Design Laboratory", 1, "ECE", 4, 2, {"NEC4202"}, {}});
    addCourse({"NEC4204", "Technopreneurship", 3, "ECE", 4, 2, {}, {}});
    addCourse({"NEC4205", "Broadcast Production & Distribution Lec", 2, "ECE", 4, 2, {"NEC3201"}, {}});
    addCourse({"NEC4206", "Broadcast Production & Distribution Lab", 1, "ECE", 4, 2, {"NEC4205"}, {}});
    addCourse({"NEC4207", "Engineering Management", 2, "ECE", 4, 2, {}, {}});
    addCourse({"NEC4208", "Seminars, Colloquium, and Field Trips", 2, "ECE", 4, 2, {}, {}});
    addCourse({"NECEL03", "ECE Elective 2 Lecture", 3, "ECE", 4, 2, {}, {}});
    addCourse({"NECEL04", "ECE Elective 2 Laboratory", 1, "ECE", 4, 2, {"NECEL03"}, {}});
    //EE
    // ==========================================
    // FIRST YEAR - FIRST SEMESTER
    // ==========================================
    addCourse({"MLC1101","Civic Welfare/Military Science 1",3,"CE",1,1,{},{}});
    addCourse({"NCE1201","Engineering Drawing and Plans",1,"CE",1,1,{},{}});
    addCourse({"NCH1101","Chemistry Lecture",3,"CE",1,1,{},{}});
    addCourse({"NCH1102","Chemistry Lab",1,"CE",1,1,{"NCH1101"},{}});
    addCourse({"NMA1101","Calculus 1",3,"CE",1,1,{},{}});
    addCourse({"PPF1101","PATHFIT 1",2,"CE",1,1,{},{}});
    addCourse({"ZGE1104","Mathematics in the Modern World",3,"CE",1,1,{},{}});
    addCourse({"ZGE1108","Understanding the Self",3,"CE",1,1,{},{}});

    // ==========================================
    // FIRST YEAR - SECOND SEMESTER
    // ==========================================
    addCourse({"MLC1102","Civic Welfare/Military Science 2",3,"CE",1,2,{"MLC1101"},{}});
    addCourse({"NCE1101","Civil Engineering Orientation",2,"CE",1,2,{},{}});
    addCourse({"NCE1102","Computer-Aided Drafting",1,"CE",1,2,{},{}});
    addCourse({"NMA1201","Calculus 2",3,"CE",1,2,{"NMA1101"},{}});
    addCourse({"NPH1201","Physics Lecture",3,"CE",1,2,{"NMA1101"},{}});
    addCourse({"NPH1202","Physics Lab",1,"CE",1,2,{"NPH1201"},{}});
    addCourse({"PPF1201","PATHFIT 2",2,"CE",1,2,{},{}});
    addCourse({"ZGE1102","Contemporary World",3,"CE",1,2,{},{}});
    addCourse({"ZGE1106","Philippine History",3,"CE",1,2,{},{}});
    addCourse({"ZGE1107","Science, Technology and Society",3,"CE",1,2,{},{}});

    // ==========================================
    // SECOND YEAR - FIRST SEMESTER
    // ==========================================
    addCourse({"NCE2102","Surveying Lecture",3,"CE",2,1,{"NCE1201"},{}});
    addCourse({"NCE2103","Surveying Lab",2,"CE",2,1,{"NCE2102"},{}});
    addCourse({"NCE2104","Statics",4,"CE",2,1,{"NMA1201","NPH1201"},{}});
    addCourse({"NCE2105","Engineering Utilities 1",3,"CE",2,1,{"NPH1201"},{}});
    addCourse({"NCE2106","Engineering Utilities 2",3,"CE",2,1,{"NPH1201"},{}});
    addCourse({"NCE2201","Building Systems",3,"CE",2,1,{"NCE2102"},{}});
    addCourse({"PPF2101","PATHFIT 3",2,"CE",2,1,{},{}});
    addCourse({"ZGE1101","Art Appreciation",3,"CE",2,1,{},{}});
    addCourse({"ZGE1109","Rizal",3,"CE",2,1,{},{}});

    // ==========================================
    // SECOND YEAR - SECOND SEMESTER
    // ==========================================
    addCourse({"NCE1102P","Programming",2,"CE",2,2,{},{}});
    addCourse({"NCE1202G","Geology",2,"CE",2,2,{"NCH1101"},{}});
    addCourse({"NCE2107","Differential Equations",3,"CE",2,2,{"NMA1201"},{}});
    addCourse({"NCE2203","Dynamics",2,"CE",2,2,{"NCE2104"},{}});
    addCourse({"NCE2204","Mechanics of Deformable Bodies",5,"CE",2,2,{"NCE2104"},{}});
    addCourse({"NCE2205","Engineering Management",2,"CE",2,2,{},{}});
    addCourse({"NCE3300","Quantity Surveying",2,"CE",2,2,{},{}});
    addCourse({"PPF2201","PATHFIT 4",2,"CE",2,2,{},{}});
    addCourse({"ZGE1105","Purposive Communication",3,"CE",2,2,{},{}});

    // ==========================================
    // THIRD YEAR - FIRST SEMESTER
    // ==========================================
    addCourse({"NCE2202","Numerical Solutions to CE Problems",3,"CE",3,1,{"NCE2107"},{}});  
    addCourse({"NCE3101","Highway and Railroad Engineering",3,"CE",3,1,{"NCE2102"},{}});  
    addCourse({"NCE3103","Structural Theory",4,"CE",3,1,{"NCE2204"},{}});  
    addCourse({"NCE3104","Geotechnical Engineering 1 (Soil Mechanics) Lecture",3,"CE",3,1,{"NCE1202","NCE2204"},{}});  
    addCourse({"NCE3105","Geotechnical Engineering 1 Lab",1,"CE",3,1,{"NCE3104"},{}});  
    addCourse({"NCE3202","Construction Methods and Project Management (BIM 3)",4,"CE",3,1,{"NCE3102"},{}});  
    addCourse({"ZGE1103","Ethics",3,"CE",3,1,{},{}});  
    addCourse({"ZGEEL01","GE Elective 1",3,"CE",3,1,{},{}});  

    // ==========================================
    // THIRD YEAR - SECOND SEMESTER
    // ==========================================
    addCourse({"NCE3106","Construction Materials and Testing Lecture",2,"CE",3,2,{"NCE1202","NCE2204"},{}});  
    addCourse({"NCE3107","Construction Materials and Testing Lab",1,"CE",3,2,{"NCE3106"},{}});  
    addCourse({"NCE3201","Principles of Transportation Engineering",3,"CE",3,2,{"NCE3101"},{}});  
    addCourse({"NCE3203","Principles of Reinforced/Prestressed Concrete",5,"CE",3,2,{"NCE3103"},{}});  
    addCourse({"NCE3204","Hydrology",2,"CE",3,2,{},{}});  
    addCourse({"NCE3205","Hydraulics Lecture",4,"CE",3,2,{"NCE2203","NCE2204"},{}});  
    addCourse({"NCE3206","Hydraulics Laboratory",1,"CE",3,2,{"NCE3205"},{}});  
    addCourse({"ZGEEL02","GE Elective 2",3,"CE",3,2,{},{}});  
    addCourse({"ZGEEL03","GE Elective 3",3,"CE",3,2,{},{}});  
    addCourse({"ZPD1101","Personality Development",1,"CE",3,2,{},{}});  

    // ==========================================
    // FOURTH YEAR - FIRST SEMESTER
    // ==========================================
    addCourse({"NCE3108","Engineering Data Analysis",3,"CE",4,1,{"NMA1101"},{}});  
    addCourse({"NCE4101","CE Integration 1",2,"CE",4,1,{"NCE3106","NCE4103","NCE2202","NCE3201"},{}});  
    addCourse({"NCE4102","Principles of Steel Design",4,"CE",4,1,{"NCE3103"},{}});  
    addCourse({"NCE4103","Engineering Economics",3,"CE",4,1,{},{}});  
    addCourse({"NCE4104","Fieldtrip and Seminars",2,"CE",4,1,{},{}});  
    addCourse({"NCE4801","CE Project 1",2,"CE",4,1,{},{}});  
    addCourse({"NCEEL01","Professional Course - Specialized 1",3,"CE",4,1,{"NCE4301"},{}});  
    addCourse({"NCEEL02","Professional Course - Specialized 2",3,"CE",4,1,{"NCE4302"},{}});  

    // ==========================================
    // FOURTH YEAR - SECOND SEMESTER
    // ==========================================
    addCourse({"NCE2206","Environmental Engineering",3,"CE",4,2,{"NCH1101"},{}});  
    addCourse({"NCE4201","CE Integration 2",2,"CE",4,2,{"NCE4102","NCE3104","NCE3203"},{}});  
    addCourse({"NCE4202","CE Law, Ethics and Contracts",3,"CE",4,2,{"NCE3202"},{}});  
    addCourse({"NCE4203","Technopreneurship",3,"CE",4,2,{"NCE4103"},{}});  
    addCourse({"NCE4900","CE Project 2",2,"CE",4,2,{"NCE4801"},{}});  
    addCourse({"NCEEL03","Professional Course - Specialized 3",3,"CE",4,2,{"NCE4303"},{}});  
    addCourse({"NCEEL04","Professional Course - Specialized 4",3,"CE",4,2,{"NCE4304"},{}});  
    addCourse({"NCEEL05","Professional Course - Specialized 5",3,"CE",4,2,{},{}});  
}

// MENU

void menu() {
    int choice;

    while (true) {
        cout << "\n=UNIVERSITY OF THE EAST  COLLEGE OF ENGINEERING=";
        cout << "\n========== PREREQUISITE COURSE MAPPER ==========\n";
        cout << "1. Display All Subjects\n";
        cout << "2. Search by Year & Semester\n";
        cout << "3. Hash Search Course\n";
        cout << "4. Show Prerequisites\n";
        cout << "5. Compute Units\n";
        cout << "6. Show Substitutes\n";
        cout << "7. Sort by Name (Bubble)\n";
        cout << "8. Sort by Units (Merge)\n";
        cout << "9. Show Dependent Subjects\n";
        cout << "0. Exit\n";
        cout << "Choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number.\n";
            pauseScreen();
            continue; 
        }

        if (choice == 0) {
            cout << "Exiting program...\n";
            break; 
        }

        if (choice == 1) {
            string prog;
            cout << "Enter program (EE, CE, ME, CpEe, ECE): ";
            cin >> prog;
            displayCoursesByProgram(prog);
        }

        else if (choice == 2) {
            int y, s;
            string prog;
            cout << "Enter Year, Semester (1, 2, or 3 for summer), Program: "; 
            cin >> y >> s >> prog;
            YearSemSearch(y, s, prog);
        }

        else if (choice == 3) {
            string code, prog;
            cout << "Enter Course Code and Program (e.g., NCE3108 (Enter) CE): ";
            cin >> code >> prog;

            searchCourse(code, prog);
        }

        else if (choice == 4) {
            string code, prog;
            cout << "Enter Course Code and Program (e.g., NCE3108 (Enter) CE): ";
            cin >> code >> prog;
            showPrerequisites(code, prog);
        }

        else if (choice == 5) {
            int y, s;
            string prog;
            cout << "Enter Year, Semester (1, 2, or 3 for summer), Program: "; 
            cin >> y >> s >> prog;
            computeUnits(y, s, prog);
        }

        else if (choice == 6) {
            string code, prog;
            cout << "Enter Course Code and Program (e.g., MLC1101 (Enter) CE): ";
            cin >> code >> prog;
            string key = code + "_" + prog;
            if (courseTable.find(key) != courseTable.end()) {
                const auto& subs = courseTable[key].substitutes;
                if (subs.empty()) {
                    cout << "No substitutes for this course.\n";
                } else {
                    cout << "Substitutes for " << code << ":\n";
                    for (const auto& sub : subs) {
                        cout << sub << endl;
                    }
                }
            } else {
                cout << "Course not found.\n";
            }
        }

        else if (choice == 7) {
            string prog;
            cout << "Enter program (EE, CE, ME, CpE, ECE): ";
            cin >> prog;
            bubbleSortByName();

            for (const auto& c : courseList) {
            if (c.program == prog) {
            cout << c.code << " - " << c.name << endl;
            }
            }
        }

        else if (choice == 8) {
            mergeSort(courseList, 0, courseList.size() - 1);

            string prog;
            cout << "Enter program (EE, CE, ME, Cpe, ECE): ";
            cin >> prog;

            cout << "Sorted by units:\n";
                for (const auto& c : courseList) {
                if (c.program == prog) {
                    cout << c.code << " - " << c.name
                         << " (" << c.units << " units)\n";
            }
            }

    
        }

        else if (choice == 9) {
            string code, prog;
            cout << "Enter Course Code and Program (e.g., NCE3108 (Enter) CE): ";
            cin >> code >> prog;
            showDependentCourses(code, prog);
        }

        else {
            cout << "Invalid choice.\n";
        }

        pauseScreen();
    }
}

// MAIN
int main() {
    loadSampleData();
    menu();
    return 0;
}
