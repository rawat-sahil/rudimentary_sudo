//
// Created by virtual on 15/01/20.
//
#include "dependency.h"
#include <stdio.h>
using namespace std;

void parseCommandLineArgumets(int argc, char ** argv,std::string & userName,std::string &pipedArgument){
  int opt;
  while((opt=getopt(argc,argv,"u:|:p:"))!=-1){
    switch (opt){
    case 'u':
      userName=std::string(optarg);
//      cout<<userName<<endl;
      break;
    case '|':
      pipedArgument=std::string(optarg);
//      cout<<pipedArgument<<endl;
      break;

    case '?' :
      cout<<"argument not found"<<endl;
      setuid(getuid());
      exit(-1);

    }
  }
}




string takeinput(){
  system("stty -echo");
  cout<<"Enter Password:";
  string password; cin>>password;
  cout<<endl;
  system("stty echo");
  return password;
}

bool checkpassword(string password){
  int uid=getuid();
  char * username=getpwuid(uid)->pw_name;
  char* hashed_password=getspnam(username)->sp_pwdp;
//  cout<<uid<<" "<<username<<" "<<hashed_password<<endl;
  const char * char_password=password.c_str();
  char * encrypted=crypt(char_password,hashed_password);
  if(strcmp(hashed_password,encrypted)==0) {
//    cout<<"authentication complete"<<endl;
    return true;}
  cout<<"Incorrect password"<<endl;

//  /////////////////////
  seteuid(getuid());
  cout<<" uid "<<getuid()<<"  euid "<<geteuid()<<endl;
  exit(-1);
}

bool isSudoer(){
  int uid=getuid();
  char * username=getpwuid(uid)->pw_name;
  char ** grp_members=getgrnam("sudo")->gr_mem;
  while(*grp_members!= NULL){
    if(strcmp(username,*grp_members)==0)return true;
    grp_members++;
  }
  cout<<"Not in sudoers. Incident will be reported"<<endl;

  /////////////////////
  seteuid(getuid());
  cout<<"uid "<<getuid()<<"  euid "<<geteuid()<<endl;
  exit(-1);
}

vector<string> returnPath(char **argv){
  vector<string> paths;
  if(argv[optind][0]=='.' && argv[optind][1]=='/'){
    paths.push_back(string(getenv("PWD")).append(string("/").append(string(argv[optind]))));
  }

  else{

    char *pathList = getenv("PATH");
    char *separated;
    separated = strtok(pathList, ":");
    while (separated != NULL) {
      paths.push_back(string(separated));
      separated = strtok(NULL, ":");
//      cout<<separated<<endl;
    }
    for (int i = 0; i < paths.size(); ++i) {
      paths[i].append("/");
      paths[i].append(string(argv[optind]));
    }
    paths.push_back(string(argv[optind]));
  }
  return paths;
}

int change_euid(std::vector<std::string> path,std::string userName,uid_t CurrentUserUid){
  struct passwd *user=getpwnam(userName.c_str());
  if(user == NULL){
  	cout<<"no user with username "<<userName<<" found"<<endl;
  	setuid(CurrentUserUid);
  	exit(0); 
  }

  uid_t userNameUid=user->pw_uid;
  
  string s;
  struct stat statbuf;
  int flag =-1;

  for(int i=0;i<path.size();++i){
    if(stat(path[i].c_str(),&statbuf)==0){
      flag=i;
      break;}
  }
  if(flag==-1){
    cout<<"command not found"<<endl;
    seteuid(getuid());
    exit(0);
  }

  if(userNameUid==statbuf.st_uid){
  	if((statbuf.st_mode & S_IXUSR)==64){
    setuid(statbuf.st_uid);
    cout<<"user same and user bit set"<<endl;
    cout<<"euid changed to the owner"<<endl;
    cout<<" uid "<<getuid()<<"  euid "<<geteuid()<<endl;
  }
  
  else if((statbuf.st_mode & S_IXOTH)==1){
  	setuid(statbuf.st_uid);
  	cout<<"user same and other execution bit set"<<endl;
    cout<<"euid changed to the owner "<<endl;
    cout<<" uid "<<getuid()<<"  euid "<<geteuid()<<endl;
  }
  else{
    cout<<"cannot execute program"<<endl;
    seteuid(getuid());
    cout<<" uid "<<getuid()<<"  euid "<<geteuid()<<endl;
    exit(-1);
  }

  }
  else{
  	if((statbuf.st_mode & S_IXOTH)==1){
  	setuid(CurrentUserUid);
  	cout<<"user not same but execute bit set"<<endl;
    cout<<"uid and euid remain unchanged "<<endl;
    cout<<" uid "<<getuid()<<"  euid "<<geteuid()<<endl;
  }
   else{
    cout<<"cannot execute program"<<endl;
    seteuid(getuid());
    cout<<" uid "<<getuid()<<"  euid "<<geteuid()<<endl;
    exit(-1);
  }
  }

  
  return flag;
}

void executeCommand(std::vector<std::string> path, int flag,char ** argv,uid_t uid){
  optind+=1;
  int i=2;
  while(argv[optind]!=NULL){
    path[flag].append(" ");
    path[flag].append(string(argv[optind]));
    optind++;
  }
  cout<<path[flag]<<endl;
  cout<<"uid and euid before executing system"<<endl;
  cout<<" uid "<<getuid()<<"  euid "<<geteuid()<<endl;
  system(path[flag].c_str());

  setuid(uid);
}

