
#include "altcontext.h"
#include "altformattersyntax.h"
#include "altfilerow.h"


class AltFormatterBlockIterator 
{
	protected:
		
		AltContext *Context;
  	const AltFormatterBlock *Block;
		int Line;
		int Column;
		int AtCharacter;
		QString LineString;
		QStack <QString> LineStack;
		QString Part;

	public:

		AltFormatterBlockIterator(AltContext *c);
		
		bool setLineNumber(int row);
		bool next();
		int getRow();
		int getColumn();
		const QString &getPart();
		const AltFormatterBlock *getFormatterBlock();
		int blockLength() const;

};


