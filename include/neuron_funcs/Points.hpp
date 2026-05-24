#pragma once

#include <vector>

struct Points
{
    Points(std::vector<double> const &xs, std::vector<double> const &ys)
        : xs(xs), ys(ys) {}

    Points() {}

    void reserve(std::size_t size) { xs.reserve(size), ys.reserve(size); }

    void add_point(double x, double y) { xs.push_back(x), ys.push_back(y); }
    const double *get_data_xs() const { return xs.data(); }
    const double *get_data_ys() const { return ys.data(); }
    const std::size_t size() const { return xs.size(); }
    const double get_x_at(std::size_t i) const { return xs.at(i); }
    const double get_y_at(std::size_t i) const { return ys.at(i); }

private:
    std::vector<double> xs, ys;
};