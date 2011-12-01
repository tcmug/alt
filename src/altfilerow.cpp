
#include "altfilerow.h"

AltFileRow::AltFileRow(const QString &str)
{
	String = str;
}

const QString &AltFileRow::getString()
{
	return String;
}


void AltFileRow::setString(const QString &str)
{
	String = str;	
}


void AltFileRow::setStack(const QStack <QString> &s)
{
	Stack = s;
}


const QStack <QString> &AltFileRow::getStack()
{
	return Stack;
}


void AltFileRow::insert(int position, const QString &str)
{
  if (position > String.length()) {
		QString padding(" ");
		String.insert(String.length(), padding.repeated(position - String.length()));
	}
	String.insert(position, str);
}


void AltFileRow::erase(int from, int to)
{
	String = String.mid(0, from) + String.mid(to);
}




