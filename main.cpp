//
// Created by virtual on 15/01/20.
//
#include "dependency.h"

using namespace std;

int main(int argc, char ** argv){
  string userName="root";
  string pipedArgument="";
  parseCommandLineArgumets(argc,argv,userName,pipedArgument);
  // cout<<" username "<<userName<<endl;
  // cout<<" piped argument "<<pipedArgument<<endl;
//  for(;optind<argc;++optind){
//    cout<<argv[optind]<<endl;
//  }

   string password=takeinput();
   uid_t Realuid=getuid();
//  cout<<password<<endl;
 if(checkpassword(password) && isSudoer()){//check the password of the user and see if the user is in sudo group
   password.erase();
//    cout<<"everything complete"<<endl;
//
   if(argv[optind]==NULL){
     cout<<"no program found"<<endl;
     return 0;
   }

      vector<string> paths=returnPath(argv);

      // for(int i=0;i<paths.size();++i){
      //   cout<<paths[i]<<endl;
      // }
   	int flag=  change_euid(paths,userName,Realuid);
   executeCommand(paths,flag,argv,Realuid);
 }
 setuid(Realuid);
//
 cout<<"uid and euid after execution"<<endl;
 cout<<" uid "<<getuid()<<"  euid "<<geteuid()<<endl;
  return 0;



}

