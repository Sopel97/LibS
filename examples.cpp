#include "LibS.h"

#include <iostream>
#include <memory>

template struct ls::Array2<int>;
template struct ls::Array3<int>;
template struct ls::Array3<int, 4, 4, 4>;

template struct ls::Vec2<float>;
template struct ls::Edge2<float>;
template struct ls::Edge3<float>;
template struct ls::Box2<float>;
template struct ls::Box3<float>;
template struct ls::Cylinder3<float>;
template struct ls::Capsule3<float>;
template struct ls::Circle2<float>;
template struct ls::Sphere3<float>;
template struct ls::ConvexPolygon2<float>;
template struct ls::Polyline2<float>;
template struct ls::Triangle2<float>;
template struct ls::Triangle3<float>;
template struct ls::Ray2<float>;
template struct ls::Ray3<float>;
template struct ls::Vec3<float>;
template struct ls::Vec4<float>;
template struct ls::Angle2<float>;
template struct ls::Plane3<float>;
template struct ls::Frustum3<float>;

template struct ls::EulerAngles<float>;
template struct ls::AxisAngle<float>;

template struct ls::CellularAutomaton2<ls::ConwaysGameOfLifeRule, ls::CellularAutomatonTopology::Finite>;

template struct ls::Matrix<float, 2, 2>;
template struct ls::Matrix<float, 2, 4>;
template struct ls::Matrix<float, 3, 3>;
template struct ls::Matrix<float, 4, 4>;
template struct ls::Quat<float>;
template struct ls::Transform3<float>;

template struct ls::NoiseSampler<float, 1>;
template struct ls::NoiseSampler<float, 2>;
template struct ls::NoiseSampler<float, 3>;
template struct ls::NoiseSampler<float, 4>;

template struct ls::PerlinNoise<float, ls::OriginalPerlinPermTable>;
template struct ls::SimplexNoise<float, ls::OriginalPerlinPermTable>;

