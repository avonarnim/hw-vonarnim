#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include "md_parser.h"
#include "txt_parser.h"
#include "util.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 3) {
        cout << "Must provide an index file and output file" << endl;
        return 1;
    }

    // ---------------------- [TO BE COMPLETED] -------------------------------
    ifstream ifile(argv[2]);
    if (ifile.fail())
      cerr << "Unable to open source file: " << argv[2] << endl;
    ofstream outfile(argv[3]);

    //make parsers
    std::map<std::string, PageParser*> parsers;
    parsers.insert(std::make_pair("md", new MDParser));
    parsers.insert(std::make_pair("txt", new TXTParser));
    //holds all filenames that've been processed
    std::set<std::string> processed;

    // crawl all the files
    string filename;
    std::map<std::string, PageParser*>::iterator pit;
    while(ifile >> filename) {
      filename = conv_to_lower(filename);
      string extension = extract_extension(filename);
      pit = parsers.find(extension);
      if (pit != parsers.end())
        pit->second->crawl(parsers, filename, processed, outfile);
    }
    ifile.close();
    outfile.close();

    // You may add cleanup code here if necessary
    for (pit = parsers.begin(); pit != parsers.end(); pit++)
    {
      delete pit->second;
    }

    return 0;
}
