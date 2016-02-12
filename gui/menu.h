#ifndef MENU_H
#define MENU_H


class Gui;
class Menu
{
public:
    Menu();
    Menu(unsigned char *i, int w, int h, int r);
    //Menu(unsigned char *zim, int zsize, int size, int r);
    Menu(int);
    ~Menu();
    void setSize(int);
    void setRegionPos(int , float, float);
    void setRegionArea(int, float, float);
    void setImage(unsigned char *);
    void setSwtchGrps(int);
    void setSwtchIds(int, int*, int);
    int checkRegion(float x, float y);
    unsigned char *getImage();
    int size, width, height, rNum, sStart;
    // the screen-space width and height (fraction of the screen width
    // and height the window's width and height take up.)
    float ssw, ssh;
    float x, y;
    struct Region {
        float x, y, w, h;
        unsigned char *im;
        int action;
    };
    unsigned char *image;
    Region *region;
    int swtchGrps;
    int *swtchSizes;
    int **swtchIds;
private:

};

#endif // MENU_H
