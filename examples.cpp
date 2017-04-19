#include <iostream>
#include <cstdlib>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "Geometry.h"
#include "CellularAutomata.h"
#include "Array2.h"
#include "EuclideanGraphAlgorithms.h"

#include "main.cpp"

using namespace std;
using namespace ls;

/*
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
}*/
/*
template <class T>
void draw(sf::RenderWindow& window, Vec2<T>& point, const sf::Color& color)
{
    sf::Vertex vertex(sf::Vector2f(point.x, point.y), color);
    window.draw(vertex);
}
template <class T>
void draw(sf::RenderWindow& window, const Circle<T>& circle, const sf::Color& color)
{
    sf::CircleShape circleShape(circle.radius);
    circleShape.setPosition(circle.origin.x, circle.origin.y);
    circleShape.setFillColor(sf::Color::Transparent);
    circleShape.setOutlineColor(color);
    circleShape.setOutlineThickness(1.0f);
    window.draw(circleShape);
}
template <class T>
void drawFilled(sf::RenderWindow& window, const Circle<T>& circle, const sf::Color& color)
{
    sf::CircleShape circleShape(circle.radius);
    circleShape.setPosition(circle.origin.x, circle.origin.y);
    circleShape.setFillColor(color);
    circleShape.setOutlineThickness(0.0f);
    window.draw(circleShape);
}
template <class T>
void draw(sf::RenderWindow& window, const ls::Rectangle<T>& rect, const sf::Color& color)
{
    sf::RectangleShape rectangleShape(sf::Vector2f(rect.width(), rect.height()));
    rectangleShape.setPosition(rect.min.x, rect.min.y);
    rectangleShape.setFillColor(sf::Color::Transparent);
    rectangleShape.setOutlineColor(color);
    rectangleShape.setOutlineThickness(1.0f);
    window.draw(rectangleShape);
}
template <class T>
void drawFilled(sf::RenderWindow& window, const ls::Rectangle<T>& rect, const sf::Color& color)
{
    sf::RectangleShape rectangleShape(sf::Vector2f(rect.width(), rect.height()));
    rectangleShape.setPosition(rect.min.x, rect.min.y);
    rectangleShape.setFillColor(color);
    rectangleShape.setOutlineThickness(0.0f);
    window.draw(rectangleShape);
}
template <class T>
void draw(sf::RenderWindow& window, const LineSegment<T>& line, const sf::Color& color)
{
    sf::ConvexShape lineShape;
    lineShape.setPointCount(2);
    lineShape.setPoint(0, sf::Vector2f(ray.origin.x, ray.origin.y));
    lineShape.setPoint(1, sf::Vector2f(ray.origin.x + ray.direction.x * 10000.0f, ray.origin.y + ray.direction.y * 10000.0f));
    lineShape.setFillColor(sf::Color::Transparent);
    lineShape.setOutlineColor(color);
    lineShape.setOutlineThickness(1.0f);
    window.draw(lineShape);
}
template <class T>
void draw(sf::RenderWindow& window, const Ray<T>& ray, const sf::Color& color)
{
    sf::ConvexShape lineShape;
    lineShape.setPointCount(2);
    lineShape.setPoint(0, sf::Vector2f(ray.origin.x, ray.origin.y));
    lineShape.setPoint(1, sf::Vector2f(ray.origin.x + ray.direction.x * 10000.0f, ray.origin.y + ray.direction.y * 10000.0f));
    lineShape.setFillColor(sf::Color::Transparent);
    lineShape.setOutlineColor(color);
    lineShape.setOutlineThickness(1.0f);
    window.draw(lineShape);
}
template <class T>
void draw(sf::RenderWindow& window, const Triangle<T>& triangle, const sf::Color& color)
{
    sf::ConvexShape triangleShape;
    triangleShape.setPointCount(3);
    triangleShape.setPoint(0, sf::Vector2f(triangle.vertices[0].x, triangle.vertices[0].y));
    triangleShape.setPoint(1, sf::Vector2f(triangle.vertices[1].x, triangle.vertices[1].y));
    triangleShape.setPoint(2, sf::Vector2f(triangle.vertices[2].x, triangle.vertices[2].y));
    triangleShape.setFillColor(sf::Color::Transparent);
    triangleShape.setOutlineColor(color);
    triangleShape.setOutlineThickness(1.0f);
    window.draw(triangleShape);
}
template <class T>
void drawFilled(sf::RenderWindow& window, const Triangle<T>& triangle, const sf::Color& color)
{
    sf::ConvexShape triangleShape;
    triangleShape.setPointCount(3);
    triangleShape.setPoint(0, sf::Vector2f(triangle.vertices[0].x, triangle.vertices[0].y));
    triangleShape.setPoint(1, sf::Vector2f(triangle.vertices[1].x, triangle.vertices[1].y));
    triangleShape.setPoint(2, sf::Vector2f(triangle.vertices[2].x, triangle.vertices[2].y));
    triangleShape.setFillColor(color);
    triangleShape.setOutlineThickness(0.0f);
    window.draw(triangleShape);
}
template <class T>
void draw(sf::RenderWindow& window, const ls::Polygon<T>& polygon, const sf::Color& color)
{
    sf::ConvexShape poly;
    size_t size = polygon.size();
    if(size < 3) return;
    poly.setPointCount(size);
    for(size_t i = 0; i <= size; ++i)
    {
        poly.setPoint(i, sf::Vector2f(polygon.vertices[i].x, polygon.vertices[i].y));
    }
    poly.setFillColor(sf::Color::Transparent);
    poly.setOutlineColor(color);
    poly.setOutlineThickness(1.0f);

    window.draw(poly);
}
template <class T>
void draw(sf::RenderWindow& window, const ls::Polyline<T>& polyline, const sf::Color& color)
{
    sf::ConvexShape poly;
    size_t size = polygon.size();
    if(size < 3) return;
    poly.setPointCount(size);
    for(size_t i = 0; i <= size; ++i)
    {
        poly.setPoint(i, sf::Vector2f(polygon.vertices[i].x, polygon.vertices[i].y));
    }
    poly.setFillColor(sf::Color::Transparent);
    poly.setOutlineColor(color);
    poly.setOutlineThickness(1.0f);

    window.draw(poly);
}
template <class ShapeType>
void draw(sf::RenderWindow& window, const Mesh2<ShapeType>& mesh, const sf::Color& color)
{
    for(const auto& shape : mesh.elements)
        draw(window, shape, color);
}
template <class ShapeType>
void draw(sf::RenderWindow& window, const std::vector<ShapeType>& mesh, const sf::Color& color)
{
    for(const auto& shape : mesh)
        draw(window, shape, color);
}

void delaunayVoronoiExamples()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "LibS examples");

    //may generate wrong polygons close to boundary because polygons that stretch to infinity are not handled yet
    constexpr size_t numberOfPoints = 100u;
    std::mt19937 randomEngine;
    RectangleD boundingRect(Vec2D{100.0, 100.0}, Vec2D{1200.0, 700.0});

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

    for(;;)
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) break;

        draw(window, triangulation.triangleMesh(), sf::Color(200, 0, 0));
        draw(window, voronoi.polygons(), sf::Color(0, 255, 0));

        window.display();
    }
}

void cellularAutomatonCaveGenerationExample()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "LibS examples");

    //white floor, black wall
    constexpr size_t cellSize = 10u;
    constexpr size_t width = 100u;
    constexpr size_t height = 70u;
    constexpr size_t padding = 10u;

    CellularAutomaton<QuantityRules<DefaultCellularAutomatonStates>> ca(
        QuantityRules<DefaultCellularAutomatonStates>(
            DefaultCellularAutomatonStates::Black,
            std::array<DefaultCellularAutomatonStates, 10u>
    {
        DefaultCellularAutomatonStates::White,
        DefaultCellularAutomatonStates::White,
        DefaultCellularAutomatonStates::White,
        DefaultCellularAutomatonStates::White,
        DefaultCellularAutomatonStates::White,
        DefaultCellularAutomatonStates::Black,
        DefaultCellularAutomatonStates::Black,
        DefaultCellularAutomatonStates::Black,
        DefaultCellularAutomatonStates::Black,
        DefaultCellularAutomatonStates::Black
    }),
        width, height);
    ca.fill([width, height] (size_t x, size_t y) -> DefaultCellularAutomatonStates {
        static std::mt19937 randomEngine;
        static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
        if(width - x <= 2 || x <= 1 || height - y <= 2 || y <= 1) return DefaultCellularAutomatonStates::Black;
        return (distribution(randomEngine) < 0.50 ? DefaultCellularAutomatonStates::Black : DefaultCellularAutomatonStates::White);
    });

    for(;;)
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) break;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            ca.iterate();
            sf::sleep(sf::milliseconds(100));
        }

        for(size_t x = 0u; x < width; ++x)
        {
            for(size_t y = 0u; y < height; ++y)
            {
                drawFilled(
                    window,
                    RectangleF(
                        Vec2F(padding + x * cellSize, padding + y * cellSize),
                        10.0f,
                        10.0f),
                    ca.cellAt(x, y) == DefaultCellularAutomatonStates::Black ? sf::Color(64, 64, 64) : sf::Color(255, 255, 255));
            }
        }
        window.display();
    }
}
void conwaysGameOfLifeExample()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "LibS examples");

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

    for(;;)
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) break;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            ca.iterate();
            sf::sleep(sf::milliseconds(50));
        }

        for(size_t x = 0u; x < width; ++x)
        {
            for(size_t y = 0u; y < height; ++y)
            {
                drawFilled(
                    window,
                    RectangleF(
                        Vec2F(padding + x * cellSize, padding + y * cellSize),
                        10.0f,
                        10.0f),
                    ca.cellAt(x, y) == ConwaysGameOfLifeRules::States::Live ? sf::Color(0, 200, 0) : sf::Color(255, 255, 255));
            }
        }
        window.display();
    }
}
int main()
{
    //delaunayVoronoiExample();
    //cellularAutomatonCaveGenerationExample();
    conwaysGameOfLifeExample();

    return 0;
}
*/

