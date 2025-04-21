#include <iostream>
#include <fstream>
#include <string>    //This header file provides the std::string class, 
//which is used to represent and manipulate sequences of characters (strings).?

#include <cstdlib>   // For system() function
#include <unistd.h>  // For UNIX system detection (Linux/macOS)
#include <windows.h> // For Windows system detection

using namespace std;

// Structure to hold student data
struct Student
{
    string prn;
    string name;
    string password;
};

// Structure to hold subject data
struct Subject
{
    string name;
    int year;
    int semester;
    string syllabusFile;
};

// Structure to hold paper data
struct Paper
{
    string subjectName;
    int year;
    int semester;
    string paperFile;
};

// Function to validate student login
bool validateLogin(const string &prn, const string &password, string &studentName)
{
    ifstream file("students.txt");
    string storedPrn, storedName, storedPassword;

    while (file >> storedPrn >> storedName >> storedPassword)
    {
        if (prn == storedPrn && password == storedPassword)
        {
            studentName = storedName;
            return true;
        }
    }
    return false;
}

// Function to get the OS type
string getOperatingSystem()
{
#if defined(_WIN32) || defined(_WIN64)
    return "Windows";
#elif defined(__linux__)
    return "Linux";
#elif defined(__APPLE__) || defined(__MACH__)
    return "macOS";
#else
    return "Unknown";
#endif
}

// Function to open a PDF file based on the operating system
void openPDF(const string &filePath)
{
    string os = getOperatingSystem();

    if (os == "Windows")
    {
        system(("start " + filePath).c_str()); // Windows command to open PDF
    }
    else if (os == "Linux")
    {
        system(("xdg-open " + filePath).c_str()); // Linux command to open PDF
    }
    else if (os == "macOS")
    {
        system(("open " + filePath).c_str()); // macOS command to open PDF
    }
    else
    {
        cout << "Unsupported OS. Cannot open PDF." << endl;
    }
}

// Function to display syllabi for all subjects in a given year
void displaySyllabiForYear(int year)
{
    ifstream file("syllabus.txt");
    int fileYear;
    string syllabusFile;

    cout << "\nSyllabus for Year " << year << ":\n";
    while (file >> fileYear >> syllabusFile)
    {
        if (fileYear == year)
        {
            cout << "- " << syllabusFile << endl;

            // Construct the full path to the syllabus (assuming it's stored in the "syllabus" folder)
            string fullPath = "syllabus/" + syllabusFile;

            // Ask the user if they want to open the syllabus PDF
            char choice;
            cout << "Do you want to open the syllabus for Year " << year << "? (y/n): ";
            cin >> choice;

            if (choice == 'y' || choice == 'Y')
            {
                // Open the PDF file using the appropriate system command
                openPDF(fullPath);
            }
        }
    }
}

// Function to display subjects for a given year and semester
void displaySubjects(int year, int semester)
{
    ifstream file("subjects.txt");
    string subjectName;
    int subjectYear, subjectSemester;

    cout << "\nSubjects for Year " << year << " Semester " << semester << ":\n";
    while (file >> subjectName >> subjectYear >> subjectSemester)
    {
        if (subjectYear == year && subjectSemester == semester)
        {
            cout << "- " << subjectName << endl;
        }
    }
}

// Function to display past papers for a given subject
void displayPapers(const string &subjectName, int year, int semester)
{
    ifstream file("papers.txt");
    if (!file)
    {
        cout << "Error: papers.txt file not found.\n";
        return;
    }

    string subject, paperFile;
    int paperYear, paperSemester;

    cout << "\nPast Papers for " << subjectName << " (" << year << " Semester " << semester << "):\n";
    bool foundPaper = false; // Flag to check if we found any paper

    while (file >> subject >> paperYear >> paperSemester >> paperFile)
    {
        if (subject == subjectName && paperYear == year && paperSemester == semester)
        {
            foundPaper = true;
            cout << "- " << paperFile << endl;

            // Ask the user if they want to open the PDF
            char choice;
            cout << "Do you want to open previous year question paper for this? (y/n): ";
            cin >> choice;

            if (choice == 'y' || choice == 'Y')
            {
                // Construct the full path to the paper (assuming papers are stored in the "papers" folder)
                string fullPath = "papers/" + paperFile;

                // Open the PDF file using the appropriate system command
                openPDF(fullPath); // Open the PDF with the correct path
            }
        }
    }

    if (!foundPaper)
    {
        cout << "No past papers found for " << subjectName << " (" << year << " Semester " << semester << ").\n";
    }
}

// Main function
int main()
{
    string prn, password, studentname;

    // Student login
    cout << "Welcome to Edutrack!\n";
    cout << "Please enter your PRN: ";
    cin >> prn;
    cout << "Please enter your password: ";
    cin >> password;

    if (!validateLogin(prn, password, studentname))
    {
        cout << "Invalid PRN or password. Exiting...\n";
        return 0;
    }

    cout << "Login successful! Welcome, " << studentname << ".\n";

    int year, semester;
    string subjectName;

    // Ask for Year once, only in the beginning
    cout << "\nSelect Year (1-4): ";
    cin >> year;
    if (year < 1 || year > 4)
    {
        cout << "Invalid year. Exiting...\n";
        return 0;
    }

    // Display syllabi for the selected year
    displaySyllabiForYear(year);

    // Main loop to allow multiple sessions
    while (true)
    {
        // Ask for semester only
        cout << "\nSelect Semester (1-2): ";
        cin >> semester;
        if (semester < 1 || semester > 2)
        {
            cout << "Invalid semester. Please try again.\n";
            continue;
        }

        // Display subjects for the selected year and semester
        displaySubjects(year, semester);

        // Select a subject
        cout << "\nEnter a subject name to view syllabus and past papers(eg. M1): ";
        cin.ignore(); // To handle the newline character left in the buffer
        getline(cin, subjectName);

        // Display past papers for the selected subject
        displayPapers(subjectName, year, semester);

        // Ask if the user wants to continue
        char choice;
        cout << "\nDo you want to continue with a new semester? (y/n): ";
        cin >> choice;
        if (choice == 'n' || choice == 'N')
        {
            cout << "\nThank you for using Edutrack. Goodbye!\n";
            break; // Exit the loop if the user chooses to quit
        }
    }

    return 0;
}
