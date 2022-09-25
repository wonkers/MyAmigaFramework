
#pragma once
#include <exec/types.h>
class Vector2d
{
	public:
		WORD x;
		WORD y;
        Vector2d(){}
        Vector2d(WORD _x, WORD _y)
        {
            x = _x;
            y = _y;
        }
};
class Vector3d : public Vector2d
{
	public:
		WORD z;
        Vector3d(){}
        Vector3d(WORD _x, WORD _y, WORD _z) :Vector2d(_x, _y) {
            z = _z;
        }
};

class Polygon
{
    public:
    Vector2d p1;
    Vector2d p2;
    Vector2d p3;
    Polygon(){}
    Polygon(Vector2d _p1, Vector2d _p2, Vector2d _p3)
    {
        p1 = _p1;
        p2 = _p2;
        p3 = _p3;
    }
};

class TexturedPolygon : public Polygon{
    public:
    UWORD *texture;
    WORD height;
    TexturedPolygon(){}
    TexturedPolygon(Vector2d _p1, Vector2d _p2, Vector2d _p3) : Polygon(_p1, _p2, _p3)
    {

    }
    TexturedPolygon(Vector2d _p1, Vector2d _p2, Vector2d _p3, UWORD* tex, WORD ht)
    : Polygon(_p1, _p2, _p3)
    {
        texture = tex;
        height = ht;
    }
};