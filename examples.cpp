#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "Geometry.h"
#include "Noise.h"
#include "Random.h"
#include "Util.h"
#include <cstdlib>

#define PrettyPrinterTest
#include "PrettyPrinter.h"

using namespace std;

template <class T>
class InteractiveShape
{
public:
    T shape;
    ALLEGRO_COLOR color;
    InteractiveShape()
    {

    }
    InteractiveShape(const T& s)
    {
        shape = s;
        color = al_map_rgb(0, 255, 0);
    }
    void draw()
    {
        ls::Polyline<typename T::ValueType> outline = shape.asPolyline();
        int outlineSize = outline.size();
        ALLEGRO_VERTEX* vertices = new ALLEGRO_VERTEX[outlineSize + 1];
        int j = 0;
        for(int i = 0; i < outlineSize; ++i)
        {
            vertices[j].x = outline.vertices[i].x;
            vertices[j].y = outline.vertices[i].y;
            vertices[j].color = color;

            j += 1;

            /* does not work LOL
            vertices[j].x = outline.elements[i].begin.x;
            vertices[j].y = outline.elements[i].begin.y;
            vertices[j].color = color;
            ++j;
            vertices[j].x = outline.elements[i].end.x;
            vertices[j].y = outline.elements[i].end.y;
            vertices[j].color = color;
            */
        }
        int i = 0;
        vertices[j].x = outline.vertices[i].x;
        vertices[j].y = outline.vertices[i].y;
        vertices[j].color = color;

        al_draw_prim(vertices, NULL, NULL, 0, outlineSize + 1, ALLEGRO_PRIM_LINE_STRIP);
    }
    bool move(ALLEGRO_MOUSE_STATE& before, ALLEGRO_MOUSE_STATE& after)
    {
        bool moved = false;
        ls::Vec2D posBefore = {(double)before.x, (double)before.y};
        ls::Vec2D posAfter = {(double)after.x, (double)after.y};
        if(before.buttons & 1)
        {
            if(shape.intersects(posBefore))
            {
                shape += posAfter - posBefore;
                moved = true;
            }
        }
        return moved;
    }
    void setColor(ALLEGRO_COLOR c)
    {
        color = c;
    }
    ~InteractiveShape()
    {

    }
};
class RigidBody
{
public:
    PolygonD polygon;
    Vec2D centerOfMass;
    double distanceToFurthestPoint;
    RigidBody(const PolygonD& poly) : polygon(poly)
    {
        centerOfMass = poly.center();
        distanceToFurthestPoint = 0.0;
        for(const auto& v : poly.vertices)
        {
            double mag = (centerOfMass - v).magnitude();
            if(mag > distanceToFurthestPoint)
            {
                distanceToFurthestPoint = mag;
            }
        }
    }
    void dragDrop(const Vec2D& start, const Vec2D& end)
    {
        Vec2D centroidToStart = start - centerOfMass; //these two won't change later, they are the initial parameters which will be used to maintain distance ratios etc.
        double distanceFromCentroidToStart = centroidToStart.magnitude();

        Vec2D centroidToEnd = end - centerOfMass;

        Vec2D translation = (end - start) * (1.0 - (distanceFromCentroidToStart / distanceToFurthestPoint)); //first translation. The closer the start point to the center of mass the more it will be translated to make rotation (applied later) smaller
        centerOfMass += translation;
        polygon += translation;

        centroidToEnd = end - centerOfMass; //this has to be racalculated after every operation on centerOfMass

        AngleD angle = centroidToEnd.angle(centroidToStart); //angle defining rotation made by mouse in centerOfMass's frame of reference

        AffineTransformationD transformation;  //rotation of points
        transformation.translate(centerOfMass); //3
        transformation.rotateAntiClockwise(angle); //2
        transformation.translate(centerOfMass * -1.0); //1
        transformation.transform(centerOfMass);
        for(auto& v : polygon.vertices) transformation.transform(v);

        centroidToEnd = end - centerOfMass; //again, racalculation

        translation = (end - centroidToEnd.normalized() * distanceFromCentroidToStart) - centerOfMass; //final translation. This one is to make centroidToStart == centroidToEnd
        centerOfMass += translation;
        polygon += translation;

        //this will result in object being moved and rotated (to mimic dragging in real world) preserving anchor point (mouse will sty in the same point in polygon)
    }
};


