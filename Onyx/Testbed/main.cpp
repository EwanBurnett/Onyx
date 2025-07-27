#include <cstdio> 
#include <Onyx.h>
#include <Version.h>

int main() {
    Onyx::Initialize(); 

    printf("Hello, world!\n"); 

    Onyx::Shutdown(); 

    return 0; 
}