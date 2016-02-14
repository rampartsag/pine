#include "menu.h"
#include <zlib.h>
#include <iostream>

// menu class holds all info pertaining to a single window.
// should be renamed to "window" in the future

Menu::Menu()
{
    image = new unsigned char[0];
    region = new struct Region[0];

}

Menu::Menu(unsigned char *i, int w, int h, int r) {
    image = i;
    rNum = r;
    size = w*h;
    width = w;
    height = h;
    region = new struct Region[r];
    sStart = 0;


}

Menu::Menu(int n)
{
    if(n == 4){};

}

Menu::~Menu()
{
    delete[] image;
    delete[] region;
    delete[] *swtchIds;
    delete[] swtchIds;
    delete[] swtchSizes;

}

void Menu::setSize(int n)
{
    image = new unsigned char[n];

}

void Menu::setRegionPos(int r, float x, float y)
{
    region[r].x = x;
    region[r].y = y;

}

void Menu::setRegionArea(int r, float w, float h)
{
    region[r].w = w;
    region[r].h = h;

}

void Menu::setImage(unsigned char *i)
{
    image = i;

}

void Menu::setSwtchGrps(int num)
{
    swtchGrps = num;
    swtchIds = new int*[swtchGrps];
    swtchSizes = new int[swtchGrps];


}

void Menu::setSwtchIds(int grp, int *ids, int num)
{
    swtchIds[grp] = new int[num];
    swtchSizes[grp] = num;
    for(int i=0; i<num; i++) {
        swtchIds[grp][i] = ids[i];

    }

}

int Menu::checkRegion(float x, float y)
{
    for(int i=0; i<rNum; i++) {
        float ls, rs, top, bottom;
        ls = region[i].x;
        rs = region[i].x + region[i].w;
        top = region[i].y;
        bottom = region[i].y + region[i].h;
        bool b = x >ls && x < rs && y > top && y < bottom;
        if(b) {
            return i;
        }
    }
    return -1;


}

unsigned char *Menu::getImage()
{
    return image;

}
