#include <string>
#include <iostream>
#include "Video.h"
int main(int argc, char** argv){
  if (argc >= 3){
    Video myVideo(argv[1], argv[2]);
    myVideo.create_gif();
  } else std::cout << "Error argument!\n";
  return 0;
}
