#ifndef GITINT_CPP
#define GITINT_CPP

#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "gitint.h"
#include <vector>
#include <set>
#include <string>
#include <map>

using namespace std;

/*********************** Messages to use for errors ***************************/
const std::string INVALID_COMMAND = "Invalid command";
const std::string INVALID_OPTION = "Invalid option";
const std::string INVALID_COMMIT_NUMBER = "Invalid commit number";
const std::string LOG_COMMIT_STARTER = "Commit: ";



// Class implementation

GitInt::GitInt() : saveParent(-1)
{
  std::map<std::string, int> dummyMap;
  CommitObj currentCommit("", dummyMap, saveParent);
  commits_.push_back(currentCommit);
  beginningCommit = true;
}

void GitInt::create(const std::string& filename, int value)
{
  if (beginningCommit)
  {
    head_++;
    beginningCommit = false;
  }
  currentFiles.insert(make_pair(filename, value));
}

void GitInt::edit(const std::string& filename, int value)
{
  if (beginningCommit)
  {
    head_++;
    beginningCommit = false;
  }
  std::map<string, int>::iterator it;
  it = currentFiles.find(filename);
  it->second = value;
}

void GitInt::display(const std::string& filename) const
{
  std::map<string, int>::const_iterator it;
  it = currentFiles.find(filename);
  if (it != currentFiles.end())
    std::cout << it->first << " : " << it->second << std::endl;
  else
    std::cout << "file not present in current files" << std::endl;
}

void GitInt::display_all() const
{
  display_helper(currentFiles);
  /*std::map<string, int>::iterator it;
  for (it = currentFiles.begin(); it != currentFiles.end(); ++it)
  {
    cout << it->first << " : " << it->second << endl;
  }*/
}

void GitInt::add(std::string filename)
{
  std::map<string, int>::iterator it;
  it = currentFiles.find(filename); //do convert currentFiles into diffs here?
  stagedFiles.insert(make_pair(it->first, it->second));
}

void GitInt::commit(std::string message)
{
  std::map<std::string, int> builtFiles = buildState(saveParent);
  std::map<std::string, int>::iterator it;
  std::map<std::string, int>::const_iterator bit;
  for (it = stagedFiles.begin(); it != stagedFiles.end(); ++it)
  {
    bit = builtFiles.find(it->first);
    it->second = it->second - bit->second;
  }
  CommitObj currentCommit(message, stagedFiles, head_);
  commits_.push_back(currentCommit);
  stagedFiles.clear();
  beginningCommit = true;
}

void GitInt::create_tag(const std::string& tagname, CommitIdx commit)
{ //needs to be regulated by whether or not data has been changed at all
  std::map<std::string,CommitIdx>::iterator tagTest = tagsMap.find(tagname);
  if (tagTest != tagsMap.end())
    throw std::invalid_argument(INVALID_OPTION);
  else
  {
    tagsMap.insert(make_pair(tagname, commit));
    tagList.push_back(tagname);
  }
}

void GitInt::tags() const
{
  for (int i = tagList.size()-1; i >= 0; --i)
  {
    cout << tagList[i] << endl;
  }
}

bool GitInt::checkout(CommitIdx commitIndex)
{
  currentFiles =  buildState(commitIndex);
  saveParent = commitIndex;
  return true;
}

bool GitInt::checkout(std::string tag)
{
  std::map<std::string,CommitIdx>::iterator checkoutIt;
  checkoutIt = tagsMap.find(tag);
  CommitIdx commitIndex = checkoutIt->second;
  currentFiles = buildState(commitIndex);
  saveParent = commitIndex;
  return true;
}

void GitInt::log() const
{
  CommitIdx i = head_;
  CommitObj trackObj(commits_[i].msg_, commits_[i].diffs_, i);
  do {
    trackObj = commits_[i];
    log_helper(i,trackObj.msg_);
    i = trackObj.parent_;
  } while (trackObj.parent_ != 0);
}

void GitInt::diff(CommitIdx to) const
{
  if (head_ < 1) //had to
    return;
  std::map<std::string, int> built = buildState(to);  //had buildState(i)
  int largeAdd = 0;
  int smallAdd = 0;
  std::map<std::string,int>::const_iterator it;
  for (it = currentFiles.begin(); it != currentFiles.end(); ++it)
  {
    largeAdd = it->second;
    std::map<std::string,int>::iterator cit = built.find(it->first);
    if (cit != built.end())
    {
      smallAdd = cit->second;
      cout << it->first << " : " << (largeAdd-smallAdd) << endl;
    }
    else
      cout << it->first << " : " << (largeAdd) << endl;
  }
}

