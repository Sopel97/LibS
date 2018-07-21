#include "LibS.h"

#include <iostream>
#include <memory>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

template <typename T, int N>
std::vector<ls::Vec2<T>> discretizeBezierCurve(const ls::BezierCurve2<T, N>& curve, int n)
{
    std::vector<ls::Vec2<T>> points;
    points.reserve(n);

    int m = n - 1;
    for (int i = 0; i <= m; ++i)
    {
        points.emplace_back(curve.at(i*(static_cast<T>(1) / m)));
    }

    return points;
}

template <typename T, int N>
std::vector<ls::Vec2<T>> discretizeBezierCurveEquidistant(const ls::BezierCurve2<T, N>& curve, int n)
{
    std::vector<ls::Vec2<T>> points;
    points.reserve(n);

    for (auto t : ls::EquidistantBezierCurveParams(curve, n, 0.0, 1.0, 4, ls::LegendreGaussIntegrator8D{}))
    {
        points.emplace_back(curve.at(t));
    }

    return points;
}

template <typename T>
void drawLines(sf::RenderWindow& window, const std::vector<ls::Vec2<T>>& points, sf::Color c)
{
    sf::VertexArray arr(sf::LinesStrip, points.size());

    int i = 0;
    for (auto&& p : points)
    {
        arr[i++] = sf::Vertex(sf::Vector2f(static_cast<float>(p.x), static_cast<float>(p.y)), c);
    }

    window.draw(arr);
}

template <typename T>
void drawLines2(sf::RenderWindow& window, const std::vector<ls::Vec2<T>>& points, sf::Color c1, sf::Color c2)
{
    sf::VertexArray arr(sf::Lines, points.size() * 2);

    for (int i = 0; i < points.size() - 1; ++i)
    {
        const auto& p1 = points[i];
        const auto& p2 = points[i + 1];

        arr[i * 2] = sf::Vertex(sf::Vector2f(static_cast<float>(p1.x), static_cast<float>(p1.y)), c1);
        arr[i * 2 + 1] = sf::Vertex(sf::Vector2f(static_cast<float>(p2.x), static_cast<float>(p2.y)), c1);

        std::swap(c1, c2);
    }

    window.draw(arr);
}

