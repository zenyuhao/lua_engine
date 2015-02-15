#include "lua_engine.h"
#include "stdlib.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <ctype.h>
#include <unistd.h>

const char* file = NULL;
const char* code = NULL;

void command(int argc, char* argv[]){

    int c;
    if(argv == NULL) return;
    while((c = getopt(argc, argv, "c:f:")) != -1){
        switch(c){
        case 'c':
            code = optarg;
            break;
        case 'f':
            file = optarg;
            break;
        case '?':
        default:
            exit(1);
            break;
        }
    }

}

int main(int argc, char* argv[]){

    command(argc,argv);

    LuaEngine engine;
    engine.addPackagePath("./");

    if(file){
        fprintf(stderr,"%s\n",file);
        engine.execFile(file);
    }

    if(code){
        fprintf(stderr,"%s\n",code);
        engine.execCode(code);
    }

    return 0;
}
