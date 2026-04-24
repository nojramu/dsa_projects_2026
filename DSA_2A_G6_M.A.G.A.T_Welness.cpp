#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <chrono>
#include <thread>
#include <limits>  // Added for cin.ignore
#include <cstdlib> // Added for atoll
#include <string>  // Added for string operations
#include <sstream> // Added for stringstream
#include <iomanip> // Added for setw
#include <cstdio>
#include <functional>
#include <algorithm>
using namespace std;    

// DATA STRUCTURES (Entities)
struct Student
{
    long long studentID; // long long yung crocodile haha
    char password[20];
    char name[50];
    int yearLevel;
    int stressLevel; // 1-10
    char assignedCounselor[20];
};

struct Admin
{
    char username[20];
    char password[20];
    char name[50]; // Added to store Admin's real name
};

struct Counselor
{
    char username[20];
    char password[20];
    char name[50];       // Added to store Counselor's real name
    char specialty[100]; // Added to store their specialty
    int assignedYear;
};

struct Case
{
    long long studentID;              // long long yung crocodile haha
    char clinicalObservations[500];   // [1] Clinical Observations
    char discussionSummary[500];      // [2] Discussion Summary
    char professionalAssessment[500]; // [3] Counselor's Assessment
    char actionPlan[500];             // [5] Action Plan (matches Concept)
    char date[20];                    // [DATE/TIME] from concept
};

struct Appointment {
    char studentID[20];
    char studentName[50];
    char reason[100];
    char preferredDate[20];
    char preferredTime[20];      
    char assignedCounselor[20];
    char type;                   
};

// HELPER FUNCTIONS
// Cross-platform screen clear (detects if you are on Windows or Mac/Linux mayabang kase tayo)
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Helper function to trim whitespace and special characters
string trimString(const string& str)
{
    string result = str;
    // Remove BOM if present
    if (!result.empty() && (unsigned char)result[0] == 0xEF) {
        result.erase(0, 3);
    }
    // Remove leading whitespace
    size_t start = result.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    result = result.substr(start);
    // Remove trailing whitespace
    size_t end = result.find_last_not_of(" \t\r\n");
    if (end != string::npos) result = result.substr(0, end + 1);
    return result;
}

// Simulated loading screen ni daddy Euken
void loadingTransition(const string &message, int seconds)
{
    cout << "\n[SYSTEM] " << message << "..." << endl;
    this_thread::sleep_for(chrono::seconds(seconds));
    clearScreen();
}

// FILE HANDLING & CREATION (CSV VERSION)
void initializeFiles()
{
    // Changed extensions to .csv
    const char* files[] = {"students.csv", "cases.csv", "counselors.csv", "admins.csv", "appointments.csv", "temp_students.csv"};
    for (const char* file : files)
    {
       
        ifstream checkFile(file);
        if (!checkFile)
        {
           
            ofstream createFile(file);
            cout << "[SYSTEM] Created missing database file: " << file << endl;
            createFile.close();
           
            // Seed Admin (Ma. Leonila V. Urrea)
            if (strcmp(file, "admins.csv") == 0)
            {
                ofstream adminFile(file, ios::app);
                // Writing as comma-separated values
                adminFile << "leonila_urrea,AdminPass123,Ma. Leonila V. Urrea\n";
                adminFile.close();
                cout << "[SYSTEM] Default admin account created." << endl;
            }
           
            // Seed Counselors based on specific list
            if (strcmp(file, "counselors.csv") == 0)
            {
                ofstream counselorFile(file, ios::app);
               
                Counselor defaultCounselors[5] = {
                    {"antonio_postrado", "Antonio@123", "Antonio C. Postrado Jr.", "Academic, Emotional, Career"},
                    {"jayson_parena", "Jayson@123", "Jayson S. Parena", "Academic, Emotional, Career"},
                    {"janine_bautista", "Janine@123", "Ma. Janine C. Bautista", "Academic, Emotional, Career"},
                    {"arnold_desilva", "Arnold@123", "Arnold A. de Silva", "Psychological Testing (Assessment)"},
                    {"jovine_delacruz", "Jovine@123", "Jovine Dimple L. Dela Cruz", "Administrative Support"}
                };

                for (int i = 0; i < 5; i++) {
                    // Writing as comma-separated values
                    counselorFile << defaultCounselors[i].username << ","
                                  << defaultCounselors[i].password << ","
                                  << defaultCounselors[i].name << ","
                                  << defaultCounselors[i].specialty << "\n";
                }
               
                counselorFile.close();
                cout << "[SYSTEM] Staff counselor accounts seeded." << endl;
            }

            // Seed default Students
            if (strcmp(file, "students.csv") == 0)
            {
                ofstream studentFile(file, ios::app);
                Student students[] = {
                    // ===== FIRST YEAR =====
                    {20131155389LL, "macal01", "Macalintal, Lev Ryan F.", 1, 1, "jayson_parena"},
                    {20251104485LL, "gran02", "Grande, Clayton Kelsey P.", 1, 1, "jayson_parena"},
                    {20231109721LL, "cana03", "Canaveral, Miguel R.", 1, 1, "jayson_parena"},
                    {20231123345LL, "ramos04", "Ramos, Rhian Zyrene G.", 1, 1, "jayson_parena"},
                    {20251120066LL, "chiu05", "Chiu, Nash Patrick G.", 1, 1, "jayson_parena"},
                    {20251123963LL, "alca06", "Alcazar, Maxine Aliyah F.", 1, 1, "jayson_parena"},
                    {20251130106LL, "cruz07", "Cruz, Sean Russel D.", 1, 1, "jayson_parena"},
                    {20251121161LL, "javi08", "Javier, Zionah Rae M.", 1, 1, "jayson_parena"},
                    {20251114564LL, "bern09", "Bernabe, Marx Aaron M.", 1, 1, "jayson_parena"},
                    {20251108846LL, "dion10", "Dionisio, Jerich Andrew", 1, 1, "jayson_parena"},
                    {20251105412LL, "acun11", "Acuna, Emmanuel Patrick R.", 1, 1, "jayson_parena"},
                    {20251131278LL, "guna12", "Gunabe, Jophielle Briggz", 1, 1, "jayson_parena"},
                    {20221141259LL, "says13", "Sayson, Ralph Jericho A.", 1, 1, "jayson_parena"},
                    {20251123850LL, "lama14", "Lamamigo, Eros", 1, 1, "jayson_parena"},
                    {20231137749LL, "igna15", "Ignacio, Axel Audrey D.", 1, 1, "jayson_parena"},
                    {20251117643LL, "chua16", "Chua, Cyrus G.", 1, 1, "jayson_parena"},
                    {20251108255LL, "isra17", "Israel, Quinn Mallory C.", 1, 1, "jayson_parena"},
                    {20191132646LL, "marq18", "Marquez, Manuel Alexander", 1, 1, "jayson_parena"},
                    {20251129609LL, "rata19", "Rata, Rean May", 1, 1, "jayson_parena"},
                    {20231128975LL, "yamb20", "Yambao, Zuriel Ethan P.", 1, 1, "jayson_parena"},

                    // ===== SECOND YEAR =====
                    {20241113019LL, "ranc21", "Rance, Princess Nicole R.", 2, 1, "antonio_postrado"},
                    {20241102346LL, "alde22", "Alde, Patrick P.", 2, 1, "antonio_postrado"},
                    {20221105062LL, "baut23", "Bautista, Argelina Ryle P.", 2, 1, "antonio_postrado"},
                    {20241123375LL, "alde24", "Aldeguer, Seun Gyrich I.", 2, 1, "antonio_postrado"},
                    {20241120229LL, "gabi25", "Gabiano, Karl Sebastian B.", 2, 1, "antonio_postrado"},
                    {20241120810LL, "gasp26", "Gaspi, Jay Lord F.", 2, 1, "antonio_postrado"},
                    {20241117362LL, "jule27", "Jules, Roven", 2, 1, "antonio_postrado"},
                    {20221112603LL, "bern28", "Bermardo, Tj", 2, 1, "antonio_postrado"},
                    {20241120809LL, "timb29", "Timbol, Alexandria T.", 2, 1, "antonio_postrado"},
                    {20241130029LL, "lope30", "Lopez, Gwen Angelynn", 2, 1, "antonio_postrado"},
                    {20241116336LL, "roqu31", "Roque, Kayne Bernadette M.", 2, 1, "antonio_postrado"},
                    {20241107313LL, "eyst32", "Eystaquio, Mariane", 2, 1, "antonio_postrado"},
                    {20241141071LL, "dela33", "Dela Cruz, John Patrick", 2, 1, "antonio_postrado"},
                    {20221124523LL, "jane34", "Janer, Jenny Rose M.", 2, 1, "antonio_postrado"},
                    {20221104923LL, "nem35", "Nem, Hernan Jay", 2, 1, "antonio_postrado"},
                    {20181114136LL, "park36", "Park, Chang Hyun A.", 2, 1, "antonio_postrado"},
                    {20241126307LL, "tamp37", "Tampus, Jevric", 2, 1, "antonio_postrado"},
                    {20241125746LL, "tole38", "Tolentino, Angela D.", 2, 1, "antonio_postrado"},
                    {20241124674LL, "adam39", "Adame, Jennilie A.", 2, 1, "antonio_postrado"},
                    {20221115635LL, "ogue40", "Oguez, John Meynaro R.", 2, 1, "antonio_postrado"},

                    // ===== THIRD YEAR =====
                    {20231110057LL, "coco41", "Coco, James H.", 3, 1, "janine_bautista"},
                    {20131147803LL, "pine42", "Pineda, Franz Luis", 3, 1, "janine_bautista"},
                    {20161137764LL, "ting43", "Ting, Aljiemer A.", 3, 1, "janine_bautista"},
                    {20237144006LL, "goro44", "Gorospe, Rein Klyde T.", 3, 1, "janine_bautista"},
                    {20211102453LL, "bonz45", "Bonzon, Aldrich", 3, 1, "janine_bautista"},
                    {20231119974LL, "magr46", "Magrata, Renz", 3, 1, "janine_bautista"},
                    {20211106739LL, "libr47", "Librada, Charles Bryan L.", 3, 1, "janine_bautista"},
                    {20231143401LL, "bedi48", "Bedis, Ronwin Adlai B.", 3, 1, "janine_bautista"},
                    {20111135659LL, "tiba49", "Tibay, Hyrile", 3, 1, "janine_bautista"},
                    {20211114668LL, "rana50", "Rana, Adrian", 3, 1, "janine_bautista"},
                    {20231128555LL, "vill51", "Villaflor, Kyle L.", 3, 1, "janine_bautista"},
                    {20191151721LL, "cara52", "Carao, Johnleez", 3, 1, "janine_bautista"},
                    {20231120140LL, "hale53", "Hale, Hannah Grace E.", 3, 1, "janine_bautista"},
                    {20231150122LL, "gamb54", "Gambao, Rhamuel G.", 3, 1, "janine_bautista"},
                    {20231123083LL, "yita55", "Yitalyu, Mark Andrew", 3, 1, "janine_bautista"},
                    {20231125874LL, "baus56", "Bauson, John Carlo T.", 3, 1, "janine_bautista"},
                    {20211115901LL, "rons57", "Ronsairo, Tordan P.", 3, 1, "janine_bautista"},
                    {20211105452LL, "dial58", "Dialino, Rny Marc A.", 3, 1, "janine_bautista"},
                    {20231135618LL, "bayo59", "Bayona, Ralph", 3, 1, "janine_bautista"},
                    {20231114933LL, "simb60", "Simbulan, Anton Francis J.", 3, 1, "janine_bautista"},

                    // ===== FOURTH YEAR =====
                    {20221129620LL, "cruz61", "Cruz, Fyi James J.", 4, 1, "arnold_desilva"},
                    {20171109323LL, "rock62", "Rock, Chester M.", 4, 1, "arnold_desilva"},
                    {20161117744LL, "laca63", "Lacay, Tristan Russel M.", 4, 1, "arnold_desilva"},
                    {20221132054LL, "go64", "Go, Hans Stephen", 4, 1, "arnold_desilva"},
                    {20201129961LL, "buen65", "Buenaventura, Ai Francis", 4, 1, "arnold_desilva"},
                    {20211106739LL, "libr66", "Librada, Charles", 4, 1, "arnold_desilva"},
                    {20221131244LL, "nava67", "Navaro, Juanabel E.", 4, 1, "arnold_desilva"},
                    {20221130354LL, "paja68", "Pajarillaga, Jasmine Rose R.", 4, 1, "arnold_desilva"},
                    {20221133228LL, "bayl69", "Baylon, Trisha", 4, 1, "arnold_desilva"},
                    {20221103288LL, "mark70", "Mark Geoffrey", 4, 1, "arnold_desilva"},
                    {20221120509LL, "dona71", "Donayre, Willianne Lorraine M.", 4, 1, "arnold_desilva"},
                    {20221119159LL, "conc72", "Conception, Liam Pyro P.", 4, 1, "arnold_desilva"},
                    {20201107564LL, "vino73", "Vinoya, John Joneil", 4, 1, "arnold_desilva"},
                    {20201107622LL, "dizo74", "Dizon, Marc", 4, 1, "arnold_desilva"},
                    {20221100830LL, "anir75", "Anirion, Cristian F.", 4, 1, "arnold_desilva"},
                    {20221100410LL, "bern76", "Bernardino, Bryan G.", 4, 1, "arnold_desilva"},
                    {20221129880LL, "fran77", "Franco, Ivan B.", 4, 1, "arnold_desilva"},
                    {2021135155LL, "agui78", "Aguire, Angelito B.", 4, 1, "arnold_desilva"},
                    {20221105312LL, "marv79", "Marivic, John", 4, 1, "arnold_desilva"},
                    {20221131038LL, "san80", "Santos, Maria", 4, 1, "arnold_desilva"}
                };

                int numStudents = sizeof(students) / sizeof(Student);

                for (int i = 0; i < numStudents; i++) {
                    // Writing as comma-separated values
                    studentFile << students[i].studentID << ","
                                << students[i].password << ","
                                << students[i].name << ","
                                << students[i].yearLevel << ","
                                << students[i].stressLevel << ","
                                << students[i].assignedCounselor << "\n";
                }

                studentFile.close();
                cout << "[SYSTEM] Default student accounts seeded." << endl;
            }
        } else {
            checkFile.close();
        }
    }
    cout << "[SYSTEM] All database files are ready." << endl;
    this_thread::sleep_for(chrono::seconds(2));
    clearScreen();
}

