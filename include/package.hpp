#include <pthread.h>

struct Package{
    std::string name  /*package name*/, logo /*package logo*/, /*name for display*/ repo, /* git repo */ size, /*size of package*/ discription, /* discription of package */ author, /*author of package*/ 
    installCmd /*cmd for installing (after cloned)*/, deleteCmd; /*cmd for installing (after cloned)*/;      
};

std::vector <Package> packages = {
    {"RSGL","logos/RSGL.png","https://github.com/ColleagueRiley/RSGL.git","~15.0 MB","A very simple GUI library that can be used to make GUIs very\neasily and quickly.Used to make many of the projects on this list","Colleague Riley / RSGL-Org","make install"},
    {"RSGLMake","logos/RSGL-Make.png","https://github.com/RSGL-Org/RSGL-Make.git","142 kB","A easy-to-use build system with c-like-syntax and\nextra functionality for RSGL projects","Colleague Riley / RSGL-Org","make install"},
    {"RSGL-Snake","logos/RSGL-Snake.png","https://github.com/RSGL-Org/RSGL-Snake.git","18.5 kB","A snake clone made with RSGL","Colleague Riley / RSGL-Org","RSGLMake install"},
    {"RSGL-Calculator","logos/RSGL-Calculator.png","https://github.com/RSGL-Org/RSGL-Calculator.git","28.7 MB","A gnome-calculator clone made with RSGL","Colleague Riley / RSGL-Org","RSGLMake install"},
    
    {"RSGL-2048","logos/RSGL-2048.png","https://github.com/RSGL-Org/RSGL-2048.git","60.4 kB","A clone of 2048 made with RSGL, based on gnome-2048","Colleague Riley / RSGL-Org","make install"},
    {"RSGL-Connect4","logos/RSGL-Connect4.png","https://github.com/RSGL-Org/RSGL-Connect4.git","5.6 MB","A simple Connect 4 game, made with RSGL","Colleague Riley / RSGL-Org","make install"},
    {"RSAL","logos/RSAL.png","https://github.com/RSGL-Org/RSAL.git","2.0 MB","A easy-to-use audio library that can be used with and for RSGL OR alone","Colleague Riley / RSGL-Org","make install"},
};

std::vector<std::string> appDir;
bool hasPackage(Package p){for (int i=0; i < appDir.size(); i++){if (appDir.at(i) == p.name){ return true;}} return false; }
static bool CLIWAIT = false;
void reloadApps();

static Package threadP={};
bool checkIfNull(Package* p){ return (p->name.size()!=0&&p->logo.size()!=0&&p->repo.size()!=0&&p->size.size()!=0&&p->discription.size()!=0&&p->author.size()!=0); }
std::string exec(const char* cmd);
void* installThread(void*){ if (!checkIfNull(&threadP)){CLIWAIT=false; return NULL;} std::string cmd= "cd /usr/share/RSGLApps/apps && git clone " + threadP.repo + " "+ threadP.name + " && cd " + threadP.name + " && "+ threadP.installCmd; exec(cmd.c_str()); reloadApps(); CLIWAIT=false; return NULL; }
void* removeThread(void*){ if (!checkIfNull(&threadP)){CLIWAIT=false; return NULL;} std::cout << "cd /usr/share/RSGLApps/apps && rm -r " + threadP.name << std::endl; std::string cmd= "cd /usr/share/RSGLApps/apps && rm -r " + threadP.name; if (threadP.deleteCmd.size()) cmd += " && " + threadP.deleteCmd; std::cout << cmd.c_str() << std::endl; exec(cmd.c_str());  exec(cmd.c_str()); reloadApps(); CLIWAIT=false; return NULL;}
void* updateThread(void*){ if (!checkIfNull(&threadP)){CLIWAIT=false; return NULL;} std::string cmd= "cd /usr/share/RSGLApps/apps/" + threadP.name + " && git pull -f && " + threadP.installCmd; exec(cmd.c_str()); reloadApps(); CLIWAIT=false; return NULL;}

void installPackage(Package p){CLIWAIT=true; threadP=p; pthread_t t; pthread_create(&t,NULL,installThread,NULL);}
void removePackage(Package p){CLIWAIT=true; threadP=p; pthread_t t; pthread_create(&t,NULL,removeThread,NULL);}
void updatePackage(Package p){CLIWAIT=true; threadP=p; pthread_t t; pthread_create(&t,NULL,updateThread,NULL);}