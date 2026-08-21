Compilation Commands:
AVL Tree:
g++ task1_timing.cpp -o timing

Interval Schedule:
g++ task2_timing.cpp -o timing2


Execution Commands (with timing report generation):
AVL Tree:
./timing testcase_avl.txt output_avl.txt > "timing_report.txt"

Interval Schedule:
./timing2 testcase_large_interval.txt output_large_interval.txt >> "timing_report.txt"