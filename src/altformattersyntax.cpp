
#include <QtGui>
#include <QRegExp>
#include <QVector>
#include <QMap>

#include "altformattersyntax.h"

QString AltFormatterSyntax::TARGET_ROOT = "ROOT";
QString AltFormatterSyntax::TARGET_POP = "POP";

AltFormatterSyntax::AltFormatterSyntax()
		{

			AltFormatterBlock *block;
			
			block = new AltFormatterBlock(AltFormatterSyntax::TARGET_ROOT);
			block->setTextColor(Qt::red);
			block->setBackgroundColor(Qt::white);
			block->addFormatterPair(new AltFormatterPair("[0-9]", "ENTER", "Character"));
			block->addFormatterPair(new AltFormatterPair("\\<\\?php", "ENTER", "PHP"));
			block->addFormatterPair(new AltFormatterPair("\\<", "ENTER", "Character"));
			BlockMap[block->getName()] = block;

			block = new AltFormatterBlock("Character");
			block->setTextColor(Qt::white);
			block->setBackgroundColor(Qt::red);
			block->addFormatterPair(new AltFormatterPair("[^0-9]", "ENTER", "POP"));
			BlockMap[block->getName()] = block;

			block = new AltFormatterBlock("PHP");
			block->setTextColor(Qt::white);
			block->setBackgroundColor(Qt::green);
			block->addFormatterPair(new AltFormatterPair("\\?\\>", "ENTER_AFTER", "POP"));
			BlockMap[block->getName()] = block;		
}


QString AltFormatterSyntax::formatize(const QString &str, int &at, QStack <QString> &stack)
{
				const AltFormatterPair *pair;
				int to = 0;

				QString blockname;
				blockname = stack.top();
				to = at;
				pair = BlockMap[blockname]->matchPatterns(str, to);
				if (pair) 
				{
					if (pair->Target == "POP") 
					{
						// Drop current
						stack.pop();
					}
					else 
					{
						// Push
						stack.push(pair->Target);
					}
				}
				int temp = at;
				at = to;
				return str.mid(temp, to-temp);
}


const AltFormatterBlock *AltFormatterSyntax::getFormatterBlock(const QString &name) const
{
  return BlockMap[name];
}





AltFormatterPair::AltFormatterPair(const QString &p, const QString &a, const QString &t)
{
	Pattern = QRegExp(p);
	Action = a;
	Target = t;
}

AltFormatterBlock::AltFormatterBlock(const QString &n)
{
	Name = n;
}


void AltFormatterBlock::addFormatterPair(AltFormatterPair *pair) 
{
	Patterns.push_back(pair);
}

const QString &AltFormatterBlock::getName() const 
{
	return Name;
}

void AltFormatterBlock::setTextColor(const QColor &c) 
{
	TextColor = c;
}


void AltFormatterBlock::setBackgroundColor(const QColor &c) 
{
	BackgroundColor = c;
}


const QColor &AltFormatterBlock::getTextColor() const
{
	return TextColor;
}


const QColor &AltFormatterBlock::getBackgroundColor() const 
{
	return BackgroundColor;
}

const AltFormatterPair *AltFormatterBlock::matchPatterns(const QString &str, int &at) const
{
  QVector <AltFormatterPair*>::const_iterator i;
	const AltFormatterPair *match = 0;
	const int max = str.length();
	int match_pos = max;
	int pos = 0;
	match_pos = max;
	for (i = Patterns.begin(); i != Patterns.end(); i++) 
  {
	  pos = (*i)->Pattern.indexIn(str, at);
	 	if (pos >= 0 && pos < match_pos) 
    {
			match_pos = pos;
	 		match = (*i);
   	}
  }
	if (match && match->Action == "ENTER_AFTER") 
	{
		match_pos += match->Pattern.matchedLength();
	}
	at = match_pos;
	return match;
	
}



