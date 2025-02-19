#ifndef slic3r_Point_hpp_
#define slic3r_Point_hpp_

#include "libslic3r.h"
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

namespace Slic3r {

class Line;
class Linef;
class MultiPoint;
class Point;
class Point3;
class Pointf;
class Pointf3;
typedef Point Vector;
typedef Point3 Vector3;
typedef Pointf Vectorf;
typedef Pointf3 Vectorf3;
typedef std::vector<Point> Points;
typedef std::vector<Point3> Point3s;
typedef std::vector<Point*> PointPtrs;
typedef std::vector<const Point*> PointConstPtrs;
typedef std::vector<Pointf> Pointfs;
typedef std::vector<Pointf3> Pointf3s;

class Point
{
    public:
    coord_t x;
    coord_t y;
    coord_t z;
    coord_t b;
    Point(coord_t _x = 0, coord_t _y = 0, coord_t _z = -1, coord_t _b = 0): x(_x), y(_y), z(_z), b(_b) {};
    Point(int _x, int _y, int _z, int _b): x(_x), y(_y), z(_z), b(_b) {};
    Point(long long _x, long long _y, long long _z, long long _b): x(_x), y(_y), z(_z), b(_b) {};  // for Clipper
    Point(double x, double y, double z, double b);
    static Point new_scale(coordf_t x, coordf_t y) {
        return Point(scale_(x), scale_(y));
    };
    static Point new_scale(coordf_t x, coordf_t y, coordf_t z) {
        return Point(scale_(x), scale_(y), scale_(z));
    };
    bool operator==(const Point& rhs) const;
    std::string wkt() const;
    std::string dump_perl() const;
    void scale(double factor);
    void translate(double x, double y);
    void translate(double x, double y, double z);
    void translate(const Vector &vector);
    void rotate(double angle);
    void rotate(double angle, const Point &center);
    Point rotated(double angle) const {
        Point p(*this);
        p.rotate(angle);
        return p;
    }
    Point rotated(double angle, const Point &center) const {
        Point p(*this);
        p.rotate(angle, center);
        return p;
    }
    bool coincides_with(const Point &point) const { return this->x == point.x && this->y == point.y && this->z == point.z; }
    bool coincides_with_epsilon(const Point &point) const;
    int nearest_point_index(const Points &points) const;
    int nearest_point_index(const PointConstPtrs &points) const;
    int nearest_point_index(const PointPtrs &points) const;
    size_t nearest_waypoint_index(const Points &points, const Point &point) const;
    bool nearest_point(const Points &points, Point* point) const;
    bool nearest_waypoint(const Points &points, const Point &dest, Point* point) const;
    double distance_to(const Point &point) const;
    double distance_to(const Line &line) const;
    double perp_distance_to(const Line &line) const;
    double ccw(const Point &p1, const Point &p2) const;
    double ccw(const Line &line) const;
    double ccw_angle(const Point &p1, const Point &p2) const;
    Point projection_onto(const MultiPoint &poly) const;
    Point projection_onto(const Line &line) const;
    Point negative() const;
    Vector vector_to(const Point &point) const;
    void align_to_grid(const Point &spacing, const Point &base = Point(0,0,0));
};

Point operator+(const Point& point1, const Point& point2);
Point operator-(const Point& point1, const Point& point2);
Point operator*(double scalar, const Point& point2);

inline Points&
operator+=(Points &dst, const Points &src) {
    append_to(dst, src);
    return dst;
};

inline Points&
operator+=(Points &dst, const Point &p) {
    dst.push_back(p);
    return dst;
};

//TODO remove point3 and Point3f
class Point3 : public Point
{
    public:
    coord_t z;
    explicit Point3(coord_t _x = 0, coord_t _y = 0, coord_t _z = 0): Point(_x, _y), z(_z) {};
    static Point3 new_scale(coordf_t x, coordf_t y, coordf_t z) {
        return Point3(scale_(x), scale_(y), scale_(z));
    };
    bool operator==(const Point3& rhs) const;
    std::string wkt() const;
    void scale(double factor);
    void translate(double x, double y, double z);
    void translate(const Vector3 &vector);
    void rotate_z(double angle);
    void rotate_z(double angle, const Point3 &center);
    bool coincides_with(const Point3 &point) const { return this->x == point.x && this->y == point.y && this->z == point.z; }
};

std::ostream& operator<<(std::ostream &stm, const Pointf &pointf);

class Pointf
{
    public:
    coordf_t x;
    coordf_t y;
    explicit Pointf(coordf_t _x = 0, coordf_t _y = 0): x(_x), y(_y) {};
    static Pointf new_unscale(coord_t x, coord_t y) {
        return Pointf(unscale(x), unscale(y));
    };
    static Pointf new_unscale(const Point &p) {
        return Pointf(unscale(p.x), unscale(p.y));
    };
    std::string wkt() const;
    std::string dump_perl() const;
    void scale(double factor);
    void translate(double x, double y);
    void translate(const Vectorf &vector);
    void rotate(double angle);
    void rotate(double angle, const Pointf &center);
    Pointf negative() const;
    Vectorf vector_to(const Pointf &point) const;
};

std::ostream& operator<<(std::ostream &stm, const Pointf3 &pointf3);

class Pointf3 : public Pointf
{
    public:
    coordf_t z;
    explicit Pointf3(coordf_t _x = 0, coordf_t _y = 0, coordf_t _z = 0): Pointf(_x, _y), z(_z) {};
    static Pointf3 new_unscale(coord_t x, coord_t y, coord_t z) {
        return Pointf3(unscale(x), unscale(y), unscale(z));
    };
    static Pointf3 new_unscale(const Point3 &p) {
        return Pointf3(unscale(p.x), unscale(p.y), unscale(p.z));
    };
    std::string wkt() const;
    void scale(double factor);
    void translate(const Vectorf3 &vector);
    void translate(double x, double y, double z);
    void rotate_z(double angle);
    void rotate_z(double angle, const Pointf3 &center);
    double distance_to(const Pointf3 &point) const;
    Pointf3 negative() const;
    Vectorf3 vector_to(const Pointf3 &point) const;
};

template <class T>
inline Points
to_points(const std::vector<T> &items)
{
    Points pp;
    for (typename std::vector<T>::const_iterator it = items.begin(); it != items.end(); ++it)
        append_to(pp, (Points)*it);
    return pp;
}

}

