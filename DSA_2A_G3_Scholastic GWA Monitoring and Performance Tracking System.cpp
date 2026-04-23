/* =======================================================================================================
 * SECTOR 1: HEADERS, DEFINITIONS & BASIC UTILITIES
 * =======================================================================================================
 * This sector contains all necessary includes, color definitions, enums, structs, and basic utility
 * functions that form the foundation of the student management system.
 * ======================================================================================================= */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <map>
#include <unordered_set>
#include <fstream>
#include <sstream>

// ANSI Color codes
#define COLOR_RESET     "\033[0m"
#define COLOR_BOLD      "\033[1m"
#define COLOR_CYAN      "\033[36m"
#define COLOR_GREEN     "\033[32m"
#define COLOR_YELLOW    "\033[33m"
#define COLOR_BLUE      "\033[34m"
#define COLOR_MAGENTA   "\033[35m"
#define COLOR_RED       "\033[31m"

// Enum for year levels
enum YearLevel { FIRST = 1, SECOND = 2, THIRD = 3, FOURTH = 4 };

// Struct to represent a student
struct Student
{
    std::string name, studentNumber, email;
    YearLevel yearLevel;
    std::vector<double> grades;
    double gwa;
    std::string status;
    int rank;
};

// BST Node
struct BSTNode {
    Student data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(Student s) : data(s), left(nullptr), right(nullptr) {}
};

// Binary Search Tree class
class BST {
private:
    BSTNode* root;
    bool compareByGWA;

    void insertHelper(BSTNode*& node, Student student) {
        if (node == nullptr) { node = new BSTNode(student); return; }
        if (compareByGWA) {
            if (student.gwa < node->data.gwa) insertHelper(node->left, student);
            else insertHelper(node->right, student);
        } else {
            if (student.name < node->data.name) insertHelper(node->left, student);
            else insertHelper(node->right, student);
        }
    }

    void inOrderTraversal(BSTNode* node, std::vector<Student>& result) {
        if (node == nullptr) return;
        inOrderTraversal(node->left, result);
        result.push_back(node->data);
        inOrderTraversal(node->right, result);
    }

    void destroyTree(BSTNode* node) {
        if (node == nullptr) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
public:
    BST(bool byGWA = true) : root(nullptr), compareByGWA(byGWA) {}
    ~BST() { destroyTree(root); }
    void insert(Student student) { insertHelper(root, student); }
    std::vector<Student> getSortedList() {
        std::vector<Student> result;
        inOrderTraversal(root, result);
        return result;
    }
};

int getNumSubjects(YearLevel year) {
    switch (year) { case FIRST: return 6; case SECOND: return 9; 
    case THIRD: return 10; case FOURTH: return 11; default: return 0; }
}

std::string getYearString(YearLevel year) {
    switch (year) { case FIRST: return "1st Year"; case SECOND: return "2nd Year";
    case THIRD: return "3rd Year"; case FOURTH: return "4th Year"; default: return ""; }
}

std::vector<std::string> getSubjectsByYear(YearLevel year) {
    switch (year) {
        case FIRST:
            return {
                "Chemistry Lecture",
                "Chemistry Laboratory", 
                "Computer Engineering as a Discipline",
                "Programming Logic and Design",
                "Calculus 1",
                "Understanding The Self"
            };
        case SECOND:
            return {
                "Logic Circuits and Design Lecture",
                "Logic Circuits and Design Laboratory",
                "Object Oriented Programming",
                "Differential Equations",
                "Engineering Data Analysis",
                "Fundamentals of Electronic Circuits Lecture",
                "Fundamentals of Electronic Circuits Laboratory",
                "Science, Technology, and Society Personality Development"
            };
        case THIRD:
            return {
                "Fundamentals of Mixed Signals and Sensors",
                "Embedded Systems Lecture",
                "Embedded Systems Laboratory",
                "Computer Networks and Security Lecture",
                "Computer Networks and Security Laboratory",
                "Software Design Lecture",
                "Software Design Laboratory",
                "Numerical Methods",
                "Purposive Communication",
                "GE Elective 1"
            };
        case FOURTH:
            return {
                "Introduction to HDL",
                "Computer Engineering Drafting and Design",
                "Digital Signal Processing Lecture",
                "Digital Signal Processing Laboratory",
                "CpE Practice and Design 1",
                "CpE Track Course 2",
                "CpE Track Course 2 Laboratory",
                "Art Appreciation",
                "The Contemporary World",
                "Ethics",
                "GE Elective 3"
            };
        default:
            return {};
    }
}

void computeGWA(Student &student) {
    double sum = 0.0;
    for (double grade : student.grades) sum += grade;
    student.gwa = sum / student.grades.size();
    student.status = (student.gwa < 4.0) ? "Passed" : "Failed";
}

Student createStudent(std::string name, std::string studentNumber, std::string email,
                     YearLevel year, std::vector<double> grades) {
    Student student;
    student.name = name;
    student.studentNumber = studentNumber;
    student.email = email;
    student.yearLevel = year;
    student.grades = grades;
    computeGWA(student);
    return student;
}

// File I/O functions for persistence
/* =======================================================================================================
 * SECTOR 2: FILE I/O & DATA PERSISTENCE
 * =======================================================================================================
 * This sector handles data persistence through file operations, allowing student data to be saved
 * to and loaded from external files for long-term storage and program continuity.
 * ======================================================================================================= */

void saveStudentsToFile(const std::vector<Student>& allStudents, const std::string& filename = "students.dat") {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for saving.\n";
        return;
    }
    
    for (const auto& student : allStudents) {
        file << student.name << "|"
             << student.studentNumber << "|"
             << student.email << "|"
             << static_cast<int>(student.yearLevel) << "|";
        
        for (size_t i = 0; i < student.grades.size(); ++i) {
            file << student.grades[i];
            if (i < student.grades.size() - 1) file << ",";
        }
        file << "\n";
    }
    file.close();
}

void loadStudentsFromFile(std::vector<Student>& allStudents, std::map<YearLevel, std::vector<Student>>& studentsByYear, const std::string& filename = "students.dat") {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return; // File doesn't exist yet, that's okay
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        std::string name, studentNumber, email, yearStr, gradesStr;
        
        // Parse pipe-delimited values
        std::getline(iss, name, '|');
        std::getline(iss, studentNumber, '|');
        std::getline(iss, email, '|');
        std::getline(iss, yearStr, '|');
        std::getline(iss, gradesStr, '|');
        
        int year;
        try {
            year = std::stoi(yearStr);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing year: " << yearStr << std::endl;
            continue;
        }
        YearLevel yearLevel = static_cast<YearLevel>(year);
        
        // Parse comma-delimited grades
        std::vector<double> grades;
        std::istringstream gradeStream(gradesStr);
        std::string gradeStr;
        while (std::getline(gradeStream, gradeStr, ',')) {
            if (!gradeStr.empty()) {
                grades.push_back(std::stod(gradeStr));
            }
        }
        
        Student student = createStudent(name, studentNumber, email, yearLevel, grades);
        allStudents.push_back(student);
        studentsByYear[yearLevel].push_back(student);
    }
    file.close();
}

/* =======================================================================================================
 * SECTOR 3: ALGORITHMS & DATA STRUCTURES
 * =======================================================================================================
 * This sector implements core algorithms including sorting (Bubble Sort, Selection Sort), searching
 * (Linear Search, Binary Search), and comparison functions for organizing student data.
 * ======================================================================================================= */

void bubbleSort(std::vector<Student>& students, bool byGWA = true) {
    int n = students.size();
    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j < n - i - 1; ++j) {
            bool shouldSwap = byGWA ? (students[j].gwa > students[j + 1].gwa) :
                                      (students[j].name > students[j + 1].name);
            if (shouldSwap) std::swap(students[j], students[j + 1]);
        }
}

void selectionSort(std::vector<Student>& students, bool byGWA = true) {
    int n = students.size();
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            bool isSmaller = byGWA ? (students[j].gwa < students[minIndex].gwa) :
                                     (students[j].name < students[minIndex].name);
            if (isSmaller) minIndex = j;
        }
        if (minIndex != i) std::swap(students[i], students[minIndex]);
    }
}

