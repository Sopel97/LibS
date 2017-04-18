#include "Geometry.h"
#include "Noise.h"
#include "DynamicPtr.h"

namespace ls
{
    template class AffineTransformation2<double>;
    template class AffineTransformation2<float>;

    template class Angle2<double>;
    template class Angle2<float>;

    template class BezierCurve2<double>;
    template class BezierCurve2<float>;

    template class BezierPath2<double>;
    template class BezierPath2<float>;

    template class CellNoise<double>;
    template class CellNoise<float>;

    template class Circle2<double>;
    template class Circle2<float>;

    template class Cuboid3<double>;
    template class Cuboid3<float>;

    template class Cylinder3<double>;
    template class Cylinder3<float>;

    //template class DelaunayTriangulation2<double>;
    //template class DelaunayTriangulation2<float>;

    template class LineSegment2<double>;
    template class LineSegment2<float>;
    template class ConstLineSegment2View<double>;
    template class ConstLineSegment2View<float>;

    template class ContinuousCollision2<double>;
    template class ContinuousCollision2<float>;

    template class Matrix<double, 2u, 2u>;
    template class Matrix<float, 2u, 2u>;
    template class Matrix<int, 2u, 2u>;

    template class Matrix<double, 2u, 3u>;
    template class Matrix<float, 2u, 3u>;
    template class Matrix<int, 2u, 3u>;

    template class Matrix<double, 2u, 4u>;
    template class Matrix<float, 2u, 4u>;
    template class Matrix<int, 2u, 4u>;

    template class Matrix<double, 3u, 2u>;
    template class Matrix<float, 3u, 2u>;
    template class Matrix<int, 3u, 2u>;

    template class Matrix<double, 3u, 3u>;
    template class Matrix<float, 3u, 3u>;
    template class Matrix<int, 3u, 3u>;

    template class Matrix<double, 3u, 4u>;
    template class Matrix<float, 3u, 4u>;
    template class Matrix<int, 3u, 4u>;

    template class Matrix<double, 4u, 2u>;
    template class Matrix<float, 4u, 2u>;
    template class Matrix<int, 4u, 2u>;

    template class Matrix<double, 4u, 3u>;
    template class Matrix<float, 4u, 3u>;
    template class Matrix<int, 4u, 3u>;

    template class Matrix<double, 4u, 4u>;
    template class Matrix<float, 4u, 4u>;
    template class Matrix<int, 4u, 4u>;

    template class EuclideanGraphNode<double, 2>;
    template class EuclideanGraphNode<float, 2>;

    template class WeightedGraphEdge<double>;
    template class WeightedGraphEdge<float>;

    template class Graph<double, NeighbourSetTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;
    template class Graph<float, NeighbourSetTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;

    template class Graph<double, AdjacencyMatrixTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;
    template class Graph<float, AdjacencyMatrixTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;

    template class Graph<double, EdgeSetTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;
    template class Graph<float, EdgeSetTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;

    template class Graph<double, NeighbourEdgeSetWithEdgeListTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;
    template class Graph<float, NeighbourEdgeSetWithEdgeListTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;

    template class Graph<double, NeighbourSetTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;
    template class Graph<float, NeighbourSetTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;

    template class Graph<double, AdjacencyMatrixTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;
    template class Graph<float, AdjacencyMatrixTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;

    template class Graph<double, EdgeSetTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;
    template class Graph<float, EdgeSetTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;

    template class Graph<double, NeighbourEdgeSetWithEdgeListTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;
    template class Graph<float, NeighbourEdgeSetWithEdgeListTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;

    template class Polygon2<double>;
    template class Polygon2<float>;
    template class ConstPolygon2View<double>;
    template class ConstPolygon2View<float>;

    template class Polyline2<double>;
    template class Polyline2<float>;
    template class ConstPolyline2View<double>;
    template class ConstPolyline2View<float>;

    template class Ray2<double>;
    template class Ray2<float>;

    template class Raycast2<double>;
    template class Raycast2<float>;

    template class Rectangle2<double>;
    template class Rectangle2<float>;
    template class Rectangle2<int>;

    template class Shape<double>;
    template class Shape<float>;
    template class Shape<int>;

    template class Shape2<double>;
    template class Shape2<float>;
    template class Shape2<int>;

    template class Shape3<double>;
    template class Shape3<float>;
    template class Shape3<int>;

    template class Shape4<double>;
    template class Shape4<float>;
    template class Shape4<int>;

    template class SimplexNoise<double, 1>;
    template class SimplexNoise<float, 1>;

    template class SimplexNoise<double, 2>;
    template class SimplexNoise<float, 2>;

    template class SimplexNoise<double, 3>;
    template class SimplexNoise<float, 3>;

    template class SimplexNoise<double, 4>;
    template class SimplexNoise<float, 4>;

    template class PerlinNoise<double, 1>;
    template class PerlinNoise<float, 1>;

    template class PerlinNoise<double, 2>;
    template class PerlinNoise<float, 2>;

    template class PerlinNoise<double, 3>;
    template class PerlinNoise<float, 3>;

    template class PerlinNoise<double, 4>;
    template class PerlinNoise<float, 4>;

    template class Transformation2<double>;
    template class Transformation2<float>;

    template class Triangle2<double>;
    template class Triangle2<float>;
    template class ConstTriangle2View<double>;
    template class ConstTriangle2View<float>;

    template class Index2Template<int>;
    template class Index3Template<int>;
    template class IndexNTemplate<int>;

    template class Vec2<double>;
    template class Vec2<float>;
    template class Vec2<int>;

    template class Vec3<double>;
    template class Vec3<float>;
    template class Vec3<int>;

    template class Vec4<double>;
    template class Vec4<float>;
    template class Vec4<int>;


	template class PointMass2<double>;
	template class PointMass2<float>;
}
