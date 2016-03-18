#include "paramparser.h"
#include <QFile>
#include <QTextStream>

paramParser::paramParser()
{    
}


void paramParser::load()
{
    QFile paramFile("param.conf");
    QString *line;
    QTextStream qin(&paramFile);


}


void paramParser::save()
{

}