int linearSearch(const std::vector<Student>& students, const std::string& target, bool searchByName = true) {
    if (searchByName) {
        // Case-insensitive and partial name matching
        std::string targetLower = target;
        std::transform(targetLower.begin(), targetLower.end(), targetLower.begin(), ::tolower);
        
        for (size_t i = 0; i < students.size(); ++i) {
            std::string nameLower = students[i].name;
            std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
            if (nameLower.find(targetLower) != std::string::npos) {
                return i;
            }
        }
    } else {
        // Exact match for student number
        for (size_t i = 0; i < students.size(); ++i) {
            if (students[i].studentNumber == target) {
                return i;
            }
        }
    }
    return -1;
}

int binarySearch(const std::vector<Student>& students, const std::string& targetStudentNumber) {
    int left = 0, right = students.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (students[mid].studentNumber == targetStudentNumber) return mid;
        else if (students[mid].studentNumber < targetStudentNumber) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

bool compareByGWA(const Student &a, const Student &b) { return a.gwa < b.gwa; }
bool compareByName(const Student &a, const Student &b) { return a.name < b.name; }
bool compareByStudentNumber(const Student &a, const Student &b) { return a.studentNumber < b.studentNumber; }

/* =======================================================================================================
 * SECTOR 4: CORE BUSINESS LOGIC & STUDENT MANAGEMENT
 * =======================================================================================================
 * This sector contains the main business logic functions for managing students including display
 * operations, CRUD operations (Create, Read, Update, Delete), and specialized features like
 * sorting demonstrations and pass/fail analysis.
 * ======================================================================================================= */

void displaySortedList(const std::vector<Student> &students, const std::string &sortType) {
    if (students.empty()) return;
    
    std::cout << "\n" << COLOR_CYAN << "═══════════════════════════════════════════════════════════════════════════════════════════════════════\n" << COLOR_RESET;
    std::cout << COLOR_GREEN << COLOR_BOLD << "  All Students - Sorted by " << sortType << "\n" << COLOR_RESET;
    std::cout << COLOR_CYAN << "═══════════════════════════════════════════════════════════════════════════════════════════════════════\n" << COLOR_RESET;
    std::cout << COLOR_BLUE << COLOR_BOLD;
    std::cout << std::left << std::setw(3) << "#" << " │ " << std::setw(30) << "Name" << " │ " 
              << std::setw(12) << "Student No" << " │ " << std::setw(10) << "Year" << " │ "
              << std::setw(6) << "GWA" << " │ " << std::setw(7) << "Status" << "\n" << COLOR_RESET;
    std::cout << COLOR_CYAN << "─────────────────────────────────────────────────────────────────────────────────────────────────────────────\n" << COLOR_RESET;
    
    int counter = 1;
    for (const auto &student : students) {
        std::cout << std::left << std::setw(3) << counter << " │ " << std::setw(30) << student.name << " │ "
                  << std::setw(12) << student.studentNumber << " │ " << std::setw(10) << getYearString(student.yearLevel) << " │ "
                  << std::fixed << std::setprecision(2) << std::setw(6) << student.gwa << " │ ";
        if (student.status == "Passed") {
            std::cout << COLOR_GREEN << std::left << std::setw(7) << "✓ PASS" << COLOR_RESET;
        } else {
            std::cout << COLOR_YELLOW << std::left << std::setw(7) << "✗ FAIL" << COLOR_RESET;
        }
        std::cout << "\n";
        counter++;
    }
    std::cout << COLOR_CYAN << "═══════════════════════════════════════════════════════════════════════════════════════════════════════\n" << COLOR_RESET;
}

void displayYearLevel(const std::vector<Student> &students, YearLevel year) {
    if (students.empty()) return;
    std::cout << "\n" << COLOR_CYAN << "═══════════════════════════════════════════════════════════════════════════════════════════════════════\n" << COLOR_RESET;
    std::cout << COLOR_GREEN << COLOR_BOLD << "  " << getYearString(year) << " Students - Ranked by GWA\n" << COLOR_RESET;
    std::cout << COLOR_CYAN << "═══════════════════════════════════════════════════════════════════════════════════════════════════════\n" << COLOR_RESET;
    std::cout << COLOR_BLUE << COLOR_BOLD;
    std::cout << std::left << std::setw(4) << "Rank" << " │ " << std::setw(30) << "Name" << " │ " 
              << std::setw(12) << "Student No" << " │ " << std::setw(20) << "Email" << " │ " 
              << std::setw(6) << "GWA" << " │ " << std::setw(7) << "Status" << "\n" << COLOR_RESET;
    std::cout << COLOR_CYAN << "─────────────────────────────────────────────────────────────────────────────────────────────────────────────\n" << COLOR_RESET;
    for (const auto &student : students) {
        std::cout << std::left << std::setw(4) << student.rank << " │ " << std::setw(30) << student.name << " │ "
                  << std::setw(12) << student.studentNumber << " │ " << std::setw(20) << student.email << " │ "
                  << std::fixed << std::setprecision(2) << std::setw(6) << student.gwa << " │ ";
        if (student.status == "Passed") {
            std::cout << COLOR_GREEN << std::left << std::setw(7) << "✓ PASS" << COLOR_RESET;
        } else {
            std::cout << COLOR_YELLOW << std::left << std::setw(7) << "✗ FAIL" << COLOR_RESET;
        }
        std::cout << "\n";
    }
    std::cout << COLOR_CYAN << "═══════════════════════════════════════════════════════════════════════════════════════════════════════\n" << COLOR_RESET;
}

YearLevel getYearLevelInput() {
    int year;
    while (true) {
        std::cout << COLOR_MAGENTA << "Enter year level (1-4) or 0 to go back: " << COLOR_RESET;
        std::cin >> year;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << COLOR_YELLOW << "Invalid input. Please enter a number.\n" << COLOR_RESET;
        } else if (year == 0) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return static_cast<YearLevel>(-1);
        } else if (year < 1 || year > 4) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << COLOR_YELLOW << "Invalid input. Please enter 1, 2, 3, or 4.\n" << COLOR_RESET;
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return static_cast<YearLevel>(year);
        }
    }
}