// ALGORITHMS
// 1. Helper function for Quick Sort (Swaps two Student records)
void swapStudents(Student &a, Student &b)
{
    Student temp = a;
    a = b;
    b = temp;
}

// 2. Partition function for Quick Sort
int partitionByID(vector<Student> &arr, int low, int high)
{
    long long pivot = arr[high].studentID; // Choosing the last element as the pivot
    int i = (low - 1);                     // Index of the smaller element

    for (int j = low; j <= high - 1; j++)
    {
        // If current element is smaller than the pivot
        if (arr[j].studentID < pivot)
        {
            i++;
            swapStudents(arr[i], arr[j]);
        }
    }
    swapStudents(arr[i + 1], arr[high]);
    return (i + 1);
}

// 3. The actual Quick Sort Function (Sort by Student ID)
void quickSortByID(vector<Student> &arr, int low, int high)
{
    if (low < high)
    {
        // pi is the partitioning index, arr[pi] is now at the right place
        int pi = partitionByID(arr, low, high);

        // Separately sort elements before and after partition
        quickSortByID(arr, low, pi - 1);
        quickSortByID(arr, pi + 1, high);
    }
}

// 4. Partition function for Quick Sort by Priority (Stress Level descending)
int partitionByPriority(Student arr[], int low, int high)
{
    int pivot = arr[high].stressLevel;
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (arr[j].stressLevel > pivot)
        {
            i++;
            swapStudents(arr[i], arr[j]);
        }
    }
    swapStudents(arr[i + 1], arr[high]);
    return i + 1;
}

// 5. Quick Sort by Priority
void quickSortPriority(Student arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partitionByPriority(arr, low, high);
        quickSortPriority(arr, low, pi - 1);
        quickSortPriority(arr, pi + 1, high);
    }
}

// Perfect for sorted, non-uniform data like real Student Numbers.
int binarySearch(const vector<Student> &arr, long long targetID)
{
    int left = 0;
    int right = arr.size() - 1;
    while (left <= right)
    {
        // Calculate mid this way to prevent integer overflow on massive arrays
        int mid = left + (right - left) / 2;

        // 1. Target found!
        if (arr[mid].studentID == targetID)
        {
            return mid;
        }

        // 2. If target is greater, ignore the left half
        if (arr[mid].studentID < targetID)
        {
            left = mid + 1;
        }

        // 3. If target is smaller, ignore the right half
        else
        {
            right = mid - 1;
        }
    }

    // Target was not found in the array
    return -1;
}

// ADMIN FUNCTIONALITIES
void adminAddStudent()
{
    clearScreen();
    cout << "\t\t\t\t\t=============================================================" << endl;
    cout << "\t\t\t\t\t                    REGISTER NEW STUDENT                               " << endl;
    cout << "\t\t\t\t\t=============================================================" << endl;
   
   
    Student newStudent;
   
    cout << "\t\t\t\t\tEnter Student ID: ";
    cin >> newStudent.studentID;
   
    cout << "\t\t\t\t\tEnter Password: ";
    cin >> newStudent.password;
   
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
   
    cout << "\t\t\t\t\tEnter Full Name: ";
    cin.getline(newStudent.name, 50);
   
    cout << "\t\t\t\t\tEnter Year Level (1-4): ";
    cin >> newStudent.yearLevel;
   
    cout << "\t\t\t\t\tEnter Initial Stress Level (1-10): ";
    cin >> newStudent.stressLevel;
   
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
   
    cout << "\t\t\t\t\tAssign to Counselor (Enter Username): ";
    cin.getline(newStudent.assignedCounselor, 20);
   
    // Write strictly to CSV! (nalimutan iadd bobo)
    ofstream outFile("students.csv", ios::app);
    outFile << newStudent.studentID << ","
            << newStudent.password << ","
            << newStudent.name << ","
            << newStudent.yearLevel << ","
            << newStudent.stressLevel << ","
            << newStudent.assignedCounselor << "\n";
    outFile.close();
   
    cout << "\t\t\t\t\t[SYSTEM] Student successfully registered to the database!" << endl;
    this_thread::sleep_for(chrono::seconds(2));
}

