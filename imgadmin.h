#ifndef IMGADMIN_H
#define IMGADMIN_H
#include <QDir>
#include <QList>
#include <QString>

class ImgAdmin
{
public:
    QDir dir;
    QDir dir_dest;
    QStringList fileList;
    bool dirExist;
    int orderBy;
    #define ORDER_BY_DATE 0
    #define ORDER_BY_NAME 1

    ImgAdmin();
    void load(QString path);
    void moveImg(QString path_origin, QString path_dest);
    void copyImg(QString path_origin, QString path_dest);
    void deleteImg();

    void dumpList();

    int loadIndex(QString path);
    void saveIndex(QString,int);
    void viewMetaData(QFile fimg);

    std::string buildPath(std::string path, std::string file);
};

#endif // IMGADMIN_H