void print(const Array2<int>& a)
{
    const int width = a.width();
    const int height = a.height();
    for(int y = 0; y < height; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
            if(a[x][y] != a(x, y)) throw runtime_error("a[x][y] != a(x, y)");
            std::cout << a(x, y) << ' ';
        }
        std::cout << '\n';
    }
}

template <class T, class GraphRepresentationTag, bool IsDirected, class StorageTypeReference>
void print(const Graph<T, GraphRepresentationTag, IsDirected, StorageTypeReference>& graph, std::ostream& out = std::cout)
{
    const int numberOfNodes = graph.numberOfNodes();

    out << "Vertices:\n";
    for(int i = 0; i < numberOfNodes; ++i)
    {
        const auto& node = graph.node(i);
        out << '(' << i << ", " << node.position() << ")\n";
    }

    std::set<std::pair<int, int>> edges;
    for(int i = 0; i < numberOfNodes; ++i)
    {
        for(const auto& j : graph.neighbours(i))
        {
            int ii = i;
            int jj = j.v;
            //if(ii > jj) std::swap(ii, jj);
            edges.emplace(ii, jj);
        }
    }

    out << "Edges:\n";
    for(const auto& edge : edges)
    {
        out << '(' << edge.first << ", " << edge.second << ")\n";
    }
}

