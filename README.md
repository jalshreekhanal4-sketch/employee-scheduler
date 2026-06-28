# Employee Schedule Manager

This repository contains two implementations of an employee shift scheduling application:

- `scheduler.py` — Python implementation
- `scheduler.cpp` — C++ implementation

The scheduler manages employee preferences for morning, afternoon, and evening shifts over 7 days, enforces a maximum of 5 work days per employee, and ensures at least 2 employees are assigned per shift each day.

## Files

- `scheduler.py` - Python source file
- `scheduler.cpp` - C++ source file
- `final_schedule.png` - Screenshot of sample schedule output
- `.gitignore` - Ignore compiled binary and temporary output

## Run the Python scheduler

```bash
python3 scheduler.py --demo
```

To run interactively:

```bash
python3 scheduler.py
```

## Build and run the C++ scheduler

```bash
g++ scheduler.cpp -std=c++17 -o scheduler_cpp
./scheduler_cpp
```

## Repository link

https://github.com/jalshreekhanal4-sketch/employee-scheduler

## Notes

- The Python version includes a `--demo` mode for sample employees.
- The C++ version currently runs with sample employee preferences.
- `scheduler_cpp` is ignored by `.gitignore` to keep the repository clean.
