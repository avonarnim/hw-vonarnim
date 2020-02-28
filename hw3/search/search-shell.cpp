#include <iostream>
#include <fstream>
#include "searcheng.h"
#include "searchui.h"
#include "md_parser.h"
#include "txt_parser.h"
#include "cmdhandler.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cout << "Must provide an index file" << endl;
        return 1;
    }

    // Declare the search engine
    SearchEng seng(new TXTParser);

    // ---------------------- [TO BE COMPLETED] -------------------------------
    // Create and register appropriate parsers for .md and .txt files
    MDParser* mdParse = new MDParser;
    TXTParser* txtParse = new TXTParser;
    seng.register_parser("md", mdParse);
    seng.register_parser("txt", txtParse);

    // Create the SearchUI object
    SearchEng* sengptr = &seng;
    SearchUI ui(sengptr);

    // Create and add appropriate Command Handlers to the SearchUI
    QuitHandler* qHand = new QuitHandler();
    PrintHandler* pHand = new PrintHandler();
    OutgoingHandler* oHand = new OutgoingHandler();
    IncomingHandler* iHand = new IncomingHandler();
    AndHandler* aHand = new AndHandler();
    OrHandler* orHand = new OrHandler();
    DiffHandler* dHand = new DiffHandler();

    ui.add_handler(qHand);
    ui.add_handler(pHand);
    ui.add_handler(oHand);
    ui.add_handler(iHand);
    ui.add_handler(aHand);
    ui.add_handler(orHand);
    ui.add_handler(dHand);

    // ------------------------------------------------------------------------

    try {
        seng.read_pages_from_index(argv[1]);
    }
    catch (std::exception& e) {
        cout << e.what() << endl;
        return 1;
    }

    bool result;
    if (argc == 2)
    {
        result = ui.run(cin, cout);

    }
    else if (argc == 3) {
        ifstream in_file(argv[2]);
        result = ui.run(in_file, cout);
    }
    else  {
        ifstream in_file(argv[2]);
        ofstream out_file(argv[3]);
        result = ui.run(in_file, out_file);
    }
    // You may add cleanup code here though you likely do not
    // need to.

    return result;
}
