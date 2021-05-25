#ifndef IMAGE_H
#define IMAGE_H
#include <QMap>
#include <QString>
#include <QImage>


class Image
{
public:
    Image();
    QImage getimage(const QString& name);
private:
    QMap<QString, QImage> images;
};

#endif // IMAGE_H
