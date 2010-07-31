/***************************************************************************
                          codegenerator.h  -  description
                             -------------------
    begin                : Die Jul 9 2002
    copyright            : (C) 2002-2009 by Andre Simon
    email                : andre.simon1@gmx.de
 ***************************************************************************/


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


#ifndef CODEPARSER_H
#define CODEPARSER_H

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

#include "languagedefinition.h"
#include "documentstyle.h"
#include "ctagsreader.h"
#include "astyle/astyle.h"
#include "preformatter.h"
#include "enums.h"
#include "stringtools.h"

/// The highlight namespace contains all classes and data structures needed for parsing input data.

namespace highlight
{
	/** \brief Regular Expession Information

	    This class associates a processing state with a keyword class and the length of the matched token.

	* @author Andre Simon
	*/
	class ReGroup
	{
		public:

			/// Constructor
			ReGroup() : length ( 0 ), state ( STANDARD ), kwClass ( 0 ), name()
			{
			}

			/// Constructor
			ReGroup ( State s, unsigned int l , unsigned int c, const string&n ) :
					length ( l ), state ( s ), kwClass ( c ), name(n)
			{
			}

			/// Copy Constructor
			ReGroup ( const ReGroup& other )
			{
				length = other.length;
				state = other.state;
				kwClass = other.kwClass;
				name=other.name;
			}

			/// Operator overloading
			ReGroup& operator= ( const ReGroup & other )
			{
				length = other.length;
				state = other.state;
				kwClass = other.kwClass;
				name=other.name;
				return *this;
			}

			~ReGroup()
			{
			}

			unsigned int length;    ///< length of the token
			State state;            ///< state of the matched token (keyword, string, etc)
			unsigned int kwClass;   ///< keyword class if state is keyword
			string name;           ///< language name needed to handle embedded languages
	};


	/** \brief Base class for parsing. Works like a finite state machine.

	    The virtual class provides source code parsing functionality, based on
	    information stored in language definitions.<br>
	    The derived classes have to define the output format.<br>
	    The colour information is stored in a DocumentStyle instance.<br>
	    Codegenerator is a singleton class.<br>
	    Use getInstance for a singleton class instance. Then call the init* methods
	    and loadLanguage to initialize the parser. Init methods have to be called first.
	    Call generate* methods to get results.

	* @author Andre Simon
	*/

	class CodeGenerator
	{

		public:

			virtual ~CodeGenerator();

			/**
			  Get appropriate Codegenerator instance (should be used with auto_ptr)
			  \param type Output file type (HTML, XHTML, RTF, LATEX, TEX, ANSI, XTERM256)
			  \return CodeGenerator
			*/
			static CodeGenerator* getInstance ( OutputType type );

			/**
			  Delete CodeGenerator instance (this is intended for SWIG integration only,
			  in normal C++ code the result of getInstance() should be saved in an auto_ptr)
			  \param CodeGenerator* CodeGenerator instance
			*/
			static void deleteInstance ( CodeGenerator* inst ) {if ( inst ) delete inst;}

			/**
			 Define colour theme information; needs to be called before using a generate* method.
			 Call this method before loadLanguage().
			 \param themePath Path of style description file
			 \return true if successfull
			*/
			bool initTheme ( const string& themePath );

			/** initialize source code indentation and reformatting scheme;
			    needs to be called before using a generate* method
			    \param indentScheme Name of indentation scheme
			    \return true if successfull
			 */
			bool initIndentationScheme ( const string&indentScheme );

			/**
			 Load ctags meta information; needs to be called before using a generate* method
			 \param ctagsPath Path of tags file
			 \return true if successfull
			*/
			bool initTagInformation ( const string& ctagsPath );

			/** \param langDefPath Absolute path to language definition, may be used multiple times for a generator instance
			    \return  LOAD_FAILED: failure,
			             LOAD_NEW:    Reload necessary,
			             LOAD_NONE:   no reload necessary
			*/
			LoadResult loadLanguage ( const string& langDefPath );