void adminAddCounselor()
{
    clearScreen();
    cout << "\t\t\t\t\t==================================================" << endl;
    cout << "\t\t\t\t\t              REGISTER NEW COUNSELOR              " << endl;
    cout << "\t\t\t\t\t==================================================" << endl;
   
    Counselor newCounselor;
   
    cout << "\t\t\t\t\tEnter System Username: ";
    cin >> newCounselor.username;
   
    cout << "\t\t\t\t\tEnter Initial Password: ";
    cin >> newCounselor.password;
   
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
   
    cout << "\t\t\t\t\tEnter Full Name: ";
    cin.getline(newCounselor.name, 50);
   
    cout << "\t\t\t\t\tEnter Department/Specialty: ";
    cin.getline(newCounselor.specialty, 100);
   
    //Write strictly to CSV! (nabaliw ako dito slight)
    ofstream outFile("counselors.csv", ios::app);
    outFile << newCounselor.username << ","
            << newCounselor.password << ","
            << newCounselor.name << ","
            << newCounselor.specialty << "\n";
    outFile.close();
   
    cout << "\t\t\t\t\t[SYSTEM] Counselor successfully added to the system!" << endl;
    this_thread::sleep_for(chrono::seconds(2));
}

void adminViewCounselors()
{
    clearScreen();
    cout << "========================================================================================================================" << endl;
    cout << "                                              ACTIVE COUNSELOR STAFF                                                    " << endl;
    cout << "========================================================================================================================" << endl;


    cout << left
         << setw(10) << "ID"
         << setw(25) << "USERNAME"
         << setw(35) << "FULL NAME"
         << "SPECIALTY" << endl;
    cout << "------------------------------------------------------------------------------------------------------------------------" << endl;


    ifstream file("counselors.csv");


    if (!file)
    {
        cout << "\n [ERROR] Counselor database not found." << endl;
    }
    else
    {
        string line;
        int count = 0;


        while (getline(file, line))
        {
            stringstream ss(line);
            string username, password, name, specialty;
            getline(ss, username, ',');
            getline(ss, password, ',');
            getline(ss, name, ',');
            getline(ss, specialty);


            Counselor temp;
            strcpy(temp.username, username.c_str());
            strcpy(temp.password, password.c_str());
            strcpy(temp.name, name.c_str());
            strcpy(temp.specialty, specialty.c_str());
            temp.assignedYear = 0;

            count++;


            // We use string(array) to ensure C++ handles the null terminator properly
            cout << left
                 << "[" << setw(7) << to_string(count) + "]"
                 << setw(25) << temp.username
                 << setw(35) << temp.name
                 << temp.specialty << endl;
        }


        if (count == 0)
            cout << " > No counselor staff currently registered." << endl;
        file.close();
    }

    cout << "========================================================================================================================" << endl;
    cout << "\nPress Enter to return...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}


void adminMatcher() {
    clearScreen();
    cout << "=========================================================" << endl;
    cout << "          STUDENT-COUNSELOR MATCHING SYSTEM              " << endl;
    cout << "=========================================================" << endl;

    // 1. Load all students
    vector<Student> students;
    ifstream sfile("students.csv");
    string sline;
    while (getline(sfile, sline)) {
        if (trimString(sline).empty()) continue;
        stringstream ss(sline);
        string idStr, pass, name, yearStr, stressStr, counselor;
        getline(ss, idStr, ',');
        getline(ss, pass, ',');
        getline(ss, name, ',');
        getline(ss, yearStr, ',');
        getline(ss, stressStr, ',');
        getline(ss, counselor);
        try {
            Student st;
            st.studentID = stoll(idStr);
            strcpy(st.password, pass.c_str());
            strcpy(st.name, name.c_str());
            st.yearLevel = stoi(yearStr);
            st.stressLevel = stoi(stressStr);
            strcpy(st.assignedCounselor, counselor.c_str());
            students.push_back(st);
        } catch (...) { continue; }
    }
    sfile.close();

    // 2. Load all counselors
    struct CounselorNode {
        string username;
        string name;
        string specialty;
        int assignedYear; // 0 = all, 1-4 = specific year
    };
    vector<CounselorNode> counselors;
    ifstream cfile("counselors.csv");
    string cline;
    while (getline(cfile, cline)) {
        stringstream ss(cline);
        string user, pass, name, spec;
        getline(ss, user, ',');
        getline(ss, pass, ',');
        getline(ss, name, ',');
        getline(ss, spec);
        CounselorNode c;
        c.username = trimString(user);
        c.name = trimString(name);
        c.specialty = trimString(spec);
        // Assign year based on specialty (simple logic, can be improved)
        if (spec.find("1") != string::npos) c.assignedYear = 1;
        else if (spec.find("2") != string::npos) c.assignedYear = 2;
        else if (spec.find("3") != string::npos) c.assignedYear = 3;
        else if (spec.find("4") != string::npos) c.assignedYear = 4;
        else c.assignedYear = 0; // 0 = all years
        counselors.push_back(c);
    }
    cfile.close();

    // 3. Build the graph: student index -> list of counselor indices
    vector<vector<int>> graph(students.size());
    for (size_t i = 0; i < students.size(); ++i) {
        for (size_t j = 0; j < counselors.size(); ++j) {
            // Match by year or all, and by stress level (example: high stress to those with "Psychological" specialty)
            bool yearMatch = (counselors[j].assignedYear == 0 || counselors[j].assignedYear == students[i].yearLevel);
            bool stressMatch = (students[i].stressLevel >= 8 && counselors[j].specialty.find("Psychological") != string::npos)
                || (students[i].stressLevel < 8 && counselors[j].specialty.find("Academic") != string::npos)
                || (counselors[j].specialty.find("Career") != string::npos)
                || (counselors[j].specialty.find("Administrative") != string::npos);
            if (yearMatch && stressMatch) {
                graph[i].push_back(j);
            }
        }
    }

    // 4. DFS to find a counselor for each student (greedy, one-to-one)
    vector<int> counselorAssigned(counselors.size(), -1); // -1 means free
    function<bool(int, vector<bool>&)> dfs = [&](int studIdx, vector<bool>& visited) {
        for (int cIdx : graph[studIdx]) {
            if (visited[cIdx]) continue;
            visited[cIdx] = true;
            if (counselorAssigned[cIdx] == -1 || dfs(counselorAssigned[cIdx], visited)) {
                counselorAssigned[cIdx] = studIdx;
                return true;
            }
        }
        return false;
    };

    int matched = 0;
    vector<int> studentToCounselor(students.size(), -1);
    for (size_t i = 0; i < students.size(); ++i) {
        vector<bool> visited(counselors.size(), false);
        if (dfs(i, visited)) matched++;
    }
    // Build reverse mapping for output
    for (size_t c = 0; c < counselors.size(); ++c) {
        if (counselorAssigned[c] != -1)
            studentToCounselor[counselorAssigned[c]] = c;
    }

    // 5. Output results
    cout << left << setw(20) << "Student Name" << setw(10) << "Year" << setw(10) << "Stress" << "Matched Counselor" << endl;
    cout << "-------------------------------------------------------------------------------" << endl;
    for (size_t i = 0; i < students.size(); ++i) {
        cout << left << setw(20) << students[i].name << setw(10) << students[i].yearLevel << setw(10) << students[i].stressLevel;
        if (studentToCounselor[i] != -1)
            cout << counselors[studentToCounselor[i]].name << " (" << counselors[studentToCounselor[i]].specialty << ")";
        else
            cout << "No suitable counselor found";
        cout << endl;
    }
    cout << "\nTotal matched: " << matched << " out of " << students.size() << endl;
    cout << "\nPress Enter to return to the dashboard...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// DASHBOARDS
void studentDashboard(Student currentStudent)
{
    int choice = 0;
    do
    {
        clearScreen();
        cout << R"( 
                                                                                                                                                               
                                                                                 ██                                                                        
              █████████████████     ███   █                                      █████████                                                  ███   █         
             ███████      ██████    ████████                                       ████████                                                 ███████         
             ████████      ███        ██████                                       ████████                                                  █████         
              ███████████             ███████      ████          ███         ███   ████████          ████         ███          ███          ██████         
               ██████████████      ████████████  █████████   █████████     ██████████████████    ██████  ██████     █████████████████     ████████████      
                ███████████████      ████████    ████████    ███████    ████████   ████████    ███████    ██████   ██████████  ███████      ███████         
                   ██████████████    ████████     ███████    ███████    ████████   ████████    ███████   ███████   █████████   ███████      ███████         
                      ████████████   ████████     ███████    ████████   ████████   ████████   █████████████████    ████████    ███████      ███████         
               ███       █████████   ████████     ███████    ████████   ████████   ████████    ███████         █   ████████    ███████      ███████         
              █████       ███████    ████████     ███████    ██████████  ████████  ████████   ████████  ███████   ████████    ███████       ███████         
             ████████████████       █████████     ██████████████████      █████████████████    ███████ ██████     █████████  █████████    ██████████      
                                                                                                    ████                                                                                                                                                                                         
            )" << endl;
        cout << "\t\t\t\t\tGreetings, " << currentStudent.name << "! How can we support you today?\n"
             << endl;

        cout << "\t\t\t\t\t 1. View Student Profile & Schedules" << endl;
        cout << "\t\t\t\t\t 2. Wellness Assessment" << endl;
        cout << "\t\t\t\t\t 3. Emergency Booking" << endl;
        cout << "\t\t\t\t\t 4. View Resource Recommendations" << endl;
        cout << "\t\t\t\t\t 5. Book Counseling Session" << endl;
        cout << "\t\t\t\t\t 6. Logout" << endl;
        cout << "\t\t\t\t\t--------------------------------------------------" << endl;
        cout << "\t\t\t\t\t Enter choice: ";

        cin >> choice;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }

        switch (choice)
        {
        case 1:
            loadingTransition("\t\t\t\t\tFetching student profile and schedules", 1);
            clearScreen();
            cout << "==================================================" << endl;
            cout << "                YOUR STUDENT PROFILE              " << endl;
            cout << "==================================================" << endl;
            cout << " Name               : " << currentStudent.name << endl;
            cout << " Student ID         : " << currentStudent.studentID << endl;
            cout << " Year Level         : " << currentStudent.yearLevel << endl;
            cout << " Stress Level       : " << currentStudent.stressLevel << " / 10" << endl;
            cout << " Assigned Counselor : " << currentStudent.assignedCounselor << endl;
            cout << "--------------------------------------------------" << endl;
            cout << "             UPCOMING COUNSELING SESSIONS         " << endl;
            cout << "--------------------------------------------------" << endl;
            cout << " > No appointments currently scheduled." << endl;
            cout << "==================================================" << endl;
            cout << "\nPress Enter to return to the dashboard...";


            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            break;


        case 2:
        {
            bool stayInCase = true;
            do {
                loadingTransition("\t\t\t\t\tLoading Wellness Assessment Module", 1);
                cout << "=========================================================\n";
                cout << "          WELLNESS ASSESSMENT MODULE (STUDENT)           \n";
                cout << "=========================================================\n";
                cout << "Likert Scale: 1(Never) 2(Rarely) 3(Sometimes) 4(Often) 5(Always)\n";
                cout << "---------------------------------------------------------\n";

                int totalScore = 0;
                int ans = 0;

                // --- 1. DYNAMIC YEAR-BASED QUESTIONNAIRE ---
                if (currentStudent.yearLevel == 1) {
                    cout << "\n[1st Year Focus: Adjustment & Transition]\n";
                    cout << "1. Difficulty adapting to the new college environment: "; cin >> ans; totalScore += ans;
                    cout << "2. I feel homesick or isolated from peers: "; cin >> ans; totalScore += ans;
                    cout << "3. I struggle to understand the university grading system: "; cin >> ans; totalScore += ans;
                    cout << "4. I have difficulty making new friends: "; cin >> ans; totalScore += ans;
                    cout << "5. I feel overwhelmed by the college workload: "; cin >> ans; totalScore += ans;
                }

                else if (currentStudent.yearLevel == 2) {
                    cout << "\n[2nd Year Focus: Academic Pressure & Social Dynamics]\n";
                    cout << "1. I feel increased pressure to perform academically: "; cin >> ans; totalScore += ans;
                    cout << "2. I struggle with time management and balancing responsibilities: "; cin >> ans; totalScore += ans;
                    cout << "3. I experience stress from social dynamics and peer relationships: "; cin >> ans; totalScore += ans;
                    cout << "4. I have difficulty finding motivation for my studies: "; cin >> ans; totalScore += ans;
                    cout << "5. I feel uncertain about my academic path and future goals: "; cin >> ans; totalScore += ans;
                }
                else if (currentStudent.yearLevel == 3) {
                    cout << "\n[3rd Year Focus: Career Uncertainty & Increased Workload]\n";
                    cout << "1. I feel anxious about choosing a career path: "; cin >> ans; totalScore += ans;
                    cout << "2. I struggle with the increased workload and academic demands: "; cin >> ans; totalScore += ans;
                    cout << "3. I experience stress from internships or job applications: "; cin >> ans; totalScore += ans;
                    cout << "4. I have difficulty maintaining a healthy work-life balance: "; cin >> ans; totalScore += ans;
                    cout << "5. I feel pressure to excel in my major subjects: "; cin >> ans; totalScore += ans;
                }   
                else if (currentStudent.yearLevel == 4) {
                    cout << "\n[4th Year Focus: Thesis & Career Anxiety]\n";
                    cout << "1. I feel extreme pressure regarding my thesis/capstone: "; cin >> ans; totalScore += ans;
                    cout << "2. I experience anxiety when thinking about job hunting: "; cin >> ans; totalScore += ans;
                    cout << "3. I worry about graduating on time: "; cin >> ans; totalScore += ans;
                    cout << "4. I feel burnt out from years of academic requirements: "; cin >> ans; totalScore += ans;
                    cout << "5. I feel unprepared for life after graduation: "; cin >> ans; totalScore += ans;
                }
                else {
                    cout << "\n[General Academic Focus]\n";
                    cout << "1. I struggle with understanding my major subjects: "; cin >> ans; totalScore += ans;
                    cout << "2. I have difficulty maintaining effective study habits: "; cin >> ans; totalScore += ans;
                    cout << "3. Financial expenses are causing me stress: "; cin >> ans; totalScore += ans;
                    cout << "4. Health issues are affecting my academic performance: "; cin >> ans; totalScore += ans;
                    cout << "5. I feel disconnected from my professors and peers: "; cin >> ans; totalScore += ans;
                }

                // --- 2. CALCULATION ---
                int calculatedStress = (int)(((totalScore - 5) / 20.0 * 9) + 1);
                if (calculatedStress < 1) calculatedStress = 1;
                if (calculatedStress > 10) calculatedStress = 10;

                cout << "\nAssessment Complete! Your calculated Stress Level: " << calculatedStress << "/10\n";
                loadingTransition("Saving results to CSV database", 1);

                // --- 3. CSV SAVING METHOD ---
                ifstream inFile("students.csv");
                ofstream tempFile("temp.csv");

                if (inFile.is_open() && tempFile.is_open()) {
                    string line;
                    bool found = false;

                    // Convert current student ID to string
                    stringstream idStream;
                    idStream << currentStudent.studentID;
                    string targetID = idStream.str();

                    // Read the CSV line by line
                    while (getline(inFile, line)) {
                        stringstream ss(line);
                        string fileID, password, name, year, stress, counselor;

                        // Parse based on your exact format: ID, password, name, yearLevel, stressLevel, assignedCounselor
                        getline(ss, fileID, ',');
                        getline(ss, password, ',');
                        getline(ss, name, ',');
                        getline(ss, year, ',');
                        getline(ss, stress, ',');
                        getline(ss, counselor); // Read the rest of the line

                        // Check if this line belongs to the logged-in student
                        if (fileID == targetID) {
                            // Write the updated record (with calculatedStress) to the temp file
                            tempFile << fileID << ","
                                     << password << ","
                                     << name << ","
                                     << year << ","
                                     << calculatedStress << ","
                                     << counselor << "\n";
                            found = true;
                        } else {
                            // Write the original unmodified record to the temp file
                            tempFile << line << "\n";
                        }
                    }

                    inFile.close();
                    tempFile.close();

                    // Swap files
                    if (found) {
                        remove("students.csv");           // Delete original file
                        rename("temp.csv", "students.csv"); // Rename temp to original
                        
                        currentStudent.stressLevel = calculatedStress; // Update current session
                        cout << "[SUCCESS] Record updated in CSV Database.\n";
                    } else {
                        remove("temp.csv"); // Clean up temp file
                        cout << "[ERROR] Could not locate student record in the database.\n";
                    }
                } else {
                    cout << "[ERROR] Unable to open database files.\n";
                }

                cout << "\nWould you like to return to the Dashboard? (y/n): ";
                char choice;
                cin >> choice;
                if (choice == 'y' || choice == 'Y') stayInCase = false;

            } while (stayInCase);
            break;
        }

       case 3: 
{
    bool stayInCase = true;
    do {
        loadingTransition("\t\t\t\t\tLoading Emergency Booking Module", 1);
        cout << "=========================================================\n";
        cout << "           EMERGENCY CRISIS & WALK-IN DROP-IN            \n";
        cout << "=========================================================\n";
        cout << "If you are in distress, please fill out this form.\n";
        cout << "---------------------------------------------------------\n";

        // 1. CLEAR BUFFER: Very important before taking string input!
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // 2. GET USER INPUT: Ask for the reason
        char emergencyReason[255];
        cout << "Please state your emergency or reason for appointment:\n> ";
        cin.getline(emergencyReason, 255);

        // 3. GET DATE/TIME INPUT
        char preferredDate[50];
        cout << "Enter preferred date/time (or type 'ASAP' for emergencies):\n> ";
        cin.getline(preferredDate, 50);

        // 4. CONFIRMATION
        cout << "\nSubmit this request? (y/n): ";
        char confirm;
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            loadingTransition("Saving appointment request", 1);

            // 5. SAVE DIRECTLY TO appointment.csv
            // ios::app ensures we add to the bottom of the list, not overwrite the whole file
            ofstream outFile("appointments.csv", ios::app); 
            
            if (outFile.is_open()) {
                // Format: StudentID,StudentName,Reason,PreferredDate,PreferredTime,AssignedCounselor,Type
                outFile << currentStudent.studentID << ","
                        << "\"" << currentStudent.name << "\","
                        << emergencyReason << ","
                        << preferredDate << ","
                        << "ASAP" << "," // Emergency time
                        << currentStudent.assignedCounselor << ","
                        << "E" << "\n";
                outFile.close();
                
                cout << "\n=========================================================\n";
                cout << "[SUCCESS] Your appointment request has been saved.\n";
                cout << "Please wait for your assigned counselor to reach out.\n";
                cout << "=========================================================\n";
            } else {
                cout << "\n[ERROR] Could not open appointments.csv!\n";
            }
        } else {
            cout << "\n[NOTICE] Request cancelled.\n";
        }

        // 6. SAFE EXIT
        cout << "\nPress Enter to return to Dashboard...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (cin.peek() == '\n') cin.get();
        stayInCase = false;

    } while (stayInCase);
    break;
}

            
        case 4:
        {
            loadingTransition("\t\t\t\t\tLoading appointments", 1);
            ifstream inFile("appointments.csv");
            string line;
            bool foundAppt = false;

            cout << "=========================================================\n";
            cout << "                MY APPOINTMENTS & RECORDS                \n";
            cout << "=========================================================\n";
            cout << "[MY WELLNESS PROFILE]\n";
            cout << "Name: " << currentStudent.name << "\n";
            cout << "Year Level: " << currentStudent.yearLevel << "\n";

            if (currentStudent.stressLevel > 0) {
                cout << "Current Stress Level: " << currentStudent.stressLevel << "/10\n";
            } else {
                cout << "Current Stress Level: Not Assessed Yet (Please take Case 2)\n";
            }
            cout << "---------------------------------------------------------\n";
            cout << "[MY APPOINTMENT REQUESTS]\n";

            if (inFile.is_open()) {
                while (getline(inFile, line)) {
                    stringstream ss(line);
                    string idStr, nameStr, reasonStr, dateStr, counselorStr, typeStr;
                    getline(ss, idStr, ',');
                    getline(ss, nameStr, ',');
                    getline(ss, reasonStr, ',');
                    getline(ss, dateStr, ',');
                    getline(ss, counselorStr, ',');
                    getline(ss, typeStr, ',');

                    if (!idStr.empty() && stoll(idStr) == currentStudent.studentID) {
                        cout << "-> Preferred Date: " << dateStr << "\n";
                        cout << "   Reason: " << reasonStr << "\n";
                        cout << "   Counselor: " << counselorStr << "\n";
                        cout << "   Status: " << (typeStr == "E" ? "Emergency request" : "Booked session") << "\n\n";
                        foundAppt = true;
                    }
                }
                inFile.close();
            }

            if (!foundAppt) {
                cout << "You have no pending appointments or emergency requests.\n";
            }

            cout << "=========================================================\n";
            cout << "Press Enter to return to the Dashboard...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            break;
        }

        case 5:
        {
            loadingTransition("\t\t\t\t\tOpening counseling session scheduler", 1);
            cout << "==================================================" << endl;
            cout << "            BOOK COUNSELING SESSION               " << endl;
            cout << "==================================================" << endl;

            Appointment newAppt;
            strcpy(newAppt.studentID, to_string(currentStudent.studentID).c_str());
            strcpy(newAppt.studentName, currentStudent.name);
            strcpy(newAppt.assignedCounselor, currentStudent.assignedCounselor);  // Add this line




            cout << "Enter reason for session: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.getline(newAppt.reason, 200);


            cout << "Enter preferred date (MM/DD/YYYY): ";
            cin.getline(newAppt.preferredDate, 20);



            newAppt.type = 'B';


            ofstream outFile("appointments.csv", ios::app);
            outFile << newAppt.studentID << ",\"" << newAppt.studentName << "\"," << newAppt.reason << "," << newAppt.preferredDate << "," << newAppt.assignedCounselor << "," << newAppt.type << "\n";  // Updated with quoted name to handle commas
            outFile.close();




            cout << "\n[SYSTEM] Counseling session booked successfully!" << endl;
            this_thread::sleep_for(chrono::seconds(2));
            break;
        }
        case 6:
            loadingTransition("\t\t\t\t\tLogging out safely", 1);
            break;
        default:
            cout << "[ERROR] Invalid choice. Please try again." << endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
    } while (choice != 6); // Updated to exit on choice 6 lmao
}





