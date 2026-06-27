#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

using namespace std;

const vector<string> DAYS = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
const vector<string> SHIFTS = {"morning", "afternoon", "evening"};
const int MIN_PER_SHIFT = 2;
const int MAX_DAYS_PER_EMPLOYEE = 5;
const int CAPACITY_PER_SHIFT = 2;

struct Employee {
    string name;
    map<string, vector<string>> preferences;
    map<string, string> assignments;
    int assigned_days = 0;
};

using Schedule = map<string, map<string, vector<string>>>;

string normalize_shift(string value) {
    transform(value.begin(), value.end(), value.begin(), ::tolower);
    return value;
}

bool valid_shift(const string& shift) {
    return find(SHIFTS.begin(), SHIFTS.end(), shift) != SHIFTS.end();
}

vector<Employee> collect_preferences() {
    vector<Employee> employees;
    cout << "Enter employee names and ranked shift preferences. Leave name blank to use sample data." << endl;
    while (true) {
        cout << "Employee name: ";
        string name;
        getline(cin, name);
        if (name.empty()) break;

        Employee emp;
        emp.name = name;
        for (const string& day : DAYS) {
            cout << "  " << day << " preferences:" << endl;
            vector<string> day_pref;
            while (day_pref.size() < SHIFTS.size()) {
                cout << "    Rank " << day_pref.size() + 1 << " shift (morning/afternoon/evening): ";
                string pref;
                getline(cin, pref);
                pref = normalize_shift(pref);
                if (valid_shift(pref) && find(day_pref.begin(), day_pref.end(), pref) == day_pref.end()) {
                    day_pref.push_back(pref);
                } else {
                    cout << "    Invalid or duplicate shift. Try again." << endl;
                }
            }
            emp.preferences[day] = day_pref;
        }
        employees.push_back(emp);
        cout << endl;
    }
    return employees;
}

vector<Employee> demo_employees() {
    auto make_prefs = [&](const string& first, const string& second, const string& third) {
        map<string, vector<string>> prefs;
        for (const auto& day : DAYS) {
            prefs[day] = {first, second, third};
        }
        return prefs;
    };

    vector<Employee> employees;
    employees.push_back({"Alice", make_prefs("morning", "afternoon", "evening"), {}, 0});
    employees.push_back({"Bob", make_prefs("afternoon", "evening", "morning"), {}, 0});
    employees.push_back({"Carol", make_prefs("evening", "morning", "afternoon"), {}, 0});
    employees.push_back({"Dan", make_prefs("morning", "evening", "afternoon"), {}, 0});
    employees.push_back({"Ella", make_prefs("afternoon", "morning", "evening"), {}, 0});
    employees.push_back({"Frank", make_prefs("morning", "afternoon", "evening"), {}, 0});
    employees.push_back({"Gina", make_prefs("evening", "afternoon", "morning"), {}, 0});
    employees.push_back({"Hugo", make_prefs("afternoon", "morning", "evening"), {}, 0});
    employees.push_back({"Ivy", make_prefs("morning", "evening", "afternoon"), {}, 0});
    return employees;
}

Schedule initialize_schedule() {
    Schedule schedule;
    for (const string& day : DAYS) {
        for (const string& shift : SHIFTS) {
            schedule[day][shift] = {};
        }
    }
    return schedule;
}

bool can_work(const Employee& emp, const string& day) {
    return emp.assigned_days < MAX_DAYS_PER_EMPLOYEE && emp.assignments.find(day) == emp.assignments.end();
}

void assign_shift(Employee& emp, const string& day, const string& shift, Schedule& schedule) {
    schedule[day][shift].push_back(emp.name);
    emp.assignments[day] = shift;
    emp.assigned_days++;
}

void schedule_day(const string& day, vector<Employee>& employees, Schedule& schedule) {
    vector<Employee*> available;
    for (auto& emp : employees) {
        if (can_work(emp, day)) {
            available.push_back(&emp);
        }
    }

    sort(available.begin(), available.end(), [](Employee* a, Employee* b) {
        if (a->assigned_days != b->assigned_days) return a->assigned_days < b->assigned_days;
        return a->name < b->name;
    });

    for (auto* emp : available) {
        for (const auto& pref : emp->preferences.at(day)) {
            if (schedule[day][pref].size() < CAPACITY_PER_SHIFT) {
                assign_shift(*emp, day, pref, schedule);
                break;
            }
        }
    }

    for (const string& shift : SHIFTS) {
        while (schedule[day][shift].size() < MIN_PER_SHIFT) {
            Employee* candidate = nullptr;
            for (auto& emp : employees) {
                if (!can_work(emp, day)) continue;
                if (find(schedule[day][shift].begin(), schedule[day][shift].end(), emp.name) != schedule[day][shift].end()) continue;
                int rank = find(emp.preferences.at(day).begin(), emp.preferences.at(day).end(), shift) - emp.preferences.at(day).begin();
                if (!candidate || rank < find(candidate->preferences.at(day).begin(), candidate->preferences.at(day).end(), shift) - candidate->preferences.at(day).begin() || (rank == find(candidate->preferences.at(day).begin(), candidate->preferences.at(day).end(), shift) - candidate->preferences.at(day).begin() && emp.assigned_days < candidate->assigned_days)) {
                    candidate = &emp;
                }
            }
            if (!candidate) break;
            assign_shift(*candidate, day, shift, schedule);
        }
    }
}

Schedule schedule_week(vector<Employee>& employees) {
    Schedule schedule = initialize_schedule();
    for (const string& day : DAYS) {
        schedule_day(day, employees, schedule);
    }
    return schedule;
}

void print_schedule(const Schedule& schedule) {
    cout << "\nFinal Weekly Schedule:\n";
    for (const string& day : DAYS) {
        cout << day << ":\n";
        for (const string& shift : SHIFTS) {
            cout << "  " << shift << ": ";
            const auto& names = schedule.at(day).at(shift);
            if (!names.empty()) {
                for (size_t i = 0; i < names.size(); i++) {
                    cout << names[i];
                    if (i + 1 < names.size()) cout << ", ";
                }
                cout << "\n";
            } else {
                cout << "(no assignment)\n";
            }
        }
        cout << "\n";
    }
}

void print_summary(const vector<Employee>& employees) {
    cout << "Employee assignment summary:\n";
    for (const auto& emp : employees) {
        cout << "  " << emp.name << ": " << emp.assigned_days << " days\n";
    }
    cout << "\n";
}

int main() {
    cout << "Employee Schedule Manager" << endl;
    vector<Employee> employees = demo_employees();
    Schedule schedule = schedule_week(employees);
    print_schedule(schedule);
    print_summary(employees);
    return 0;
}