			/**
			 Generate output file from input file
			 \param inFileName Path of input file (if empty use stdin)
			 \param outFileName Path of output file (if empty use stdout)
			 \return ParseError
			*/
			ParseError generateFile ( const string &inFileName, const string &outFileName );

			/**
			 Generate output string from input string
			 \param input input code string
			 \return formatted output code
			*/
			string generateString ( const string &input );

			/**
			 Generate output string from input file
			 \param inFileName file path
			 \return formatted output code
			*/
			string generateStringFromFile ( const string &inFileName );

			/** Print style definitions to external file or stdout
			  \param outFile Path of external style definition; print to stdout if empty
			   \return true if successfull
			  */
			bool printExternalStyle ( const string &outFile );

			/** Print index file with all input file names
			   \param fileList List of output file names
			   \param outPath Output path
			    \return true if successfull
			 */
			virtual bool printIndexFile ( const vector<string> & fileList,
			                              const string &outPath );


			/** define the preformatting parameters. Preformatting takes place before
			    the optional astyle reformatting and indenting is performed (defined by initIndentationScheme)
			   \param lineWrappingStyle wrapping style (WRAP_DISABLED, WRAP_SIMPLE, WRAP_DEFAULT)
			   \param lineLength max line length
			   \param numberSpaces number of spaces which replace a tab
			*/
			void setPreformatting ( WrapMode lineWrappingStyle, unsigned int lineLength,int numberSpaces );

			/** \deprecated
			    \return True if document style was found */
			bool styleFound();

			/** \return True if reformatting of current input is disabled */
			bool formattingDisabled();

			/** \return True if reformatting of current input is possible */
			bool formattingIsPossible();

			/** \deprecated
			    \param langDefPath Absolute path to language definition; use loadLanguage instead
			    \return  LOAD_FAILED: failure,
			             LOAD_NEW:    Reload necessary,
			             LOAD_NONE:   no reload necessary
			*/
			LoadResult initLanguage ( const string& langDefPath ) { return loadLanguage ( langDefPath );}

			/** \return Language definition*/
			const LanguageDefinition &getLanguage();

			/** output line numbers
			   \param flag true if line numbers should be printed
			   \param startCnt line number starting count
			*/
			void setPrintLineNumbers ( bool flag, unsigned int startCnt=1 );

			/** \return line number flag */
			bool getPrintLineNumbers();

			/** output line numbers filled with zeroes
			    \param  flag true if zeroes should be printed
			*/
			void setPrintZeroes ( bool flag );

			/** \return print zeroes flag */
			bool getPrintZeroes();

			/** omit document header and footer
			   \param  flag true if output should be fragmented
			*/
			void setFragmentCode ( bool flag );

			/** \return fragment flag */
			bool getFragmentCode();

			/** define line number width
			   \param  w width
			*/
			void setLineNumberWidth ( int w );

			/** \return line number width */
			int getLineNumberWidth();

			/** check if input is binary or text
			   \param  flag true if input should be checked
			*/
			void setValidateInput ( bool flag );

			/** \return input validation flag */
			bool getValidateInput();

			/** \return style path */
			const string& getStyleName();

			/** use this font as base font
			\param s the font name, e.g. "Courier New"
			 */
			void setBaseFont ( const string& s );

			/** \return base font */
			const string getBaseFont() const ;

			/** use this size as base font size
			\param s the font size, e.g. "12"
			 */
			void setBaseFontSize ( const string& s );

			/** \return base font size*/
			const string getBaseFontSize();

			/** tell parser the include style definition in output
			    \param flag true if style should be included
			 */
			void setIncludeStyle ( bool flag );

			
			/** tell parser to omit trailing newline character
			    \param flag true if no trailing newline should be printed
			 */
			void disableTrailingNL ( bool flag );
			
