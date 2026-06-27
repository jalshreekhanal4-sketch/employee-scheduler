import argparse
import random
from dataclasses import dataclass, field
from typing import Dict, List

DAYS = ["Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"]
SHIFTS = ["morning", "afternoon", "evening"]
MIN_PER_SHIFT = 2
MAX_DAYS_PER_EMPLOYEE = 5
CAPACITY_PER_SHIFT = 2


@dataclass
class Employee:
    name: str
    preferences: Dict[str, List[str]]
    assigned_days: int = 0
    assignments: Dict[str, str] = field(default_factory=dict)


def collect_preferences() -> List[Employee]:
    employees: List[Employee] = []
    print("Enter employee names and shift preferences.")
    print("Leave the name blank to finish entering employees.")

    while True:
        name = input("Employee name: ").strip()
        if not name:
            break

        preferences: Dict[str, List[str]] = {}
        for day in DAYS:
            print(f"  {day} preferences:")
            day_pref: List[str] = []
            for rank in range(1, len(SHIFTS) + 1):
                while True:
                    pref = input(f"    Rank {rank} shift (morning/afternoon/evening): ").strip().lower()
                    if pref in SHIFTS and pref not in day_pref:
                        day_pref.append(pref)
                        break
                    print("    Invalid or duplicate shift. Try again.")
            preferences[day] = day_pref

        employees.append(Employee(name=name, preferences=preferences))
        print("")

    return employees


def demo_employees() -> List[Employee]:
    return [
        Employee("Alice", {day: ["morning", "afternoon", "evening"] for day in DAYS}),
        Employee("Bob", {day: ["afternoon", "evening", "morning"] for day in DAYS}),
        Employee("Carol", {day: ["evening", "morning", "afternoon"] for day in DAYS}),
        Employee("Dan", {day: ["morning", "evening", "afternoon"] for day in DAYS}),
        Employee("Ella", {day: ["afternoon", "morning", "evening"] for day in DAYS}),
        Employee("Frank", {day: ["morning", "afternoon", "evening"] for day in DAYS}),
        Employee("Gina", {day: ["evening", "afternoon", "morning"] for day in DAYS}),
        Employee("Hugo", {day: ["afternoon", "morning", "evening"] for day in DAYS}),
        Employee("Ivy", {day: ["morning", "evening", "afternoon"] for day in DAYS}),
    ]


def initialize_schedule() -> Dict[str, Dict[str, List[str]]]:
    return {day: {shift: [] for shift in SHIFTS} for day in DAYS}


def can_work(emp: Employee, day: str) -> bool:
    return emp.assigned_days < MAX_DAYS_PER_EMPLOYEE and day not in emp.assignments


def assign_shift(emp: Employee, day: str, shift: str, schedule: Dict[str, Dict[str, List[str]]]) -> None:
    schedule[day][shift].append(emp.name)
    emp.assignments[day] = shift
    emp.assigned_days += 1


def schedule_day(day: str, employees: List[Employee], schedule: Dict[str, Dict[str, List[str]]]) -> None:
    available = [emp for emp in employees if can_work(emp, day)]
    available.sort(key=lambda emp: (emp.assigned_days, emp.name))

    for emp in available:
        for pref in emp.preferences[day]:
            if len(schedule[day][pref]) < CAPACITY_PER_SHIFT:
                assign_shift(emp, day, pref, schedule)
                break

    for shift in SHIFTS:
        while len(schedule[day][shift]) < MIN_PER_SHIFT:
            candidates = [emp for emp in employees if can_work(emp, day) and emp.name not in sum((schedule[day][s] for s in SHIFTS), [])]
            if not candidates:
                break
            candidates.sort(key=lambda emp: (emp.preferences[day].index(shift) if shift in emp.preferences[day] else len(SHIFTS), emp.assigned_days, emp.name))
            assign_shift(candidates[0], day, shift, schedule)


def schedule_week(employees: List[Employee]) -> Dict[str, Dict[str, List[str]]]:
    schedule = initialize_schedule()
    for day in DAYS:
        schedule_day(day, employees, schedule)
    return schedule


def print_schedule(schedule: Dict[str, Dict[str, List[str]]]) -> None:
    print("\nFinal Weekly Schedule:\n")
    for day in DAYS:
        print(f"{day}:")
        for shift in SHIFTS:
            names = schedule[day][shift]
            print(f"  {shift.capitalize()}: {', '.join(names) if names else '(no assignment)'}")
        print("")


def print_employee_summary(employees: List[Employee]) -> None:
    print("Employee assignment summary:")
    for emp in sorted(employees, key=lambda e: e.name):
        assigned_days = len(emp.assignments)
        print(f"  {emp.name}: {assigned_days} days")
    print("")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Employee shift scheduler")
    parser.add_argument("--demo", action="store_true", help="Run the scheduler with sample employees")
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    employees = demo_employees() if args.demo else collect_preferences()
    if not employees:
        print("No employees entered. Exiting.")
        return

    schedule = schedule_week(employees)
    print_schedule(schedule)
    print_employee_summary(employees)


if __name__ == "__main__":
    main()