std::vector<double> getGradesInput(int numSubjects) {
    std::vector<double> grades;
    std::cout << COLOR_MAGENTA << "Enter " << numSubjects << " grades (separated by spaces): " << COLOR_RESET;
    for (int i = 0; i < numSubjects; ++i) {
        double grade;
        std::cin >> grade;
        grades.push_back(grade);
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return grades;
}

void addStudent(std::vector<Student>& allStudents, std::map<YearLevel, std::vector<Student>>& studentsByYear) {
    std::string name, studentNumber, email;
    std::cout << "\n" << COLOR_CYAN << COLOR_BOLD << "╔══════════════════════════════════════╗\n"
              << "║      ADD NEW STUDENT                 ║\n"
              << "╚══════════════════════════════════════╝\n" << COLOR_RESET;
    std::cout << COLOR_GREEN << "Enter student name (or type 'back' to cancel): " << COLOR_RESET;
    std::getline(std::cin, name);
    if (name == "back" || name == "BACK") {
        std::cout << COLOR_YELLOW << "✗ Operation cancelled.\n" << COLOR_RESET;
        return;
    }

    std::cout << COLOR_GREEN << "Enter student number: " << COLOR_RESET;
    std::getline(std::cin, studentNumber);
    if (studentNumber == "back" || studentNumber == "BACK") {
        std::cout << COLOR_YELLOW << "✗ Operation cancelled.\n" << COLOR_RESET;
        return;
    }

    std::cout << COLOR_GREEN << "Enter email: " << COLOR_RESET;
    std::getline(std::cin, email);
    if (email == "back" || email == "BACK") {
        std::cout << COLOR_YELLOW << "✗ Operation cancelled.\n" << COLOR_RESET;
        return;
    }

    YearLevel yearLevel = getYearLevelInput();
    if (yearLevel == static_cast<YearLevel>(-1)) {
        std::cout << COLOR_YELLOW << "✗ Operation cancelled.\n" << COLOR_RESET;
        return;
    }

    int numSubjects = getNumSubjects(yearLevel);
    std::vector<double> grades = getGradesInput(numSubjects);

    Student newStudent = createStudent(name, studentNumber, email, yearLevel, grades);
    allStudents.push_back(newStudent);
    studentsByYear[yearLevel].push_back(newStudent);

    std::cout << COLOR_GREEN << COLOR_BOLD << "\n✓ Student added successfully!\n" << COLOR_RESET;
    saveStudentsToFile(allStudents);
}

void displayAllStudents(const std::map<YearLevel, std::vector<Student>>& studentsByYear) {
    for (int i = 1; i <= 4; ++i) {
        YearLevel year = static_cast<YearLevel>(i);
        auto it = studentsByYear.find(year);
        if (it != studentsByYear.end() && !it->second.empty()) {
            std::vector<Student> sortedStudents = it->second;
            // Sort by GWA ascending (lower GWA is better)
            std::sort(sortedStudents.begin(), sortedStudents.end(), [](const Student& a, const Student& b) {
                return a.gwa < b.gwa;
            });
            // Assign ranks
            for (size_t j = 0; j < sortedStudents.size(); ++j) {
                sortedStudents[j].rank = j + 1;
            }
            displayYearLevel(sortedStudents, year);
        }
    }
}

void searchStudent(const std::vector<Student>& allStudents) {
    bool continueSearching = true;
    while (continueSearching) {
        int choice;
        std::cout << "\n" << COLOR_CYAN << COLOR_BOLD << "╔══════════════════════════════════════╗\n"
                  << "║      SEARCH STUDENT                  ║\n"
                  << "╚══════════════════════════════════════╝\n" << COLOR_RESET;
        std::cout << COLOR_GREEN << "1. Search by Name (Linear Search)\n"
                  << "2. Search by Student Number (Binary Search)\n"
                  << "0. Go Back\n" << COLOR_RESET;
        std::cout << COLOR_MAGENTA << "Enter choice: " << COLOR_RESET;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 0) {
            std::cout << COLOR_YELLOW << "✗ Operation cancelled.\n" << COLOR_RESET;
            return;
        }

        if (choice == 1) {
            std::string name;
            std::cout << COLOR_GREEN << "Enter student name (or type 'back' to cancel): " << COLOR_RESET;
            std::getline(std::cin, name);
            if (name == "back" || name == "BACK") {
                std::cout << COLOR_YELLOW << "✗ Operation cancelled.\n" << COLOR_RESET;
                return;
            }
            
            // Find all matching students
            std::string nameLower = name;
            std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
            
            std::vector<int> matchingIndices;
            for (size_t i = 0; i < allStudents.size(); ++i) {
                std::string studentNameLower = allStudents[i].name;
                std::transform(studentNameLower.begin(), studentNameLower.end(), studentNameLower.begin(), ::tolower);
                if (studentNameLower.find(nameLower) != std::string::npos) {
                    matchingIndices.push_back(i);
                }
            }
            
            if (matchingIndices.empty()) {
                std::cout << COLOR_YELLOW << "✗ No students found with that name.\n" << COLOR_RESET;
            } else if (matchingIndices.size() == 1) {
                // Single match - display it directly
                int index = matchingIndices[0];
                std::cout << COLOR_GREEN << "\n✓ Student found:\n" << COLOR_RESET;
                std::cout << COLOR_BLUE << "Name: " << allStudents[index].name 
                          << "\nStudent Number: " << allStudents[index].studentNumber 
                          << "\nEmail: " << allStudents[index].email 
                          << "\nYear Level: " << getYearString(allStudents[index].yearLevel) 
                          << "\nGWA: " << std::fixed << std::setprecision(2) << allStudents[index].gwa 
                          << "\nStatus: " << allStudents[index].status << "\n" << COLOR_RESET;
            } else {
                // Multiple matches - display list and let user select
                std::cout << COLOR_GREEN << "\n✓ Found " << matchingIndices.size() << " students matching \"" << name << "\":\n\n" << COLOR_RESET;
                std::cout << COLOR_BLUE << COLOR_BOLD;
                std::cout << std::left << std::setw(3) << "#" << " │ " << std::setw(30) << "Name" << " │ " 
                          << std::setw(12) << "Student No" << " │ " << std::setw(10) << "Year" << " │ "
                          << std::setw(6) << "GWA" << "\n" << COLOR_RESET;
                std::cout << COLOR_CYAN << "─────────────────────────────────────────────────────────────────────────────────────────\n" << COLOR_RESET;
                
                for (size_t i = 0; i < matchingIndices.size(); ++i) {
                    int idx = matchingIndices[i];
                    std::cout << std::left << std::setw(3) << (i + 1) << " │ " << std::setw(30) << allStudents[idx].name << " │ "
                              << std::setw(12) << allStudents[idx].studentNumber << " │ " << std::setw(10) << getYearString(allStudents[idx].yearLevel) << " │ "
                              << std::fixed << std::setprecision(2) << std::setw(6) << allStudents[idx].gwa << "\n";
                }
                std::cout << COLOR_CYAN << "─────────────────────────────────────────────────────────────────────────────────────────\n" << COLOR_RESET;
                
                std::cout << COLOR_MAGENTA << "Select a student number to view details (or 0 to skip): " << COLOR_RESET;
                int selection;
                std::cin >> selection;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (selection > 0 && selection <= static_cast<int>(matchingIndices.size())) {
                    int index = matchingIndices[selection - 1];
                    std::cout << COLOR_GREEN << "\n✓ Student Details:\n" << COLOR_RESET;
                    std::cout << COLOR_BLUE << "Name: " << allStudents[index].name 
                              << "\nStudent Number: " << allStudents[index].studentNumber 
                              << "\nEmail: " << allStudents[index].email 
                              << "\nYear Level: " << getYearString(allStudents[index].yearLevel) 
                              << "\nGWA: " << std::fixed << std::setprecision(2) << allStudents[index].gwa 
                              << "\nStatus: " << allStudents[index].status << "\n" << COLOR_RESET;
                }
            }
        } else if (choice == 2) {
            std::vector<Student> sortedStudents = allStudents;
            std::sort(sortedStudents.begin(), sortedStudents.end(), compareByStudentNumber);
            std::string studentNumber;
            std::cout << COLOR_GREEN << "Enter student number (or type 'back' to cancel): " << COLOR_RESET;
            std::getline(std::cin, studentNumber);
            if (studentNumber == "back" || studentNumber == "BACK") {
                std::cout << COLOR_YELLOW << "✗ Operation cancelled.\n" << COLOR_RESET;
                return;
            }
            int index = binarySearch(sortedStudents, studentNumber);
            if (index != -1) {
                std::cout << COLOR_GREEN << "\n✓ Student found:\n" << COLOR_RESET;
                std::cout << COLOR_BLUE << "Name: " << sortedStudents[index].name 
                          << "\nStudent Number: " << sortedStudents[index].studentNumber 
                          << "\nEmail: " << sortedStudents[index].email 
                          << "\nYear Level: " << getYearString(sortedStudents[index].yearLevel) 
                          << "\nGWA: " << std::fixed << std::setprecision(2) << sortedStudents[index].gwa 
                          << "\nStatus: " << sortedStudents[index].status << "\n" << COLOR_RESET;
            } else std::cout << COLOR_YELLOW << "✗ Student not found.\n" << COLOR_RESET;
        } else {
            std::cout << COLOR_YELLOW << "Invalid choice.\n" << COLOR_RESET;
        }
    }
}

