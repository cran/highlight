#ifndef __MATCHER_H__
#define __MATCHER_H__

#include <string>
#include <vector>

class Vector;
class NFANode;
class NFAStartNode;
class NFAEndNode;
class NFAGroupHeadNode;
class NFAGroupLoopNode;
class NFAGroupLoopPrologueNode;
class NFAGroupTailNode;
class NFALookBehindNode;
class NFAStartOfLineNode;
class NFAEndOfLineNode;
class NFAEndOfMatchNode;
class NFAReferenceNode;
class Pattern;

/**
  A matcher is a non thread-safe object used to scan strings using a given
  {@link Pattern Pattern} object. Using a <code>Matcher</code> is the preferred
  method for scanning strings. Matchers are not thread-safe. Matchers require
  very little dynamic memory, hence one is encouraged to create several
  instances of a matcher when necessary as opposed to sharing a single instance
  of a matcher.
  <p>
  The most common methods needed by the matcher are <code>matches</code>,
  <code>findNextMatch</code>, and <code>getGroup</code>. <code>matches</code>
  and <code>findNextMatch</code> both return success or failure, and further
  details can be gathered from their documentation.
  <p>
  Unlike Java's <code>Matcher</code>, this class allows you to change the string
  you are matching against. This provides a small optimization, since you no
  longer need multiple matchers for a single pattern in a single thread.
  <p>
  This class also provides an extremely handy method for replacing text with
  captured data via the <code>replaceWithGroups</code> method. A typical
  invocation looks like:
  <pre>
  char buf[10000];
  std::string str = "\\5 (user name \\1) uses \\7 for his/her shell and \\6 is their home directory";
  FILE * fp = fopen("/etc/passwd", "r");
  Pattern::registerPattern("entry", "[^:]+");
  Pattern * p = Pattern::compile("^({entry}):({entry}):({entry}):({entry}):({entry}):({entry}):({entry})$",
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pattern::MULTILINE_MATCHING | Pattern::UNIX_LINE_MODE);
  Matcher * m = p->createMatcher("");
  while (fgets(buf, 9999, fp))
  {
  &nbsp;&nbsp;m->setString(buf);
  &nbsp;&nbsp;if (m->matches())
  &nbsp;&nbsp;{
  &nbsp;&nbsp;&nbsp;&nbsp;printf("%s\n", m->replaceWithGroups(str).c_str());
  &nbsp;&nbsp;}
  }
  fclose(fp);

  </pre>
  Calling any of the following functions before first calling
  <code>matches</code>, <code>findFirstMatch</code>, or
  <code>findNextMatch</code> results in undefined behavior and may cause your
  program to crash.
  <code>
  <ul>
    <li>replaceWithGroups</code>
    <li>getStartingIndex</li>
    <li>getEndingIndex</li>
    <li>getGroup</li>
    <li>getGroups</li>
  </ul>
  </code>
  <p>
  The function <code>findFirstMatch</code> will attempt to find the first match
  in the input string. The same results can be obtained by first calling
  <code>reset</code> followed by <code>findNextMatch</code>.
  <p>
  To eliminate the necessity of looping through a string to find all the
  matching substrings, <code>findAll</code> was created. The function will find
  all matching substrings and return them in a <code>vector</code>. If you need
  to examine specific capture groups within the substrings, then this method
  should not be used.

  @author    Jeffery Stuart
  @since     March 2003, Stable Since November 2004
  @version   1.04
  @memo Mutable object used on instances of a Pattern class
 */
