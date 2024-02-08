#include "story.hpp"

int main(int argc, char * argv[]) {
  // check the arguments
  if (argc != 2) {
    std::cerr << "Error: wrong number of arguments." << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string directoryPath = argv[1];
  
  try {
    Story story(directoryPath);
    story.checkPages();
    story.begin();
  }
  catch (const std::runtime_error & e) {
    std::cerr << "Error: " << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