void next(int counter, const RayD& ray, const Mesh2<CircleD>& mesh, const std::vector<RigidBody>& bodies)
{
    if(counter < 0) return; //max recursion depth
    ALLEGRO_COLOR rayColor = al_map_rgb(255, 0, 0);
    double tmin = 111111;
    RaycastD::Hit closestHit;
    bool anyHit = false;
    for(const auto& circle : mesh.elements)
    {
        auto t = Raycaster::raycast(ray, circle);
        for(const auto& hit : t.hits)
        {
            if(hit.distance < tmin)
            {
                tmin = hit.distance;
                closestHit = hit;
                anyHit = true;
            }
        }
    }
    for(const auto& body : bodies)
    {
        const PolygonD& poly = body.polygon;
        auto t = Raycaster::raycast(ray, poly);
        for(const auto& hit : t.hits)
        {
            if(hit.distance < tmin)
            {
                tmin = hit.distance;
                closestHit = hit;
                anyHit = true;
            }
        }
    }
    al_draw_line(ray.origin.x, ray.origin.y, ray.origin.x + ray.direction.x * tmin, ray.origin.y + ray.direction.y * tmin, rayColor, 1.0);
    if(anyHit)
    {
        Vec2D n = closestHit.normal;
        Vec2D directionAfterReflection = ray.direction - n * 2.0 * ray.direction.dot(n);
        next(counter - 1, RayD(closestHit.hitPoint - ray.direction * 0.001, directionAfterReflection), mesh, bodies);
    }
}
template <class T>
void draw(const Vec2<T>& point, const ALLEGRO_COLOR& color)
{
    al_draw_pixel(point.x, point.y, color);
}
template <class T>
void draw(const Circle<T>& circle, const ALLEGRO_COLOR& color)
{
    al_draw_circle(circle.origin.x, circle.origin.y, circle.radius, color);
}
template <class T>
void drawFilled(const Circle<T>& circle, const ALLEGRO_COLOR& color)
{
    al_draw_filled_circle(circle.origin.x, circle.origin.y, circle.radius, color);
}
template <class T>
void draw(const ls::Rectangle<T>& rect, const ALLEGRO_COLOR& color)
{
    al_draw_rectangle(rect.min.x, rect.min.y, rect.max.x, rect.max.y, color, 1);
}
template <class T>
void drawFilled(const ls::Rectangle<T>& rect, const ALLEGRO_COLOR& color)
{
    al_draw_filled_rectangle(rect.min.x, rect.min.y, rect.max.x, rect.max.y, color);
}
template <class T>
void draw(const LineSegment<T>& line, const ALLEGRO_COLOR& color)
{
    al_draw_line(line.begin.x, line.begin.y, line.end.x, line.end.y, color, 1);
}
template <class T>
void draw(const Ray<T>& ray, const ALLEGRO_COLOR& color)
{
    al_draw_line(ray.origin.x, ray.origin.y, ray.origin.x + ray.direction.x * 10000.0f, ray.origin.y + ray.direction.y * 10000.0f, color, 1);
}
template <class T>
void draw(const Triangle<T>& triangle, const ALLEGRO_COLOR& color)
{
    al_draw_triangle(triangle.vertices[0].x, triangle.vertices[0].y,
                     triangle.vertices[1].x, triangle.vertices[1].y,
                     triangle.vertices[2].x, triangle.vertices[2].y,
                     color, 1);
}
template <class T>
void drawFilled(const Triangle<T>& triangle, const ALLEGRO_COLOR& color)
{
    al_draw_filled_triangle(triangle.vertices[0].x, triangle.vertices[0].y,
                            triangle.vertices[1].x, triangle.vertices[1].y,
                            triangle.vertices[2].x, triangle.vertices[2].y,
                            color);
}
template <class T>
void draw(const ls::Polygon<T>& polygon, const ALLEGRO_COLOR& color)
{
    std::vector<ALLEGRO_VERTEX> vertices;
    size_t size = polygon.size();
    if(size < 3) return;
    vertices.reserve(size + 1);
    for(size_t i = 0; i <= size; ++i)
    {
        const Vec2<T>& v = polygon.vertices[i % size];
        vertices.push_back(ALLEGRO_VERTEX {static_cast<float>(v.x), static_cast<float>(v.y), 0.0f, 0.0f, 0.0f, color});
    }

    al_draw_prim(vertices.data(), nullptr, nullptr, 0, size + 1, ALLEGRO_PRIM_LINE_STRIP);
}
template <class T>
void draw(const ls::Polyline<T>& polyline, const ALLEGRO_COLOR& color)
{
    std::vector<ALLEGRO_VERTEX> vertices;
    size_t size = polyline.size();
    vertices.reserve(size);
    for(size_t i = 0; i < size; ++i)
    {
        const Vec2<T>& v = polyline[i];
        vertices.push_back(ALLEGRO_VERTEX {static_cast<float>(v.x), static_cast<float>(v.y), 0.0f, 0.0f, 0.0f, color});
    }

    al_draw_prim(vertices.data(), nullptr, nullptr, 0, size, ALLEGRO_PRIM_LINE_STRIP);
}
template <class ShapeType>
void draw(const Mesh2<ShapeType>& mesh, const ALLEGRO_COLOR& color)
{
    for(const auto& shape : mesh.elements)
        draw(shape, color);
}
template <class ShapeType>
void draw(const std::vector<ShapeType>& mesh, const ALLEGRO_COLOR& color)
{
    for(const auto& shape : mesh)
        draw(shape, color);
}