			/** Set style input path
			    \param path path to style input file
			  */
			void setStyleInputPath ( const string& path );

			/** Set style output path
			  \param path path to style output file
			*/
			void setStyleOutputPath ( const string& path );

			/** Set encoding (output encoding must match input file)
			  \param encodingName encoding name
			*/
			void setEncoding ( const string& encodingName );

			/** \return style input file path */
			const string&  getStyleInputPath();

			/** \return style output file path */
			const string&  getStyleOutputPath();

			/** \param title Document title */
			void setTitle ( const string & title );

			/** \return Document title */
			string getTitle();

			/** \param cnt maximum number of input lines to be processed */
			void setMaxInputLineCnt ( unsigned int cnt );

			/** \return true if chosen document style has white background colour */
			bool hasWhiteBGColour();

			/** \param keyCase Keyword case */
			void setKeyWordCase ( StringTools::KeywordCase keyCase );

			/** \param lineNo number of line that should be marked
			    \param helpTxt additional help text */
			void addMarkedLine ( int lineNo, string &helpTxt );

			/** \param delim End of line delimiter (default: NL)
			*/
			void setEOLDelimiter(char delim);

			/** Define the name of a nested langage which is located at the beginning of input.
			    The opening embedded delimiter is missing, but the closing delimiter must exist.
				\param langName name of nested language
			*/
			void setStartingNestedLang(const string &langName);


			/** set HTML output anchor flag
			 */
			virtual void setHTMLAttachAnchors ( bool )  {};

			/** set HTML output ordered list flag
			 */
			virtual void setHTMLOrderedList ( bool )  {};

			/** set HTML output inline CSS flag
			 */
			virtual void setHTMLInlineCSS ( bool )  {};

			/** set HTML output enclose pre tag flag
			 */
			virtual void setHTMLEnclosePreTag ( bool )  {};

			/** set HTML output anchor prefix
			 */
			virtual void setHTMLAnchorPrefix ( const string& )  {};

			/** set HTML output class name
			 */
			virtual void setHTMLClassName ( const string& )  {};

			/** set LaTeX replace quotes flag
			 */
			virtual void setLATEXReplaceQuotes ( bool )  {};

			/** set LaTeX no Babel shorthands flag
			 */
			virtual void setLATEXNoShorthands ( bool )  {};

			/** set LaTeX pretty Symbols flag
			 */
			virtual void setLATEXPrettySymbols ( bool )  {};

			/** set RTF page size
			 */
			virtual void setRTFPageSize ( const string& )  {};

			/** set RTF output character styles flag
			 */
			virtual void setRTFCharStyles ( bool )  {};

			/** set SVG page size
			 */
			virtual void setSVGSize ( const string&, const string& )  {};


		protected:

			static const unsigned int NUMBER_BUILTIN_STATES;  ///< number of token states (without keyword group IDs)

			static const string STY_NAME_STD;
			static const string STY_NAME_STR;
			static const string STY_NAME_NUM;
			static const string STY_NAME_SLC;
			static const string STY_NAME_COM;
			static const string STY_NAME_ESC;
			static const string STY_NAME_DIR;
			static const string STY_NAME_DST;
			static const string STY_NAME_LIN;
			static const string STY_NAME_SYM;

			/** \param type Output type */
			CodeGenerator ( highlight::OutputType type );
			CodeGenerator() {};

			/** \param c Character to be masked
			    \return Escape sequence of output format */
			virtual string maskCharacter ( unsigned char c ) = 0;

			/** \param ss destination stream
			    \param s string */
			void maskString ( ostream& ss, const string &s ) ;

			/** \param s Symbol string
			    \param searchPos Position where search starts
			    \return Found state (integer value)  */
			State getState ( const string &s, unsigned int searchPos );

			/** Get current line number
			  \return line number  */
			unsigned int getLineNumber();