// start Boost
#include <boost/version.hpp>
#include <boost/polygon/polygon.hpp>
namespace boost { namespace polygon {
    template <>
    struct geometry_concept<coord_t> { typedef coordinate_concept type; };

/* Boost.Polygon already defines a specialization for coordinate_traits<long> as of 1.60:
   https://github.com/boostorg/polygon/commit/0ac7230dd1f8f34cb12b86c8bb121ae86d3d9b97 */
#if BOOST_VERSION < 106000
    template <>
    struct coordinate_traits<coord_t> {
        typedef coord_t coordinate_type;
        typedef long double area_type;
        typedef long long manhattan_area_type;
        typedef unsigned long long unsigned_area_type;
        typedef long long coordinate_difference;
        typedef long double coordinate_distance;
    };
#endif

    template <>
    struct geometry_concept<Point> { typedef point_concept type; };

    template <>
    struct point_traits<Point> {
        typedef coord_t coordinate_type;

        static inline coordinate_type get(const Point& point, orientation_2d orient) {
            return (orient == HORIZONTAL) ? point.x : point.y;
        }
    };

    template <>
    struct point_mutable_traits<Point> {
        typedef coord_t coordinate_type;
        static inline void set(Point& point, orientation_2d orient, coord_t value) {
            if (orient == HORIZONTAL)
                point.x = value;
            else
                point.y = value;
        }
        static inline Point construct(coord_t x_value, coord_t y_value) {
            Point retval;
            retval.x = x_value;
            retval.y = y_value;
            return retval;
        }
    };
} }
// end Boost

#endif
