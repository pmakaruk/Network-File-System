#ifndef COMMAND_H
#define COMMAND_H
#include <string>

struct C{

    std::string command_string;


    int type = -1;
    static const int Invalid_command = -1;
    static const int Get = 1;
    static const int Put = 2;
    static const int Help = 3;
}typedef Command;

Command get_command();

std::string get_command_type(const std::string& s);

#endif