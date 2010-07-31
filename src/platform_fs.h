
/*
This file is part of Highlight.

Highlight is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Highlight is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Highlight.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PLATFORM_FS__H__INCLUDED
#define PLATFORM_FS__H__INCLUDED

#include <string>
#include <iostream>
#include <vector>
/*
#ifdef USE_FN_MATCH
  #include <fnmatch.h>
#endif
*/

namespace Platform
{
	extern const char pathSeparator;

	/* std::string getAppPath(); */

	/** \param fileList Vector where found entries will be stored
	    \param  wildcard Directory path and wildcard
	    \param recursiveSearch Test if directory should be searched recursively */
	bool getDirectoryEntries ( std::vector<std::string> &fileList,
	                           std::string wildcard,
	                           bool recursiveSearch=false );

	void getFileNames ( const std::string &directory,const std::string &wildcard,
	                    std::vector<std::string> &fileName );

	int wildcmp ( const char *wild, const char *data );

	/*
	#ifdef USE_FN_MATCH
	  struct FnMatcher
	  {
	    FnMatcher(const char* pattern, int flags)
	        : pattern_(pattern)
	        , flags_(flags)
	    {}
	    bool operator()(const std::string& e) const {
	        return ! ::fnmatch(pattern_, e.c_str(), flags_);
	    }
	   private:
	    const char* pattern_;
	    int flags_;
	  };
	#endif
	*/

}
#endif
