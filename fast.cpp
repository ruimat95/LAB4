/*
 * fast < input.txt
 *
 * Compute and plot all line segments involving 4 points in input.txt
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include "point.h"
#include "window.h"

using namespace std;

int main(int argc, const char *argv[])
{
    WindowPtr window = create_window(argc, argv);
    if (!window)
        return 1;

    // The array of points
    vector<Point> points;

    // Read tolerance from cin
    double tolerance{};
    cin >> tolerance;

    // Read points from cin
    int N{};
    cin >> N;

    for (int i{0}; i < N; ++i)
    {
        double x{}, y{};
        cin >> x >> y;
        points.push_back(Point{x, y});
    }

    // draw points to screen all at once
    window->draw_points(points);

    auto begin = chrono::high_resolution_clock::now();

    for (size_t i = 0; i < N; ++i)
    {
        Point p = points[i];
        vector<Point> others;
        for (size_t j = 0; j < N; ++j)
        {
            if (j != i)
            {
                others.push_back(points[j]);
            }
        }
            std::sort(others.begin(), others.end(), PolarSorter{p});
            for (size_t h = 0; h < others.size(); ++h)
            {
                vector<Point> line{p};
                line.push_back(others[h]);
                for (size_t k = h + 1; k < others.size(); ++k)
                {
                    if (p.sameSlope(others[h], others[k], tolerance))
                    {
                        line.push_back(others[k]);
                    }
                    else
                    {
                        break;
                    }
                }
                if (line.size() >= 4)
                {
                    std::sort(line.begin(), line.end());
                    window->draw_line(line.front(), line.back());
                }
            }
        
    };

    /////////////////////////////////////////////////////////////////////////////
    // Draw any lines that you find in 'points' using the function 'window->draw_line'.
    /////////////////////////////////////////////////////////////////////////////

    auto end = chrono::high_resolution_clock::now();
    cout << "Computing line segments took "
         << std::chrono::duration_cast<chrono::milliseconds>(end - begin).count()
         << " milliseconds." << endl;

    // wait for user to terminate program
    window->run();

    return 0;
}