void delaunayVoronoiExamples()
{
    //may generate wrong polygons close to boundary because polygons that stretch to infinity are not handled yet
    constexpr size_t numberOfPoints = 100u;
    StandardRandomNumberGeneratorWrapper<std::mt19937> randomEngine;
    RectangleD boundingRect(Vec2D {100.0, 100.0}, Vec2D {1200.0, 700.0});

    std::vector<Vec2D> points;
    points.reserve(numberOfPoints);

    RandomPointOnRectanglePicker<double> pointPicker(boundingRect);
    for(size_t i = 0; i < numberOfPoints; ++i)
    {
        points.push_back(pointPicker.nextPoint(randomEngine));
    }

    PointSetDelaunayTriangulationD triangulation(points);
    VoronoiDiagramD voronoi(triangulation);
    std::cout << "Number of generated triangles: " << triangulation.triangleMesh().size() << '\n';
    std::cout << "Number of generated polygons: " << voronoi.polygons().size() << '\n';

    ALLEGRO_KEYBOARD_STATE keyboardState;
    for(;;)
    {
        al_get_keyboard_state(&keyboardState);
        if(al_key_down(&keyboardState, ALLEGRO_KEY_ESCAPE)) break;

        al_clear_to_color(al_map_rgb(0, 0, 0));

        draw(triangulation.triangleMesh(), al_map_rgb(200, 0, 0));
        draw(voronoi.polygons(), al_map_rgb(0, 255, 0));

        al_flip_display();
    }
}

