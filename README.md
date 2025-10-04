This is the repository for CPSS's onboard camera system

All code here are development code 

Code comitted to this repository should not rely on platform dependent code. (aka Windows, Linux, RPI, TOM should all be able to compile and run this repo)

Targets should be compiled to amd64 or arm x64 architectures

C++ Version is 23

Configure and build libs_sources first before attemping to configure the top level CMakeLists. 
libs_sources contain a CMakeLists file that builds necessary libraries that is required by the rest of the program