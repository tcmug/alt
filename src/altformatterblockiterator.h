
#include "altcontext.h"
#include "altformattersyntax.h"
#include "altfilerow.h"


class AltFormatterBlockIterator 
{
	protected:
		
		const AltContext *Context;
  	const AltFormatterBlock *Block;
		int Line;
		int Column;
		int AtCharacter;
		QString LineString;
		QStack <QString> LineStack;
		QString Part;

	public:

		AltFormatterBlockIterator(const AltContext *c);
		
		bool setRow(int row);
		bool next();
		int getRow() const;
		int getColumn() const;
		const QString &getPart() const;
		const AltFormatterBlock *getFormatterBlock() const;
		bool endOfLine() const;
		const QStack <QString> &getLineStack() const;
		int blockLength() const;

};


