/*
 ============================================================================
 Name		: XML.h
 Author	  : artem78
 Version	 : 
 Copyright   : 
 Description : Utilities for manipulate with XML
 ============================================================================
 */

#ifndef XML_H
#define XML_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <f32file.h>
#include <s32file.h>
#include <badesca.h>

// FORWARD DECLARATION

class TXMLVersion;

// CLASS DECLARATION

class CSimpleXMLWriter : public CBase
	{
	// ToDo: Add using user specified output encoding
	// ToDo: Add validation for tag and attribute names (only ASCII, etc...)
	// FixMe: Disable escaping for CDATA section
	// ToDo: Add check for closing tags or maybe autoclose all opened tags on exit
	// ToDo: Print empty tags like <item /> not <item></item>
	// ToDo: Add check that there is only one root element
	// ToDo: Use line breakes with long or many attributes, like this:
	//	<gpx xmlns="http://www.topografix.com/GPX/1/1" version="1.1"
	//		creator="GPS Tracker CLI 1.2.1" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	//		xsi:schemaLocation="http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd">
	
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CSimpleXMLWriter();

	/**
	 * Two-phased constructor.
	 */
	static CSimpleXMLWriter* NewL(RFile &aFile, const TXMLVersion &aXMLVersion,
			TBool anIsPrettyPrint = EFalse);

	/**
	 * Two-phased constructor.
	 */
	static CSimpleXMLWriter* NewLC(RFile &aFile, const TXMLVersion &aXMLVersion,
			TBool anIsPrettyPrint = EFalse);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CSimpleXMLWriter(RFile &aFile, TBool anIsPrettyPrint);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL(const TXMLVersion &aXMLVersion);
	
// Custom properties and methods
public:
	void OpenTagL(const TDesC &aTagName);
	void CloseTagL();
	void AddAttributeL(const TDesC &aName, const TDesC &aValue);
	void AddAttributeL(const TDesC &aName, TInt aValue);
	void AddAttributeL(const TDesC &aName, TReal aValue, const TRealFormat &aFmt);
	void AddTextL(const TDesC &aText);
	void AddTextL/*AddNumL*/(TInt aValue);
	void AddTextL/*AddNumL*/(TReal aValue, const TRealFormat &aFmt);
	//void AddCDataL(const TDesC &aDes);
	//void AddCommentL(const TDesC &aDes);
	
private:
	enum /*TSimpleXMLWriterState*/ TWriterState
		{
		EXMLDeclaration = 0, // When nothing except xml declaration has wrote yet
		EOpeningTag,
		//EClosingTag,
		ETagBody
		};
	
	RFileBuf iFileBuf;
	CDesCArraySeg* iTagStack;
	TBool iIsPrettyPrint;
	TWriterState iState;
	TInt iClosingTagsInARow;
	
	/* Encodes string to output charset and writes to the file.
	 * Note: Do not write directly to iFileBuf, use this method instead.
	 */
	void WriteL(const TDesC &aDes);
	
	void WriteLineBreakL();
	void WriteIdentation(TInt aWidth);
	void SetStateL(TWriterState aState);
	void CloseOpeningTagL();
	HBufC* EscapeStringL(const TDesC &aDes);
	};

class TXMLVersion
	{
public:
	TUint iMajor;
	TUint iMinor;
	TXMLVersion(TUint aMajor /*= 0*/, TUint aMinor = 0);
	void AsString(TDes &aDes) const;
	};

#endif // XML_H
