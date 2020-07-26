/*
 ============================================================================
 Name		: XML.cpp
 Author	  : artem78
 Version	 : 
 Copyright   : 
 Description :
 ============================================================================
 */

#include "XML.h"
#include <utf.h>


// CSimpleXMLWriter

CSimpleXMLWriter::CSimpleXMLWriter(RFile &aFile, TBool anIsPrettyPrint) :
		iIsPrettyPrint(anIsPrettyPrint)
	{
	iFileBuf.Attach(aFile, 0);
	}

CSimpleXMLWriter::~CSimpleXMLWriter()
	{
	delete iTagStack;
	iFileBuf.Synch();
	iFileBuf.Detach(); // Prevent to close file with file buffer
	iFileBuf.Close();
	}

CSimpleXMLWriter* CSimpleXMLWriter::NewLC(RFile &aFile, const TXMLVersion &aXMLVersion,
		TBool anIsPrettyPrint)
	{
	CSimpleXMLWriter* self = new (ELeave) CSimpleXMLWriter(aFile, anIsPrettyPrint);
	CleanupStack::PushL(self);
	self->ConstructL(aXMLVersion);
	return self;
	}

CSimpleXMLWriter* CSimpleXMLWriter::NewL(RFile &aFile, const TXMLVersion &aXMLVersion,
		TBool anIsPrettyPrint)
	{
	CSimpleXMLWriter* self = CSimpleXMLWriter::NewLC(aFile, aXMLVersion, anIsPrettyPrint);
	CleanupStack::Pop(); // self;
	return self;
	}

void CSimpleXMLWriter::ConstructL(const TXMLVersion &aXMLVersion)
	{
	iTagStack = new (ELeave) CDesCArraySeg(10);	
	
	// Write XML declaration (prolog)
	_LIT(KXMLDeclarationStart, "<?xml version=\"");
	_LIT(KXMLDeclarationEnd, "\" encoding=\"UTF-8\"?>");
	TBuf<16> verBuff;
	aXMLVersion.AsString(verBuff);
	WriteL(KXMLDeclarationStart);
	WriteL(verBuff);
	WriteL(KXMLDeclarationEnd);
	
	SetStateL(EXMLDeclaration);
	
	iClosingTagsInARow = 0;
	}

void CSimpleXMLWriter::OpenTagL(const TDesC &aTagName)
	{
	iTagStack->AppendL(aTagName);
	SetStateL(ETagBody);
	SetStateL(EOpeningTag);
	
	iClosingTagsInARow = 0;
	
	if (iIsPrettyPrint)
		{
		WriteLineBreakL();
		WriteIdentation(iTagStack->Count() - 1);
		}
	
	_LIT(KOpeningTagStart, "<");
	WriteL(KOpeningTagStart);
	WriteL(aTagName);
	}

void CSimpleXMLWriter::CloseTagL()
	{
	if (!iTagStack->Count())
		return; // Nothing to close
	
	TInt lastIdx = iTagStack->Count() - 1;
	TPtrC tagName((*iTagStack)[lastIdx]);
	
	SetStateL(ETagBody);
	
	iClosingTagsInARow++;
	
	if (iIsPrettyPrint && iClosingTagsInARow > 1)
		{
		WriteLineBreakL();
		WriteIdentation(iTagStack->Count() - 1);
		}
	
	_LIT(KClosingTagStart, "</");
	_LIT(KClosingTagEnd, ">");
	WriteL(KClosingTagStart);
	WriteL(tagName);
	WriteL(KClosingTagEnd);
	
	//SetStateL(ETagBody);
	
	iTagStack->Delete(lastIdx);
	}

void CSimpleXMLWriter::AddAttributeL(const TDesC &aName, const TDesC &aValue)
	{
	if (iState != EOpeningTag)
		return;
	
	HBufC* escapedValue = EscapeStringL(aValue);
	CleanupStack::PushL(escapedValue);
	
	_LIT(KSpace, " ");
	_LIT(KEqualsAndQuote, "=\"");
	_LIT(KQuote, "\"");
	WriteL(KSpace);
	WriteL(aName);
	WriteL(KEqualsAndQuote);
	WriteL(*escapedValue);
	WriteL(KQuote);
	
	CleanupStack::PopAndDestroy(escapedValue);
	}

void CSimpleXMLWriter::AddAttributeL(const TDesC &aName, TInt aValue)
	{
	TBuf<32> valBuff;
	valBuff.Num(aValue);
	AddAttributeL(aName, valBuff);
	}

