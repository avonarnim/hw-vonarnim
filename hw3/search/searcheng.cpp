#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include "searcheng.h"

using namespace std;

std::string extract_extension(const std::string& filename);
//if i have a seg fault, consider a destructor for combiners

SearchEng::SearchEng(PageParser* noExtensionParser)
{
    if (NULL == noExtensionParser)
    {
        throw std::invalid_argument("default parser cannot be NULL");
    }
    this->noExtensionParser_ = noExtensionParser;

    // Add additional code if necessary
}

SearchEng::~SearchEng()
{
  std::map<std::string, WebPage*>::iterator it;
  for (it = allWebPages.begin(); it != allWebPages.end(); ++it)
    delete it->second;

  std::map<std::string,PageParser*>::iterator pit;
  for (pit = parserMap.begin(); pit != parserMap.end(); ++pit)
    delete pit->second;

  delete noExtensionParser_;

}

void SearchEng::register_parser(const std::string& extension, PageParser* parser)
{
  //might want to make sure the extension is in the correct format??
  parserMap.insert(make_pair(extension, parser));
}

void SearchEng::read_pages_from_index(const std::string& index_file)
{
    ifstream ifile(index_file.c_str());
    if(ifile.fail()) {
        cerr << "Unable to open index file: " << index_file << endl;
    }

    // Parse all the files
    string filename;
    while(ifile >> filename) {
#ifdef DEBUG
        cout << "Reading " << filename << endl;
#endif
        read_page(filename);
    }
    ifile.close();
}

WebPage* SearchEng::retrieve_page(const std::string& page_name) const
{
  std::map<std::string,WebPage*>::const_iterator it = allWebPages.find(page_name);
  if (it == allWebPages.end())
    return NULL;
  else
    {
      WebPage* temp = it->second;
      return temp;
    }
}

void SearchEng::display_page(std::ostream& ostr, const std::string& page_name) const
{
  WebPage* temp = retrieve_page(page_name);
  if (temp == NULL)
    throw std::invalid_argument("page name does not exist");
  string file_extension = extract_extension(page_name);
  //if file_extension != NULL, do the following?
  std::map<std::string,PageParser*>::const_iterator it = parserMap.find(file_extension);
  if (it == parserMap.end())
    noExtensionParser_->display_text(page_name);  //find out if i do noextensionparser vs throw error
    //throw std::logic_error("parser not found");
  else {
    ostr << it->second->display_text(page_name); //i don't know if my display_text function is working properly
    //does it have access to the info it needs? and am i outputting from ostr here or in the shell?
  }
  return;
}

WebPageSet SearchEng::search(const std::vector<std::string>& terms, WebPageSetCombiner* combiner) const
{
  std::map<std::string, WebPageSet>::const_iterator it;
  std::map<std::string, WebPageSet>::const_iterator wit;
  WebPageSet a, b;  //could make a new set of references or smt and iterate through them?
  it = allKeywords.find(terms[0]);  //what if its empty
  if (it != allKeywords.end())
    a = it->second;
  for (int i = 1; i < terms.size(); i++)
  {
    wit = allKeywords.find(terms[i]);  //gets WebPageSet of all pages w/ term i
    a = combiner->combine(a, wit->second);
  }
  return a;
}

void SearchEng::read_page(const std::string& filename)
{
  //keeps track of terms and links in a file being read-in
  std::set<std::string> containedTerms;
  std::set<std::string> outgoingLinks;
  //finds the file extension type
  string file_extension = extract_extension(filename);
  std::map<std::string,PageParser*>::iterator it = parserMap.find(file_extension);
  //parses the file depending on if there exists a file extension type for it
  //exits with containedTerms and outgoingLinks being updated
  if (it == parserMap.end())
    noExtensionParser_->parse(filename, containedTerms, outgoingLinks);
  else {  //parser was found
    it->second->parse(filename, containedTerms, outgoingLinks);
    }
  std::map<std::string, WebPage*>::iterator wit = allWebPages.find(filename);
  if (wit == allWebPages.end())
    {
      WebPage* newInsertion = new WebPage(filename);
      allWebPages.insert(make_pair(filename, newInsertion));
      wit = allWebPages.find(filename);
    }
  wit->second->all_terms(containedTerms); //stores newly parsed webPage's contained terms
  //need to update terms + pages in the allKeywords member
  std::set<std::string>::iterator cit;
  std::map<std::string, WebPageSet>::iterator smit;
  for (cit = containedTerms.begin(); cit != containedTerms.end(); ++cit)
  {
    smit = allKeywords.find(*cit);
    if (smit != allKeywords.end())
      smit->second.insert(wit->second);
    else
      { //note: there could be problems with inserting pointers vs actual WebPages
        WebPageSet a;
        a.insert(wit->second);
        allKeywords.insert(make_pair(*cit, a));
      }
  }
  std::set<std::string>::iterator sit;
  std::map<std::string, WebPage*>::iterator wit2;
  for (sit = outgoingLinks.begin(); sit != outgoingLinks.end(); ++sit)
  {
    //sit = any outgoing link, wit2 = outgoing link destination.
    string s = *sit;
    wit2 = allWebPages.find(s);
    if (wit2 == allWebPages.end())
      {
        WebPage* unmadeWebpage = new WebPage(s);
        allWebPages.insert(make_pair(s, unmadeWebpage));
        wit2 = allWebPages.find(s);
      }
    wit2->second->add_incoming_link(wit->second);
    wit->second->add_outgoing_link(wit2->second);
  }
}


std::string extract_extension(const std::string& filename)
{
    size_t idx = filename.rfind(".");
    if (idx == std::string::npos) {
        return std::string();
    }
    return filename.substr(idx + 1);
}
