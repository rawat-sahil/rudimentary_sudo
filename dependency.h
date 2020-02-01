//
// Created by virtual on 15/01/20.
//

#ifndef SUDO_DEPENDENCY_H
#define SUDO_DEPENDENCY_H
#include <iostream>
#include <string>
#include <unistd.h>
#include<getopt.h>
#include <pwd.h>
#include<shadow.h>
#include <crypt.h>
#include<grp.h>
#include<vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>

std::string takeinput();
bool checkpassword(std::string password);
bool isSudoer();
std::vector<std::string> returnPath(char ** argv);
int change_euid(std::vector<std::string> path,std::string userName,uid_t CurrentUserUid);
void executeCommand(std::vector<std::string> path, int flag,char** argv,uid_t uid);
void parseCommandLineArgumets(int argc, char ** argv,std::string &userName,std::string & pipedArgument);
#endif //SUDO_DEPENDENCY_H
