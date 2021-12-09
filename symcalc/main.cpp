#include "cmdline.hpp" // parse_command
#include "app.hpp" // handle_expr_line
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char *argv[])
{
    vector<Commands::Command> cmd(argc - 1);
    for (int i = 0; i < argc - 1; i++) {
        cmd[i] = parse_command(argv[i+1]);
    }

    string line;
    while (getline(cin, line)) {
        try{
            handle_expr_line(cout, line, cmd);
        } catch (...){
            cout << "! something wrong brou" << endl;
        }
    }
    return 0;
}
