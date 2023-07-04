// Implement all includes and namespaces
#include "cacheCDN.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

// Implement functions and methods here
// In total, there are:
// 1 constructor
// 1 destroyer
// 8 methods

// Implement constructor
Project::CDN::Cache::Cache(unsigned int freshnessCount) {
  this->freshnessCount = freshnessCount;
  textfiles = nullptr;
  binaryfiles = nullptr;
  timesread = 0;
}

// Implement destroyer
Project::CDN::Cache::~Cache() {
  // delete all the files
  while (textfiles != nullptr) {
    TextFile* next = textfiles->next;
    delete textfiles;
    textfiles = next;
  }

  while (binaryfiles != nullptr) {
    BinaryFile* next = binaryfiles->next;
    delete binaryfiles;
    binaryfiles = next;
  }
}

// First method - getText
// Parameter; 1; string filepath
// Description: Can pull data from filepath by reading text in it as linkedlist
std::string Project::CDN::Cache::getText(std::string filepath) {
  TextFile* current = textfiles;

  while (current != nullptr) {
    if (current->filepath == filepath) {
      current->freshness--;
      current->requests++;
      if (current->freshness == 0) {
        // reread the file
        current->text = "";

        std::ifstream inFile;
        std::string text;
        inFile.open(filepath);

        while (getline(inFile, text)) {
          current->text += text;
          current->text += "\n";
        }

        inFile.close();

        current->freshness = freshnessCount;
        timesread++;

        // return the new text
        return current->text;
      } else {
        return current->text;
      }
    } else {
      current = current->next;
    }
  }

  // read the file
  // return the text

  std::ifstream inFile;
  std::string totaltext = "";
  std::string text;
  inFile.open(filepath);

  while (getline(inFile, text)) {
    totaltext += text;
    totaltext += "\n";
  }

  inFile.close();

  current = new TextFile(totaltext, freshnessCount, 1, filepath, textfiles);
  textfiles = current;

  // return the new text
  return current->text;
}

// Second method - getBinary
// Parameter; 1; string filepath
// Description: Essentially same as getText, however, all content inside given
// file is in binary char instead of text
char* Project::CDN::Cache::getBinary(std::string filepath) {
  BinaryFile* current = binaryfiles;

  while (current != nullptr) {
    if (current->filepath == filepath) {
      current->freshness--;
      current->requests++;
      if (current->freshness == 0) {
        // reread the file
        current->text = nullptr;

        std::streampos size;
        char* memblock;
        std::ifstream inBinFile(
            filepath, std::ios::in | std::ios::binary | std::ios::ate);

        if (inBinFile.is_open()) {
          size = inBinFile.tellg();
          memblock = new char[size];
          inBinFile.seekg(0, std::ios::beg);
          inBinFile.read(memblock, size);
          inBinFile.close();
          current->text = memblock;
        }

        current->freshness = freshnessCount;
        timesread++;

        // return the new text
        return current->text;
      } else {
        return current->text;
      }
    } else {
      current = current->next;
    }
  }

  //   // read the file
  //   // return the text

  std::streampos size;
  char* memblock;
  std::ifstream inBinFile(filepath,
                          std::ios::in | std::ios::binary | std::ios::ate);

  if (inBinFile.is_open()) {
    size = inBinFile.tellg();
    memblock = new char[size];
    inBinFile.seekg(0, std::ios::beg);
    inBinFile.read(memblock, size);
    inBinFile.close();
  }

  current = new BinaryFile(memblock, freshnessCount, 1, filepath, binaryfiles);
  binaryfiles = current;

  // return the new text
  return current->text;
}

// Third method - isCached
// Parameter; 1; string filepath
// Description: Returns bool True if the file you received matches a previously
// saved file
bool Project::CDN::Cache::isCached(std::string filepath) {
  TextFile* current = textfiles;

  while (current != nullptr) {
    if (current->filepath == filepath) {
      return true;
    }

    current = current->next;
  }

  BinaryFile* current2 = binaryfiles;

  while (current2 != nullptr) {
    if (current2->filepath == filepath) {
      return true;
    }

    current2 = current2->next;
  }

  return false;
}