void CSimpleXMLWriter::AddAttributeL(const TDesC &aName, TReal aValue, const TRealFormat &aFmt)
	{
	TBuf<32> valBuff;
	valBuff.Num(aValue, aFmt);
	AddAttributeL(aName, valBuff);
	}

void CSimpleXMLWriter::AddTextL(const TDesC &aText)
	{	
	iClosingTagsInARow = 0;
	
	SetStateL(ETagBody);
	
	HBufC* escapedText = EscapeStringL(aText);
	CleanupStack::PushL(escapedText);
	
	WriteL(*escapedText);
	
	CleanupStack::PopAndDestroy(escapedText);
	}

void CSimpleXMLWriter::AddTextL/*AddNumL*/(TInt aValue)
	{
	TBuf<32> buff;
	buff.Num(aValue);
	AddTextL(buff);
	}

void CSimpleXMLWriter::AddTextL/*AddNumL*/(TReal aValue, const TRealFormat &aFmt)
	{
	TBuf<32> buff;
	buff.Num(aValue, aFmt);
	AddTextL(buff);
	}

void CSimpleXMLWriter::WriteL(const TDesC &aDes)
	{
	// Convert string to UTF-8
	HBufC8* buff = CnvUtfConverter::ConvertFromUnicodeToUtf8L(aDes);
	CleanupStack::PushL(buff);
	
	TRequestStatus stat;
	iFileBuf.WriteL(*buff, stat);
	User::WaitForRequest(stat);
	
	CleanupStack::PopAndDestroy(buff);
	}

void CSimpleXMLWriter::WriteLineBreakL()
	{
	_LIT(KLineBreak, "\r\n"); // ToDo: or \n? It`s better to make this configurable.
	WriteL(KLineBreak);
	}

void CSimpleXMLWriter::WriteIdentation(TInt aWidth)
	{
	_LIT(KTab, "\t");
	for (TInt i = 0; i < aWidth; i++)
		WriteL(KTab);
	}

void CSimpleXMLWriter::SetStateL(TWriterState aState)
	{
	if (iState != aState)
		{
		if (iState == EOpeningTag && aState != EOpeningTag)
			CloseOpeningTagL();
		
		iState = aState;
		}
	}

void CSimpleXMLWriter::CloseOpeningTagL()
	{
	_LIT(KClosingTagEnd, ">");
	WriteL(KClosingTagEnd);
	}

HBufC* CSimpleXMLWriter::EscapeStringL(const TDesC &aDes)
	{
	_LIT(KLt, "&lt;");
	_LIT(KGt, "&gt;");
	_LIT(KAmp, "&amp;");
	_LIT(KApos, "&apos;");
	_LIT(KQuot, "&quot;");
	
	// Calculate length of escaped string
	TInt resLength = aDes.Length();
	for (TInt i = 0; i < aDes.Length(); i++)
		{
		switch (aDes[i])
			{
			case '<':
				resLength = resLength - 1 + KLt().Length();
				break;
				
			case '>': // Not mandatory
				resLength = resLength - 1 + KGt().Length();
				break;
				
			case '&':
				resLength = resLength - 1 + KAmp().Length();
				break;
				
			case '\'': // Not mandatory
				resLength = resLength - 1 + KApos().Length();
				break;
				
			case '"': // Not mandatory for tag body
				resLength = resLength - 1 + KQuot().Length();
				break;
				
			default:
				break;	
			}
		}
	
	// Convert 
	HBufC* res = HBufC::NewL/*NewMaxL*/(resLength);
	TPtr p(res->Des());
	
	for (TInt i = 0; i < aDes.Length(); i++)
		{
		switch (aDes[i])
			{
			case '<':
				p.Append(KLt);
				break;
				
			case '>': // Not mandatory
				p.Append(KGt);
				break;
				
			case '&':
				p.Append(KAmp);
				break;
				
			case '\'': // Not mandatory
				p.Append(KApos);
				break;
				
			case '"': // Not mandatory for tag body
				p.Append(KQuot);
				break;
				
			default:
				p.Append(aDes[i]);
				break;
			}
		}
	
	return res;
	}


// TXMLVersion

TXMLVersion::TXMLVersion(TUint aMajor, TUint aMinor) :
		iMajor(aMajor), iMinor(aMinor)
	{
	// No implementation required
	}

void TXMLVersion::AsString(TDes &aDes) const
	{
	aDes.Zero();
	aDes.AppendNum(iMajor);
	const TChar KDot(46); 
	aDes.Append(KDot);
	aDes.AppendNum(iMinor);
	}
