/***************************************************************************
                     htmlgenerator.cpp  -  description
                             -------------------
    begin                : Wed Nov 28 2001
    copyright            : (C) 2001-2007 by Andre Simon
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


#include <fstream>
#include <iostream>
#include <sstream>

#include "htmlgenerator.h"
#include "version.h"

using namespace std;

namespace highlight
{

	HtmlGenerator::HtmlGenerator () :
			CodeGenerator ( HTML ),
			brTag ( "<br>" ),
			hrTag ( "<hr>" ),
			idAttr ( "name" ),
			fileSuffix ( ".html" ),
			cssClassName ( "hl" ),
			orderedList ( false ),
			useInlineCSS ( false ),
			enclosePreTag ( false ),
			attachAnchors ( false ),
			anchorPrefix ( "l" )
	{
		spacer = " ";
		styleCommentOpen="/*";
		styleCommentClose="*/";
	}

	string HtmlGenerator::getHeader()
	{
		ostringstream os;
		os << getHeaderStart ( docTitle );
		if ( !useInlineCSS )
		{
			if ( includeStyleDef )
			{
				os << "<style type=\"text/css\">\n<!--\n";
				os << getStyleDefinition();
				os << CodeGenerator::readUserStyleDef();
				os << "//-->\n</style>\n";
			}
			else
			{
				os << "<link rel=\"stylesheet\" type=\"text/css\" href=\""
				<< getStyleOutputPath()
				<< "\">\n";
			}
			os << "</head>\n<body class=\"" << cssClassName
			<< "\">\n";
		}
		else
		{
			os << "</head>\n<body style=\""
			<< "background-color:#"
			<< ( docStyle.getBgColour().getRed ( HTML ) )
			<< ( docStyle.getBgColour().getGreen ( HTML ) )
			<< ( docStyle.getBgColour().getBlue ( HTML ) )
			<< "\">\n";
		}

		return os.str();
	}

	string HtmlGenerator::getFooter()
	{
		return getGeneratorComment();
	}

	void HtmlGenerator::printBody()
	{
		if ( !fragmentOutput || enclosePreTag )
		{
			if ( !useInlineCSS )
			{
				*out << "<pre class=\"" << cssClassName
				<< "\">";
			}
			else
			{
				*out << "<pre style=\""
				<< "color:#"
				<< ( docStyle.getDefaultStyle().getColour().getRed ( HTML ) )
				<< ( docStyle.getDefaultStyle().getColour().getGreen ( HTML ) )
				<< ( docStyle.getDefaultStyle().getColour().getBlue ( HTML ) )
				<< "; background-color:#"
				<< ( docStyle.getBgColour().getRed ( HTML ) )
				<< ( docStyle.getBgColour().getGreen ( HTML ) )
				<< ( docStyle.getBgColour().getBlue ( HTML ) )
				<< "; font-size:" << this->getBaseFontSize()
				<< "pt; font-family:'" << this->getBaseFont() <<"';\">";
			}
		}
		if ( showLineNumbers && orderedList ) *out << "<ol>";

		processRootState();

		if ( showLineNumbers && orderedList ) *out << "\n</ol>";

		if ( !fragmentOutput || enclosePreTag )  *out << "</pre>";
	}


	void HtmlGenerator::initOutputTags ()
	{
		openTags.push_back ( "" );
		if ( useInlineCSS )
		{
			//embedBlockOpen = "<div  style=\"background-color:#efefef;\">";
			openTags.push_back ( getOpenTag ( docStyle.getStringStyle() ) );
			openTags.push_back ( getOpenTag ( docStyle.getNumberStyle() ) );
			openTags.push_back ( getOpenTag ( docStyle.getSingleLineCommentStyle() ) );
			openTags.push_back ( getOpenTag ( docStyle.getCommentStyle() ) );
			openTags.push_back ( getOpenTag ( docStyle.getEscapeCharStyle() ) );
			openTags.push_back ( getOpenTag ( docStyle.getDirectiveStyle() ) );
			openTags.push_back ( getOpenTag ( docStyle.getDirectiveStringStyle() ) );
			openTags.push_back ( getOpenTag ( docStyle.getLineStyle() ) );
			openTags.push_back ( getOpenTag ( docStyle.getSymbolStyle() ) );
		}
		else
		{
			//embedBlockOpen = "<div  style=\"background-color:#efefef;\">";
			openTags.push_back ( getOpenTag ( STY_NAME_STR ) );
			openTags.push_back ( getOpenTag ( STY_NAME_NUM ) );
			openTags.push_back ( getOpenTag ( STY_NAME_SLC ) );
			openTags.push_back ( getOpenTag ( STY_NAME_COM ) );
			openTags.push_back ( getOpenTag ( STY_NAME_ESC ) );
			openTags.push_back ( getOpenTag ( STY_NAME_DIR ) );
			openTags.push_back ( getOpenTag ( STY_NAME_DST ) );
			openTags.push_back ( getOpenTag ( STY_NAME_LIN ) );
			openTags.push_back ( getOpenTag ( STY_NAME_SYM ) );
		}

		closeTags.push_back ( "" );
		for ( unsigned int i=1;i<NUMBER_BUILTIN_STATES; i++ )
		{
			closeTags.push_back ( "</span>" );
		}
		//embedBlockClose = "</div>";
	}

	string  HtmlGenerator::getAttributes ( const string & elemName, const ElementStyle & elem )
	{
		ostringstream s;
		if ( !elemName.empty() )
		{
			s << "."<<cssClassName<<"."<<elemName<<" { ";
		}
		s << "color:#"
		<< ( elem.getColour().getRed ( HTML ) )
		<< ( elem.getColour().getGreen ( HTML ) )
		<< ( elem.getColour().getBlue ( HTML ) )
		<< ( elem.isBold() ?     "; font-weight:bold" :"" )
		<< ( elem.isItalic() ?   "; font-style:italic" :"" )
		<< ( elem.isUnderline() ? "; text-decoration:underline" :"" );
		if ( !elemName.empty() )
		{
			s << "; }\n" ;
		}
		return s.str();
	}

	string  HtmlGenerator::getOpenTag ( const string& styleName )
	{
		return "<span class=\""+cssClassName+ " " + styleName + "\">";
	}

	string  HtmlGenerator::getOpenTag ( const ElementStyle & elem )
	{
		return "<span style=\""+getAttributes ( "",elem ) + "\">";
	}

	string HtmlGenerator::getGeneratorComment()
	{
		ostringstream s;
		s <<"\n</body>\n</html>\n<!--HTML generated by highlight "
		<< HIGHLIGHT_VERSION << ", " <<  HIGHLIGHT_URL <<"-->\n";

		return s.str();
	}

	string HtmlGenerator::getStyleDefinition()
	{
		if ( styleDefinitionCache.empty() )
		{
			ostringstream os;
			os << "body."<<cssClassName<<"\t{ background-color:#"
			<< ( docStyle.getBgColour().getRed ( HTML ) )
			<< ( docStyle.getBgColour().getGreen ( HTML ) )
			<< ( docStyle.getBgColour().getBlue ( HTML ) )
			<< "; }\n";
			os << "pre."<<cssClassName<<"\t{ color:#"
			<< ( docStyle.getDefaultStyle().getColour().getRed ( HTML ) )
			<< ( docStyle.getDefaultStyle().getColour().getGreen ( HTML ) )
			<< ( docStyle.getDefaultStyle().getColour().getBlue ( HTML ) )
			<< "; background-color:#"
			<< ( docStyle.getBgColour().getRed ( HTML ) )
			<< ( docStyle.getBgColour().getGreen ( HTML ) )
			<< ( docStyle.getBgColour().getBlue ( HTML ) )
			<< "; font-size:" << this->getBaseFontSize();

			os << "pt; font-family:'" << this->getBaseFont() << "';}\n";

			if ( orderedList )
			{
				os << "li."<<cssClassName<<"\t{ margin-bottom:-"<<this->getBaseFontSize() <<"pt; }\n";
			}

			os << getAttributes ( STY_NAME_NUM, docStyle.getNumberStyle() )
			<< getAttributes ( STY_NAME_ESC, docStyle.getEscapeCharStyle() )
			<< getAttributes ( STY_NAME_STR, docStyle.getStringStyle() )
			<< getAttributes ( STY_NAME_DST, docStyle.getDirectiveStringStyle() )
			<< getAttributes ( STY_NAME_SLC, docStyle.getSingleLineCommentStyle() )
			<< getAttributes ( STY_NAME_COM, docStyle.getCommentStyle() )
			<< getAttributes ( STY_NAME_DIR, docStyle.getDirectiveStyle() )
			<< getAttributes ( STY_NAME_SYM, docStyle.getSymbolStyle() )
			<< getAttributes ( STY_NAME_LIN, docStyle.getLineStyle() );

			os << "."<<cssClassName<<".mark\t{ background-color:#"
			<< ( docStyle.getMarkLineColour().getRed ( HTML ) )
			<< ( docStyle.getMarkLineColour().getGreen ( HTML ) )
			<< ( docStyle.getMarkLineColour().getBlue ( HTML ) )
			<< ";}\n";

			KeywordStyles styles = docStyle.getKeywordStyles();
			for ( KSIterator it=styles.begin(); it!=styles.end(); it++ )
			{
				os << getAttributes ( it->first, it->second );
			}
			styleDefinitionCache=os.str();
		}
		return styleDefinitionCache;
	}

	string HtmlGenerator::maskCharacter ( unsigned char c )
	{
		switch ( c )
		{
			case '<' :
				return "&lt;";
				break;
			case '>' :
				return "&gt;";
				break;
			case '&' :
				return "&amp;";
				break;
			case '\"' :
				return "&quot;";
				break;

			case '@' :
				return "&#64;";
				break;

			default :
				return string ( 1, c );
		}
	}

	string HtmlGenerator::getNewLine()
	{

		string nlStr;

		if ( markLines.count ( lineNumber-1 ) ) nlStr +="</span>";

		if ( showLineNumbers && orderedList ) nlStr +="</li>";
		/// set wrapping arrow if previous line was wrapped
		//else if (preFormatter.isWrappedLine(lineNumber-1)) nlStr += "&crarr;";

		if (printNewLines) nlStr+="\n";
		
		return nlStr;
	}

	void HtmlGenerator::insertLineNumber ( bool insertNewLine )
	{
		if ( insertNewLine )
		{
			wsBuffer += getNewLine();
		}
		if ( showLineNumbers )
		{
			ostringstream numberPrefix;
			int lineNo = lineNumber+lineNumberOffset;
			if ( orderedList )
			{
				if ( useInlineCSS )
				{
					numberPrefix<<"<li style=\""<<getAttributes ( "", docStyle.getLineStyle() ) <<"\">";
				}
				else
				{
					numberPrefix<<"<li class=\""<<cssClassName<<"\">";
				}
				// Opera 8 ignores empty list items -> add &nbsp;
				if ( line.empty() ) numberPrefix<<"&nbsp;";
			}
			if ( attachAnchors )
				numberPrefix << "<a "
				<< idAttr
				<< "=\""
				<< anchorPrefix
				<< "_"
				<< lineNo
				<< "\"></a>";

			if ( !orderedList )
			{
				ostringstream os;
				if ( lineNumberFillZeroes ) os.fill ( '0' );
				os <<setw ( getLineNumberWidth() ) <<right<< lineNo;
				numberPrefix << openTags[LINENUMBER]
				<< os.str()
				<< spacer
				<< closeTags[LINENUMBER];
			}
			wsBuffer += numberPrefix.str();
		}

		if ( markLines.count ( lineNumber ) )
		{
			if ( useInlineCSS )
			{
				ostringstream markingFmt;
				markingFmt <<"<span style=\""
				<<"background-color:#"
				<< ( docStyle.getMarkLineColour().getRed ( HTML ) )
				<< ( docStyle.getMarkLineColour().getGreen ( HTML ) )
				<< ( docStyle.getMarkLineColour().getBlue ( HTML ) )
				<< ";\"";
				wsBuffer+=markingFmt.str();
			}
			else
			{
				wsBuffer +="<span class=\""+cssClassName+" mark\"";
			}
			if ( !markLines[lineNumber].empty() )
				wsBuffer +=" title=\""+markLines[lineNumber]+"\"";
			wsBuffer +=">";
		}

	}

	string HtmlGenerator::getHeaderStart ( const string &title )
	{
		ostringstream header;
		header<<  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">"
		<< "\n<html>\n<head>\n";
		if ( encodingDefined() )
		{
			header << "<meta http-equiv=\"content-type\" content=\"text/html; charset="
			<< encoding
			<< "\">\n";
		}
		header << "<title>"
		<< title
		<< "</title>\n";
		return header.str();
	}

	bool HtmlGenerator::printIndexFile ( const vector<string> &fileList,
	                                     const string &outPath )
	{
		string suffix = fileSuffix;
		string outFilePath = outPath + "index" + suffix;
		ofstream indexfile ( outFilePath.c_str() );

		if ( !indexfile.fail() )
		{
			string inFileName;
			string inFilePath, newInFilePath;
			indexfile << getHeaderStart ( "Source Index" );
			indexfile << "</head>\n<body>\n<h1> Source Index</h1>\n"
			<< hrTag
			<<  "\n<ul>\n";
			string::size_type pos;
			for ( unsigned int i=0; i < fileList.size();  i++ )
			{
				pos= ( fileList[i] ).find_last_of ( Platform::pathSeparator );
				if ( pos!=string::npos )
				{
					newInFilePath = ( fileList[i] ).substr ( 0, pos+1 );
				}
				else
				{
					newInFilePath=Platform::pathSeparator;
				}
				if ( newInFilePath!=inFilePath )
				{
					indexfile << "</ul>\n<h2>";
					indexfile << newInFilePath;
					indexfile << "</h2>\n<ul>\n";
					inFilePath=newInFilePath;
				}
				inFileName = ( fileList[i] ).substr ( pos+1 );
				indexfile << "<li><a href=\"" << inFileName << suffix << "\">";
				indexfile << inFileName << suffix <<"</a></li>\n";
			}

			indexfile << "</ul>\n"
			<< hrTag << brTag
			<< "<small>Generated by highlight "
			<< HIGHLIGHT_VERSION
			<< ", <a href=\"" << HIGHLIGHT_URL << "\" target=\"new\">"
			<< HIGHLIGHT_URL << "</a></small>";
			indexfile << getGeneratorComment();
		}
		else
		{
			return false;
		}
		return true;
	}

	string HtmlGenerator::getKeywordOpenTag ( unsigned int styleID )
	{
		if ( useInlineCSS )
		{
			return getOpenTag ( docStyle.getKeywordStyle ( langInfo.getKeywordClasses() [styleID] ) );
		}
		return getOpenTag ( langInfo.getKeywordClasses() [styleID] );
	}

	string HtmlGenerator::getKeywordCloseTag ( unsigned int styleID )
	{
		return "</span>";
	}

	string  HtmlGenerator::getMetaInfoOpenTag ( const TagInfo& info )
	{
		ostringstream tagStream;
		tagStream<<"<span title=\""<<info.getKind() <<" | ";
		if ( !info.name_space.empty() )
		{
			maskString ( tagStream, info.name_space );
			tagStream<<" | ";
		}
		maskString ( tagStream, info.file ) ;
		tagStream<<"\">";
		return tagStream.str();
	}
	string  HtmlGenerator::getMetaInfoCloseTag()
	{
		return "</span>";
	}

}