void counselorDashboard(Counselor currentCounselor)
{
    int choice = 0;
    do
    {
        clearScreen();  
        cout << R"(                                                                                                                                                                       
                                                                                                                                                                        
                                                                                                                                                                        

         ████████  ██                                                                                      ████████                                                     
       █████   ██████                                                                                       ███████                                                     
      █████    █████                                                                                        ███████                                                     
     ██████    ██████     ████████     ██████   ███████   ███████  ██████      ████████████    ████████     ███████      ████████     ██████  ██████    ███████████     
     ██████      ██    ██████  █████    ██████  ███████    ██████ █ ██████    ██████   ███   █████   █████  ███████    █████  █████   ████████  █████ ███████   ███     
     ██████            ██████  ██████   ██████   ██████    ███████  ██████   █████████      ██████   █████  ███████   ██████  ██████   ███████  █████ █████████         
     ██████     █████ ███████  ███████  ██████   ██████    ███████  ██████     ███████████  ████████████    ███████  ███████  ██████   ██████  █████    ██████████      
      ██████    █████  ██████  ██████   ██████  ███████    ██████   ██████   ██   █████████ ██████      ██  ███████   ██████  ██████   ██████         ██   █████████    
       ██████   ████    █████  █████    ██████ █ ██████    ███████   ██████  ████   ███████  █████  █████    ██████    █████  █████    ██████         ████   ██████     
          ███████          ███████        █████    █████     █████    █████     ████████        ███████       █████      ████████       ██████           ████████       
                                                                                                                                                                        
    )" << endl;                                                                                                                                                              
                                                                                                                                                                        
                                                                                                                                                                       
        // Updated to show their real name instead of username
        cout << "Welcome back, " << currentCounselor.name << ".\n";
        cout << "Specialty: " << currentCounselor.specialty << "\n"
             << endl;



        cout << "\t\t\t\t\t 1. View Assigned Cases" << endl;
        cout << "\t\t\t\t\t 2. Log New Counseling Case " << endl;
        cout << "\t\t\t\t\t 3. Search Case History by ID" << endl;
        cout << "\t\t\t\t\t 4. View Emergency Walk-in " << endl;
        cout << "\t\t\t\t\t 5. Sort Students by Wellness Priority " << endl;
        cout << "\t\t\t\t\t 6. Logout" << endl;
        cout << "\t\t\t\t\t--------------------------------------------------" << endl;
        cout << "\t\t\t\t\tEnter choice: ";



        cin >> choice;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }



        switch (choice)
        {
        case 1:
        {
            loadingTransition("\t\t\t\t\tFetching assigned case files", 1);
            clearScreen();
            cout << "==================================================" << endl;
            cout << "               YOUR ASSIGNED CASES                " << endl;
            cout << "==================================================" << endl;




            ifstream studentFile("students.csv");            int caseCount = 0;
            string line;




            while (getline(studentFile, line))
            {
                if (line.empty()) continue;
               
                stringstream ss(line);
                string idStr, password, name, yearStr, stressStr, counselor;
                getline(ss, idStr, ',');
                getline(ss, password, ',');
                getline(ss, name, ',');
                getline(ss, yearStr, ',');
                getline(ss, stressStr, ',');
                getline(ss, counselor);



                // Trim all strings
                idStr = trimString(idStr);
                yearStr = trimString(yearStr);
                stressStr = trimString(stressStr);
                counselor = trimString(counselor);
               
                if (idStr.empty() || yearStr.empty() || stressStr.empty()) continue;


                try {
                    Student tempStudent;
                    tempStudent.studentID = stoll(idStr);
                    strcpy(tempStudent.password, password.c_str());
                    strcpy(tempStudent.name, name.c_str());
                    tempStudent.yearLevel = stoi(yearStr);
                    tempStudent.stressLevel = stoi(stressStr);
                    strcpy(tempStudent.assignedCounselor, counselor.c_str());


                    //  ONLY shows students explicitly assigned to THIS counselor
                    if (strcmp(tempStudent.assignedCounselor, currentCounselor.username) == 0)
                    {
                        caseCount++;
                        cout << " [" << caseCount << "] ID: " << tempStudent.studentID
                             << " | Name: " << tempStudent.name << " | Stress Level: " << tempStudent.stressLevel << endl;
                    }
                } catch (...) { continue; }
            }
            studentFile.close();



            // Now display BOOKED APPOINTMENTS for assigned students
            cout << "\n--------------------------------------------------" << endl;
            cout << "            BOOKED COUNSELING SESSIONS                " << endl;
            cout << "----------------------------------------------------" << endl;
           
            ifstream apptFile("appointments.csv");
            int apptCount = 0;
           
            if (apptFile) {
                string apptLine;
                while (getline(apptFile, apptLine)) {
                    if (apptLine.empty()) continue;
                   
                    // Parse appointments with quoted names
                    stringstream ss(apptLine);
                    string sid, sname, reason, date, counselor;
                   
                    getline(ss, sid, ',');
                   
                    // Handle quoted name
                    if (ss.peek() == '\"') {
                        ss.ignore(); // skip opening quote
                        getline(ss, sname, '\"');
                        ss.ignore(); // skip comma after closing quote
                    } else {
                        getline(ss, sname, ',');
                    }
                   
                    getline(ss, reason, ',');
                    getline(ss, date, ',');
                    getline(ss, counselor, ',');
                   
                    // Trim strings
                    sid = trimString(sid);
                    sname = trimString(sname);
                    reason = trimString(reason);
                    date = trimString(date);
                    counselor = trimString(counselor);
                   
                    if (sid.empty()) continue;
                   
                    // Only show if assigned to this counselor
                    if (counselor == string(currentCounselor.username)) {
                        apptCount++;
                        cout << " [" << (caseCount + apptCount) << "] BOOKING - Student: " << sid << " (" << sname << ")" << endl;
                        cout << "    Date: " << date << " | Reason: " << reason << endl;
                    }
                }
                apptFile.close();
            }

            if (caseCount == 0 && apptCount == 0)
            {
                cout << " > No cases or bookings currently assigned to you." << endl;
            }


            cout << "==================================================" << endl;
            cout << "\nPress Enter to return to the dashboard...";


            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            break;
        }



           case 2: {
    bool stayInCase = true;
    do {
        system("clear"); // Requirement: Clear Screen
        cout << "=========================================================\n";
        cout << "               Log New Counseling Case\n";
        cout << "=========================================================\n";
        
        string searchID;
        cout << "Enter Student Number (SN) or '0' to cancel: "; // Added 0 to cancel
        cin >> searchID;

        
        if (searchID == "0") {
            stayInCase = false;
            break; 
        }

        bool found = false;
        string fName, lName, counselor;
        string currentID, user, d1, d2, d3;

        // 1. FAST SCANNING (Sequential Search with break)
        ifstream studentFile("students.csv");
        if (studentFile.is_open()) {
            while (getline(studentFile, currentID, ',')) {
                getline(studentFile, user, ',');
                getline(studentFile, lName, ',');
                getline(studentFile, fName, ',');
                getline(studentFile, d1, ','); // Year
                getline(studentFile, d2, ','); // Stress
                getline(studentFile, counselor); // Counselor

                if (trimString(currentID) == trimString(searchID)) {
                    found = true;
                    break; 
                }
            }
            studentFile.close();
        }

        // 2. IMMEDIATE VERIFICATION
        if (!found) {
            cout << "\n[Error] Student number '" << searchID << "' not found.\n";
            cout << "Returning to input prompt...";
            this_thread::sleep_for(chrono::seconds(2));
            continue; // Requirement: return to "Enter Student Number" but not break
        }

        // 3. REFRESH & INPUT
        system("clear");
        cout << "=========================================================\n";
        cout << "               Log New Counseling Case\n";
        cout << "=========================================================\n";
        cout << "Student: " << fName << " " << lName << " (" << searchID << ")\n";
        cout << "---------------------------------------------------------\n";

        Case newCase;
        newCase.studentID = stoll(searchID);
       
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter Clinical Observations: ";
        cin.getline(newCase.clinicalObservations, sizeof(newCase.clinicalObservations));
       
        cout << "Enter Discussion Summary: ";
        cin.getline(newCase.discussionSummary, sizeof(newCase.discussionSummary));
       
        cout << "Enter Professional Assessment: ";
        cin.getline(newCase.professionalAssessment, sizeof(newCase.professionalAssessment));
       
        cout << "Enter Action Plan: ";
        cin.getline(newCase.actionPlan, sizeof(newCase.actionPlan));
       
        cout << "Enter Date (MM/DD/YY): ";
        cin.getline(newCase.date, sizeof(newCase.date));

        // 4. DATA SAVING TO HISTORY (case_history.csv)
        ofstream outFile("case_history.csv", ios::app);
        if (outFile.is_open()) {
            outFile << newCase.studentID << ","
                    << newCase.clinicalObservations << ","
                    << newCase.discussionSummary << ","
                    << newCase.professionalAssessment << ","
                    << newCase.actionPlan << ","
                    << newCase.date << "\n";
            outFile.close();
            cout << "\n[Success] Counseling history saved.\n";
        }

        // --- PROFESSIONAL OVERRIDE (STRESS LEVEL UPDATE) ---
        int professionalStressLevel;
        cout << "\n--------------------------------------------------" << endl;
        cout << "[STRESS LEVEL RE-EVALUATION]" << endl;
        cout << ">> Enter new Professional Stress Level (1-10): ";
        cin >> professionalStressLevel;
        cin.ignore(1000, '\n');

        vector<Student> students; 
        ifstream inFile("students.csv");
        string csvline;
        bool studentUpdated = false;

        while (getline(inFile, csvline)) {
            if (csvline.empty()) continue;
           
            stringstream ss(csvline);
            string idStr, password, name, yearStr, stressStr, assignedCounselor;

            getline(ss, idStr, ',');
            getline(ss, password, ',');
            getline(ss, name, ',');
            getline(ss, yearStr, ',');
            getline(ss, stressStr, ',');
            getline(ss, assignedCounselor);

            idStr = trimString(idStr);
           
            try {
                Student tempStud;
                tempStud.studentID = stoll(idStr);
                strcpy(tempStud.password, password.c_str());
                strcpy(tempStud.name, name.c_str());
                tempStud.yearLevel = stoi(yearStr);
                tempStud.stressLevel = stoi(stressStr);
                strcpy(tempStud.assignedCounselor, assignedCounselor.c_str());

                if (tempStud.studentID == newCase.studentID) {
                    tempStud.stressLevel = professionalStressLevel;
                    studentUpdated = true;
                }
                students.push_back(tempStud);
            } catch (...) { continue; }
        }
        inFile.close();

        if (studentUpdated) {
            ofstream outFile2("students.csv");
            for (const auto& stud : students) {
                outFile2 << stud.studentID << ","
                         << stud.password << ","
                         << stud.name << ","
                         << stud.yearLevel << ","
                         << stud.stressLevel << ","
                         << stud.assignedCounselor << "\n";
            }
            outFile2.close();
            cout << "[SYSTEM] stressLevel updated to " << professionalStressLevel << " in records." << endl;
        }

        cout << "\nCase logged. Log another student? (y/n): ";
        char choice;
        cin >> choice;
        if (choice == 'n' || choice == 'N') stayInCase = false;

    } while (stayInCase);

    break;
}


 case 3: {
    bool stayInSearch = true;
    do {
        system("clear"); // Clean look for every new attempt
        cout << "=========================================================\n";
        cout << "              Search Student Case History\n";
        cout << "=========================================================\n";
        
        string searchID;
        cout << "Enter Student Number (SN) or '0' to cancel: ";
        cin >> searchID;

        // [Requirement] If 0, exit to dashboard
        if (searchID == "0") {
            stayInSearch = false;
            break; 
        }

        bool found = false;
        ifstream caseFile("case_history.csv");
        string line;

        if (!caseFile.is_open()) {
            cout << "\n[ERROR] Could not open cases database.\n";
            this_thread::sleep_for(chrono::seconds(3));
            stayInSearch = false; // Stop the loop if the file is missing
            break;
        }

        system("clear");
        cout << "=========================================================\n";
        cout << "            RECORDS FOR STUDENT: " << searchID << "\n";
        cout << "=========================================================\n";

        // Search the CSV for matching entries
        while (getline(caseFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string sid, obs, summary, assessment, plan, date;

            getline(ss, sid, ',');
            getline(ss, obs, ',');
            getline(ss, summary, ',');
            getline(ss, assessment, ',');
            getline(ss, plan, ',');
            getline(ss, date);

            if (sid == searchID) {
                found = true;
                cout << "DATE: " << date << "\n";
                cout << "OBSERVATIONS: " << obs << "\n";
                cout << "SUMMARY: " << summary << "\n";
                cout << "ASSESSMENT: " << assessment << "\n";
                cout << "ACTION PLAN: " << plan << "\n";
                cout << "---------------------------------------------------------\n";
            }
        }
        caseFile.close();

        // [Requirement] If not exist, return to SN entry instead of dashboard
        if (!found) {
            cout << "\n[ERROR] No case history found for Student ID: " << searchID << endl;
            cout << "Please check the number and try again." << endl;
            this_thread::sleep_for(chrono::seconds(3));
            // stayInSearch is still true, so it loops back to the start
        } else {
            cout << "\nEnd of records. Press any key to search another or '0' to exit...";
            char choice;
            cin >> choice;
            if (choice == '0') stayInSearch = false;
        }

    } while (stayInSearch);

    break; // Final break to exit case 3 switch back to Counselor Dashboard
}


               case 4:
        {
            bool foundAny = false;




            loadingTransition("\t\t\t\t\tSyncing with appointments.csv", 1);
            clearScreen();
            cout << "==================================================" << endl;
            cout << "              ACTIVE SESSION QUEUE                " << endl;
            cout << "==================================================" << endl;




            ifstream inFile("appointments.csv");
            if (!inFile)
            {
                cout << "[System] No appointments or emergencies found." << endl;
                cout << "\nPress Enter to return to Dashboard...";
                cin.ignore(1000, '\n');
                cin.get();
                break;
            }




            vector<Appointment> appointments;
            string line;
            while (getline(inFile, line))
            {
                if (line.empty()) continue;


                Appointment tempAppt;
                stringstream ss(line);
                string idStr, name, reason, date, time, counselor, typeStr;


                getline(ss, idStr, ',');
                getline(ss, name, ',');
                getline(ss, reason, ',');
                getline(ss, date, ',');
                getline(ss, time, ',');
                getline(ss, counselor, ',');
                getline(ss, typeStr);


                strcpy(tempAppt.studentID, idStr.c_str());
                strcpy(tempAppt.studentName, name.c_str());
                strcpy(tempAppt.reason, reason.c_str());
                strcpy(tempAppt.preferredDate, date.c_str());
                strcpy(tempAppt.preferredTime, time.c_str());
                strcpy(tempAppt.assignedCounselor, counselor.c_str());
                tempAppt.type = typeStr[0];


                appointments.push_back(tempAppt);
            }
            inFile.close();




            loadingTransition("\t\t\t\t\tSyncing with appointments.csv", 1);
            clearScreen();
            cout << "==================================================" << endl;
            cout << "             PASS 1: EMERGENCY QUEUE              " << endl;
            cout << "==================================================" << endl;




            cout << "\n[ !!! EMERGENCY WALK-IN QUEUE !!! ]" << endl;
            bool hasEmergency = false;
            for (const auto& appt : appointments)
            {
                if (appt.type == 'E' || appt.type == 'e')
                {
                    if (strcmp(appt.assignedCounselor, currentCounselor.username) == 0)
                    {
                        foundAny = true;
                        hasEmergency = true;
                        // Displaying Date, Time, and Reason
                        cout << "DATE & TIME : " << appt.preferredDate << " | " << appt.preferredTime << endl;
                        cout << "STUDENT     : " << appt.studentID << " | " << appt.studentName << endl;
                        cout << "REASON      : " << appt.reason << endl;
                        cout << "--------------------------------------------------" << endl;
                    }
                }
            }
            if (!hasEmergency) cout << " > No pending emergency cases.\n";




           loadingTransition("\t\t\t\t\tSyncing with appointments.csv", 1);
            clearScreen();
            cout << "==================================================" << endl;
            cout << "            PASS 2: NORMAL BOOKINGS QUEUE        " << endl;
            cout << "==================================================" << endl;
            cout << "\n[ NORMAL / STANDARD BOOKINGS ]" << endl;
            bool hasNormal = false;
            for (const auto& appt : appointments)
            {
                if (appt.type == 'B' || appt.type == 'b' || appt.type == 'N' || appt.type == 'n')
                {
                    if (strcmp(appt.assignedCounselor, currentCounselor.username) == 0)
                    {
                        foundAny = true;
                        hasNormal = true;
                        // Displaying Date, Time, and Reason
                        cout << "DATE & TIME : " << appt.preferredDate << " | " << appt.preferredTime << endl;
                        cout << "STUDENT     : " << appt.studentID << " | " << appt.studentName << endl;
                        cout << "REASON      : " << appt.reason << endl;
                        cout << "--------------------------------------------------" << endl;
                    }
                }
            }
            if (!hasNormal) cout << " > No pending standard bookings.\n";




            cout << "\nPress Enter to return to Dashboard...";
            cin.ignore(1000, '\n');
            cin.get();
            break;
        }


case 5: {
    system("clear");
    cout << "==============================================================\n";
    cout << "Rank | ID            | Name                      | Stress Level\n";
    cout << "--------------------------------------------------------------\n";


    vector<Student> students; // Temporary vector to hold all students for sorting
    ifstream inFile("students.csv");
    string csvline;


    if (!inFile.is_open()) {
        cout << "[Error] Could not open students.csv\n";
    } else {
     
        while (getline(inFile, csvline)) {
            if (trimString(csvline).empty()) continue; // Skip ghost lines
           
            stringstream ss(csvline);
            string idStr, user, last, first, yearStr, stressStr, counselor;


            getline(ss, idStr, ',');
            getline(ss, user, ',');
            getline(ss, last, ',');
            getline(ss, first, ',');
            getline(ss, yearStr, ',');
            getline(ss, stressStr, ',');
            getline(ss, counselor);


            idStr = trimString(idStr);
            yearStr = trimString(yearStr);
            stressStr = trimString(stressStr);


            // Validation check bago ang stoi/stoll
            if (!idStr.empty() && !yearStr.empty() && !stressStr.empty()) {
                try {
                    Student temp;
                    temp.studentID = stoll(idStr);
                    temp.yearLevel = stoi(yearStr);
                    temp.stressLevel = stoi(stressStr);
                   
                    strncpy(temp.password, user.c_str(), sizeof(temp.password));
                    string fullName = last + ", " + first;
                    strncpy(temp.name, fullName.c_str(), sizeof(temp.name));
                    strncpy(temp.assignedCounselor, counselor.c_str(), sizeof(temp.assignedCounselor));


                    students.push_back(temp);
                } catch (...) {
                    // Ito ang sasalo sa header row ("ID,Name...") para hindi mag-crash
                    continue;
                }
            }
        }
        inFile.close();
    }




    bool foundAny = false;
    for (size_t i = 0; i < students.size(); i++) {
        if (students[i].stressLevel > 1) {
            foundAny = true;
            cout << left << setw(5) << i + 1
                 << "| " << setw(14) << students[i].studentID
                 << "| " << setw(26) << students[i].name
                 << "| " << students[i].stressLevel << "\n";
        }
    }


    if (!foundAny) {
        cout << "\n > No priority cases found. (All levels are 1 or 0).\n";
    }


    cout << "\nPress Enter to return to Dashboard...";
    cin.ignore(1000, '\n'); cin.get();
    break;
}
        case 6: // Logout
            cout << "Logging out... Returning to Main Menu." << endl;
            return;








        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    } while (choice != 6);
}