void GitInt::diff(CommitIdx from, CommitIdx to) const
{
  if (from < 1)
    return;
  std::map<std::string, int> builtFrom = buildState(from);
  std::map<std::string, int> builtTo = buildState(to);
  int largeAdd = 0;
  int smallAdd = 0;
  std::map<std::string,int>::iterator it;
  for (it = builtFrom.begin(); it != builtFrom.end(); ++it)
  {
    largeAdd = it->second;
    std::map<std::string,int>::iterator cit = builtTo.find(it->first);
    if (cit != builtTo.end())
    {
      smallAdd = cit->second;
      cout << it->first << " : " << (largeAdd-smallAdd) << endl;
    }
    else
      cout << it->first << " : " << (largeAdd) << endl;
  }
}

bool GitInt::valid_commit(CommitIdx commit) const
{
  if (commit < commits_.size())
    return true;
  else
    return false;
}

//sums up all diffs_ values for all files in a lineage
//first inserts the files and values starting at the "from" index
//continues adding to those files down up the lineage
std::map<std::string, int> GitInt::buildState(CommitIdx from, CommitIdx to) const
{
  std::map<std::string, int> retMap;
  //std::map<std::string, int>::iterator j;
  //std::map<std::string, int>::iterator k;
  CommitIdx i = commits_[from].parent_;
  for (std::map<std::string, int>::iterator j = commits_[from].diffs_.begin(); j != commits_[from].diffs_.end(); ++j)
      {
        retMap.insert(std::make_pair(j->first, j->second));
        while (commits_[i].parent_ != to)
        {
          std::map<std::string, int>::iterator k = commits_[i].diffs_.find(j->first);
          j->second += k->second;
          i = commits_[i].parent_;
        }
      }
  return retMap;
}

void GitInt::print_menu() const
{
    cout << "Menu:                          " << endl;
    cout << "===============================" << endl;
    cout << "create   filename int-value    " << endl;
    cout << "edit     filename int-value    " << endl;
    cout << "display  (filename)            " << endl;
    cout << "display  commit-num            " << endl;
    cout << "add      file1 (file2 ...)     " << endl;
    cout << "commit   \"log-message\"       " << endl;
    cout << "tag      (-a tag-name)         " << endl;
    cout << "log                            " << endl;
    cout << "checkout commit-num/tag-name   " << endl;
    cout << "diff                           " << endl;
    cout << "diff     commit                " << endl;
    cout << "diff     commit-n commit-m     " << endl;
}


bool GitInt::process_command(std::string cmd_line)
{
    bool quit = false;
    std::stringstream ss(cmd_line);
    std::string cmd;
    ss >> cmd;
    std::string input1;
    if (ss.fail()) throw std::runtime_error(INVALID_COMMAND);

    if (cmd == "quit") {
        quit = true;
        return quit;
    }
    // Continue checking/processing commands
    std::string input2 = NULL, input3 = NULL;
    ss >> input1;
    if (!ss.fail())
    {
      ss >> input2;
      if (ss.fail())
        {
          if (input1 == "display")
            display_all();
          else if (input1 == "tag")
            tags();
          else if (input1 == "log")
            log();
          else if (input1 == "diff")
            diff(head_);
          else if (input1 == "checkout")
            throw std::runtime_error(INVALID_COMMIT_NUMBER);
        }
      ss >> input3;
      if (ss.fail())
        {
          if (input1 == "diff")
            diff(stoi(input2));
          else if (input1 == "display")
            display(input2);
          else if (input1 == "add")
            add(input2);
          else if (input1 == "commit")
            commit(input2);
          else if (input1 == "checkout")
          {
            if (valid_commit(stoi(input2)))
              checkout(input2);
            else
              checkout((string)input2);
          }
          else if (input1 == "tag")
            if (input2 == "-a")
              throw std::runtime_error(INVALID_COMMAND);
        }
      else
        {
          if (input1 == "create")
            create(input2, stoi(input3));
          else if (input1 == "edit")
            edit(input2, stoi(input3));
          else if (input1 == "diff")
            diff(stoi(input2), stoi(input3));
          else if (input1 == "tag")
            if (input2 == "-a")
              create_tag(input3, head_);
            else
              {
                throw std::runtime_error(INVALID_COMMAND);
              }
          else if (input1 == "add")
            {
              add(input2);
              while (!ss.fail())
              {
                add(input3);
                ss >> input3;

              }
            }
        }
    }
    return quit;
}

void GitInt::display_commit(CommitIdx commit) const
{
    if ( false == valid_commit(commit) ) {
        throw std::invalid_argument(INVALID_COMMIT_NUMBER);
    }
    display_helper(commits_[commit].diffs_);
}


void GitInt::display_helper(const std::map<std::string, int>& dat) const
{
    for (std::map<std::string, int>::const_iterator cit = dat.begin();
            cit != dat.end();
            ++cit) {
        std::cout << cit->first << " : " << cit->second << std::endl;
    }
}


void GitInt::log_helper(CommitIdx commit_num, const std::string& log_message) const
{
    std::cout << LOG_COMMIT_STARTER << commit_num << std::endl;
    std::cout << log_message << std::endl << std::endl;

}

#endif
