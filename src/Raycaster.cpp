template <class T>
Raycast<T> Raycaster::raycast(const Ray<T>& ray, const Circle<T>& circle)
{
    double dx = ray.direction.x;
    double dy = ray.direction.y;
    double px = ray.origin.x;
    double py = ray.origin.y;
    double cx = circle.origin.x;
    double cy = circle.origin.y;
    double r = circle.radius;

    double a = dx * dx + dy * dy;
    double b = 2.0 * px * dx - 2.0 * cx * dx + 2.0 * py * dy - 2.0 * cy * dy;
    double c = -(r * r - px * px - cx * cx - py * py - cy * cy + 2.0 * py * cy + 2.0 * px * cx);
    double delta = b * b - 4.0 * a * c;
    if(delta < 0.0) Raycast<T>(ray, std::vector<typename Raycast<T>::Hit> {});

    double sqrtDelta = sqrt(delta);
    double t1 = (-b - sqrtDelta) / (2.0 * a);
    double t2 = (-b + sqrtDelta) / (2.0 * a);

    std::vector<typename Raycast<T>::Hit> hits;
    if(t1 < t2) std::swap(t1, t2);
    if(t1 >= 0.0)
    {
        Vec2D collisionPoint = ray.origin + ray.direction * t1;
        Vec2D n = ((circle.origin - collisionPoint) * -1.0).normalized();
        hits.push_back(typename Raycast<T>::Hit {t1, n, collisionPoint});
    }
    if(t2 >= 0.0)
    {
        Vec2D collisionPoint = ray.origin + ray.direction * t2;
        Vec2D n = ((circle.origin - collisionPoint) * -1.0).normalized();
        hits.push_back(typename Raycast<T>::Hit {t2, n, collisionPoint});
    }
    return Raycast<T>(ray, std::move(hits));
}
template <class T>
Raycast<T> Raycaster::raycast(const Ray<T>& ray, const Mesh<Circle<T>>& circleMesh)
{
    std::vector<typename Raycast<T>::Hit> hits;
    for(const auto& circle : circleMesh.elements)
    {
        double dx = ray.direction.x;
        double dy = ray.direction.y;
        double px = ray.origin.x;
        double py = ray.origin.y;
        double cx = circle.origin.x;
        double cy = circle.origin.y;
        double r = circle.radius;

        double a = dx * dx + dy * dy;
        double b = 2.0 * px * dx - 2.0 * cx * dx + 2.0 * py * dy - 2.0 * cy * dy;
        double c = -(r * r - px * px - cx * cx - py * py - cy * cy + 2.0 * py * cy + 2.0 * px * cx);
        double delta = b * b - 4.0 * a * c;
        if(delta < 0.0) continue;

        double sqrtDelta = sqrt(delta);
        double t1 = (-b - sqrtDelta) / (2.0 * a);
        double t2 = (-b + sqrtDelta) / (2.0 * a);

        if(t1 >= 0.0)
        {
            Vec2D collisionPoint = ray.origin + ray.direction * t1;
            Vec2D n = ((circle.origin - collisionPoint) * -1.0).normalized();
            hits.push_back(typename Raycast<T>::Hit {t1, n, collisionPoint});
        }
        if(t2 >= 0.0)
        {
            Vec2D collisionPoint = ray.origin + ray.direction * t2;
            Vec2D n = ((circle.origin - collisionPoint) * -1.0).normalized();
            hits.push_back(typename Raycast<T>::Hit {t2, n, collisionPoint});
        }
    }
    Raycast<T> cast(ray, std::move(hits));
    cast.sortHitPoints();
    return cast;
}
template <class T>
Raycast<T> Raycaster::raycast(const Ray<T>& ray, const LineSegment<T>& lineSegment)
{
    double rdx = ray.direction.x;
    double rdy = ray.direction.y;
    double spx = lineSegment.begin.x;
    double spy = lineSegment.begin.y;
    Vec2D sd = (lineSegment.end - lineSegment.begin);
    double sdx = sd.x;
    double sdy = sd.y;
    double rpx = ray.origin.x;
    double rpy = ray.origin.y;

    double t1 = (rdx * (spy - rpy) + rdy * (rpx - spx)) / (sdx * rdy - sdy * rdx);
    double t0 = (spx + sdx * t1 - rpx) / rdx;
    if(t0 < 0.0 || t1 < 0.0 || t1 > 1.0) return Raycast<T>(ray, std::vector<typename Raycast<T>::Hit> {});
    Vec2D collisionPoint = ray.origin + ray.direction * t0;
    return Raycast<T>(ray, std::vector<typename Raycast<T>::Hit> {t0, (lineSegment.end - lineSegment.begin).normalRight(), collisionPoint});
}
template <class T>
Raycast<T> Raycaster::raycast(const Ray<T>& ray, const Polygon<T>& polygon)
{
    std::vector<typename Raycast<T>::Hit> hits;
    int numberOfEdges = polygon.size();
    for(int i = 0; i < numberOfEdges; ++i)
    {
        LineSegmentD edge {polygon.vertices[i], polygon.vertices[(i + 1) % numberOfEdges]};

        //LineSegment raycast
        double rdx = ray.direction.x;
        double rdy = ray.direction.y;
        double spx = edge.begin.x;
        double spy = edge.begin.y;
        Vec2D sd = (edge.end - edge.begin);
        double sdx = sd.x;
        double sdy = sd.y;
        double rpx = ray.origin.x;
        double rpy = ray.origin.y;

        double t1 = (rdx * (spy - rpy) + rdy * (rpx - spx)) / (sdx * rdy - sdy * rdx);
        double t0 = (spx + sdx * t1 - rpx) / rdx;
        if(t0 < 0.0 || t1 < 0.0 || t1 > 1.0) continue;
        Vec2D collisionPoint = ray.origin + ray.direction * t0;
        hits.push_back(typename Raycast<T>::Hit {t0, (edge.end - edge.begin).normalRight(), collisionPoint});
    }
    Raycast<T> cast(ray, std::move(hits));
    cast.sortHitPointsByDistance();
    return cast;
}