void adminDashboard(Admin currentAdmin)
{
    int choice = 0;
    do
    {
        loadingTransition("\t\t\t\t\tLoading administrator dashboard", 1);
        cout << R"(                                                       
                                ███████    ██████████    ██████    █████   ██████  ██████    ███        
                               ██████      ████   ████   █████     █████    █████  ██████    ██        
                              ██ ████      ████   █████  ██████    █████    █████  ███████   ██        
                              █  █████     ████    ████  ██ █████ ██ ████   █████  █  ████   ██        
                             ██   █████    ████    ████  ██ ███████  ████   █████  ██  ████  ██        
                             ██████████    ████   █████  ██  █████   ████   █████  ██   ████ ██        
                             ██     █████  █████  ████   ██   ████   ████   █████  ██    ██████        
                            ███     ██████ ████████      ██         █████  ██████  ██       ███ 
                                                    
                                               DASHBOARD - ADMINISTRATOR
                             ----------------------------------------------------------------- )" << endl;
        cout << "\t\t\t\t\t System Administrator: " << currentAdmin.name << ".\n" << endl;

        cout << "\t\t\t\t\t 1. Add New Student" << endl;
        cout << "\t\t\t\t\t 2. Add New Counselor Staff" << endl;
        cout << "\t\t\t\t\t 3. View Active Counselor Staff" << endl;
        cout << "\t\t\t\t\t 4. Access Student-Counselor Matcher" << endl;
        cout << "\t\t\t\t\t 5. Logout" << endl;
        cout << "\t\t\t\t\t--------------------------------------------------" << endl;
        cout << "\t\t\t\t\t Enter choice: ";

        cin >> choice;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }

        switch (choice)
        {
        case 1:
            loadingTransition("Opening student registration form", 1);
            adminAddStudent();
            break;
        case 2:
            loadingTransition("Opening counselor registration form", 1);
            adminAddCounselor();
            break;
        case 3:
            loadingTransition("Retrieving counselor staff list", 1);
            adminViewCounselors();
            break;
        case 4:
            loadingTransition("Accesing Student-Counselor Matcher", 1);
            adminMatcher();
            break;
        case 5:
            loadingTransition("Logging out securely", 1);
            return; // Exit the dashboard immediately on logout
        default:
            cout << "[ERROR] Invalid choice. Please try again." << endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
    } while (choice != 4);
}

