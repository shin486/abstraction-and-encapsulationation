#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

class Employee {
protected:
    string id;
    string name;
    double salary;

public:
    Employee(string idE, string n, double s) : id(idE), name(n), salary(s) {}

    string getID() { return id; }
    string getName() { return name; }
    virtual double calculateSalary() { return salary; }
    virtual void displayDetails() = 0;
};

class FullTime : public Employee {
public:
    FullTime(string idE, string n, double s) : Employee(idE, n, s) {}

    void displayDetails() override {
        cout << "Employee: " << name << " (ID: " << id << ")\n";
        cout << "Fixed Monthly Salary: $" << salary << "\n\n";
    }
};

class PartTime : public Employee {
    double workHours;

public:
    PartTime(string idE, string n, double s, double wh) : Employee(idE, n, s), workHours(wh) {}

    double calculateSalary() override { return salary * workHours; }

    void displayDetails() override {
        cout << "Employee: " << name << " (ID: " << id << ")\n";
        cout << "Hourly Wage: $" << salary << "\n";
        cout << "Hours Worked: " << workHours << "\n";
        cout << "Total Salary: $" << calculateSalary() << "\n\n";
    }
};

class ContractualEmployee : public Employee {
    double projects;

public:
    ContractualEmployee(string idE, string n, double s, double np) : Employee(idE, n, s), projects(np) {}

    double calculateSalary() override { return salary * projects; }

    void displayDetails() override {
        cout << "Employee: " << name << " (ID: " << id << ")\n";
        cout << "Contract Payment Per Project: $" << salary << "\n";
        cout << "Projects Completed: " << projects << "\n";
        cout << "Total Salary: $" << calculateSalary() << "\n\n";
    }
};

bool isDuplicateID(const vector<Employee*>& employees, const string& empID) {
    for (const auto& emp : employees) {
        if (emp->getID() == empID) return true;
    }
    return false;
}

bool isValidEmployeeID(const string& id) {
    if (id.empty()) return false;
    for (char c : id) {
        if (!isdigit(c)) return false;
    }
    return true;
}

string getValidEmployeeID(const vector<Employee*>& employees) {
    string id;
    bool valid = false;
    
    while (!valid) {
        cout << "Enter Employee ID (Numbers only, no spaces): ";
        cin >> ws;
        getline(cin, id);
        
        if (!isValidEmployeeID(id)) {
            cout << "Error: Employee ID should contain only numbers and no spaces.\n";
            continue;
        }
        if (isDuplicateID(employees, id)) {
            cout << "Duplicate ID!\n";
            continue;
        }
        valid = true;
    }
    return id;
}

bool isValidName(const string& name) {
    if (name.empty()) return false;
    for (char c : name) {
        if (!isalpha(c) && c != ' ') return false;
    }
    return true;
}

string getValidName() {
    string name;
    do {
        cout << "Enter Employee Name: ";
        getline(cin, name);
        if (!isValidName(name)) {
            cout << "Invalid input! Name should contain only letters and spaces.\n";
        }
    } while (!isValidName(name));
    return name;
}

double getValidNumberInput(const string& prompt) {
    string input;
    double number;
    bool valid = false;

    while (!valid) {
        cout << prompt;
        cin >> ws;
        getline(cin, input);

        bool isNumber = true;
        for (char c : input) {
            if (!isdigit(c) && c != '.') {
                isNumber = false;
                break;
            }
        }

        if (!isNumber || input.empty()) {
            cout << "Invalid input! Enter a positive number without letters or spaces.\n";
            continue;
        }

        number = stod(input);

        if (number <= 0) {
            cout << "Value must be greater than 0.\n";
        } else {
            valid = true;
        }
    }
    return number;
}

void displayPayrollReport(const vector<Employee*>& employees) {
    if (employees.empty()) {
        cout << "No employees registered yet.\n";
        return;
    }

    cout << "\n------ Employee Payroll Report ------\n\n";
    for (auto emp : employees) {
        emp->displayDetails();
    }
}

int main() {
    vector<Employee*> employees;
    bool running = true;

    while (running) {
        cout << "\n------ Menu ------\n";
        cout << "1 - Add Full-time Employee\n";
        cout << "2 - Add Part-time Employee\n";
        cout << "3 - Add Contractual Employee\n";
        cout << "4 - Display Payroll Report\n";
        cout << "5 - Exit\n";
        cout << "Enter choice: ";

        string input;
        cin >> ws;
        getline(cin, input);

        if (input.length() != 1 || !isdigit(input[0])) {
            cout << "Invalid choice! Please enter a number between 1 and 5.\n";
            continue;
        }
        
        int choice = input[0] - '0';

        if (choice == 5) {
            running = false;
            continue;
        }
        
        if (choice == 4) {
            displayPayrollReport(employees);
            continue;
        }
        
        string empID = getValidEmployeeID(employees);
        string empName = getValidName();
        double empSalary;
        if (choice == 2) {
            empSalary = getValidNumberInput("Enter Hourly Wage (Numbers only): ");
        } else if (choice == 3) {
            empSalary = getValidNumberInput("Enter Salary per Project (Numbers only): ");
        } else {
            empSalary = getValidNumberInput("Enter Salary (Numbers only): ");
        }

        if (choice == 1) {
            employees.push_back(new FullTime(empID, empName, empSalary));
        } else if (choice == 2) {
            double workHours = getValidNumberInput("Enter Number of Hours Worked: ");
            employees.push_back(new PartTime(empID, empName, empSalary, workHours));
        } else if (choice == 3) {
            double projects = getValidNumberInput("Enter Number of Projects Completed: ");
            employees.push_back(new ContractualEmployee(empID, empName, empSalary, projects));
        }

        cout << "Employee added successfully!\n";
    }

    for (auto emp : employees) delete emp;
    cout << "Program exited successfully.\n";
    return 0;
}