void graphicalExamples()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear();

        auto curve4 = ls::BezierCurve2F<4>(ls::Vec2F{ 120, 42 }, ls::Vec2F{ 35, 200 }, ls::Vec2F{ 220, 260 }, ls::Vec2F{ 210, 68 }, ls::Vec2F{ 210, 0 });
        //auto curve3 = ls::CubicBezierCurve2D(ls::Vec2D{ 120, 42 }, ls::Vec2D{ 35, 200 }, ls::Vec2D{ 220, 260 }, ls::Vec2D{ 210, 68 });
        auto curve3 = ls::CubicBezierCurve2D(ls::Vec2D{ 146, 43 }, ls::Vec2D{ 168, 263 }, ls::Vec2D{ 56, 74 }, ls::Vec2D{ 263, 183 });
        auto curve3_1 = ls::CubicBezierCurve2D(ls::Vec2D{ 91, 88 }, ls::Vec2D{ 24, 233 }, ls::Vec2D{ 208, 102 }, ls::Vec2D{ 102, 123 });
        auto curve3_2 = ls::CubicBezierCurve2D(ls::Vec2D{ 59, 18 }, ls::Vec2D{ 79, 221 }, ls::Vec2D{ 10, 193 }, ls::Vec2D{ 223, 151 });
        auto curve3_3 = ls::CubicBezierCurve2D(ls::Vec2D{ 59, 18 }, ls::Vec2D{ 143, 88 }, ls::Vec2D{ 10, 193 }, ls::Vec2D{ 223, 151 });
        auto curve2 = ls::QuadraticBezierCurve2F(ls::Vec2F{ 257, 192 }, ls::Vec2F{ 151, 83 }, ls::Vec2F{ 156, 214 });


        /*
        auto[ex, ey] = curve3_1.extrema();

        for (auto x : ex) std::cout << x << ' ';
        std::cout << '\n';
        for (auto y : ey) std::cout << y << ' ';
        std::cout << '\n';
        std::cout << '\n';
        */

        auto inflections2 = curve3_2.inflections();

        /*
        drawLines(window, discretizeBezierCurve(curve3_2, 100), sf::Color::White);
        for (auto t : inflections2)
        {
            sf::CircleShape c;
            c.setFillColor(sf::Color::Red);
            c.setRadius(5.0f);
            c.setOrigin(5.0f, 5.0f);
            auto xy = curve3_2.at(t);
            c.setPosition(xy.x, xy.y);
            window.draw(c);
        }
        */

        auto inflections3 = curve3_3.inflections();
        drawLines(window, discretizeBezierCurve(curve3_3, 100), sf::Color::White);
        for (auto t : inflections3)
        {
            sf::CircleShape c;
            c.setFillColor(sf::Color::Red);
            c.setRadius(5.0f);
            c.setOrigin(5.0f, 5.0f);
            auto xy = curve3_3.at(t);
            c.setPosition(xy.x, xy.y);
            window.draw(c);
        }

        {

            //drawLines(window, discretizeBezierCurve(curve, 100) sf::Color::Red);

            //std::cout << curve3.length(ls::LegendreGaussIntegrator8D{});
            auto[c1, c2] = curve3.split(0.36f);
            drawLines(window, discretizeBezierCurve(ls::BezierCurve2D<3>(curve3).left(1.0f), 100), sf::Color::Red);
            drawLines(window, discretizeBezierCurve(ls::BezierCurve2D<5>(curve3).right(0.1f), 100), sf::Color::Blue);
            //drawLines(window, discretizeBezierCurve(ls::BezierCurve2D<100>(curve3).left(1.0f), 100), sf::Color::Green); // ok if double, float gets wrong ~25
            //drawLines(window, discretizeBezierCurve(c2.elevate().elevate(), 100), sf::Color::Red);
            drawLines(window, discretizeBezierCurve(curve3.canonical(100), 100), sf::Color::Red);
            drawLines(window, discretizeBezierCurve(curve3.aligned({curve3.controlPoints[0], curve3.controlPoints[3] + ls::Vec2D(100, 200)}), 100), sf::Color::Green);

            drawLines2(window, discretizeBezierCurveEquidistant(curve3.translated({ 200, 200 }), 15), sf::Color::Red, sf::Color::Blue);
            //drawLines(window, discretizeBezierCurveEquidistant(curve3.translated({ 200, 200 }).left(curve3.lengthToParam(curve3.length(ls::LegendreGaussIntegrator8D{})/2.0, 8, ls::LegendreGaussIntegrator8D{})), 16), sf::Color::Magenta);
        }

        {

            //drawLines(window, discretizeBezierCurve(curve, 100) sf::Color::Red);

            auto[c1, c2] = curve2.split(0.36f);
            drawLines(window, discretizeBezierCurve(c1, 100), sf::Color::Blue);
            drawLines(window, discretizeBezierCurve(c2, 100), sf::Color::Green);
        }

        {

            //drawLines(window, discretizeBezierCurve(curve, 100) sf::Color::Red);

            //auto[c1, c2] = curve4.split(0.5f);
            auto c1 = curve4.left(0.5f);
            auto c2 = curve4.right(0.5f);
            drawLines(window, discretizeBezierCurve(c1, 100), sf::Color::Magenta);
            drawLines(window, discretizeBezierCurve(c2, 100), sf::Color::Cyan);

            auto c3 = curve4.subcurve(0.33f, 0.67f);
            c3.translate({ 0, 50 });
            drawLines(window, discretizeBezierCurve(c3, 100), sf::Color::Yellow);
        }

        {
            auto curve = curve3.derivative();
            curve.translate({ 300, 300 });

            //drawLines(window, discretizeBezierCurve(curve, 100) sf::Color::Red);

            auto[c1, c2] = curve.split(0.36f);
            drawLines(window, discretizeBezierCurve(c1, 100), sf::Color::Blue);
            drawLines(window, discretizeBezierCurve(c2, 100), sf::Color::Green);
        }

        {
            auto curve = curve2.elevate().derivative();
            curve.translate({ 800, 600 });

            //drawLines(window, discretizeBezierCurve(curve, 100) sf::Color::Red);

            auto[c1, c2] = curve.split(0.36f);
            drawLines(window, discretizeBezierCurve(c1, 100), sf::Color::Blue);
            drawLines(window, discretizeBezierCurve(c2, 100), sf::Color::Green);
        }

        // Update the window
        window.display();
    }
}
