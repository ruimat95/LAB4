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

    // likt brute.cpp itererar vi genom alla möjliga ursprungspunkter,
    // men istället för att iterera genom alla kombinationer av andra punkter,
    // sorterar vi dem efter lutning, sedan letar vi efter sekvenser av punkter
    // med samma lutning och ritar linjer för de som har minst 4 punkter.
    for (int i = 0; i < N; ++i)
    {
        // ursprungspunkten
        Point p = points[i];
        // vektor för att lagra de andra punkterna
        vector<Point> others;
        for (int j = 0; j < N; ++j)
        {
            if (j != i)
            {
                others.push_back(points[j]);
            }
        }
        // sorterar punkterna efter lutning med hjälp av PolarSorter
        std::sort(others.begin(), others.end(), PolarSorter{p});

        size_t h = 0;
        // itererar genom de sorterade punkterna och letar efter sekvenser av punkter med samma lutning
        while (h < others.size())
        {
            // vektor för att lagra punkterna i linjen
            vector<Point> line{p};
            // lägger till den första punkten i sekvensen
            line.push_back(others[h]);
            size_t k = h + 1;
            // fortsätter att lägga till punkter i sekvensen så länge de har samma lutning
            while (h < others.size() && p.sameSlope(others[h], others[k], tolerance))
            {
                line.push_back(others[k]);
                ++k;
            }
            // om sekvensen har minst 4 punkter, ritar vi linjen
            if (line.size() >= 4)
            {
                // sorterar punkterna i linjen för att rita linjen från den minsta till den största punkten
                std::sort(line.begin(), line.end());
                // vi ritar linjen bara om den första punkten är mindre än ursprungspunkten, för att inte rita samma linje
                // flera gånger med olika ursprungspunkter
                if (!(line.front() < p))
                {
                    window->draw_line(line.front(), line.back());
                }
            }
            // uppdaterar h för att hoppa över de punkter som vi redan gått igenom
            h = k;
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