// LOGIN SYSTEMS
bool adminLogin(Admin &loggedInAdmin)
{
    char inputUser[20]; char inputPass[20]; bool loginSuccess = false;








    while (!loginSuccess) {
        loadingTransition("Accessing administrator database", 1);
        cout << R"(                                                       
                                ███████    ██████████    ██████    █████   ██████  ██████    ███        
                               ██████      ████   ████   █████     █████    █████  ██████    ██        
                              ██ ████      ████   █████  ██████    █████    █████  ███████   ██        
                              █  █████     ████    ████  ██ █████ ██ ████   █████  █  ████   ██        
                             ██   █████    ████    ████  ██ ███████  ████   █████  ██  ████  ██        
                             ██████████    ████   █████  ██  █████   ████   █████  ██   ████ ██        
                             ██     █████  █████  ████   ██   ████   ████   █████  ██    ██████        
                            ███     ██████ ████████      ██         █████  ██████  ██       ███ 
                                                    
                                               DASHBOARD - ADMINISTRATOR
                             ----------------------------------------------------------------- )" << endl;
        cout << "\t\t\t\t\tUsername (or type 'exit' to cancel): "; cin >> inputUser;
        if (strcmp(inputUser, "exit") == 0) return false;



        cout << "\t\t\t\t\tPassword: "; cin >> inputPass;

        // Look in CSV!
        ifstream adminFile("admins.csv");
        string line;




        while (getline(adminFile, line)) {
            if (line.empty()) continue;
           
            stringstream ss(line);
            string user, pass, name;
            getline(ss, user, ','); getline(ss, pass, ','); getline(ss, name, ',');



            // Cleanup invisible formatting (taena nasisira nung una pag inopen mo yung csv file)
            user = trimString(user);
            pass = trimString(pass);
            name = trimString(name);
           
            if (user.empty() || pass.empty()) continue;



            if (user == string(inputUser) && pass == string(inputPass)) {
                loginSuccess = true;
                strcpy(loggedInAdmin.username, user.c_str());
                strcpy(loggedInAdmin.password, pass.c_str());
                strcpy(loggedInAdmin.name, name.c_str());
                break;
            }
        }
        adminFile.close();



        if (!loginSuccess) {
            cout << "\t\t\t\t\t[ERROR] Invalid credentials. Please try again.\n";
            this_thread::sleep_for(chrono::seconds(2)); clearScreen();
        }
    }
    return loginSuccess;
}