// Fourth method - getFreshness
// Parameters; 1; string filepath
// Description: Takes a given file, and returns an unsigned int depicting how
// many requests are left for the file
unsigned int Project::CDN::Cache::getFreshness(std::string filepath) {
  if (freshnessCount == 0 || freshnessCount == 1) {
    return 0;
  }

  TextFile* current = textfiles;

  while (current != nullptr) {
    if (current->filepath == filepath) {
      return current->freshness - 1;
    }

    current = current->next;
  }

  BinaryFile* current2 = binaryfiles;

  while (current2 != nullptr) {
    if (current2->filepath == filepath) {
      return current->freshness - 1;
    }

    current2 = current2->next;
  }

  return 0;
}

// Fifth method - markFileFresh
// Parameters; 1; string filepath
// Description: Recieves a file name, if the file is currently stored in cache,
// reset its freshness to original value
void Project::CDN::Cache::markFileFresh(std::string filepath) {
  TextFile* current = textfiles;

  while (current != nullptr) {
    if (current->filepath == filepath) {
      current->freshness = freshnessCount + 1;
      return;
    }

    current = current->next;
  }

  BinaryFile* current2 = binaryfiles;

  while (current2 != nullptr) {
    if (current2->filepath == filepath) {
      current2->freshness = freshnessCount + 1;
      return;
    }

    current2 = current2->next;
  }
}

// Sixth method - purgeCache
// Parameters; 0; none
// Description: Deletes all currently stored files in your cache, any calls
// after this has class read the file from the disk
void Project::CDN::Cache::purgeCache() {
  // delete all the files
  while (textfiles != nullptr) {
    TextFile* next = textfiles->next;
    delete textfiles;
    textfiles = next;
  }

  while (binaryfiles != nullptr) {
    BinaryFile* next = binaryfiles->next;
    delete binaryfiles;
    binaryfiles = next;
  }

  textfiles = nullptr;
  binaryfiles = nullptr;
}

// Seventh method - topFile
// Parameters; 0; none
// Description: method returns the file thats been requested the most (same
// concept as search engine optimization)
std::string Project::CDN::Cache::topFile() {
  TextFile* textmax = nullptr;
  BinaryFile* binmax = nullptr;
  unsigned int requestsmax = 0;

  TextFile* current = textfiles;

  while (current != nullptr) {
    if (current->requests > requestsmax) {
      requestsmax = current->requests;
      textmax = current;
      binmax = nullptr;
    }

    current = current->next;
  }

  BinaryFile* current2 = binaryfiles;

  while (current2 != nullptr) {
    if (current2->requests > requestsmax) {
      requestsmax = current2->requests;
      binmax = current2;
      textmax = nullptr;
    }

    current2 = current2->next;
  }

  if (requestsmax == 0) {
    return "";
  } else if (textmax == nullptr) {
    return binmax->filepath;
  } else {
    return textmax->filepath;
  }
}

// Eighth method - getStats
// Parameters; 0; none
// Description: method returns a string report of the stats of the cache
std::string Project::CDN::Cache::getStats() {
  unsigned int numrequests = 0;
  unsigned int numfiles = 0;

  std::string topfile = topFile();
  std::stringstream ss;
  ss.precision(2);
  unsigned int toprequests = 0;

  TextFile* current = textfiles;

  while (current != nullptr) {
    if (current->filepath == topfile) {
      toprequests = current->requests;
    }

    numrequests += current->requests;
    numfiles++;
    current = current->next;
  }

  BinaryFile* current2 = binaryfiles;

  while (current2 != nullptr) {
    if (current2->filepath == topfile) {
      toprequests = current2->requests;
    }

    numrequests += current2->requests;
    numfiles++;
    current2 = current2->next;
  }

  ss << "Cache Stats\n----------\n";
  ss << "Total requests: " << std::to_string(numrequests) << "\n";
  ss << "Total files requested: " << std::to_string(numfiles) << "\n";
  if (numfiles == 0) {
    ss << "Average requests per file: "
       << "0.00"
       << "\n";
  } else {
    ss << "Average requests per file: " << (numrequests * 1.0 / numfiles)
       << "\n";
  }
  ss << "Top file: " << topfile << " (" + std::to_string(toprequests)
     << " requests)\n";
  ss << "Total times read from disk: " << std::to_string(timesread)
     << "\n----------\n";
  return ss.str();
}
