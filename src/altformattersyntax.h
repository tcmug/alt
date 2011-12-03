#ifndef ALTFORMATTER_H
#define ALTFORMATTER_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <QMap>
#include <QVector>
#include <QStack>


class AltFormatterPair
{
  public:

		AltFormatterPair(const QString &p, const QString &a, const QString &t = "");

    QRegExp Pattern;
    QString Action;
    QString Target;
};



class AltFormatterBlock
{
  protected:
    QString Name;
    QVector <AltFormatterPair*> Patterns;
    QColor TextColor;
    QColor BackgroundColor;

	public:
		AltFormatterBlock(const QString &name);
    /**
     * Match text and find match for patterns
     */
    AltFormatterPair *matchText(const QString &str);

		const QString &getName() const;

		void setTextColor(const QColor &c);
		void setBackgroundColor(const QColor &c);
		const QColor &getTextColor() const;
		const QColor &getBackgroundColor() const;

		void apply(QPainter &painter) const;
		void addFormatterPair(AltFormatterPair *pair);
		const AltFormatterPair *matchPatterns(const QString &str, int &at) const;
};




class AltFormatterSyntax
{
  protected:

		QMap <QString, AltFormatterBlock*> BlockMap;

  public:

		AltFormatterSyntax();
		
		QString formatize(const QString &str, int &at, QStack <QString> &stack);
		const AltFormatterBlock *getFormatterBlock(const QString &name) const;

    static QString TARGET_POP;
    static QString TARGET_ROOT;

};


#endif