void sortStudents(std::vector<Student>& allStudents, std::map<YearLevel, std::vector<Student>>& studentsByYear) {
    int choice;
    std::cout << "\n" << COLOR_CYAN << COLOR_BOLD << "╔══════════════════════════════════════╗\n"
              << "║      SORT STUDENTS                   ║\n"
              << "╚══════════════════════════════════════╝\n" << COLOR_RESET;
    std::cout << COLOR_GREEN << "1. Sort by GWA (Bubble Sort)\n"
              << "2. Sort by Name (Selection Sort)\n"
              << "0. Go Back\n" << COLOR_RESET;
    std::cout << COLOR_MAGENTA << "Enter choice: " << COLOR_RESET;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0) {
        std::cout << COLOR_YELLOW << "✗ Operation cancelled.\n" << COLOR_RESET;
        return;
    }

    std::string sortType;
    if (choice == 1) {
        bubbleSort(allStudents, true);
        sortType = "GWA (Ascending) - Bubble Sort";
        std::cout << COLOR_GREEN << COLOR_BOLD << "✓ Students sorted by GWA using Bubble Sort.\n" << COLOR_RESET;
    } else if (choice == 2) {
        selectionSort(allStudents, false);
        sortType = "Name (Alphabetical) - Selection Sort";
        std::cout << COLOR_GREEN << COLOR_BOLD << "✓ Students sorted by Name using Selection Sort.\n" << COLOR_RESET;
    } else {
        std::cout << COLOR_YELLOW << "Invalid choice.\n" << COLOR_RESET;
        return;
    }

    // Display the sorted list
    displaySortedList(allStudents, sortType);

    // Update rankings for each year
    for (auto& pair : studentsByYear) {
        std::sort(pair.second.begin(), pair.second.end(), compareByGWA);
        for (size_t j = 0; j < pair.second.size(); ++j) {
            pair.second[j].rank = j + 1;
        }
    }
}

void displayTopNStudents(const std::map<YearLevel, std::vector<Student>>& studentsByYear) {
    int n;
    std::cout << "\n" << COLOR_CYAN << COLOR_BOLD << "╔══════════════════════════════════════════╗\n"
              << "║      TOP N STUDENTS PER YEAR          ║\n"
              << "╚══════════════════════════════════════════╝\n" << COLOR_RESET;
    std::cout << COLOR_MAGENTA << "Enter N (or 0 to go back): " << COLOR_RESET;
    std::cin >> n;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (n == 0) {
        std::cout << COLOR_YELLOW << "✗ Operation cancelled.\n" << COLOR_RESET;
        return;
    }

    for (int i = 1; i <= 4; ++i) {
        YearLevel year = static_cast<YearLevel>(i);
        auto it = studentsByYear.find(year);
        if (it != studentsByYear.end() && !it->second.empty()) {
            std::vector<Student> topStudents;
            int count = std::min(n, static_cast<int>(it->second.size()));
            for (int j = 0; j < count; ++j) topStudents.push_back(it->second[j]);
            displayYearLevel(topStudents, year);
        }
    }
}

void demonstrateBST(const std::vector<Student>& allStudents) {
    if (allStudents.empty()) { 
        std::cout << COLOR_YELLOW << "No students to demonstrate BST.\n" << COLOR_RESET; 
        return; 
    }

    int choice;
    std::cout << "\n" << COLOR_CYAN << COLOR_BOLD << "╔══════════════════════════════════════════╗\n"
              << "║      BST DEMONSTRATION                ║\n"
              << "╚══════════════════════════════════════════╝\n" << COLOR_RESET;
    std::cout << COLOR_GREEN << "1. Sort by GWA using BST\n"
              << "2. Sort by Name using BST\n"
              << "0. Go Back\n" << COLOR_RESET;
    std::cout << COLOR_MAGENTA << "Enter choice: " << COLOR_RESET;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0) {
        std::cout << COLOR_YELLOW << "✗ Operation cancelled.\n" << COLOR_RESET;
        return;
    }

    BST bst(choice == 1);
    for (const auto& student : allStudents) bst.insert(student);
    std::vector<Student> sortedStudents = bst.getSortedList();

    std::cout << "\n" << COLOR_CYAN << COLOR_BOLD << "Students sorted using BST:\n" << COLOR_RESET;
    std::cout << COLOR_BLUE << COLOR_BOLD;
    std::cout << std::left << std::setw(30) << "Name" << " │ " << std::setw(12) << "Student No" << " │ " << std::setw(6) << "GWA" << "\n" << COLOR_RESET;
    std::cout << COLOR_CYAN << "─────────────────────────────────────────────────────────────────────\n" << COLOR_RESET;
    for (const auto& student : sortedStudents) {
        std::cout << std::left << std::setw(30) << student.name << " │ " << std::setw(12) << student.studentNumber 
                  << " │ " << std::fixed << std::setprecision(2) << std::setw(6) << student.gwa << "\n";
    }
    std::cout << COLOR_CYAN << "─────────────────────────────────────────────────────────────────────\n" << COLOR_RESET;
}

void displayPassFail(const std::vector<Student>& allStudents) {
    std::vector<Student> passed, failed;
    for (const auto& student : allStudents) {
        if (student.status == "Passed") passed.push_back(student);
        else failed.push_back(student);
    }

    std::cout << "\n" << COLOR_CYAN << "═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════\n" << COLOR_RESET;
    std::cout << COLOR_GREEN << COLOR_BOLD << "                                      PASSED STUDENTS                                      │                                      FAILED STUDENTS\n" << COLOR_RESET;
    std::cout << COLOR_CYAN << "═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════\n" << COLOR_RESET;
    std::cout << COLOR_BLUE << COLOR_BOLD;
    std::cout << std::left << std::setw(3) << "#" << " │ " << std::setw(30) << "Name" << " │ " 
              << std::setw(12) << "Student No" << " │ " << std::setw(10) << "Year" << " │ "
              << std::setw(6) << "GWA" << " │ " << std::setw(3) << "#" << " │ " << std::setw(30) << "Name" << " │ " 
              << std::setw(12) << "Student No" << " │ " << std::setw(10) << "Year" << " │ "
              << std::setw(6) << "GWA" << "\n" << COLOR_RESET;
    std::cout << COLOR_CYAN << "─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────\n" << COLOR_RESET;
    
    size_t maxRows = std::max(passed.size(), failed.size());
    for (size_t i = 0; i < maxRows; ++i) {
        // Passed student
        if (i < passed.size()) {
            std::cout << std::left << std::setw(3) << (i + 1) << " │ " << std::setw(30) << passed[i].name << " │ "
                      << std::setw(12) << passed[i].studentNumber << " │ " << std::setw(10) << getYearString(passed[i].yearLevel) << " │ "
                      << std::fixed << std::setprecision(2) << std::setw(6) << passed[i].gwa << " │ ";
        } else {
            std::cout << std::setw(3) << "" << " │ " << std::setw(30) << "" << " │ "
                      << std::setw(12) << "" << " │ " << std::setw(10) << "" << " │ "
                      << std::setw(6) << "" << " │ ";
        }
        
        // Failed student
        if (i < failed.size()) {
            std::cout << std::left << std::setw(3) << (i + 1) << " │ " << std::setw(30) << failed[i].name << " │ "
                      << std::setw(12) << failed[i].studentNumber << " │ " << std::setw(10) << getYearString(failed[i].yearLevel) << " │ "
                      << std::fixed << std::setprecision(2) << std::setw(6) << failed[i].gwa;
        } else {
            std::cout << std::setw(3) << "" << " │ " << std::setw(30) << "" << " │ "
                      << std::setw(12) << "" << " │ " << std::setw(10) << "" << " │ "
                      << std::setw(6) << "";
        }
        std::cout << "\n";
    }
    std::cout << COLOR_CYAN << "═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════\n" << COLOR_RESET;
    std::cout << COLOR_GREEN << "Total Passed: " << passed.size() << "  │  Total Failed: " << failed.size() << "\n" << COLOR_RESET;
}

