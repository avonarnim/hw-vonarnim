#include "cmdhandler.h"
#include "util.h"
#include "msort.h"
#include "combiners.h"

using namespace std;

QuitHandler::QuitHandler()
{

}

QuitHandler::QuitHandler(Handler* next)
    : Handler(next)
{

}

bool QuitHandler::canHandle(const std::string& cmd) const
{
    return cmd == "QUIT";
}

Handler::HANDLER_STATUS_T QuitHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    return HANDLER_QUIT;
}

PrintHandler::PrintHandler()
{

}

PrintHandler::PrintHandler(Handler* next)
    : Handler(next)
{

}

bool PrintHandler::canHandle(const std::string& cmd) const
{
    return cmd == "PRINT";
}

Handler::HANDLER_STATUS_T PrintHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string name;

    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
        eng->display_page(ostr, name);
    }
    catch (std::exception& e) {
        return HANDLER_ERROR;
    }
    return HANDLER_OK;
}

//Incoming implementation
IncomingHandler::IncomingHandler()
{

}

IncomingHandler::IncomingHandler(Handler* next)
    : Handler(next)
{

}

bool IncomingHandler::canHandle(const std::string& cmd) const
{
  return cmd == "INCOMING";
}

Handler::HANDLER_STATUS_T IncomingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
  string name;

  if (!(instr >> name)) {
      return HANDLER_ERROR;
  }
  try {
    const WebPageSet& temp = eng->retrieve_page(name)->incoming_links();
    display_hits(temp, ostr);
  }
  catch (std:: exception& e)  {
    return HANDLER_ERROR;
  }
  return HANDLER_OK;
}

//Outgoing implementation
OutgoingHandler::OutgoingHandler()
{

}

OutgoingHandler::OutgoingHandler(Handler* next)
    : Handler(next)
{

}

bool OutgoingHandler::canHandle(const std::string& cmd) const
{
  return cmd == "OUTGOING";
}

Handler::HANDLER_STATUS_T OutgoingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
  string name;  //error checking: make sure that the names are case-sensitive

  if (!(instr >> name)) {
      return HANDLER_ERROR;
  }
  try {
    const WebPageSet& temp = eng->retrieve_page(name)->outgoing_links();
    display_hits(temp, ostr); //may have a type-casting problem (WebPageSet& to WebPageSet*)?
  }
  catch (std:: exception& e)  {
    return HANDLER_ERROR;
  }
  return HANDLER_OK;
}

//And implementation
AndHandler::AndHandler()
{

}

AndHandler::AndHandler(Handler* next) : Handler(next)
{

}

bool AndHandler::canHandle(const std::string& cmd) const
{
  return cmd == "AND";
}

Handler::HANDLER_STATUS_T AndHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
  std::vector<std::string> terms;
  string temp;
  WebPageSetCombiner* an = new ANDWebPageSetCombiner();
  while(instr >> temp)
  {
    temp = conv_to_lower(temp);
    terms.push_back(temp);
  }

  const WebPageSet a = eng->search(terms, an);
  display_hits(a, ostr);
  delete an;
  return HANDLER_OK;
}

//Diff implementation
DiffHandler::DiffHandler()
{

}

DiffHandler::DiffHandler(Handler* next) : Handler(next)
{

}

bool DiffHandler::canHandle(const std::string& cmd) const
{
  return cmd == "DIFF";
}

Handler::HANDLER_STATUS_T DiffHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
  std::vector<std::string> terms;
  string temp;
  WebPageSetCombiner* diff = new DiffWebPageSetCombiner();
  while(instr >> temp)
  {
    temp = conv_to_lower(temp);
    terms.push_back(temp);
  }

  const WebPageSet a = eng->search(terms, diff);
  display_hits(a, ostr);
  delete diff;
  return HANDLER_OK;
}

//Or implementation
OrHandler::OrHandler()
{

}

OrHandler::OrHandler(Handler* next)
    : Handler(next)
{

}

bool OrHandler::canHandle(const std::string& cmd) const
{
  return cmd == "OR";
}

Handler::HANDLER_STATUS_T OrHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
  std::vector<std::string> terms;
  string temp;
  WebPageSetCombiner* orObj = new ORWebPageSetCombiner();
  while(instr >> temp)
  {
    temp = conv_to_lower(temp);
    terms.push_back(temp);
  }

  const WebPageSet a = eng->search(terms, orObj);
  display_hits(a, ostr);
  delete orObj;
  return HANDLER_OK;
}

struct RankComp {
  bool operator()(const std::pair<WebPage*, double>& lhs, const std::pair<WebPage*, double>& rhs)
  {
    return lhs.second > rhs.second;
  }
};

struct AlphabetComp {
  bool operator()(const std::pair<WebPage*, double>& lhs, const std::pair<WebPage*, double>& rhs)
  {
    return lhs.first->filename() < rhs.first->filename();
  }
};


PrandHandler::PrandHandler()
{}
PrandHandler::PrandHandler(Handler* next) : Handler(next)
{}
bool PrandHandler::canHandle(const std::string& cmd) const
{
  return cmd == "PRAND";
}
Handler::HANDLER_STATUS_T PrandHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
  char preference;
  string temp;
  std::vector<std::string> terms;
  std::vector<std::pair<WebPage*, double>> prVector;
  WebPageSetCombiner* andObj = new ANDWebPageSetCombiner();
  instr >> preference;
  while (instr >> temp)
  {
    temp = conv_to_lower(temp);
    terms.push_back(temp);
  }
  const WebPageSet a = eng->search(terms, andObj);
  prVector = eng->pageRank(a);
  if (preference == 'r' || preference == 'R')
  {
    RankComp compare;
    mergeSort(prVector, compare);
  }
  else if (preference == 'n' || preference == 'N')
  {
    AlphabetComp compare;
    mergeSort(prVector, compare);
  }
  else return HANDLER_ERROR;
  display_ranked_hits(prVector, ostr);
  delete andObj;
  return HANDLER_OK;
}

ProrHandler::ProrHandler()
{

}
ProrHandler::ProrHandler(Handler* next) : Handler(next)
{

}
bool ProrHandler::canHandle(const std::string& cmd) const
{
  return cmd == "PROR";
}
Handler::HANDLER_STATUS_T ProrHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
  char preference;
  string temp;
  std::vector<std::string> terms;
  std::vector<std::pair<WebPage*, double>> prVector;
  WebPageSetCombiner* orObj = new ORWebPageSetCombiner();
  instr >> preference;
  while (instr >> temp)
  {
    temp = conv_to_lower(temp);
    terms.push_back(temp);
  }
  const WebPageSet a = eng->search(terms, orObj);
  prVector = eng->pageRank(a);
  if (preference == 'r' || preference == 'R')
  {
    RankComp compare;
    mergeSort(prVector, compare);
  }
  else if (preference == 'n' || preference == 'N')
  {
    AlphabetComp compare;
    mergeSort(prVector, compare);
  }
  else return HANDLER_ERROR;
  display_ranked_hits(prVector, ostr);
  delete orObj;
  return HANDLER_OK;
}