			vector <string> openTags,   ///< list of format delimiters (open new format descriptions)
			closeTags;   ///< list of format delimiters (close format descriptions)

			/** Description of document colour style*/
			DocumentStyle docStyle;

			/** Language definition*/
			LanguageDefinition langInfo;

			/** CTags meta information */
			CTagsReader metaInfo;

			/** Tag for inserting line feeds*/
			string newLineTag;

			/** String that represents a white space in output */
			string spacer;

			/** file input*/
			istream *in;

			/** file output*/
			ostream *out;

			string maskWsBegin,  ///< open whitespace mask
			maskWsEnd;    ///< close whitespace mask

			string styleCommentOpen,  ///< open comment delimiter
			styleCommentClose; ///< close comment delimiter

			string embedBlockOpen,  ///< open block delimiter to highlight embedded code
			embedBlockClose; ///< close block delimiter

			/** Encoding name */
			string encoding;

			/** document title */
			string docTitle;

			string inFile,   ///< input file name
			outFile; ///< output file name

			/** Test if maskWsBegin and maskWsEnd should be applied */
			bool maskWs;

			/** Test if whitespace sould always be separated from enclosing tokens */
			bool excludeWs;

			/** Test if header and footer should be omitted */
			bool fragmentOutput;

			/** Test if line numbers should be printed */
			bool showLineNumbers;

			/** Test if leading spyce of line number should be filled with zeroes*/
			bool lineNumberFillZeroes;
			
			/** Flag to test if newlines should be printed */
			bool printNewLines;

			/** The base font to use */
			string baseFont ;

			/** The base font size to use */
			string baseFontSize ;

			/** Current line of input file*/
			string line;

			/** Current line number */
			unsigned int lineNumber;

			/**output line number count start */
			int lineNumberOffset;

			/** Current state*/
			State currentState;

			/** keyword class id, used to apply the corresponding keyword style*/
			unsigned int currentKeywordClass;

			/** Processes origin state */
			void processRootState();

			/** \return line break sequence */
			virtual string getNewLine();

			/**
			   \param s current state
			   \param kwClassID keyword class (has to be set when s=KEYWORD)
			   \return Index of style tag corresponding to the states
			*/
			unsigned int getStyleID ( State s, unsigned int kwClassID = 0 );

			/** \return line index */
			unsigned int getLineIndex();

			/** print all remaining white space*/
			void flushWs();

			/** \return Content of user defined input style */
			string readUserStyleDef();

			/** \return Style definition of the chosen output format */
			virtual string  getStyleDefinition() {return "";};

			/** \return true id encoding is defined */
			bool encodingDefined() {return StringTools::change_case ( encoding ) !="none";}

			/** contains white space, which will be printed after a closing tag */
			string wsBuffer;

			/** Flag to test if style definition should be included in output document */
			bool includeStyleDef;

			/** map which saves all lines that should be highlghted */
			map <int, string> markLines;

			/** Class for line wrapping and tab replacement*/
			PreFormatter preFormatter;

		private:

			CodeGenerator ( const CodeGenerator& ) {}

			CodeGenerator& operator= ( CodeGenerator& ) { return *this;}

			/** Insert line number at the beginning of current output line */
			virtual void insertLineNumber ( bool insertNewLine=true );

			/** Prints document footer
			    @return footer */
			virtual string getFooter() = 0;

			/** Prints document body*/
			virtual void printBody() = 0;

			/** Prints document header
			    @return header
			*/
			virtual string getHeader() = 0;

			/** initialize tags in specific format according to colouring information provided in DucumentStyle */
			virtual void initOutputTags() = 0;

			/** 	\param keyword group id
				\return  open tag  */
			virtual string getKeywordOpenTag ( unsigned int ) = 0;

			/** 	\param keyword group id
				\return  close tag  */
			virtual string getKeywordCloseTag ( unsigned int ) = 0;