bool counselorLogin(Counselor &loggedInCounselor)
{
    loadingTransition("Accessing counselor database", 1);
    char inputUser[20]; char inputPass[20]; bool loginSuccess = false;

    while (!loginSuccess) {
         cout << R"(                                                                                                                                                                       
                                                                                                                                                                        
                                                                                                                                                                        

         ████████  ██                                                                                      ████████                                                     
       █████   ██████                                                                                       ███████                                                     
      █████    █████                                                                                        ███████                                                     
     ██████    ██████     ████████     ██████   ███████   ███████  ██████      ████████████    ████████     ███████      ████████     ██████  ██████    ███████████     
     ██████      ██    ██████  █████    ██████  ███████    ██████ █ ██████    ██████   ███   █████   █████  ███████    █████  █████   ████████  █████ ███████   ███     
     ██████            ██████  ██████   ██████   ██████    ███████  ██████   █████████      ██████   █████  ███████   ██████  ██████   ███████  █████ █████████         
     ██████     █████ ███████  ███████  ██████   ██████    ███████  ██████     ███████████  ████████████    ███████  ███████  ██████   ██████  █████    ██████████      
      ██████    █████  ██████  ██████   ██████  ███████    ██████   ██████   ██   █████████ ██████      ██  ███████   ██████  ██████   ██████         ██   █████████    
       ██████   ████    █████  █████    ██████ █ ██████    ███████   ██████  ████   ███████  █████  █████    ██████    █████  █████    ██████         ████   ██████     
          ███████          ███████        █████    █████     █████    █████     ████████        ███████       █████      ████████       ██████           ████████       
                                                                                                                                                                        
    )" << endl;             
        cout << "\t\t\t\t\tUsername (or type 'exit' to cancel): "; cin >> inputUser;
        if (strcmp(inputUser, "exit") == 0) return false;

        cout << "\t\t\t\t\tPassword: "; cin >> inputPass;

        ifstream counselorFile("counselors.csv");
        string line;

        while (getline(counselorFile, line)) {
            if (line.empty()) continue;
           
            stringstream ss(line);
            string user, pass, name, spec;
            getline(ss, user, ','); getline(ss, pass, ','); getline(ss, name, ','); getline(ss, spec, ',');

            // Cleanup invisible formatting
            user = trimString(user);
            pass = trimString(pass);
            name = trimString(name);
            spec = trimString(spec);
           
            if (user.empty() || pass.empty()) continue;

            if (user == string(inputUser) && pass == string(inputPass)) {
                loginSuccess = true;
                strcpy(loggedInCounselor.username, user.c_str());
                strcpy(loggedInCounselor.password, pass.c_str());
                strcpy(loggedInCounselor.name, name.c_str());
                strcpy(loggedInCounselor.specialty, spec.c_str());
                break;
            }
        }
        counselorFile.close();



        if (!loginSuccess) {
            cout << "\t\t\t\t\t[ERROR] Invalid credentials. Please try again.\n";
            this_thread::sleep_for(chrono::seconds(2)); clearScreen();
        }
    }
    return loginSuccess;
}