template <class T, bool IsDirected, class StorageTypeReference>
void print(const Graph<T, EdgeSetTag, IsDirected, StorageTypeReference>& graph, std::ostream& out = std::cout)
{
    const int numberOfVertices = graph.numberOfNodes();
    out << "Vertices:\n";
    for(int i = 0; i < numberOfVertices; ++i)
    {
        const auto& vertex = graph.node(i);
        out << '(' << i << ", " << vertex.position() << ")\n";
    }

    out << "Edges:\n";
    for(const auto& edge : graph.edges())
    {
        out << '(' << edge.u << ", " << edge.v << ")\n";
    }
}

template <class T, class GraphRepresentationTag, bool IsDirected, class StorageTypeReference>
void draw(sf::RenderTarget& window, const sf::RenderStates& renderStates, const Graph<T, GraphRepresentationTag, IsDirected, StorageTypeReference>& graph)
{
    constexpr float radius = 4.0f;
    const int numberOfNodes = graph.numberOfNodes();

    for(int i = 0; i < numberOfNodes; ++i)
    {
        const auto& node = graph.node(i);
        sf::CircleShape circle;
        circle.setPosition(node.position().x - radius, node.position().y - radius);
        circle.setFillColor(sf::Color::Red);
        circle.setRadius(radius);
        window.draw(circle);
    }

    std::set<std::pair<int, int>> edges;
    for(int i = 0; i < numberOfNodes; ++i)
    {
        for(const auto& j : graph.neighbours(i))
        {
            int ii = i;
            int jj = j.v;
            if(ii > jj) std::swap(ii, jj);
            edges.emplace(ii, jj);
        }
    }

    const int numberOfEdges = edges.size();
    sf::VertexArray vertexBuffer(sf::PrimitiveType::Lines, numberOfEdges * 2);
    for(const auto& edge : edges)
    {
        const auto& from = graph.node(edge.first).position();
        const auto& to = graph.node(edge.second).position();

        vertexBuffer.append(sf::Vertex(sf::Vector2f(from.x, from.y), sf::Color::Green));
        vertexBuffer.append(sf::Vertex(sf::Vector2f(to.x, to.y), sf::Color::Green));
    }
    window.draw(vertexBuffer);
}