void cellularAutomatonCaveGenerationExample()
{
    //white floor, black wall
    constexpr size_t cellSize = 10u;
    constexpr size_t width = 100u;
    constexpr size_t height = 70u;
    constexpr size_t padding = 10u;

    CellularAutomaton<QuantityRules<OriginalCellularAutomatonStates>> ca(
                QuantityRules<OriginalCellularAutomatonStates>(
                    OriginalCellularAutomatonStates::Black,
                    std::array<OriginalCellularAutomatonStates, 10u>
    {
        OriginalCellularAutomatonStates::White,
        OriginalCellularAutomatonStates::White,
        OriginalCellularAutomatonStates::White,
        OriginalCellularAutomatonStates::White,
        OriginalCellularAutomatonStates::White,
        OriginalCellularAutomatonStates::Black,
        OriginalCellularAutomatonStates::Black,
        OriginalCellularAutomatonStates::Black,
        OriginalCellularAutomatonStates::Black,
        OriginalCellularAutomatonStates::Black
    }),
    width, height);
    ca.fill([width, height](size_t x, size_t y) -> OriginalCellularAutomatonStates
    {
        static StandardRandomNumberGeneratorWrapper<std::mt19937> randomEngine;
        if(width - x <= 2 || x <= 1 || height - y <= 2 || y <= 1) return OriginalCellularAutomatonStates::Black;
        return (randomEngine.nextDouble() < 0.50 ? OriginalCellularAutomatonStates::Black : OriginalCellularAutomatonStates::White);
    });
    ALLEGRO_KEYBOARD_STATE keyboardState;
    for(;;)
    {
        al_get_keyboard_state(&keyboardState);
        if(al_key_down(&keyboardState, ALLEGRO_KEY_ESCAPE)) break;
        if(al_key_down(&keyboardState, ALLEGRO_KEY_UP))
        {
            ca.iterate();
            al_rest(0.1f);
        }

        al_clear_to_color(al_map_rgb(0, 0, 0));

        for(size_t x = 0u; x < width; ++x)
        {
            for(size_t y = 0u; y < height; ++y)
            {
                drawFilled(
                    RectangleF(
                        Vec2F(padding + x * cellSize, padding + y * cellSize),
                        10.0f,
                        10.0f),
                    ca.cellAt(x, y) == OriginalCellularAutomatonStates::Black ? al_map_rgb(64, 64, 64) : al_map_rgb(255, 255, 255));
            }
        }
        al_flip_display();
    }
}
void conwaysGameOfLifeExample()
{
    //white floor, black wall
    constexpr size_t cellSize = 10u;
    constexpr size_t width = 100u;
    constexpr size_t height = 70u;
    constexpr size_t padding = 10u;

    CellularAutomaton<ConwaysGameOfLifeRules> ca(ConwaysGameOfLifeRules(), width, height, ConwaysGameOfLifeRules::States::Dead, CellularAutomatonGridTopology::Toroidal);

    constexpr auto D = ConwaysGameOfLifeRules::States::Dead;
    constexpr auto L = ConwaysGameOfLifeRules::States::Live;

    std::vector<Array2<ConwaysGameOfLifeRules::States>> patterns(
    {
        {
            {D, D, L, D},
            {L, D, D, L},
            {L, D, D, L},
            {D, L, D, D}
        },

        {
            {D, D, D, D, L, D, D, D, D, D, L, D, D, D, D},
            {D, D, D, D, L, D, D, D, D, D, L, D, D, D, D},
            {D, D, D, D, L, L, D, D, D, L, L, D, D, D, D},
            {D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
            {L, L, L, D, D, L, L, D, L, L, D, D, L, L, L},
            {D, D, L, D, L, D, L, D, L, D, L, D, L, D, D},
            {D, D, D, D, L, L, D, D, D, L, L, D, D, D, D},
            {D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
            {D, D, D, D, L, L, D, D, D, L, L, D, D, D, D},
            {D, D, L, D, L, D, L, D, L, D, L, D, L, D, D},
            {L, L, L, D, D, L, L, D, L, L, D, D, L, L, L},
            {D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
            {D, D, D, D, L, L, D, D, D, L, L, D, D, D, D},
            {D, D, D, D, L, D, D, D, D, D, L, D, D, D, D},
            {D, D, D, D, L, D, D, D, D, D, L, D, D, D, D}
        },
        {
            {D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, L, D, D, D, D, D, D, D, D},
            {D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, L, D, L, D, D, D, D, D, D, D},
            {D, D, D, D, D, D, D, D, D, L, L, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, L, L, D, L, D, D, D, D, D, D},
            {D, D, D, D, D, D, D, D, D, L, D, L, D, D, D, D, D, D, D, D, D, D, D, D, D, D, L, L, D, L, L, D, D, D, L, L},
            {D, D, D, D, L, L, D, D, D, D, D, D, L, D, D, D, D, D, D, D, D, D, D, D, D, D, L, L, D, L, D, D, D, D, L, L},
            {L, L, D, L, D, D, L, D, D, L, D, D, L, D, D, D, D, D, D, D, D, D, D, D, D, D, L, D, L, D, D, D, D, D, D, D},
            {L, L, D, D, L, L, D, D, D, D, D, D, L, D, D, D, D, D, D, D, D, D, D, D, D, D, D, L, D, D, D, D, D, D, D, D},
            {D, D, D, D, D, D, D, D, D, L, D, L, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
            {D, D, D, D, D, D, D, D, D, L, L, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D}
        }
    });

    constexpr size_t selectedPattern = 2u;

    size_t patternWidth = patterns[selectedPattern].sizeX();
    size_t patternHeight = patterns[selectedPattern].sizeY();

    size_t patternOriginX = width / 2 - patternWidth / 2;
    size_t patternOriginY = height / 2 - patternHeight / 2;


    for(size_t x = 0u; x < patternWidth; ++x)
    {
        for(size_t y = 0u; y < patternHeight; ++y)
        {
            ca.setCell(x + patternOriginX, y + patternOriginY, patterns[selectedPattern](x, y));
        }
    }

    ALLEGRO_KEYBOARD_STATE keyboardState;
    for(;;)
    {
        al_get_keyboard_state(&keyboardState);
        if(al_key_down(&keyboardState, ALLEGRO_KEY_ESCAPE)) break;
        if(al_key_down(&keyboardState, ALLEGRO_KEY_UP))
        {
            ca.iterate();
            al_rest(0.05f);
        }

        al_clear_to_color(al_map_rgb(0, 0, 0));

        for(size_t x = 0u; x < width; ++x)
        {
            for(size_t y = 0u; y < height; ++y)
            {
                drawFilled(
                    RectangleF(
                        Vec2F(padding + x * cellSize, padding + y * cellSize),
                        10.0f,
                        10.0f),
                    ca.cellAt(x, y) == ConwaysGameOfLifeRules::States::Live ? al_map_rgb(0, 200, 0) : al_map_rgb(255, 255, 255));
            }
        }
        al_flip_display();
    }
}
int main()
{
    al_init();
    al_init_primitives_addon();
    al_install_mouse();
    al_install_keyboard();
    al_create_display(1280, 800);

    //delaunayVoronoiExample();
    //cellularAutomatonCaveGenerationExample();
    conwaysGameOfLifeExample();

    return 0;
}