void studentLogin()
{
    long long id; char inputPass[20]; bool loginSuccess = false;


    while (!loginSuccess) {
        cout << R"( 
                                                                                                                                                               
                                                                                 ██                                                                        
              █████████████████     ███   █                                      █████████                                                  ███   █         
             ███████      ██████    ████████                                       ████████                                                 ███████         
             ████████      ███        ██████                                       ████████                                                  █████         
              ███████████             ███████      ████          ███         ███   ████████          ████         ███          ███          ██████         
               ██████████████      ████████████  █████████   █████████     ██████████████████    ██████  ██████     █████████████████     ████████████      
                ███████████████      ████████    ████████    ███████    ████████   ████████    ███████    ██████   ██████████  ███████      ███████         
                   ██████████████    ████████     ███████    ███████    ████████   ████████    ███████   ███████   █████████   ███████      ███████         
                      ████████████   ████████     ███████    ████████   ████████   ████████   █████████████████    ████████    ███████      ███████         
               ███       █████████   ████████     ███████    ████████   ████████   ████████    ███████         █   ████████    ███████      ███████         
              █████       ███████    ████████     ███████    ██████████  ████████  ████████   ████████  ███████   ████████    ███████       ███████         
             ████████████████       █████████     ██████████████████      █████████████████    ███████ ██████     █████████  █████████    ██████████      
                                                                                                    ████                                                                                                                                                                                         
            )" << endl;
        cout << "\t\t\t\t\t" << "Enter your Student ID (or 0 to cancel): ";
        cin >> id;
        if (cin.fail()) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n[ERROR] Invalid input. Please enter numbers only.\n";
            this_thread::sleep_for(chrono::seconds(2)); clearScreen(); continue;
        }
        if (id == 0) return;


        cout << "\t\t\t\t\t" << "Password: "; cin >> inputPass;
       
        ifstream studentFile("students.csv");
        vector<Student> students; string line;

        while (getline(studentFile, line)) {
            if (line.empty()) continue;
           
            // Parse from the end backwards because name contains commas
            // Format: ID,password,name,yearLevel,stressLevel,counselor
            vector<string> fields;
            size_t pos = 0;
           
            // Get counselor (last field)
            size_t lastComma = line.rfind(',');
            if (lastComma == string::npos) continue;
            string couns = line.substr(lastComma + 1);
           
            // Get stressLevel (second to last)
            size_t secondLastComma = line.rfind(',', lastComma - 1);
            if (secondLastComma == string::npos) continue;
            string stressStr = line.substr(secondLastComma + 1, lastComma - secondLastComma - 1);
           
            // Get yearLevel (third to last)
            size_t thirdLastComma = line.rfind(',', secondLastComma - 1);
            if (thirdLastComma == string::npos) continue;
            string yrStr = line.substr(thirdLastComma + 1, secondLastComma - thirdLastComma - 1);
           
            // Get password and ID (first two fields, separated by comma)
            stringstream ss(line.substr(0, thirdLastComma));
            string idStr, pass;
            getline(ss, idStr, ',');
            getline(ss, pass, ',');
           
            // Everything after password and before year is the name
            string name = line.substr(idStr.length() + pass.length() + 2, thirdLastComma - idStr.length() - pass.length() - 2);

            // Cleanup invisible formatting
            idStr = trimString(idStr);
            pass = trimString(pass);
            name = trimString(name);
            yrStr = trimString(yrStr);
            stressStr = trimString(stressStr);
            couns = trimString(couns);
           
            if (idStr.empty() || pass.empty()) continue;

            try {
                Student tempStudent;
                tempStudent.studentID = stoll(idStr);
                strcpy(tempStudent.password, pass.c_str());
                strcpy(tempStudent.name, name.c_str());
                tempStudent.yearLevel = stoi(yrStr);
                tempStudent.stressLevel = stoi(stressStr);
                strcpy(tempStudent.assignedCounselor, couns.c_str());
                students.push_back(tempStudent);
            } catch (...) { continue; }
        }
        studentFile.close();

        if (!students.empty()) quickSortByID(students, 0, students.size() - 1);
        int index = -1;
        if (!students.empty() && id != -1) index = binarySearch(students, id);

        if (index != -1 && string(students[index].password) == string(inputPass)) {
            loginSuccess = true;
            loadingTransition("Verifying credentials", 1);
            studentDashboard(students[index]);
        } else {
            cout << "\n[ERROR] Invalid credentials. Student ID or password incorrect." << endl;
            cout << "Please try again..." << endl;
            this_thread::sleep_for(chrono::seconds(2)); clearScreen();
        }
    }
}

void loginMenu()
{
    int choice = 0;
    do
    {
        clearScreen();
        
        cout << R"(      
           
          
                          =================================================================================================================================|                                                                                                                                                                                                                                                              
                          |                                                                 ██     ██                                                       |    
                          |     ████████     ████████             █████████               ██████████████            █████████          █████ ██████ █████   |      
                          |    █████████   █████████            ██████████              ██  ██████████             █████████           ████  ██████  ████   |     
                          |    █  ██████  ██  ██████            █   ██████             ██       ████              ██  ███████          ███   ██████   ███   |     
                          |    █   ██████ ██  ██████            █   ███████            ██                         ██   ██████          ██    ██████    ██   |     
                          |    ██  ████████   ██████           ██    ██████           ███   ███ ██████            ██   ██████                ██████         |     
                          |    ██   ███████   ██████           ███    ██████          ████  ████████ █           ███    ██████               ██████         |     
                          |    ███   █████    ██████           ████ █ ██████          █████  █████   █           ████ █ ██████               ██████         |     
                          |    ████  █████    ██████          █████   ███████          █████         █          ██████  ███████              ██████         |     
                          |    █████  ███     ██████   ██     ██████   ██████    ███   ████████    ███   ███    ██████   ██████    ███       ██████         |     
                          |    ██████  ██     ██████ ██████  ████████  ███████  ██████  █████████████  ██████  ████████  ███████  ██████     ██████         |    
                          |    ███████         ██████ ██████ █████████  ████████ █████     ██████████   ██████ █████████  ████████ ██████      ██████       |
                          |                                                                                                                                 |
                          |                                     Mental-health Assessment & Guidance Academic Tracking                                       |
                          |=================================================================================================================================|
                                                                | Created by Rosario | Unciano | Pasion | Roque |
             )" 
             << endl;
        cout << " " << endl;
        cout << "\t\t\t\t\t\t\t\t 1. Admin Login" << endl;
        cout << "\t\t\t\t\t\t\t\t 2. Counselor Login" << endl;
        cout << "\t\t\t\t\t\t\t\t 3. Student Login (Walk-in / Dashboard)" << endl;
        cout << "\t\t\t\t\t\t\t\t 4. Exit" << endl;
        cout << "\t\t\t\t\t\t\t\t--------------------------------------------------" << endl;
        cout << "\t\t\t\t\t\t\t\tEnter choice: ";

        cin >> choice;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }

        switch (choice)
        {
        case 1:
        {
            Admin currentAdmin;
            if (adminLogin(currentAdmin))
            {
                loadingTransition("\t\t\t\t\tAuthenticating Admin", 1);
                adminDashboard(currentAdmin);
            }
            break;
        }
        case 2:
        {
            Counselor currentCounselor;
            if (counselorLogin(currentCounselor))
            {
                loadingTransition("\t\t\t\t\tAuthenticating Counselor", 1);
                counselorDashboard(currentCounselor);
            }
            break;
        }
        case 3:
            clearScreen();
            studentLogin();
            break;
        case 4:
            loadingTransition("\t\t\t\t\tShutting down M.A.G.A.T Wellness System. Stay safe", 2);
            break;
        default:
            cout << "[ERROR] Invalid choice. Please try again." << endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
    } while (choice != 4);
}

// MAIN FUNCTION
int main()
{
    initializeFiles();
    loginMenu();
    return 0;
}



