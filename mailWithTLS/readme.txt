copy the entire "openssl-0.9.8l" directory to the root directory of your project 
and add "openssl-0.9.8l\inc32" to "Additional Include Directories",
and also add "openssl-0.9.8l\out32" to "Additional Library Directories".


additional:
   1. in CSmtp.cpp ,there are three OutputDebugStringA() to show message ,when you release your program,you should cut them off.
   2. when you release your exe with static lib ,you should include following code in you .cpp file:
     because the lib libeay32.lib need some function:
     #pragma comment(lib, "User32.lib")
     #pragma comment(lib, "GDI32.lib")
     #pragma comment(lib, "Advapi32.lib")