class Matcher
{
  friend class NFANode;
  friend class NFAStartNode;
  friend class NFAEndNode;
  friend class NFAGroupHeadNode;
  friend class NFAGroupLoopNode;
  friend class NFAGroupLoopPrologueNode;
  friend class NFAGroupTailNode;
  friend class NFALookBehindNode;
  friend class NFAStartOfLineNode;
  friend class NFAEndOfLineNode;
  friend class NFAEndOfMatchNode;
  friend class NFAReferenceNode;
  friend class Pattern;
  private:
    /**
      Creates a new matcher object against <code>text</code> using
      <code>pattern</code>.

      @param pattern  The pattern with which to search
      @param text     The text in which to search
     */
    Matcher(Pattern * pattern, const std::string & text);
  protected:
    /// The pattern we use to match
    Pattern * pat;
    /// The string in which we are matching
    std::string str;
    /// The starting point of our match
    int start;
    /// An array of the starting positions for each group
    int * starts;
    /// An array of the ending positions for each group
    int * ends;
    /// An array of private data used by NFANodes during matching
    int * groups;
    /// An array of private data used by NFANodes during matching
    int * groupIndeces;
    /// An array of private data used by NFANodes during matching
    int * groupPos;
    /// The ending index of the last match
    int lm;
    /// The number of capturing groups we have
    int gc;
    /// The number of non-capturing groups we havew
    int ncgc;
    /// Whether or not we have matched something (used only by findFirstMatch and findNextMatch)
    int matchedSomething;
    /// The flags with which we were made
    unsigned long flags;
    /// Called by reset to clear the group arrays
    void clearGroups();
  public:
    /// Used internally by match to signify we want the entire string matched
    const static int MATCH_ENTIRE_STRING;
  public:
    /// Cleans up the dynamic memory used by this matcher
    ~Matcher();
    /**
      Replaces the contents of <code>str</code> with the appropriate captured
      text. <code>str</code> should have at least one back reference, otherwise
      this function does nothing.
      @param str The string in which to replace text
      @return A string with all backreferences appropriately replaced
     */
    std::string replaceWithGroups(const std::string & str);
    /**
      The flags currently being used by the matcher.
      @return Zero
     */
    unsigned long getFlags() const;
    /**
      The text being searched by the matcher.
      @return the text being searched by the matcher.
     */
    std::string getText() const;

    /**
      Scans the string from start to finish for a match. The entire string must
      match for this function to return success. Group variables are
      appropriately set and can be queried after this function returns.

      @return Success if and only if the entire string matches the pattern
     */
    bool matches();
    /**
      Scans the string for the first substring matching the pattern. The entire
      string does not necessarily have to match for this function to return
      success. Group variables are appropriately set and can be queried after
      this function returns.

      @return Success if any substring matches the specified pattern
     */
    bool findFirstMatch();
    /**
      Scans the string for the next substring matching the pattern. If no calls
      have been made to findFirstMatch of findNextMatch since the last call to
      reset, matches, or setString, then this function's behavior results to
      that of findFirstMatch.

      @return Success if another substring can be found that matches the pattern
     */
    bool findNextMatch();
    /**
      Returns a vector of every substring in order which matches the given
      pattern.

      @return Every substring in order which matches the given pattern
     */
    std::vector<std::string> findAll();
    /**
      Resets the internal state of the matcher
     */
    void reset();
    /**
      Same as getText. Left n for backwards compatibilty with old source code
      @return Returns the string that is currently being used for matching
     */
    inline std::string  getString()                           const { return str; }
    /**
      Sets the string to scan
      @param newStr The string to scan for subsequent matches
     */
    inline void         setString(const std::string & newStr)       { str = newStr; reset(); }

    /**
      Returns the starting index of the specified group.
      @param groupNum The group to query
      @return The starting index of the group if it was matched, -1 for an
              invalid group or if the group was not matched
     */
    int getStartingIndex(const int groupNum = 0) const;
    /**
      Returns the ending index of the specified group.
      @param groupNum The group to query
      @return The ending index of the group if it was matched, -1 for an
              invalid group or if the group was not matched
     */
    int getEndingIndex(const int groupNum = 0) const;
    /**
      Returns the specified group. An empty string ("") does not necessarily
      mean the group was not matched. A group such as (a*b?) could be matched by
      a zero length. If an empty string is returned, getStartingIndex can be
      called to determine if the group was actually matched.
      @param groupNum The group to query
      @return The text of the group
     */
    std::string getGroup(const int groupNum = 0) const;
    /**
      Returns every capture group in a vector

      @param includeGroupZero Whether or not include capture group zero
      @return Every capture group
     */
    std::vector<std::string> getGroups(const bool includeGroupZero = 0) const;

    /**
      Number of captured groups
      @return number of captured groups.
    */
    int getGroupNum() {return gc;}

};

#endif
