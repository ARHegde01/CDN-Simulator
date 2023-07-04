#ifndef __CACHECDN_H__
#define __CACHECDN_H__

// All utlised includes and namespaces
#include <fstream>
#include <iostream>
#include <string>

namespace ICS45C {
namespace CDN {

// Class defined here
class Cache {
  // Establish private members of Cache class
 private:
  // struct to hold data pulled from a textfile
  struct TextFile {
    std::string text;
    unsigned int freshness;
    unsigned int requests;
    std::string filepath;
    TextFile* next;

    TextFile(std::string text, unsigned int freshness, unsigned int requests,
             std::string filepath, TextFile* next) {
      this->text = text;
      this->freshness = freshness;
      this->requests = requests;
      this->filepath = filepath;
      this->next = next;
    }

    TextFile() {
      text = "";
      freshness = 0;
      requests = 0;
      filepath = "";
      next = nullptr;
    }
  };
  // struct to hold data pulled from a binary file
  struct BinaryFile {
    char* text;
    unsigned int freshness;
    unsigned int requests;
    std::string filepath;
    BinaryFile* next;

    BinaryFile(char* text, unsigned int freshness, unsigned int requests,
               std::string filepath, BinaryFile* next) {
      this->text = text;
      this->freshness = freshness;
      this->requests = requests;
      this->filepath = filepath;
      this->next = next;
    }

    BinaryFile() {
      text = 0;
      freshness = 0;
      requests = 0;
      filepath = "";
      next = nullptr;
    }

    ~BinaryFile() { delete text; }
  };

  TextFile* textfiles;
  BinaryFile* binaryfiles;
  unsigned int freshnessCount;
  unsigned int timesread;

  // Establish all public members of Cache class
 public:
  Cache(unsigned int freshnessCount);

  ~Cache();

  std::string getText(std::string filepath);

  char* getBinary(std::string filepath);

  bool isCached(std::string filepath);

  unsigned int getFreshness(std::string filepath);

  void markFileFresh(std::string filepath);

  void purgeCache();

  std::string topFile();

  std::string getStats();
};

}  // namespace CDN
}  // namespace ICS45C

#endif  // __CACHECDN_H__