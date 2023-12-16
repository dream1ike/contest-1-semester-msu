#include <stdio.h>
#include "os_file.h"
#include <iostream>

using namespace std;

void checkFileResult(const char* command, int result, int expected) {
    if (result != expected)
    {
        cout << command << " expected: " << expected << ", FAILED" << endl;
    }
    else
    {
        cout << command << " expected: " << expected << ", passed" << endl;
    }
}

void  checkDirResult(const char* command, char* cur_dir, int expected) {
    if (expected) {
        if (strcmp(cur_dir, "/dir1") == 0 || strcmp(cur_dir, "/dir1/") == 0)
            cout << command << " expected: " << expected << ", passed" << endl;
        else
            cout << command << " expected: " << expected << ", not passed" << endl;
    } else {
        cout << command << " expected: " << expected << ", passed" << endl;
    }
}

int main() {

    file_manager_t fm;
    setup_file_manager(&fm);

    // Common file manager test 1
    checkFileResult("fm.create(1000);", fm.create(1000), 1);
    checkFileResult("fm.create_file(\"test_file\",1000+1);", fm.create_file("test_file", 1000 + 1), 0);
    checkFileResult("fm.destroy();", fm.destroy(), 1);
    checkFileResult("fm.destroy();", fm.destroy(), 0);
    checkFileResult("fm.create(1000);", fm.create(1000), 1);
    checkFileResult("fm.create(1000);", fm.create(1000), 0);
    checkFileResult("fm.destroy();", fm.destroy(), 1);
    cout << "Common File Manager Test 1 completed" << endl;
    // Common file manager test 2
    checkFileResult("fm.create(1000);", fm.create(1000), 1);
    checkFileResult("fm.create_file(\"file1.txt\",1000);", fm.create_file("file1.txt", 1000), 1);
    checkFileResult("fm.create_file(\"file2.txt\",1);", fm.create_file("file2.txt", 1), 0);
    checkFileResult("fm.remove(\"file1.txt\",0);", fm.remove("file1.txt", 0), 1);
    checkFileResult("fm.remove(\"file1.txt\",0);", fm.remove("file1.txt", 0), 0);
    checkFileResult("fm.create_file(\"file2.txt\",1000/2);", fm.create_file("file2.txt", 1000 / 2), 1);
    checkFileResult("fm.create_file(\"file2.txt\",1000/2);", fm.create_file("file2.txt", 1000 / 2), 0);
    checkFileResult("fm.create_file(\"file3.txt\",1000/2);", fm.create_file("file3.txt", 1000 / 2), 1);
    checkFileResult("fm.create_dir(\"test_dir\");", fm.create_dir("test_dir"), 1);
    checkFileResult("fm.remove(\"file2.txt\",0);", fm.remove("file2.txt", 0), 1);
    checkFileResult("fm.destroy();", fm.destroy(), 1);
    cout << "Common File Manager Test 2 completed" << endl;
    // Common file manager test 3
    checkFileResult("fm.create(1000);", fm.create(1000), 1);
    checkFileResult("fm.create_dir(\"dir1\");", fm.create_dir("dir1"), 1);
    checkFileResult("fm.create_dir(\"dir1/dir11\");", fm.create_dir("dir1/dir11"), 1);
    cout << "fm.change_dir(\"dir1\");" << (fm.change_dir("dir1") == 1) << endl;
    checkFileResult("fm.create_dir(\"../dir2\");", fm.create_dir("../dir2"), 1);
    checkFileResult("fm.create_dir(\"../dir2/dir3\");", fm.create_dir("../dir2/dir3"), 1);
    checkFileResult("fm.remove(\"/dir2/dir3\", 0);", fm.remove("/dir2/dir3", 0), 1);
    checkFileResult("fm.create_dir(\"/dir3/dir31\");", fm.create_dir("/dir3/dir31"), 0);
    checkFileResult("fm.create_dir(\"../dir3/dir31\");", fm.create_dir("../dir3/dir31"), 0);
    checkFileResult("fm.create_dir(\"../dir2\");", fm.create_dir("../dir2"), 0);
    checkFileResult("fm.create_file(\"/dir2/file1\", 1);", fm.create_file("/dir2/file1", 1), 1);
    checkFileResult("fm.create_dir(\"/dir2/dir21\");", fm.create_dir("/dir2/dir21"), 1);
    checkFileResult("fm.create_dir(\"/dir2/file1\");", fm.create_dir("/dir2/file1"), 0);
    checkFileResult("fm.create_dir(\"../dir2/file1\");", fm.create_dir("../dir2/file1"), 0);
    checkFileResult("fm.create_dir(\"../dir2/file1/dir\");", fm.create_dir("../dir2/file1/dir"), 0);
    checkFileResult("fm.create_dir(\"../dir2/dir22\");", fm.create_dir("../dir2/dir22"), 1);
    checkFileResult("fm.create_dir(\"..\");", fm.create_dir(".."), 0);
    checkFileResult("fm.create_dir(\"../dir2/.\";", fm.create_dir("../dir2/."), 0);
    checkFileResult("fm.change_dir(\"dir2\");", fm.change_dir("dir2"), 0);
    checkFileResult("fm.change_dir(\"dir11\");", fm.change_dir("dir11"), 1);
    checkFileResult("fm.remove(\"../../dir2/file1\", 0);", fm.remove("../../dir2/file1", 0), 1);
    checkFileResult("fm.create_dir(\"../../dir2/file1\");", fm.create_dir("../../dir2/file1"), 1);
    checkFileResult("fm.remove(\"../../dir2/file1\", 0);", fm.remove("../../dir2/file1", 0), 1);
    checkFileResult("fm.create_dir(\"../../dir2/file1\");", fm.create_dir("../../dir2/file1"), 1);
    checkFileResult("fm.remove(\"../../dir2/file1\", 0);", fm.remove("../../dir2/file1", 0), 1);
    checkFileResult("fm.create_file(\"../../dir2/file1\", 1);", fm.create_file("../../dir2/file1", 1), 1);
    checkFileResult("fm.change_dir(\".\");", fm.change_dir("."), 1);
    checkFileResult("fm.change_dir(\"/dir1/dir11\");", fm.change_dir("/dir1/dir11"), 1);
    checkFileResult("fm.change_dir(\"/dir1/dir11/dir3\");", fm.change_dir("/dir1/dir11/dir3"), 0);
    checkFileResult("fm.change_dir(\"/dir1\");", fm.change_dir("/dir1"), 1);
    checkFileResult("fm.change_dir(\"./dir11\");", fm.change_dir("./dir11"), 1);
    checkFileResult("fm.change_dir(\"..\");", fm.change_dir(".."), 1);
    checkFileResult("fm.create_file(\"ddir2/file1\", 1000-1);", fm.create_file("ddir2/file1", 1000 - 1), 0);
    checkFileResult("fm.create_file(\"./dir11/file.txt\", 1);", fm.create_file("./dir11/file.txt", 1), 1);
    checkFileResult("fm.remove(\"dir11\", 0);", fm.remove("dir11", 0), 0);
    checkFileResult("fm.remove(\"./dir11\", 0);", fm.remove("./dir11", 0), 0);
    checkFileResult("fm.remove(\"./dir11\", 1);", fm.remove("./dir11", 1), 1);
    char cur_dir[256];
    fm.get_cur_dir(cur_dir);
    cout << "fm.get_cur_dir(cur_dir)" << cur_dir << endl;
    checkFileResult("fm.destroy();", fm.destroy(), 1);
    return 0;
}
