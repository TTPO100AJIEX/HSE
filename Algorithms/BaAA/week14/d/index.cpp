#include <ios>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <utility>
#include <vector>
#include <algorithm>

struct Point
{
public:
    double x;
    double y;
    Point() { }
    Point(double x, double y) : x(x), y(y) { }

    double distance(const Point& other) { return (this->x - other.x) * (this->x - other.x) + (this->y - other.y) * (this->y - other.y); }

    friend std::istream& operator>>(std::istream& is, Point& point) { return is >> point.x >> point.y; }
};

struct Circle
{
private:
    struct Center
    {
    public:
        double x;
        double y;

        double distance(const Center& other) { return (this->x - other.x) * (this->x - other.x) + (this->y - other.y) * (this->y - other.y); }
    
        friend std::istream& operator>>(std::istream& is, Center& point)
        {
            int x, y;
            is >> x >> y;
            point.x = x;
            point.y = y;
            return is;
        }
    };
    
    double SquaredCenterDistance(const Circle& other) { return this->center.distance(other.center); }

public:
    struct Center center;
    double radius;

    std::vector<Point> intersection(const Circle& other)
    {
        double centerDistanceSquared = this->SquaredCenterDistance(other);
        std::vector<Point> answer;

        double radiusSum = this->radius + other.radius, radiusDiff = this->radius - other.radius;
        if (centerDistanceSquared > radiusSum * radiusSum || centerDistanceSquared < radiusDiff * radiusDiff) return answer;

        double a = (this->radius * this->radius - other.radius * other.radius + centerDistanceSquared) / (2 * std::sqrt(centerDistanceSquared));
        double h = std::sqrt(this->radius * this->radius - a * a);
        double xt = this->center.x + a * (other.center.x - this->center.x) / std::sqrt(centerDistanceSquared),
                yt = this->center.y + a * (other.center.y - this->center.y) / std::sqrt(centerDistanceSquared);

        if (h < 1e-8)
        {
            answer.emplace_back(xt, yt);
            return answer;
        }

        double x3 = xt + h * (other.center.y - this->center.y) / std::sqrt(centerDistanceSquared),
                y3 = yt - h * (other.center.x - this->center.x) / std::sqrt(centerDistanceSquared);
                
        double x4 = xt - h * (other.center.y - this->center.y) / std::sqrt(centerDistanceSquared),
                y4 = yt + h * (other.center.x - this->center.x) / std::sqrt(centerDistanceSquared);
                
        answer.emplace_back(x3, y3);
        answer.emplace_back(x4, y4);
        return answer;
    }

    double isOnCircle(Point& point)
    {
        double dist = std::sqrt((point.x - this->center.x) * (point.x - this->center.x) + (point.y - this->center.y) * (point.y - this->center.y));
        return std::abs(dist - this->radius) <= 1e-8;
    }

    double distanceBetween(Point& first, Point& second)
    {
        if (!this->isOnCircle(first) || !this->isOnCircle(second)) return 1e18;
        double scal = (first.x - this->center.x) * (second.x - this->center.x) + (first.y - this->center.y) * (second.y - this->center.y);
        return std::acos(scal / (this->radius * this->radius)) * this->radius;
    }
    
    friend std::istream& operator>>(std::istream& is, Circle& circle)
    {
        is >> circle.center;
        int radius;
        is >> radius;
        circle.radius = radius;
        return is;
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    Circle circle1, circle2;
    std::cin >> circle1 >> circle2;

    std::vector<Point> points = circle1.intersection(circle2);
    points.resize(points.size() + 2);
    std::cin >> points[points.size() - 2] >> points[points.size() - 1];

    std::vector< std::vector<double> > distances(points.size(), std::vector<double>(points.size(), 1e18));
    for (uint8_t i = 0; i < points.size(); ++i)
    {
        for (uint8_t j = 0; j < points.size(); ++j)
        {
            if (i == j) { distances[i][j] = 0; continue; }
            distances[i][j] = std::min(distances[i][j], circle1.distanceBetween(points[i], points[j]));
            distances[i][j] = std::min(distances[i][j], circle2.distanceBetween(points[i], points[j]));
        }
    }
    
    for (uint8_t k = 0; k < points.size(); ++k)
    {
        for (uint8_t i = 0; i < points.size(); ++i)
        {
            for (uint8_t j = 0; j < points.size(); ++j)
            {
                distances[i][j] = std::min(distances[i][j], distances[i][k] + distances[k][j]);
            }
        }
    }
    
    if (distances[points.size() - 2][points.size() - 1] > 1e7) distances[points.size() - 2][points.size() - 1] = -1;
    std::cout << std::fixed << std::setprecision(5) << distances[points.size() - 2][points.size() - 1];
}