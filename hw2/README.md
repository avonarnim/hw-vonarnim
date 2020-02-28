Adam von Arnim
vonarnim@usc.edu
9081417994

Compile Instructions:
  Use "make" and run using "./gitint-shell".

Design and Approach:
  I used vectors to hold a list of CommitObjs and list of tags, since those were specified as having order.
  I used maps to hold the current file states, tags with their commit associations, and staged files.
  I used a set to hold all the file names.
  I originally wrote the buildState function implicitly in the diffs and checkout function. After writing it, I substituted a call to the buildState function into those functions and the commit function.

Known Problems:
  No known problems.