void deleteStudent(std::vector<Student>& allStudents, std::map<YearLevel, std::vector<Student>>& studentsByYear) {
    int choice;
    std::cout << "\n" << COLOR_CYAN << COLOR_BOLD << "╔══════════════════════════════════════╗\n"
              << "║      DELETE STUDENT                  ║\n"
              << "╚══════════════════════════════════════╝\n" << COLOR_RESET;
    std::cout << COLOR_GREEN << "1. Search by Name\n"
              << "2. Search by Student Number\n"
              << "0. Go Back\n" << COLOR_RESET;
    std::cout << COLOR_MAGENTA << "Enter choice: " << COLOR_RESET;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0) {
        std::cout << COLOR_YELLOW << "✗ Operation cancelled.\n" << COLOR_RESET;
        return;
    }

    Student* studentToDelete = nullptr;
    size_t indexToDelete = -1;

    if (choice == 1) {
        std::string name;
        std::cout << COLOR_GREEN << "Enter student name (or type 'back' to cancel): " << COLOR_RESET;
        std::getline(std::cin, name);
        if (name == "back" || name == "BACK") {
            std::cout << COLOR_YELLOW << "✗ Operation cancelled.\n" << COLOR_RESET;
            return;
        }
        int index = linearSearch(allStudents, name, true);
        if (index != -1) {
            studentToDelete = &allStudents[index];
            indexToDelete = index;
        }
    } else if (choice == 2) {
        std::vector<Student> sortedStudents = allStudents;
        std::sort(sortedStudents.begin(), sortedStudents.end(), compareByStudentNumber);
        std::string studentNumber;
        std::cout << COLOR_GREEN << "Enter student number (or type 'back' to cancel): " << COLOR_RESET;
        std::getline(std::cin, studentNumber);
        if (studentNumber == "back" || studentNumber == "BACK") {
            std::cout << COLOR_YELLOW << "✗ Operation cancelled.\n" << COLOR_RESET;
            return;
        }
        int index = binarySearch(sortedStudents, studentNumber);
        if (index != -1) {
            // Find the index in the original allStudents vector
            for (size_t i = 0; i < allStudents.size(); ++i) {
                if (allStudents[i].studentNumber == studentNumber) {
                    studentToDelete = &allStudents[i];
                    indexToDelete = i;
                    break;
                }
            }
        }
    } else {
        std::cout << COLOR_YELLOW << "Invalid choice.\n" << COLOR_RESET;
        return;
    }

    if (studentToDelete == nullptr) {
        std::cout << COLOR_YELLOW << "✗ Student not found.\n" << COLOR_RESET;
        return;
    }

    // Display student info for confirmation
    std::cout << COLOR_GREEN << "\n✓ Student found:\n" << COLOR_RESET;
    std::cout << COLOR_BLUE << "Name: " << studentToDelete->name << "\n"
              << "Student Number: " << studentToDelete->studentNumber << "\n"
              << "Email: " << studentToDelete->email << "\n"
              << "Year Level: " << getYearString(studentToDelete->yearLevel) << "\n"
              << "GWA: " << std::fixed << std::setprecision(2) << studentToDelete->gwa << "\n"
              << "Status: " << studentToDelete->status << "\n" << COLOR_RESET;

    std::cout << COLOR_RED << COLOR_BOLD << "\n⚠  Are you sure you want to delete this student? (y/n): " << COLOR_RESET;
    char confirm;
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (confirm == 'y' || confirm == 'Y') {
        // Remove from allStudents
        allStudents.erase(allStudents.begin() + indexToDelete);
        
        // Remove from studentsByYear
        auto& yearStudents = studentsByYear[studentToDelete->yearLevel];
        for (auto it = yearStudents.begin(); it != yearStudents.end(); ++it) {
            if (it->studentNumber == studentToDelete->studentNumber) {
                yearStudents.erase(it);
                break;
            }
        }
        
        // Update rankings for the year
        std::sort(yearStudents.begin(), yearStudents.end(), compareByGWA);
        for (size_t j = 0; j < yearStudents.size(); ++j) {
            yearStudents[j].rank = j + 1;
        }
        
        std::cout << COLOR_GREEN << COLOR_BOLD << "✓ Student deleted successfully!\n" << COLOR_RESET;
        saveStudentsToFile(allStudents);
    } else {
        std::cout << COLOR_YELLOW << "✗ Deletion cancelled.\n" << COLOR_RESET;
    }
}

/* =======================================================================================================
 * SECTOR 5: USER INTERFACE & MAIN PROGRAM
 * =======================================================================================================
 * This sector handles all user interface components including menus, banners, input validation,
 * and the main program loop that orchestrates the entire student management system.
 * ======================================================================================================= */

void displayWelcomeBanner() {
    std::cout << COLOR_CYAN << COLOR_BOLD;
    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                                ║\n";
    std::cout << "║" << COLOR_GREEN << COLOR_BOLD << "        ✨ STUDENT GWA RANKING SYSTEM ✨" << COLOR_RESET << COLOR_CYAN << COLOR_BOLD << "               ║\n";
    std::cout << "║" << COLOR_YELLOW << "           Manage & Track Student Performance" << COLOR_RESET << COLOR_CYAN << COLOR_BOLD << "           ║\n";
    std::cout << "║                                                                ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n" << COLOR_RESET;
}

void sortBySubjectGWAName(const std::map<YearLevel, std::vector<Student>>& studentsByYear) {
    YearLevel year = getYearLevelInput();
    if (year == static_cast<YearLevel>(-1)) return;

    auto it = studentsByYear.find(year);
    if (it == studentsByYear.end() || it->second.empty()) {
        std::cout << COLOR_YELLOW << "No students found for " << getYearString(year) << ".\n" << COLOR_RESET;
        return;
    }

    std::vector<std::string> subjects = getSubjectsByYear(year);
    std::cout << "\n" << COLOR_CYAN << "Subjects for " << getYearString(year) << ":\n" << COLOR_RESET;
    for (size_t i = 0; i < subjects.size(); ++i) {
        std::cout << COLOR_BLUE << (i + 1) << ". " << subjects[i] << "\n" << COLOR_RESET;
    }

    int sortChoice;
    std::cout << "\n" << COLOR_MAGENTA << "Sort by:\n1. Subject Grade\n2. GWA\n3. Name\nEnter choice: " << COLOR_RESET;
    std::cin >> sortChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (sortChoice < 1 || sortChoice > 3) {
        std::cout << COLOR_YELLOW << "Invalid choice.\n" << COLOR_RESET;
        return;
    }

    char orderChoice;
    std::cout << COLOR_MAGENTA << "Order:\nA. Ascending\nD. Descending\nEnter choice: " << COLOR_RESET;
    std::cin >> orderChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    orderChoice = std::toupper(orderChoice);

    if (orderChoice != 'A' && orderChoice != 'D') {
        std::cout << COLOR_YELLOW << "Invalid choice.\n" << COLOR_RESET;
        return;
    }

    std::vector<Student> studentsToSort = it->second;
    std::string subjectName = "";
    int subjectIndex = -1;

    if (sortChoice == 1) {
        std::cout << COLOR_MAGENTA << "Enter subject number (1-" << subjects.size() << "): " << COLOR_RESET;
        std::cin >> subjectIndex;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        subjectIndex--;
        if (subjectIndex < 0 || subjectIndex >= static_cast<int>(subjects.size())) {
            std::cout << COLOR_YELLOW << "Invalid subject number.\n" << COLOR_RESET;
            return;
        }
        subjectName = subjects[subjectIndex];
    }

    // Sort the students
    std::sort(studentsToSort.begin(), studentsToSort.end(), [&](const Student& a, const Student& b) {
        bool ascending = (orderChoice == 'A');
        if (sortChoice == 1) { // Subject grade
            if (ascending) return a.grades[subjectIndex] < b.grades[subjectIndex];
            else return a.grades[subjectIndex] > b.grades[subjectIndex];
        } else if (sortChoice == 2) { // GWA
            if (ascending) return a.gwa < b.gwa;
            else return a.gwa > b.gwa;
        } else { // Name
            if (ascending) return a.name < b.name;
            else return a.name > b.name;
        }
    });

    // Display the results
    std::string sortType;
    if (sortChoice == 1) sortType = "Subject: " + subjectName;
    else if (sortChoice == 2) sortType = "GWA";
    else sortType = "Name";
    sortType += (orderChoice == 'A' ? " (Ascending)" : " (Descending)");

    std::cout << "\n" << COLOR_CYAN << "═══════════════════════════════════════════════════════════════════════════════════════════════════════\n" << COLOR_RESET;
    std::cout << COLOR_GREEN << COLOR_BOLD << "  " << getYearString(year) << " Students - Sorted by " << sortType << "\n" << COLOR_RESET;
    if (!subjectName.empty()) {
        std::cout << COLOR_YELLOW << "  Subject: " << subjectName << "\n" << COLOR_RESET;
    }
    std::cout << COLOR_CYAN << "═══════════════════════════════════════════════════════════════════════════════════════════════════════\n" << COLOR_RESET;
    std::cout << COLOR_BLUE << COLOR_BOLD;
    std::cout << std::left << std::setw(3) << "#" << " │ " << std::setw(30) << "Name" << " │ " 
              << std::setw(12) << "Student No" << " │ " << std::setw(12) << "Subject Grade" << " │ " 
              << std::setw(6) << "GWA" << " │ " << std::setw(7) << "Status" << "\n" << COLOR_RESET;
    std::cout << COLOR_CYAN << "─────────────────────────────────────────────────────────────────────────────────────────────────────────────\n" << COLOR_RESET;

    int counter = 1;
    for (const auto& student : studentsToSort) {
        std::cout << std::left << std::setw(3) << counter << " │ " << std::setw(30) << student.name << " │ "
                  << std::setw(12) << student.studentNumber << " │ ";
        if (sortChoice == 1) {
            std::cout << std::fixed << std::setprecision(2) << std::setw(12) << student.grades[subjectIndex] << " │ ";
        } else {
            std::cout << std::setw(12) << "-" << " │ ";
        }
        std::cout << std::fixed << std::setprecision(2) << std::setw(6) << student.gwa << " │ ";
        
        // Determine status based on sort criteria
        std::string displayStatus;
        if (sortChoice == 1) {
            // When sorting by subject, base status on subject grade
            displayStatus = (student.grades[subjectIndex] < 4.0) ? "Passed" : "Failed";
        } else {
            // For GWA and Name sorting, use the stored status (based on GWA)
            displayStatus = student.status;
        }
        
        if (displayStatus == "Passed") {
            std::cout << COLOR_GREEN << std::left << std::setw(7) << "✓ PASS" << COLOR_RESET;
        } else {
            std::cout << COLOR_YELLOW << std::left << std::setw(7) << "✗ FAIL" << COLOR_RESET;
        }
        std::cout << "\n";
        counter++;
    }
    std::cout << COLOR_CYAN << "═══════════════════════════════════════════════════════════════════════════════════════════════════════\n" << COLOR_RESET;
}

