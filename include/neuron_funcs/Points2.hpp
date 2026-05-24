#pragma once

#include <vector>
#include "../Point2x2.hpp"
struct Points2
{
    Points2(std::vector<double> const &x1s, std::vector<double> const &y1s,
            std::vector<double> const &x2s, std::vector<double> const &y2s)
        : x1s(x1s), y1s(y1s), x2s(x2s), y2s(y2s) {}

    Points2() {}

    void reserve(std::size_t size) { x1s.reserve(size), y1s.reserve(size), x2s.reserve(size), y2s.reserve(size); }
    void add_point(double x1, double y1, double x2, double y2) { x1s.push_back(x1), y1s.push_back(y1), x2s.push_back(x2), y2s.push_back(y2); }
    void add_point(Point2x2 const &p) { add_point(p.get_x1(), p.get_y1(), p.get_x2(), p.get_y2()); }
    double const *get_data_x1s() const { return x1s.data(); }
    double const *get_data_y1s() const { return y1s.data(); }
    double const *get_data_x2s() const { return x2s.data(); }
    double const *get_data_y2s() const { return y2s.data(); }
    const std::size_t size() const { return x1s.size(); }
    double const get_x1_at(std::size_t i) const { return x1s.at(i); }
    double const get_y1_at(std::size_t i) const { return y1s.at(i); }
    double const get_x2_at(std::size_t i) const { return x2s.at(i); }
    double const get_y2_at(std::size_t i) const { return y2s.at(i); }

private:
    std::vector<double> x1s, y1s, x2s, y2s;
};
