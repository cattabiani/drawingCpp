#include <iostream>
#include <memory>
#include <algorithm>

class Figure
{
public:
    Figure(const int nrow, const int ncol, const int color) : nrow_(std::abs(nrow)), ncol_(std::abs(ncol)), nelem_(ncol * nrow)
    {
        // checks
        if (nrow_ > 46340) { std::cout << "Too many rows!"; return; }
        if (ncol_ > 46340) { std::cout << "Too many columns!"; return; }

        storage = new int[nelem_];
        for (int ii=0; ii < nelem_; ++ii)
        {
                storage[ii] = color;
        }
    }

    ~Figure()
    {
        // delete the heap
        delete storage;
    }

    void Print() const
    {
        // reverse order to get a nicer representation
        for (int jj = nrow_ - 1; jj >= 0; --jj)
        {			
            for (int ii = 0; ii<ncol_; ++ii)
            {
                // coordinate conversion
                int zz = XY2Z(ii, jj);
                // print the point
                std::cout << storage[zz];
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    void DrawPoint(const int xx, const int yy, const int color)
    {
        // check
        if (xx < 0 || xx >= ncol_ || yy < 0 || yy >= nrow_) { std::cout << "The point is out of the figure. Nothing happens.\n"; return; }
        // coordinate conversion
        int zz = XY2Z(xx, yy);

        // write the point
        storage[zz] = color;
    }

    void DrawRectangle(int x1, int y1, int x2, int y2, const int color)
    {
        // sorting
        if (x1 > x2) { std::swap(x1, x2); }
        if (y1 > y2) { std::swap(y1, y2); }

        // resizing if necessary
        if (x1 < 0) { x1 = 0; }
        if (y1 < 0) { y1 = 0; }
        if (x2 >= ncol_) { x2 = ncol_-1; }
        if (y2 >= nrow_) { y2 = nrow_-1; }

        for (int ii = x1; ii <= x2; ++ii)
        {
            for (int jj = y1; jj <= y2; ++jj)
            {
                // coordinate conversion
                int zz = XY2Z(ii, jj);
                // write the point
                storage[zz] = color;
            }
        }
    }

    void DrawLine(int x1, int y1,int x2,int y2, const int color)
    {
        // resolve the point case
        if (x1 == x2 && y1 == y2) { DrawPoint(x1, y1, color); return; }

        // checks
        bool noClip = true;
        if (x1 < 0 || x1 >= ncol_ || y1 < 0 || y1 >= nrow_ || x2 < 0 || x2 >= ncol_ || y2 < 0 || y2 >= nrow_) 
        { 
            std::cout << "Point 1 and/or point 2 is/are out of the figure. Clipping happens.\n"; 
            noClip = false;
            return; 
        }

        int deltaXabs = std::abs(x1 - x2);
        int deltaYabs = std::abs(y1 - y2);

        if (noClip)
        {
            if (deltaXabs >= deltaYabs)
            {
                // sort points based on x1, x2
                if (x1 > x2) { std::swap(x1, x2); std::swap(y1, y2); }

                const int deltaX = x2 - x1;
                const int deltaY = y2 - y1;

                for (int ii = x1; ii <= x2; ++ii)
                {
                    const int jj = FX2Y(x1, y1, deltaX, deltaY, ii);
                    // coordinate conversion
                    int zz = XY2Z(ii, jj);
                    // write the point
                    storage[zz] = color;
                }
            }
            else
            {
                // sort points based on y1, y2
                if (y1 > y2) { std::swap(x1, x2); std::swap(y1, y2); }

                const int deltaX = x2 - x1;
                const int deltaY = y2 - y1;

                for (int jj = y1; jj <= y2; ++jj)
                {
                    const int ii = FX2Y(y1, x1, deltaY, deltaX, jj);
                    // coordinate conversion
                    int zz = XY2Z(ii, jj);
                    // write the point
                    storage[zz] = color;
                }
            }
        }
        else
        {
            if (deltaXabs >= deltaYabs)
            {
                // sort points based on x1, x2
                if (x1 > x2) { std::swap(x1, x2); std::swap(y1, y2); }

                const int deltaX = x2 - x1;
                const int deltaY = y2 - y1;

                for (int ii = x1; ii <= x2; ++ii)
                {
                    const int jj = FX2Y(x1, y1, deltaX, deltaY, ii);
                    DrawPointNoMessages(ii, jj, color);
                }
            }
            else
            {
                // sort points based on y1, y2
                if (y1 > y2) { std::swap(x1, x2); std::swap(y1, y2); }

                const int deltaX = x2 - x1;
                const int deltaY = y2 - y1;

                for (int jj = y1; jj <= y2; ++jj)
                {
                    const int ii = FX2Y(y1, x1, deltaY, deltaX, jj);
                    DrawPointNoMessages(ii, jj, color);
                }
            }
        }
    }

private:

    inline int XY2Z(const int X, const int Y) const { return ncol_ * Y + X; }

    inline void Z2XY(const int Z, int& X, int& Y) const { X = Z % ncol_; Y = Z / ncol_; }

    // determine the jj index of a discrete line y = f(x). x1 < x2 is a requirement
    inline int FX2Y(const int x1, const int y1, const int deltaX, const int deltaY, const int ii) { return (deltaY * (ii - x1 + (deltaX+1)/(deltaY+1) -1) + deltaX * y1) / deltaX; }

    inline void DrawPointNoMessages(const int xx, const int yy, const int color)
    {
        // check
        if (xx >= 0 && xx < ncol_ && yy >= 0 && yy < nrow_) 
        { 
            // coordinate conversion
            int zz = XY2Z(xx, yy); 
            // write the point
            storage[zz] = color;
        }
    }

    // --- variables --- //
    int* storage;
    const int nrow_;
    const int ncol_;
    const int nelem_;
};

int main()
{
    auto figp = std::make_unique<Figure>(10, 10, 0);

    figp->DrawLine(9, 9, 0, 0, 1);
    figp->Print();

    std::cin.get();
    return 0;
}