void displayMenu() {
    std::cout << "\n" << COLOR_CYAN << "╔═══════════════════════════════════════════════════════════╗\n" << COLOR_RESET;
    std::cout << COLOR_BLUE << COLOR_BOLD << "               MAIN MENU - SELECT AN OPTION\n" << COLOR_RESET;
    std::cout << COLOR_CYAN << "║\n" << COLOR_RESET;
    std::cout << COLOR_GREEN << "║ " << COLOR_BOLD << "1." << COLOR_RESET << COLOR_GREEN << " Add New Student\n"
              << "║ " << COLOR_BOLD << "2." << COLOR_RESET << COLOR_GREEN << " Display All Students (Grouped by Year Level)\n"
              << "║ " << COLOR_BOLD << "3." << COLOR_RESET << COLOR_GREEN << " Search Student\n"
              << "║ " << COLOR_BOLD << "4." << COLOR_RESET << COLOR_GREEN << " Sort Students\n"
              << "║ " << COLOR_BOLD << "5." << COLOR_RESET << COLOR_GREEN << " Display Top N Students per Year\n"
              << "║ " << COLOR_BOLD << "6." << COLOR_RESET << COLOR_GREEN << " Demonstrate BST\n"
              << "║ " << COLOR_BOLD << "7." << COLOR_RESET << COLOR_GREEN << " Display Pass/Fail Status\n"
              << "║ " << COLOR_BOLD << "8." << COLOR_RESET << COLOR_GREEN << " Delete Student\n"
              << "║ " << COLOR_BOLD << "9." << COLOR_RESET << COLOR_GREEN << " View Sorted Students (Subject / GWA / Name)\n" << COLOR_RESET;
    std::cout << COLOR_YELLOW << "║ " << COLOR_BOLD << "10." << COLOR_RESET << COLOR_YELLOW << " Exit\n" << COLOR_RESET;
    std::cout << COLOR_CYAN << "║\n╚═══════════════════════════════════════════════════════════╝\n" << COLOR_RESET;
    std::cout << COLOR_MAGENTA << COLOR_BOLD << "Enter your choice (1-10): " << COLOR_RESET;
}