template <class T, int R, int C, ls::MatrixLayout LayoutV>
void printRoundAlmostZero(const ls::Matrix<T, R, C, LayoutV>& m)
{
    static constexpr T eps = T(0.00001);

    for (int r = 0; r < R; ++r)
    {
        for (int c = 0; c < C; ++c)
        {
            if (std::abs(m[r][c]) < eps)
            {
                std::cout << "0" << ' ';
            }
            else
            {
                std::cout << m[r][c] << ' ';
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

void mainExamples()
{
    ls::Angle2F angle;
    angle = ls::Angle2F::fullTurn();
    angle.normalize();

    ls::Vec2F vec(0.0f, 0.0f);
    vec.normal();

    std::cout << ls::intersect(ls::Vec2F{}, ls::Circle2F{});

    std::cout << alignof(ls::Vec4F) << alignof(ls::Vec4D) << '\n';

    auto box = ls::Box3F::withSize({ 0.0f, 0.0f, 0.0f }, { 1.0f, 2.0f, 3.0f });
    std::cout << box.volume() << '\n';

    {
        ls::PerlinNoiseF noise;
        ls::NoiseSampler2F sampler;

        std::cout << sampler.sample(ls::Vec2<float>(124.0f, 321.0f), noise) << '\n';
    }

    {
        ls::SimplexNoiseF noise;
        ls::NoiseSampler2F sampler;

        std::cout << sampler.sample(ls::Vec2<float>(124.0f, 321.0f), noise) << '\n';
    }

    std::cout << ls::Triangle3F({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }).area() << '\n';

    ls::Vec2F a(32, 3241);
    float b = 321.f;
    ls::Vec2I aa = ls::floorToInt(a);
    int bb = ls::floorToInt(b);

    try
    {
        ls::json::Document doc2 = ls::json::Document::fromString("      [{\"asd\"    : null, \"dsa\" : 123, \"arr\":[321.3124123121, 321, 3]   }  ,  321, false, {}, [123, 321, 555, 4],[\"str1\",\"str2\",\"str3\"]]");
        std::cout << doc2.stringify() << '\n';
        ls::json::Document doc3 = ls::json::Document::fromString(ls::json::Writer(doc2, ls::json::WriterParams::compact()).result());
        doc3 = doc2;

        std::cout << "\n\n\n\n\n";
        std::cout << doc2[0]["asd"].exists() << " =? " << doc3[0]["asd"].exists() << '\n';
        std::cout << doc2[0]["dsa"].getDouble() << " =? " << doc3[0]["dsa"].getDouble() << '\n';
        std::cout << doc2[1].getInt() << " =? " << doc3[1].getInt() << '\n';
        std::cout << doc2[2].getBool() << " =? " << doc3[2].getBool() << '\n';
        std::cout << doc2[4][3].getInt() << " =? " << doc3[4][3].getInt() << '\n';
        std::cout << doc2[0]["arr"][2].getInt() << " =? " << doc3[0]["arr"][2].getInt() << '\n';
        std::cout << doc2[0]["arrr"].exists() << " =? " << doc3[0]["arrr"].exists() << '\n';

        for (const auto& str : ls::json::fromJson<std::vector<std::string>>(doc2[5]))
        {
            std::cout << str << '\n';
        }
        for (const auto& i : ls::json::fromJson<std::vector<unsigned short>>(doc2[4]))
        {
            std::cout << i << '\n';
        }
        for (const auto& f : ls::json::fromJson<std::vector<long double>>(doc2[4]))
        {
            std::cout << f << '\n';
        }

        std::cout << ls::json::fromJson<std::string>(doc2[0]["arrrr"], "default") << '\n';

        doc3[0]["arr"].setInt(12312);
        std::cout << doc3[0]["arr"].getInt() << '\n';
        std::cout << ls::json::fromJson<int>(doc3[0]["arr"]) << '\n';

        doc2[0]["arr"].addValue(ls::json::Value(321ll));
        std::cout << doc2[0]["arr"][3].getInt() << '\n';

        doc2[0].addMember("new!", ls::json::Value("NEW VALUE"));
        std::cout << doc2[0]["new!"].getString() << '\n';
        std::cout << ls::json::fromJson<std::string>(doc2[0]["new!"], "default") << '\n';

        doc2[0].addMember("new object!", ls::json::Value::Object{});
        auto& v = doc2[0].addMember("new array!", ls::json::Value::Array{});
        auto& v2 = v.addValue(ls::json::Value(2222222ll));
        std::cout << v[0].getInt() << ' ' << v2.getInt() << '\n';

        ls::json::Document docc = ls::json::Document::emptyObject();
        docc.addMember("FIRST", ls::json::Value(true));
        std::cout << docc["FIRST"].getBool() << '\n';

        ls::json::Document doccc = ls::json::Document::singleValue(ls::json::Value("IM SINGLE"));
        std::cout << doccc.getString() << '\n';

    }
    catch (const std::runtime_error& err)
    {
        std::cout << err.what();
    }

    {
        using T = float;

        ls::Vec2<T> v0(0.0f, 0.0f);
        std::cout << v0[1] << '\n' << '\n';

        auto transform1 = ls::Transform3<T>::withTranslation(ls::Vec3<T>(1.0f, 2.0f, 3.0f));
        auto transform2 = ls::Transform3<T>::withScale(2.0f);
        auto transform3 = ls::Transform3<T>::withRotation(ls::Quat<T>(ls::AxisAngle<T>({ 3.0f, 2.0f, 1.41f }, ls::Angle2<T>::degrees(35.0f))));
        auto transform4 = ls::Transform3<T>::withRotation(ls::Quat<T>(ls::AxisAngle<T>({ 1.0f, 0.0f, 0.0f }, ls::Angle2<T>::degrees(90.0f))), ls::Vec3<T>(0.0f, 0.0f, 1.0f));
        auto t = transform1 * transform3 * transform2;
        //auto t = transform1;
        //auto t = transform2;
        //auto t = transform3;
        auto invt = t.inverse();
        auto id = t * invt;

        auto m0 = ls::Matrix<T, 3, 3>::rotation(transform3.rotation);
        printRoundAlmostZero(m0);
        printRoundAlmostZero(m0.inverse());

        auto m1 = ls::Matrix<T, 4, 4>::lookAt({ 1.0f, 2.0f, 3.0f }, { 3.0f, 2.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
        auto m1inv = m1.inverse();
        printRoundAlmostZero(m1);
        printRoundAlmostZero(m1inv*7.0f);

        printRoundAlmostZero(ls::Matrix<T, 4, 4>::rotation(ls::Quat<T>(ls::AxisAngle<T>({ 1.0f, 0.0f, 0.0f }, ls::Angle2<T>::degrees(90.0f))), ls::Vec3<T>(0.0f, 0.0f, 1.0f)) - ls::Matrix<T, 4, 4>::transform(transform4));

        auto v = (transform4 * transform1 * transform2).apply({ 2.0f, 3.0f, 5.0f });
        std::cout << v.x << ' ' << v.y << ' ' << v.z << "\n\n";

        auto mat1 = ls::Matrix<T, 4, 4>::transform(t);
        auto mat2 = ls::Matrix<T, 4, 4>::transform(invt);
        auto mat22 = mat1.inverse();
        auto mat3 = ls::Matrix<T, 4, 4>::transform(id);
        auto mat4 = mat1 * mat2;
        auto mat5 = mat1 * mat22;
        auto mat6 = mat1.transposed();

        printRoundAlmostZero(mat1);
        printRoundAlmostZero(mat2);
        printRoundAlmostZero(mat22);
        printRoundAlmostZero(mat3);
        printRoundAlmostZero(mat4);
        printRoundAlmostZero(mat5);
        printRoundAlmostZero(mat6);

        {
            float aspect = 1920.0f / 1080.0f;
            ls::Matrix4x4F projection = ls::Matrix4x4F::perspective(ls::Angle2F::degrees(45.0f), aspect, 0.1f, 100.0f);
            ls::Vec3F pos{ 0, 0, 0 };
            ls::Matrix4x4F view = ls::Matrix4x4F::lookAt(pos, pos + ls::Vec3F{ 0, 0, -1 }, { 0, 1, 0 });
            ls::Matrix4x4F model = ls::Matrix4x4F::translation({ 30, 20, -30 });

            auto mvp = projection * view *model;
            auto frustum = ls::Frustum3F::fromMatrix(mvp);

            for (auto& p : frustum.planes)
            {
                p.normalize();
                float dist = (p.a*pos.x + p.b*pos.y + p.c*pos.z + p.d);
                std::cout << p.a << ' ' << p.b << ' ' << p.c << ' ' << p.d << ' ' << "dist from pos: " << dist << '\n';
            }
        }
    }

    {
        constexpr ls::AutoArray3<int, 2, 3, 4> c;
        std::cout << "asdasdsad: " << c.at(1, 2, 3) << '\n';
        ls::AutoArray3<int, 2, 3, 4> b;
        auto a = b;
        a = b;
        int i = 0;
        for (auto& e : a) e = i++;
        for (int z = 0; z < 4; ++z)
        {
            for (int y = 0; y < 3; ++y)
            {
                for (int x = 0; x < 2; ++x)
                {
                    std::cout << a(x, y, z) << ' ';
                }
                std::cout << '\n';
            }
            std::cout << '\n';
            std::cout << '\n';
        }
        std::cout << '\n';
        std::cout << '\n';

        for (auto&&[x, y, z, e] : a.cenumerate())
        {
            std::cout << x << ' ' << y << ' ' << z << ' ' << e << '\n';
        }
        std::cout << '\n';
        std::cout << '\n';
    }

    {
        ls::Array3<int, 2, 3, 4> b;
        auto a = b;
        a = b;
        int i = 0;
        for (auto& e : a) e = i++;
        for (int z = 0; z < 4; ++z)
        {
            for (int y = 0; y < 3; ++y)
            {
                for (int x = 0; x < 2; ++x)
                {
                    std::cout << a(x, y, z) << ' ';
                }
                std::cout << '\n';
            }
            std::cout << '\n';
            std::cout << '\n';
        }
        std::cout << '\n';
        std::cout << '\n';

        for (auto&&[x, y, z, e] : a.cenumerate())
        {
            std::cout << x << ' ' << y << ' ' << z << ' ' << e << '\n';
        }

        std::cout << '\n';
        std::cout << '\n';
    }

    {
        ls::Array3<int> b(2, 3, 4);
        auto a = b;
        a = b;
        int i = 0;
        for (auto& e : a) e = i++;
        for (int z = 0; z < 4; ++z)
        {
            for (int y = 0; y < 3; ++y)
            {
                for (int x = 0; x < 2; ++x)
                {
                    std::cout << a(x, y, z) << ' ';
                }
                std::cout << '\n';
            }
            std::cout << '\n';
            std::cout << '\n';
        }
        std::cout << '\n';
        std::cout << '\n';

        for (auto&&[x, y, z, e] : a.cenumerate())
        {
            std::cout << x << ' ' << y << ' ' << z << ' ' << e << '\n';
        }
        std::cout << '\n';
        std::cout << '\n';
    }



    {
        constexpr ls::AutoArray2<int, 2, 3> c;
        std::cout << "asdasdsad: " << c.at(1, 2) << '\n';
        ls::AutoArray2<int, 2, 3> b;
        auto a = b;
        a = b;
        int i = 0;
        for (auto& e : a) e = i++;
        for (int y = 0; y < 3; ++y)
        {
            for (int x = 0; x < 2; ++x)
            {
                std::cout << a(x, y) << ' ';
            }
            std::cout << '\n';
        }
        std::cout << '\n';
        std::cout << '\n';

        for (auto&&[x, y, e] : a.cenumerate())
        {
            std::cout << x << ' ' << y << ' ' << e << '\n';
        }
        std::cout << '\n';
        std::cout << '\n';
    }

    {
        ls::Array2<int, 2, 3> b;
        auto a = b;
        a = b;
        int i = 0;
        for (auto& e : a) e = i++;
        for (int y = 0; y < 3; ++y)
        {
            for (int x = 0; x < 2; ++x)
            {
                std::cout << a(x, y) << ' ';
            }
            std::cout << '\n';
        }
        std::cout << '\n';
        std::cout << '\n';

        for (auto&&[x, y, e] : a.cenumerate())
        {
            std::cout << x << ' ' << y << ' ' << e << '\n';
        }
        std::cout << '\n';
        std::cout << '\n';
    }

    {
        const ls::Array2<int> b(2, 3);
        auto a = b;
        int i = 0;
        for (auto& e : a) e = i++;
        for (int y = 0; y < 3; ++y)
        {
            for (int x = 0; x < 2; ++x)
            {
                std::cout << a(x, y) << ' ';
            }
            std::cout << '\n';
        }
        std::cout << '\n';
        std::cout << '\n';

        for (auto&&[x, y, e] : a.cenumerate())
        {
            std::cout << x << ' ' << y << ' ' << e << '\n';
        }
        std::cout << '\n';
        std::cout << '\n';
    }

    {
        std::cout << sizeof(ls::AutoArray3<int, 32, 32, 32>) << '\n';
        std::cout << sizeof(ls::AutoArray2<int, 32, 32>) << '\n';
    }

    {
        std::mt19937 rng(1233);
        ls::Circle2D c(ls::Vec2D(1, 1), 2);
        ls::UniformVec2Distribution1<ls::Circle2D> d1(c);
        ls::UniformVec2Distribution2<ls::Circle2D> d2(c);
        ls::UniformVec2Distribution1<ls::Edge2D> d3(ls::Edge2D({ 0, 0 }, { 1, 2 }));
        ls::UniformVec2Distribution2<ls::Box2D> d4(ls::Box2D({ 0, 0 }, { 1, 1 }));
        ls::UniformVec2Distribution1<ls::Triangle2D> d5(ls::Triangle2D({ 0, 0 }, { 1, 1 }, { 2, 0 }));
        ls::UniformVec2Distribution2<ls::Triangle2D> d6(ls::Triangle2D({ 0, 0 }, { 1, 1 }, { 2, 0 }));
        const auto v1 = d1(rng);
        const auto v2 = d2(rng);
        const auto v4 = d4(rng);
        const auto v3 = d3(rng);
        const auto v5 = d5(rng);
        const auto v6 = d6(rng);
        std::cout << v1.x << ' ' << v1.y << '\n';
        std::cout << v2.x << ' ' << v2.y << '\n';
        std::cout << v3.x << ' ' << v3.y << '\n';
        std::cout << v4.x << ' ' << v4.y << '\n';
        std::cout << v5.x << ' ' << v5.y << '\n';
        std::cout << v6.x << ' ' << v6.y << '\n';

    }

    {
        auto ca = ls::ToroidalCellularAutomaton2<ls::ConwaysGameOfLifeRule>(100, 100);
        ca.fill([](auto x, auto y) {return ls::ConwaysGameOfLifeRule::StateType::Live; });
        ca.iterate(1);

        std::cout << static_cast<int>(ca(99, 99)) << '\n';
    }

    {
        std::ranlux48 rng;
        const std::vector<std::string> data{ "abc", "fdg", "hfd", "asdga", "qweqrtqw" };
        ls::TrainedSequenceExtender<char> dict(data.begin(), data.end(), 3);
        std::cout << dict.generate<std::string>(10, rng) << '\n';
    }

    {
        ls::WangTileSet2 set;
        for (int north = 0; north <= 1; ++north)
        {
            for (int south = 0; south <= 1; ++south)
            {
                for (int east = 0; east <= 1; ++east)
                {
                    for (int west = 0; west <= 1; ++west)
                    {
                        set.emplaceTile(north, south, east, west);
                    }
                }
            }
        }

        std::mt19937 rng;

        std::cout << std::hex << set.randomFittingTile(
            ls::WangTile2(0, ls::WangTile2::wildcard(), 0, 0),
            ls::WangTile2(1, 0, 0, 0),
            ls::WangTile2(0, 0, 0, 1),
            ls::WangTile2(0, 0, 1, 0),
            rng
        )->id() << '\n';

        ls::WangTileSet3 set3;
        for (int north = 0; north <= 1; ++north)
        {
            for (int south = 0; south <= 1; ++south)
            {
                for (int east = 0; east <= 1; ++east)
                {
                    for (int west = 0; west <= 1; ++west)
                    {
                        for (int top = 0; top <= 1; ++top)
                        {
                            for (int bottom = 0; bottom <= 1; ++bottom)
                            {
                                set3.emplaceTile(north, south, east, west, top, bottom);
                            }
                        }
                    }
                }
            }
        }
        std::cout << std::bitset<32>(set3.randomFittingTile(
            ls::WangTile3(0, 1, 0, 0, 0, 0),
            ls::WangTile3(1, 0, 0, 0, 0, 0),
            ls::WangTile3(0, 0, 0, 1, 0, 0),
            ls::WangTile3(0, 0, 1, 0, 0, 0),
            ls::WangTile3(0, 0, 0, 0, 0, 1),
            ls::WangTile3(0, 0, 0, 0, 1, 0),
            rng
        )->id()) << '\n';

        struct ConstantKFunc
        {
            int operator()(int) const
            {
                return 10;
            }
        };

        ls::ApproximatePoissonDiscSampler1<double, std::uniform_real_distribution<double>> s(std::uniform_real_distribution<double>(0, 10), 20, rng);

        std::vector<double> samples = s.samples();
        std::sort(samples.begin(), samples.end());

        int i = 0;
        for (auto&& sample : samples)
        {
            std::cout << std::dec << i++ << '\t' << sample << '\n';
        }
    }

    {
        const auto f = [](float f) {std::cout << "float!\n"; };
        const auto c = f;
        ls::overload(
            [](double a) {std::cout << a << '\n'; },
            [](int b) {std::cout << b << '\n'; },
            std::move(c)
        )(123.3f);
    }

    {
        ls::Circle2D c({ 100.0, 200.0 }, 50.0);
        ls::Circle2D c2({ 400.0, 200.0 }, 50.0);
        std::cout << ls::closestPoints(c, c2).distance() << '\n';
        std::cout << ls::distance(c, c2) << '\n';
        auto b = ls::bounding<ls::Box2>(c);
        std::cout << b.min.x << ' ' << b.min.y << ' ' << b.max.x << ' ' << b.max.y << '\n';
        
        auto cp = ls::closestPoints(c, c);
        std::cout << cp.distance() << '\n';
    }
}

