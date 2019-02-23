#include "Luna-mbx.h"


bool LunaCollision::Point_Polygon( POINT2D &pt, POLYGON2D &poly )
{
    if (poly.count - 1 <= 0) return true;

    long num = 0;
    POINT2D *polypt = poly.pt;
    for (long count = poly.count - 1; count != 0; count--)
    {
        if (((pt.y - polypt[1].y) * (pt.x - polypt[0].x)) - ((pt.y - polypt[0].y) * (pt.x - polypt[1].x)) < 0.0)
        {
            num++;
        }
        polypt++;
    }

    return ((num == 0) || (num == (poly.count - 1)));
}

bool LunaCollision::Line_Polygon( LINE2D &l, POLYGON2D &poly )
{
    if (Point_Polygon(l.a, poly)) return true;
    if (Point_Polygon(l.b, poly)) return true;

    if (poly.count - 1 <= 0) return false;

    for (long index = 0; index < poly.count - 1; index++)
    {
        float p0x = poly.pt[index].x;
        float p0y = poly.pt[index].y;
        float p1x = poly.pt[index + 1].x;
        float p1y = poly.pt[index + 1].y;

        float n1 = ((l.a.y - p1y) * (l.a.x - p0x)) - ((l.a.x - p1x) * (l.a.y - p0y));
        double nd = ((l.b.y - p1y) * (l.b.x - p0x)) - ((l.b.x - p1x) * (l.b.y - p0y));
        float n2 = nd;
        nd = nd * n1;
        n1 = ((p0x - l.a.x) * (p0y - l.b.y)) - ((p0y - l.a.y) * (p0x - l.b.x));
        n2 = ((p1x - l.a.x) * (p1y - l.b.y)) - ((p1y - l.a.y) * (p1x - l.b.x));

        if ((nd <= 0.0) && (n2 * n1 <= 0.0)) return true;
    }
    return false;
}

bool LunaCollision::Polygon_Polygon( POLYGON2D &poly1, POLYGON2D &poly2 )
{
    float p1maxx = poly1.pt->x;
    float p1minx = poly1.pt->x;
    float p1maxy = poly1.pt->y;
    float p1miny = poly1.pt->y;
    long p1count_1 = poly1.count - 1;

    if (p1count_1 > 1)
    {
        POINT2D *pt = poly1.pt + 1;
        for (long count = p1count_1 - 1; count != 0; pt++, count--)
        {
            if (pt->x < p1minx)
            {
                p1minx = pt->x;
            }
            else if (pt->x > p1maxx)
            {
                p1maxx = pt->x;
            }

            if (pt->y < p1miny)
            {
                p1miny = pt->y;
            }
            else if (pt->y > p1maxy)
            {
                p1maxy = pt->y;
            }
        }
    }

    float p2minx = poly2.pt->x;
    float p2maxx = poly2.pt->x;
    float p2miny = poly2.pt->y;
    float p2maxy = poly2.pt->y;
    long p2count_1 = poly2.count - 1;

    if (p2count_1 > 1)
    {
        POINT2D *pt = poly2.pt + 1;
        for (long count = p2count_1 - 1; count != 0; pt++, count--)
        {
            if (pt->x < p2minx)
            {
                p2minx = pt->x;
            }
            else if (pt->x > p2maxx)
            {
                p2maxx = pt->x;
            }


            if (pt->y < p2miny)
            {
                p2miny = pt->y;
            }
            else if (pt->y > p2maxy)
            {
                p2maxy = pt->y;
            }
        }
    }

    if ((p1minx > p2maxx) || (p1maxx < p2minx) || (p1miny > p2maxy) || (p1maxy < p2miny))
    {
        return false;
    }


    for (long index = 0; index < p1count_1; index++)
    {
        if (Point_Polygon(poly1.pt[index], poly2)) return true;
    }

    for (long index = 0; index < p2count_1; index++)
    {
        if (Point_Polygon(poly2.pt[index], poly1)) return true;
    }

    for (long index1 = 0; index1 < p1count_1; index1++)
    {
        for (long index2 = 0; index2 < p2count_1; index2++)
        {
            float p10x = poly1.pt[index1].x;
            float p10y = poly1.pt[index1].y;
            float p11x = poly1.pt[index1 + 1].x;
            float p11y = poly1.pt[index1 + 1].y;
            float p20x = poly2.pt[index2].x;
            float p20y = poly2.pt[index2].y;
            float p21x = poly2.pt[index2 + 1].x;
            float p21y = poly2.pt[index2 + 1].y;
            float n1 = ((p10y - p21y) * (p10x - p20x)) - ((p10x - p21x) * (p10y - p20y));
            float n2 = ((p11y - p21y) * (p11x - p20x)) - ((p11x - p21x) * (p11y - p20y));
            double nd1 = ((p21y - p11y) * (p21x - p10x)) - ((p21x - p11x) * (p21y - p10y));
            double nd2 = ((p20y - p11y) * (p20x - p10x)) - ((p20x - p11x) * (p20y - p10y));

            if ((nd2 * nd1 <= 0.0) && (n2 * n1 <= 0.0)) return true;
        }
    }

    return false;
}

