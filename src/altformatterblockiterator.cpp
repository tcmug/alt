
#include "altcontext.h"
#include "altformattersyntax.h"
#include "altfilerow.h"
#include "altformatterblockiterator.h"


AltFormatterBlockIterator::AltFormatterBlockIterator(AltContext *c)
{
	Context = c;
	AtCharacter = 0;
	Column = 0;
  Line = -1; // since next increments always by +1
}


bool AltFormatterBlockIterator::setLineNumber(int n) 
{
	Line = n - 1;
	return next();
}


bool AltFormatterBlockIterator::next()
{
	// next line
	if (Line == -1 || AtCharacter >= LineString.length()) 
	{ 
		if (Line >= 0)
		{
			// Store existing Stack
			Context->Lines[Line].setStack(LineStack);
		}

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


int AltFormatterBlockIterator::getRow()
{
	return Line;
}

int AltFormatterBlockIterator::getColumn()
{
	return Column;
}

const QString &AltFormatterBlockIterator::getPart()
{
	return Part;
}

const AltFormatterBlock *AltFormatterBlockIterator::getFormatterBlock()
{
	return Block;
}



