
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