template <class T, bool IsDirected, class StorageTypeReference>
void draw(sf::RenderTarget& window, const sf::RenderStates& renderStates, const Graph<T, EdgeSetTag, IsDirected, StorageTypeReference>& graph)
{
    constexpr float radius = 4.0f;
    const int numberOfNodes = graph.numberOfNodes();

    for(int i = 0; i < numberOfNodes; ++i)
    {
        const auto& node = graph.node(i);
        sf::CircleShape circle;
        circle.setPosition(node.position().x - radius, node.position().y - radius);
        circle.setFillColor(sf::Color::Red);
        circle.setRadius(radius);
        window.draw(circle);
    }

    const int numberOfEdges = graph.edges().size();
    sf::VertexArray vertexBuffer(sf::PrimitiveType::Lines, numberOfEdges * 2);
    for(const auto& edge : graph.edges())
    {
        const auto& from = graph.node(edge.u).position();
        const auto& to = graph.node(edge.v).position();

        vertexBuffer.append(sf::Vertex(sf::Vector2f(from.x, from.y), sf::Color::Green));
        vertexBuffer.append(sf::Vertex(sf::Vector2f(to.x, to.y), sf::Color::Green));
    }
    window.draw(vertexBuffer);
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Examples");

    const Array2<int> a = {{1,2,3}, {4,5,6}};
    print(a);

    std::cout << "\n\n";

    for(const auto& el : a[1].moveToNextCol()) std::cout << el << ' ';

    std::cout << "\n\n";

    for(const auto& el : a.row(1)) std::cout << el << ' ';

    std::cout << "\n\n";

    //IndexRegion2 r = a.possibleIndices();
    IndexRegion2 r = IndexRegion2(1, 1, a.width() - 1, a.height() - 1);
    for(auto i : r)
    {
        std::cout << "(" << i.x << ", " << i.y << "), ";
    }

    std::cout << "\n\n";

    DynamicPtr<int> ptr = std::make_unique<int>(13);
    std::cout << *ptr;
    std::cout << ptr.isOwning();

    Circle2D c1(Vec2D(100, 0), 100);
    Circle2D c2(Vec2D(400, 0), 100);
    Vec2D v1(100, 0);
    Vec2D v2(-20, 0);
    auto c = continuousCollision(moving(c1, v1), moving(c2, v2));
    std::cout << c.value().timeToImpact();

    Matrix2x2D m1;
    std::cout << m1.determinant();
    Matrix3x3D m2;
    std::cout << m2.determinant();
    Matrix4x4D m3;
    std::cout << m3.trace();

    std::cout << "\n\n\n\n";
    Vec2D v3(10.0, 5.0);
    Vec2D v4(3.0, 4.0);
    Vec2D v5 = v3 / v4;
    Vec2D v6 = v3 / 4;
    Vec2D v7 = 4 / v4;
    Vec2D v8 = v3 * v4;
    Vec2D v9 = v3 * 4;
    Vec2D v10 = 4 * v4;
    std::cout << v5 << '\n';
    std::cout << v6 << '\n';
    std::cout << v7 << '\n';
    std::cout << v8 << '\n';
    std::cout << v9 << '\n';
    std::cout << v10 << '\n';

    struct F
    {
        uint32_t operator()(uint32_t x) const { return Util::hash(x); }
    };

    SimplexNoise<double, 1, F> p1;
    SimplexNoise2D p2;
    SimplexNoise3D p3;
    SimplexNoise4D p4;
    std::cout << p1(1.4) << '\n';
    std::cout << p1(256.4) << '\n';
    std::cout << p2({1.4, 2.4}) << '\n';
    std::cout << p2({256.4, 2.4}) << '\n';
    std::cout << p3({1.4, 2.4, 3.4}) << '\n';
    std::cout << p3({256.4, 2.4, 3.4}) << '\n';
    std::cout << p4({1.4, 2.4, 3.4, 4.4}) << '\n';
    std::cout << p4({256.4, 2.4, 3.4, 4.4}) << '\n';

    using GraphType = AdjacencyListEuclideanGraph2F;
    //using GraphType = UndirectedAdjacencyMatrixEuclideanGraph2F;
    //using GraphType = UndirectedEdgeListEuclideanGraph2F;

    GraphType g;
    /*
    const int numberOfVertices = 20;
    const int numberOfEdges = numberOfVertices*(numberOfVertices - 1) / 2 / 8;
    std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<float> xDistr(50.0f, 750.0f);
    std::uniform_real_distribution<float> yDistr(50.0f, 550.0f);
    std::uniform_int_distribution<int> vDistr(0, numberOfVertices - 1);

    for(int i = 0; i < numberOfVertices; ++i)
    {
        float x = xDistr(rng);
        float y = yDistr(rng);
        g.emplaceVertex(Vec2F(x, y));
    }
    std::vector<std::pair<int, int>> allEdges;
    for(int i = 0; i < numberOfVertices; ++i)
    {
        for(int j = i + 1; j < numberOfVertices; ++j)
        {
            allEdges.emplace_back(i, j);
        }
    }
    std::sort(allEdges.begin(), allEdges.end(), [&g] (const std::pair<int, int>& lhs, const std::pair<int, int>& rhs)->bool {return g.vector(lhs.first, lhs.second).magnitude() < g.vector(rhs.first, rhs.second).magnitude(); });


    for(int i = 0; i < numberOfEdges; ++i)
    {
        g.addEdge(allEdges[i].first, allEdges[i].second);
    }
    */

    const int numberOfVertices = 5;
    std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<float> xDistr(50.0f, 750.0f);
    std::uniform_real_distribution<float> yDistr(50.0f, 550.0f);
    std::uniform_int_distribution<int> vDistr(0, numberOfVertices - 1);

    for(int i = 0; i < numberOfVertices; ++i)
    {
        //float x = xDistr(rng);
        //float y = yDistr(rng);
        //g.emplaceVertex(Vec2F(x, y));
    }
    g.emplaceNode(Vec2F(200, 100));
    g.emplaceNode(Vec2F(100, 200));
    g.emplaceNode(Vec2F(300, 200));
    g.emplaceNode(Vec2F(150, 300));
    g.emplaceNode(Vec2F(250, 300));
    g.emplaceNode(Vec2F(250, 300));
    std::vector<std::pair<int, int>> allEdges;
    for(int i = 0; i < numberOfVertices; ++i)
    {
        for(int j = i + 1; j < numberOfVertices; ++j)
        {
            if(xDistr(rng) > 400.0f)
            {
                allEdges.emplace_back(i, j);
            }
            else
            {
                allEdges.emplace_back(j, i);
            }
        }
    }
    std::sort(allEdges.begin(), allEdges.end(), [&g] (const std::pair<int, int>& lhs, const std::pair<int, int>& rhs)->bool {return g.edgeVector(lhs.first, lhs.second).magnitude() < g.edgeVector(rhs.first, rhs.second).magnitude(); });


    for(const auto& edge : allEdges)
    {
        g.emplaceEdge(edge.first, edge.second);
        std::cout << g.edgeWeight(edge.first, edge.second) << '\n';
    }

    print(g);

    draw(window, sf::RenderStates::Default, g);
    window.display();

    auto path = closedEulerianPath(g);
    for(int v : path.vertices()) std::cout << v << ' ';


    std::cout << '\n';
    std::cout << detail::HasMethod_weight<EuclideanGraphNode2D>::value << '\n';
    std::cout << detail::HasMethod_weight<WeightedGraphEdge<float>>::value << '\n';

	std::cout << "\n\n\n\n\n\n";
	PointMass2D pa(Vec2D(0, 0), 0.5);
	PointMass2D pb(Vec2D(0, 0), 1.0);
	pa.setVelocity(Vec2D(5.0, 0.0));
	pb.setVelocity(Vec2D(0, 0));
	double eps = 0.5;
	resolveImpactVelocities(pa, pb, Vec2D(1.0, 0), eps, ImpactType::Reflection);
	std::cout << pa.velocity().x << ' ' << pa.velocity().y << '\n';
	std::cout << pb.velocity().x << ' ' << pb.velocity().y << '\n';

	BinaryTree<std::string> b("asd");
	b.emplaceLeft(b.rootHandle(), "asdvz");
	b.emplaceLeft(b.emplaceRight(b.rootHandle(), "123"), "321");
	std::cout << b.iterator(b.rootHandle()).data() << '\n';
	std::cout << b.iterator(b.rootHandle()).left().data() << '\n';
	std::cout << b.iterator(b.rootHandle()).left().data() << '\n';
	std::cout << b.iterator(b.rootHandle()).right().left().data() << '\n';
	std::cout << '\n';
	BinaryTree<std::string> bb(b);
	std::cout << bb.node(bb.rootHandle()) << '\n';
	std::cout << bb.node(bb.left(bb.rootHandle())) << '\n';
	std::cout << bb.node(bb.right(bb.rootHandle())) << '\n';
	std::cout << bb.node(bb.left(bb.right(bb.rootHandle()))) << '\n';
	std::cout << '\n';
	BinaryTree<std::string> bbb("vvv");
	bbb = bb;
	std::cout << bbb.node(bbb.rootHandle()) << '\n';
	std::cout << bbb.node(bbb.left(bbb.rootHandle())) << '\n';
	std::cout << bbb.node(bbb.right(bbb.rootHandle())) << '\n';
	std::cout << bbb.node(bbb.left(bbb.right(bbb.rootHandle()))) << '\n';
	std::cout << '\n';
	bbb = std::move(b);
	std::cout << bbb.node(bbb.rootHandle()) << '\n';
	std::cout << bbb.node(bbb.left(bbb.rootHandle())) << '\n';
	std::cout << bbb.node(bbb.right(bbb.rootHandle())) << '\n';
	std::cout << bbb.node(bbb.left(bbb.right(bbb.rootHandle()))) << '\n';
	std::cout << bbb.node(bbb.parent(bbb.left(bbb.right(bbb.rootHandle())))) << '\n';
	

    for(;;)
    {
        sf::Event ev;
        while(window.pollEvent(ev));
    }
}

