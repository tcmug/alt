
#include "altcontext.h"
#include "altformattersyntax.h"
#include "altfilerow.h"
#include "altformatterblockiterator.h"


AltFormatterBlockIterator::AltFormatterBlockIterator(const AltContext *c)
{
	Context = c;
	AtCharacter = 0;
	Column = 0;
  Line = -1; // since next increments always by +1
}


bool AltFormatterBlockIterator::setRow(int n) 
{
	Line = n - 1;
	if (Line >= 0)
	{
		// n line
	  LineStack = Context->Lines[Line].getStack();
	}
	else
	{
		// 1st line
		LineStack.clear();
		LineStack.push_back("ROOT");
	}
	return true;
}


bool AltFormatterBlockIterator::next()
{
	// next line
	if (Line == -1 || AtCharacter >= LineString.length()) 
	{ 
/*		if (Line >= 0)
		{
			// Store existing Stack
			Context->Lines[Line].setStack(LineStack);
		}
*/
    Line++;

	  if (Line == Context->Lines.size()) 
    {
			// We're at the end
	  	return false;
    }
		else if (Line == 0) 
		{
			LineStack.clear();
			LineStack.push_back("ROOT");
		}
		else 
		{
		  LineStack = Context->Lines[Line-1].getStack();
		}

		// Grab line data		
		AtCharacter = 0;
		LineString = Context->Lines[Line].getString();
  }

	Column = AtCharacter;
  Block = Context->Formatter->getFormatterBlock(LineStack.top());
	Part = Context->Formatter->formatize(LineString, AtCharacter, LineStack);
	return true;
}


int AltFormatterBlockIterator::getRow() const
{
	return Line;
}

int AltFormatterBlockIterator::getColumn() const
{
	return Column;
}

const QString &AltFormatterBlockIterator::getPart() const
{
	return Part;
}

const AltFormatterBlock *AltFormatterBlockIterator::getFormatterBlock() const
{
	return Block;
}

bool AltFormatterBlockIterator::endOfLine() const 
{
	return (Line >= 0 || AtCharacter >= LineString.length());
}

const QStack <QString> &AltFormatterBlockIterator::getLineStack() const 
{
	return LineStack;
}

