#ifndef ALTFILEROW_H
#define ALTFILEROW_H

#include <QString>
#include <QStack>


class AltFileRow 
{
	private:
	  QString String;
		QStack <QString> Stack;
		
	public:

		AltFileRow(const QString &str = "");
	
		const QString &getString();
		void setString(const QString &str);
		void setStack(const QStack <QString> &stack);
		const QStack <QString> &getStack();
		
		void insert(int pos, const QString &str);
		void erase(int from, int to);
};



#endif
