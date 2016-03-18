#include "imgadmin.h"
#include <iostream>
#include <QPixmap>
#include <QTextStream>
#include <JarrSettings/settings.h>
ImgAdmin::ImgAdmin()
{
    orderBy =  ORDER_BY_DATE;
}

void ImgAdmin::load(QString path)
{
    QStringList filters;
    filters << "*.jpg" << "*.png" << "*.JPG" << "*.JPEG" << "*.PNG"
            << "*.gif" << "*.GIF" << "*.bmp" << "*.BMP" << "*.jpeg";
               // << "*.webm" << "*.WEBM";  // not yet support for webm

    if (dir.cd(path.toStdString().c_str()))
    {
       QDir::SortFlag order;
       switch(this->orderBy)
       {
       case ORDER_BY_DATE:
           order = QDir::Time;
           break;
       case ORDER_BY_NAME:
           order = QDir::Name;
           break;
       default:
           order = QDir::Time;
           break;
       }
       std::cout << "order by:" << this->orderBy << std::endl;

       fileList = dir.entryList(filters,QDir::Files,order);

        for(unsigned int i = 0; i < dir.count(); i++)
        {
             //cout << fileList.value(i).toStdString().c_str() << std::endl;
        }
        dirExist = true;
    }
    else
    {
        dirExist = false;
    }
}

void ImgAdmin::moveImg(QString path_origin, QString path_dest)
{
    QFile source_file;
    if(source_file.copy(path_origin,path_dest))
    {
        std::cout << "Copy Success" << std::endl;
        if(source_file.remove(path_origin))
            std::cout << "delete Success" << std::endl;
    }
}

void ImgAdmin::copyImg(QString path_origin, QString path_dest)
{
    QFile source_file;
    if(source_file.copy(path_origin,path_dest))
    {
        std::cout << "Copy Succesfull" << std::endl;
    }
}

std::string ImgAdmin::buildPath(std::string path, std::string file)
{
    std::string out;
    out = path;
    if(out.at(out.size()-1) == '/')
    {
        out.append(file);
    }
    else
    {
        out.append("/");
        out.append(file);
    }
    return out;
}

int ImgAdmin::loadIndex(QString path)
{
    QFile *file = new QFile(path);
    if(file->open(QFile::ReadWrite))
    {
        QTextStream input(file);
        QString line = input.readLine();
        return line.toInt();
        file->close();
    }
    return 0;
}

void ImgAdmin::saveIndex(QString path,int i)
{
    QFile *file = new QFile(path);
    QString str = jarr::Attribute::to_string(i).c_str() ;
    std::cout << path.toStdString() << " : " << str.toStdString() << std::endl;

    if(file->open(QFile::ReadWrite))
    {
        std::cout << "writing .." << std::endl;
        const char *out = str.toStdString().c_str();
        file->write(out,qstrlen(out)*sizeof(char));
        file->close();
    }
}


void ImgAdmin::viewMetaData(QFile fimg)
{

}