			/** return open tag to include ctags meta information
			   \param info tag information of current token
			   \return opening tag
			 */
			virtual string getMetaInfoOpenTag ( const TagInfo& info ) {return "";}

			/** return close tag of meta information
			   \return closing tag
			 */
			virtual string getMetaInfoCloseTag() {return "";}

			/** open a new tag, set current state to s*/
			void openTag ( State s );

			/** close opened tag, clear current state */
			void closeTag ( State s );

			/** close Keyword tag of corresponding style ID */
			void closeKWTag ( unsigned int styleID );

			/** open Keyword tag of corresponding style ID */
			void openKWTag ( unsigned int styleID );

			

			/*void closeTag ( unsigned int styleID );

			void openTag ( unsigned int styleID );*/

			/// path to style definition file
			string themePath;

			/// path to host language definition
			string hostLangDefPath;

			/// path to embedded language definition
			string embedLangDefPath;

			/// name of nested language which starts the input (ie opening delim missing, but closing delim exists)
			string embedLangStart;

			/// contains current position in line
			unsigned int lineIndex;

			/// width of line numbering coloumn
			unsigned int lineNumberWidth;

			/**maximum count of input lines to be processed*/
			unsigned int maxLineCnt;

			/**last character of the last line*/
			unsigned char terminatingChar;

			/** Class for reformatting */
			astyle::ASFormatter *formatter;

			/** Flag to test if formatting is enabled with current input document*/
			bool formattingEnabled;

			/** Flag to test if formatting is possible with current input document*/
			bool formattingPossible;

			/** Flag to test if input should be validated (binary or text) */
			bool validateInput;

			/** Flag to test if ctags information is available */
			bool tagsEnabled;
			
			/** Flag to test if trailing newline should be printed */
			bool noTrailingNewLine;

			/** flag which determines keyword output (unchangeed, uppercase, lowercase)*/
			StringTools::KeywordCase keywordCase;

			/** contains the current token*/
			string token;

			string styleInputPath,   ///< style input file path
			styleOutputPath;  ///< style output file path

			/** end-of-line delimiter*/
			char eolDelimiter;

			/** Resets parser to origin state, call this after every file conversion */
			void reset();

			/** read new line from in stream */
			bool readNewLine ( string &newLine );

			/** return next character from in stream */
			unsigned char getInputChar();

			/** output file type */
			OutputType outputType;

			/** return new state */
			State getCurrentState ();

			/* Methods that represent a parsing state */
			bool processKeywordState ( State myState );  ///< process keywords
			bool processNumberState() ;               ///< process numbers
			bool processMultiLineCommentState();      ///< process multi line comments
			bool processSingleLineCommentState();     ///< process single line comments
			bool processStringState ( State oldState );  ///< process strings
			bool processEscapeCharState();            ///< process escape characters
			bool processDirectiveState();             ///< process directives
			bool processTagState();                   ///< process tags
			bool processSymbolState();                ///< process symbols
			void processWsState();                    ///< process whitespace
			bool processSyntaxChangeState(State myState ); ///< process syntax change of embedded languages

			/** print escaped token and clears it
			   \param addMetaInfo set true if token may have meta information
			   \param flushWhiteSpace set true if white space should be flushed
			   \param tcase keyword case
			*/
			void printMaskedToken ( bool addMetaInfo = false, bool flushWhiteSpace = true,
			                        StringTools::KeywordCase tcase = StringTools::CASE_UNCHANGED );

			/** look for special commands in comments
			    \return true if command was found
			*/
			bool checkSpecialCmd();

			/** association of matched regexes and the corresponding keyword class ids*/
			map <int, ReGroup> regexGroups;

			/** test for regular expressions
			    \param line current input line*/
			void matchRegex ( const string &line );

			/** \return true if input is no binary stream */
			bool validateInputStream();

			void loadEmbeddedLang(const string&embedLangDefPath);

	};

}

#endif
