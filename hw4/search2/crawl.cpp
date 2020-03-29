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
    ifstream ifile(argv[2].c_str());
    if (ifile.fail())
      cerr << Unable to open source file: << argv[2] << endl;
    ostream ofile(argv[3]);

    //make parsers
    std::map<std::string, PageParser*> parsers;
    parsers.insert(std::make_pair("md", new MDParser));
    parsers.insert(std::make_pair("txt", new TXTParser));

    // crawl all the files
    string filename;
    std::map<std::string, PageParser*>::iterator pit;
    while(ifile >> filename) {
      filename = conv_to_lower(filename);
      string extension = extract_extension(filename);
      pit = parsers.find(extension);
      if (pit != parsers.end())
        it->second->crawl(parsers, filename, processed, ofile);
    }
    ifile.close();
    ofile.close();

    // You may add cleanup code here if necessary
    for (pit = parsers.begin(); pit != parsers.end(); pit++)
    {
      delete parsers[pit]->second;
    }

    return 0;
}
