#include <stdio.h>
#include "os_mem.h"
#include <iostream>
using namespace std;

void checkResult(const char* command, int result, int expected) {
    cout << "command: " << command << " expected: " << expected << ", ";
    if (result == expected) {
        cout << "passed" << endl;
    } else {
        cout << "not passed" << endl;
    }
}

int main() {
    memory_manager_t mm;
    setup_memory_manager(&mm);

    // Common test 1
    checkResult("mm.create(1000, 2);", mm.create(1000, 2), 1);
    checkResult("mm.destroy();", mm.destroy(), 1);
    checkResult("mm.destroy();", mm.destroy(), 0);
    checkResult("mm.create(1000, 1);", mm.create(1000, 1), 1);
    checkResult("mm.create(1000, 3);", mm.create(1000, 3), 0);
    checkResult("mm.destroy();", mm.destroy(), 1);
    cout << "Common Test 1 completed" << endl;

    // Test 1
    checkResult("mm.create(2, 1) != 0;", mm.create(2, 1), 1);
    checkResult("mm.create_object(\"o1\");", mm.create_object("o1"), 1);
    checkResult("mm.create_object(\"o2\");", mm.create_object("o2"), 1);
    checkResult("mm.destroy_object(\"abc\");", mm.destroy_object("abc"), 0);
    checkResult("mm.destroy_object(\"o1\");", mm.destroy_object("o1"), 1);
    checkResult("mm.destroy_object(\"o1\");", mm.destroy_object("o1"), 0);
    checkResult("mm.destroy_object(\"o2\");", mm.destroy_object("o2"), 1);
    checkResult("mm.create_object(\"o1\");", mm.create_object("o1"), 1);
    checkResult("mm.destroy_object(\"o1\");", mm.destroy_object("o1"), 1);
    checkResult("mm.create_object(\"o1\");", mm.create_object("o1"), 1);
    checkResult("mm.create_object(\"o1\");", mm.create_object("o1"), 0);
    checkResult("mm.create_object(\"o2\");", mm.create_object("o2"), 1);
    checkResult("mm.create_object(\"o3\");", mm.create_object("o3"), 0);
    checkResult("mm.destroy_object(\"o1\");", mm.destroy_object("o1"), 1);
    checkResult("mm.destroy();", mm.destroy(), 1);
    cout << "Test 1 passed" << endl;

    // Test 2
    checkResult("mm.create(10, 1) != 0;", mm.create(10, 1), 1);
    checkResult("mm.create_object(\"o9\");", mm.create_object("o9"), 1);
    checkResult("mm.destroy();", mm.destroy(), 1);
    cout << "Test 2 completed" << endl;

	// Test 3
	checkResult("mm.create(10, 1) != 0;", mm.create(10, 1), 1);
	checkResult("mm.create_object(\"o9\");", mm.create_object("o9"), 1);
	checkResult("mm.create_object(\"o1\");", mm.create_object("o1"), 1);
	checkResult("mm.create_object(\"o2\");", mm.create_object("o2"), 1);
	checkResult("mm.create_object(\"o3\");", mm.create_object("o3"), 1);
	checkResult("mm.create_object(\"o4\");", mm.create_object("o4"), 1);
	checkResult("mm.create_object(\"o5\");", mm.create_object("o5"), 1);
	checkResult("mm.create_object(\"o6\");", mm.create_object("o6"), 1);
	checkResult("mm.create_object(\"o7\");", mm.create_object("o7"), 1);
	checkResult("mm.create_object(\"o8\");", mm.create_object("o8"), 1);
	checkResult("mm.create_object(\"o0\");", mm.create_object("o0"), 1);

	checkResult("mm.link(\"o1\", \"1234567\");", mm.link("o1", "1234567"), 0);
	checkResult("mm.link(\"o1\", \"o3\");", mm.link("o1", "o3"), 1);

	checkResult("mm.destroy_object(\"o1\");", mm.destroy_object("o1"), 1);
	checkResult("mm.link(\"o1\", \"o4\");", mm.link("o1", "o4"), 0);
	checkResult("mm.link(\"o2\", \"o4\");", mm.link("o2", "o4"), 1);
	checkResult("mm.link(\"o3\", \"o4\");", mm.link("o3", "o4"), 1);
	checkResult("mm.link(\"o4\", \"o2\");", mm.link("o4", "o2"), 1);
	checkResult("mm.link(\"o4\", \"o6\");", mm.link("o4", "o6"), 1);
	checkResult("mm.link(\"o5\", \"o7\");", mm.link("o5", "o7"), 1);
	checkResult("mm.link(\"o7\", \"o1\");", mm.link("o7", "o1"), 0);
	checkResult("mm.link(\"o4\", \"o8\");", mm.link("o4", "o8"), 1);

	cout << "command: mm.print_objects()" << endl;
	mm.print_objects();

	cout << "command: mm.print_link_counts()" << endl;
	mm.print_link_counts();

	checkResult("mm.destroy_object(\"o4\");", mm.destroy_object("o4"), 1);
	checkResult("mm.destroy_object(\"o2\");", mm.destroy_object("o2"), 1);
	// checkResult("mm.destroy();", mm.destroy(), 1);
	// Добавьте другие тесты по необходимости

	cout << "Test 3 completed" << endl;

    return 0;
}
