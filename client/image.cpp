#include "image.h"

Image::Image()
{
    images.insert("dot", QImage("://dot.png"));
    images.insert("half", QImage("://half.png"));
    images.insert("full", QImage("://full.png"));
    images.insert("redhalf", QImage("://redfull.png"));
    images.insert("redfull", QImage("://redfull.png"));
    images.insert("field", QImage("://field.png"));
    images.insert("girl", QImage("://unnamed.jpg"));
}

QImage Image::getimage(const QString &name) {
    return images.find(name).value();
}