int main() {
    std::vector<Student> allStudents;
    std::map<YearLevel, std::vector<Student>> studentsByYear;

    // Try to load students from file
    loadStudentsFromFile(allStudents, studentsByYear);
    
    // Always include default data
        // 1st Year Students (6 subjects)
        studentsByYear[FIRST].push_back(createStudent("Zuriel Ethan Yambao", "20231128975", "yambao.zurielethan@ue.edu.ph", FIRST, {2.50, 2.00, 1.25, 1.25, 2.00, 1.75}));
        studentsByYear[FIRST].push_back(createStudent("Lev Ryan Macalintal", "20131155389", "macalintal.levryan@ue.edu.ph", FIRST, {2.50, 2.00, 1.50, 1.25, 2.00, 1.75}));
        studentsByYear[FIRST].push_back(createStudent("Axel Audrey Ignacio", "20231137748", "ignacio.axelaudrey@eu.edu.ph", FIRST, {3.0, 2.25, 1.50, 1.50, 4, 1.75}));
        studentsByYear[FIRST].push_back(createStudent("Christian Arvy Pasion", "20251123225", "pasion.christianarvy@eu.edu.ph", FIRST, {2.75, 2.00, 1.75, 1.75, 2.00, 1.75}));
        studentsByYear[FIRST].push_back(createStudent("Euken Rosario", "20251123861", "rosario.euken@eu.edu.ph", FIRST, {2.75, 1.75, 1.25, 2.25, 2.00, 1.75}));
        studentsByYear[FIRST].push_back(createStudent("Toby Unciano", "20231101396", "unciano.toby@eu.edu.ph", FIRST, {3.00, 1.75, 1.50, 1.25, 2.75, 1.75}));
        studentsByYear[FIRST].push_back(createStudent("Princess Roque", "20251120566", "roque.princessangeline@eu.edu.ph", FIRST, {2.75, 1.75, 1.25, 2.25, 3.00, 1.50}));
        studentsByYear[FIRST].push_back(createStudent("Jophielle Gunabe", "20251131278", "gunabe.jophielle@eu.edu.ph", FIRST, {3.00, 1.75, 1.25, 2.25, 3.00, 1.50}));
        studentsByYear[FIRST].push_back(createStudent("Emmanuel Patrick Acuna", "202311052112", "acuna.emmanuelpatrick@eu.edu.ph", FIRST, {2.50, 2.00, 1.75, 2.00, 2.00, 1.75}));
        studentsByYear[FIRST].push_back(createStudent("Manuel Marquez", "20191132646", "marquez.manuelalexander@eu.edu.ph", FIRST, {2.50, 1.50, 1.25, 1.25, 1.75, 1.75}));
        studentsByYear[FIRST].push_back(createStudent("Aaron Bernabe", "20251114564", "bernabe.marxaaron@eu.edu.ph", FIRST, {2.00, 1.50, 2.00, 1.75, 2.25, 2.00}));
        studentsByYear[FIRST].push_back(createStudent("Eros Lamamigo", "2025113850", "lamamigo.erosmatthew@eu.edu.ph", FIRST, {2.50, 2.00, 1.50, 1.50, 2.25, 1.75}));
        studentsByYear[FIRST].push_back(createStudent("Bryce Gabriel Abordo", "20251107412", "abordo.brycebengabriel@eu.edu.ph", FIRST, {1.75, 2.00, 1.75, 1.25, 1.75, 1.75}));
        studentsByYear[FIRST].push_back(createStudent("Zionah Rae Javier", "20251121161", "javier.zionahrae@eu.edu.ph", FIRST, {2.25, 2.25, 1.50, 1.75, 1.75, 1.75}));
        studentsByYear[FIRST].push_back(createStudent("Maxine Aliyah Alcazar", "20251123963", "alcazar.maxinealiyah@eu.edu.ph", FIRST, {1.25, 1.25, 1.00, 1.00, 1.00, 1.25}));
        studentsByYear[FIRST].push_back(createStudent("Rhian Zyrelle Ramos", "20231123345", "ramos.rhjianzyrelle@eu.edu.ph", FIRST, {2.50, 1.50, 1.25, 1.75, 1.25, 1.25}));
        studentsByYear[FIRST].push_back(createStudent("Sean Russel Cruz", "20251130106", "cruz.seanrussel@eu.edu.ph", FIRST, {2.75, 2.50, 1.25, 1.50, 2.25, 1.25}));
        studentsByYear[FIRST].push_back(createStudent("Clayton Kelsey Grande", "20251104485", "grande.claytonkelsey@eu.edu.ph", FIRST, {2.50, 2.00, 2.25, 1.50, 2.75, 2.00}));

        // 2nd Year Students (9 subjects)
        studentsByYear[SECOND].push_back(createStudent("Gwen Lopez", "20241130029", "gwengelynn@gmail.com", SECOND, {1.25, 1.00, 1.00, 1.25, 1.25, 1.50, 1.00, 1.50, 1.50}));
        studentsByYear[SECOND].push_back(createStudent("Chang Park", "20181114136", "park.changhyun@ue.edu.ph", SECOND, {1.25, 1.00, 1.25, 1.50, 1.25, 1.75, 1.00, 1.50, 1.75}));
        studentsByYear[SECOND].push_back(createStudent("Angela Tolentino", "202411245746", "tolentino.angela@eu.edu.ph", SECOND, {1.25, 1.00, 1.00, 1.50, 1.25, 1.75, 1.00, 1.25, 1.25}));
        studentsByYear[SECOND].push_back(createStudent("Alexandrina Timbol", "20251120809", "timbol.alexandrina@eu.edu.ph", SECOND, {1.25, 1.25, 1.25, 1.25, 2.25, 2.00, 1.50, 1.25, 1.25}));
        studentsByYear[SECOND].push_back(createStudent("Patrick Alde", "20241113019", "alde.patrick@ue.edu.ph", SECOND, {1.25, 1.25, 1.75, 2.00, 1.25, 1.00, 1.25, 1.25, 1.75}));
        studentsByYear[SECOND].push_back(createStudent("Princess Nicole Rance", "20241102346", "rance.nicoleprincess@ue.edu.ph", SECOND, {1.75, 1.25, 1.00, 1.25, 1.00, 1.75, 1.75, 1.25, 1.25}));
        studentsByYear[SECOND].push_back(createStudent("Argelina Ryle Bautista", "20221105062", "pedro@email.com", SECOND, {1.25, 1.75, 1.25, 1.00, 1.25, 1.00, 1.25, 1.25, 1.75}));
        studentsByYear[SECOND].push_back(createStudent("Seun Gyrich Aldeguer", "20241123375", "aldeguer.seungyrich@ue.edu.ph", SECOND, {1.75, 1.25, 1.00, 1.75, 1.25, 1.00, 1.25, 1.25, 1.75}));
        studentsByYear[SECOND].push_back(createStudent("Jay Lord Gaspi", "20241120810", "gaspi.jaylord@ue.edu.ph", SECOND, {1.75, 1.25, 2.00, 1.75, 1.25, 1.75, 1.25, 1.00, 1.25}));
        studentsByYear[SECOND].push_back(createStudent("Roven Jules", "20241117362", "jules.roven@ue.edu.ph", SECOND, {1.25, 1.75, 2.00, 1.75, 1.25, 2.00, 1.25, 1.25, 1.25}));
        studentsByYear[SECOND].push_back(createStudent("Kayne Bernadette Roque", "20241116336", "roque.kaynebernadette@ue.edu.ph", SECOND, {1.25, 1.25, 1.00, 1.75, 1.25, 1.00, 1.25, 1.00, 1.25}));
        studentsByYear[SECOND].push_back(createStudent("Mariane Eystaquio", "20241107313", "eystaquio.mariane@ue.edu.ph", SECOND, {1.75, 1.25, 1.00, 1.75, 1.25, 1.00, 1.75, 1.00, 1.25}));
        studentsByYear[SECOND].push_back(createStudent("John Patrick Dela Cruz", "20241141071", "delacruz.johnpatrick@ue.edu.ph", SECOND, {1.25, 1.00, 1.25, 1.75, 1.25, 1.75, 1.00, 1.25, 1.75}));
        studentsByYear[SECOND].push_back(createStudent("Jenny Rose Janer", "20221124523", "janer.jennyrose@ue.edu.ph", SECOND, {1.75, 1.25, 1.00, 1.25, 1.75, 1.00, 1.25, 1.25, 1.25}));
        studentsByYear[SECOND].push_back(createStudent("Hernan Jay Nem", "20221104923", "nem.hernanjay@ue.edu.ph", SECOND, {1.25, 1.25, 1.00, 1.75, 1.25, 1.00, 1.25, 1.25, 2.00}));
        studentsByYear[SECOND].push_back(createStudent("Jevric Tampus", "20241126307", "tampus.jevric@ue.edu.ph", SECOND, {1.00, 1.25, 1.00, 1.75, 1.25, 1.25, 1.25, 1.00, 1.75}));
        studentsByYear[SECOND].push_back(createStudent("Jennilie Adame", "20241124674", "adame.jennilie@ue.edu.ph", SECOND, {1.25, 1.00, 1.25, 1.75, 1.25, 1.75, 1.25, 1.25, 1.75}));
        studentsByYear[SECOND].push_back(createStudent("John Meynaro Oguez", "20221115635", "oguez.johnmeynaro@ue.edu.ph", SECOND, {1.75,2.00,1.00,1.75,1.25,1.00,1.75,1.25,1.75}));
        // 3rd Year Students (10 subjects)
        studentsByYear[THIRD].push_back(createStudent("John Carlo Bauzon", "20231125874", "bauzon.johncarlo@ue.edu.ph", THIRD, {1.75, 1.50, 1.00, 1.75, 1.25, 1.75, 1.00, 1.25, 1.25, 1.50}));
        studentsByYear[THIRD].push_back(createStudent("Rhamuel Gamboa", "20231150122", "gamboa.rhamuel@ue.edu.ph", THIRD, {1.25, 1.50, 1.00, 1.50, 1.25, 1.25, 1.00, 1.25, 1.00, 1.00}));
        studentsByYear[THIRD].push_back(createStudent("Ralph Daniel Bayona", "20231135618", "bayona.ralph@ue.edu.ph", THIRD, {1.75, 1.25, 1.25, 1.50, 1.75, 1.75, 1.00, 1.25, 1.00, 1.00}));
        studentsByYear[THIRD].push_back(createStudent("Elija Brent Tarog", "20231127024", "tarog.elijabrent@ue.edu.ph", THIRD, {2.5, 4.00, 3.00, 1.75, 1.25, 1.75, 1.25, 2.25, 1.00, 1.00}));
        studentsByYear[THIRD].push_back(createStudent("Timothy P. Villegas", "2023131990", "villegas.timothy@ue.edu.ph", THIRD, {3.00, 1.75, 1.25, 1.75, 1.5, 1.75, 1.25, 3.00, 1.00, 1.00}));
        studentsByYear[THIRD].push_back(createStudent("Pharrell Morgan Basalo", "2023113924", "basalo.pharrellmorgan@ue.edu.ph", THIRD, {1.5, 2.00, 1.00, 1.50, 1.75, 1.75, 1.00, 2.00, 1.00, 1.00}));
        studentsByYear[THIRD].push_back(createStudent("Marc Andrew Pitalo", "20231123083", "pitalo.marcandrew@ue.edu.ph", THIRD, {1.5, 1.25, 1.25, 1.75, 1.25, 1.5, 1.25, 1.00, 1.25, 1.5}));
        studentsByYear[THIRD].push_back(createStudent("Rein Klyde Gorospe", "20237144006", "gorospe.reinklyde@ue.edu.ph", THIRD, {2.00, 1.25, 2.00, 1.75, 1.25, 1.50, 1.00, 1.25, 1.25, 1.75}));
        studentsByYear[THIRD].push_back(createStudent("Aldrich Bonzon", "20211102453", "bonzon.aldrich@ue.edu.ph", THIRD, {1.25, 1.00, 1.25, 1.75, 1.00, 1.25, 1.00, 1.25, 1.25, 1.00}));
        studentsByYear[THIRD].push_back(createStudent("Renz Magrata", "20231119974", "magrata.renz@ue.edu.ph", THIRD, {1.50, 1.25, 1.00, 1.75, 1.00, 1.50, 1.25, 1.25, 1.00, 1.25}));
        studentsByYear[THIRD].push_back(createStudent("Charles Bryan Librada", "20211106739", "librada.charlesbryan@ue.edu.ph", THIRD, {1.75, 1.00, 1.25, 1.50, 1.00, 1.50, 1.00, 1.25, 1.25, 1.25}));
        studentsByYear[THIRD].push_back(createStudent("Ronwin Adlai Bedis", "20231143401", "bedis.ronwinadlai@ue.edu.ph", THIRD, {1.50, 1.50, 1.00, 1.75, 1.00, 1.50, 1.25, 1.25, 1.75, 1.00}));
        studentsByYear[THIRD].push_back(createStudent("Adrian Rana", "20211114668", "rana.adrian@ue.edu.ph", THIRD, {1.50, 1.25, 1.00, 1.75, 1.00, 1.50, 1.25, 1.00, 1.75, 1.25}));
        studentsByYear[THIRD].push_back(createStudent("Franz Luis Pineda", "20131147803", "pineda.franzluis@ue.edu.ph", THIRD, {1.75, 1.25, 1.25, 1.25, 1.25, 1.50, 1.00, 1.00, 1.25, 1.00}));
        studentsByYear[THIRD].push_back(createStudent("Johnleez Carao", "20191151721", "carao.johnleez@ue.edu.ph", THIRD, {1.25, 1.25, 1.00, 1.75, 1.00, 1.50, 1.00, 1.25, 1.75, 1.25}));
        studentsByYear[THIRD].push_back(createStudent("Ryn Marc Dialino", "20211105452", "dialino.rynmarc@ue.edu.ph", THIRD, {1.25, 1.50, 1.00, 1.50, 1.25, 1.25, 1.00, 1.25, 1.25, 1.00}));
        // 4th Year Students (11 subjects)
        studentsByYear[FOURTH].push_back(createStudent("Kurt Regala", "20221106190", "regala.kurt@ue.edu.ph", FOURTH, {1.25, 1.50, 2.50, 1.75, 1.25, 1.75, 1.75, 1.50, 1.50, 2.00, 1.50}));
        studentsByYear[FOURTH].push_back(createStudent("William Monzon", "20191110200", "monzon.william@ue.edu.ph", FOURTH, {1.00, 1.25, 1.75, 1.50, 1.00, 1.50, 1.50, 1.25, 1.50, 1.00, 1.50}));
        studentsByYear[FOURTH].push_back(createStudent("Liam Concepcion", "20221119159", "concepcion.liam@ue.edu.ph", FOURTH, {1.00, 1.50, 1.25, 1.00, 1.25, 1.50, 1.25, 1.25, 1.75, 1.00, 1.50}));
        studentsByYear[FOURTH].push_back(createStudent("Al Francis B. Buenaventura", "20201124961", "buenaventura.alfrancis@ue.edu.ph", FOURTH, {1.00, 1.25, 1.25, 1.50, 1.25, 1.50, 1.50, 1.00, 1.50, 1.00, 1.50}));
        studentsByYear[FOURTH].push_back(createStudent("Bryan G. Bernardino", "20221112410", "bernardino.bryan@ue.edu.ph", FOURTH, {1.00, 1.00, 1.25, 1.50, 1.25, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00}));
        studentsByYear[FOURTH].push_back(createStudent("Ernest Dimaunahan", "20221117448", "dimaunahan.ernest@ue.edu.ph", FOURTH, {1.00, 1.25, 1.25, 1.00, 1.25, 1.50, 1.00, 1.00, 1.00, 1.00, 1.00}));
        studentsByYear[FOURTH].push_back(createStudent("Harold Orbita", "20161140083", "orbita.harold@ue.edu.ph", FOURTH, {1.00, 1.00, 1.50, 1.50, 1.25, 1.25, 1.00, 1.25, 1.25, 1.25, 1.00}));
        studentsByYear[FOURTH].push_back(createStudent("Trisha Baylon", "20211132228", "baylon.trisha@ue.edu.ph", FOURTH, {1.00, 1.00, 1.25, 1.25, 1.00, 1.25, 1.00, 1.00, 1.00, 1.00, 1.00}));
        studentsByYear[FOURTH].push_back(createStudent("Junabel Payam", "20221131244", "payam.junabel@ue.edu.ph", FOURTH, {1.00, 1.00, 1.25, 1.25, 1.00, 1.25, 1.00, 1.00, 1.00, 1.00, 1.00}));
        studentsByYear[FOURTH].push_back(createStudent("Jasmine Rose Pajarillaga", "20221130354", "pajarillaga.jasminerose@ue.edu.ph", FOURTH, {1.00, 1.00, 1.25, 1.25, 1.00, 1.25, 1.00, 1.00, 1.00, 1.00, 1.00}));
        studentsByYear[FOURTH].push_back(createStudent("Ryl Cruz", "20221124626", "cruz.ryl@ue.edu.ph", FOURTH, {1.00, 1.50, 1.75, 1.50, 1.25, 1.25, 1.25, 1.00, 2.50, 1.00, 1.00}));

        // Add all students to allStudents vector
        for (const auto &year : {FIRST, SECOND, THIRD, FOURTH})
            for (const auto &student : studentsByYear[year])
                allStudents.push_back(student);
        
        // Save initial data to file
        saveStudentsToFile(allStudents);

        // Remove duplicates based on student number
        std::unordered_set<std::string> seen;
        std::vector<Student> uniqueAllStudents;
        for (const auto& student : allStudents) {
            if (seen.find(student.studentNumber) == seen.end()) {
                seen.insert(student.studentNumber);
                uniqueAllStudents.push_back(student);
            }
        }
        allStudents = uniqueAllStudents;

        // Rebuild studentsByYear from unique allStudents
        studentsByYear.clear();
        for (const auto& student : allStudents) {
            studentsByYear[student.yearLevel].push_back(student);
        }

    // Sort and assign ranks for each year
    for (const auto &year : {FIRST, SECOND, THIRD, FOURTH}) {
        std::sort(studentsByYear[year].begin(), studentsByYear[year].end(), compareByGWA);
        for (size_t j = 0; j < studentsByYear[year].size(); ++j)
            studentsByYear[year][j].rank = j + 1;
    }

    // Display welcome banner
    displayWelcomeBanner();

    // Main interactive menu loop
    int choice = 0;
    while (choice != 10) {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
        case 1: addStudent(allStudents, studentsByYear); break;
        case 2: displayAllStudents(studentsByYear); break;
        case 3: searchStudent(allStudents); break;
        case 4: sortStudents(allStudents, studentsByYear); break;
        case 5: displayTopNStudents(studentsByYear); break;
        case 6: demonstrateBST(allStudents); break;
        case 7: displayPassFail(allStudents); break;
        case 8: deleteStudent(allStudents, studentsByYear); break;
        case 9: sortBySubjectGWAName(studentsByYear); break;
        case 10:
            std::cout << COLOR_MAGENTA << COLOR_BOLD << "\n╔════════════════════════════════════════════╗\n"
                      << "║  Thank you for using Student GWA System!  ║\n"
                      << "║              See you next time!           ║\n"
                      << "╚════════════════════════════════════════════╝\n" << COLOR_RESET;
            break;
        default:
            std::cout << COLOR_YELLOW << "\n⚠  Invalid choice! Please enter 1-10.\n" << COLOR_RESET;
        }
    }

    return 0;
}
