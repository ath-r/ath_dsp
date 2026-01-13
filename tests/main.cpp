
#include <array>
#include <iostream>
#include <chrono>
#include <ranges>
#include <vector>

#include <matplot/matplot.h>

#include "matplot/core/legend.h"
#include "matplot/freestanding/axes_functions.h"

#include "../math/Math.h"
#include "../math/Special.h"

static volatile double sink = 0.0; // prevents optimization

auto plot = [](auto& x, auto& y, const char* name)
{
    auto p = matplot::plot(x, y);
    p->display_name(name);
    p->line_width(2.0);
};

int main(int argc, char *argv[])
{

    // Plot pade approximations of sin(x)
    {
        auto x1 = matplot::linspace(-Ath::Math::fpi, Ath::Math::fpi, 600);
        std::vector<double> y1;
        for (auto& x : x1) y1.push_back(Ath::Math::sin(x));

        std::vector<double> y1_error;
        for (auto [x, y] : std::views::zip(x1, y1)) y1_error.push_back((y - std::sin(x)) * 1e4);

        std::vector<double> y2;
        for (auto& x : x1) y2.push_back(Ath::Math::sin9(x));

        std::vector<double> y2_error;
        for (auto [x, y] : std::views::zip(x1, y2)) y2_error.push_back((y - std::sin(x)) * 1e12);

        auto x2 = matplot::linspace(-0.5, 0.5, 600);
        std::vector<double> y3;
        for (auto& x : x2) y3.push_back(Ath::Math::sin2pi9(x));

        matplot::hold(matplot::on);
        plot(x1, y1,       "Math::sin(x)");
        plot(x1, y1_error, "Error * 1e4");

        plot(x1, y2,       "Math::sin9(x)");
        plot(x1, y2_error, "Error * 1e12");

        plot(x2, y3,       "Math::sin2pi9(x)");

        auto lg = matplot::legend();
        lg->location(matplot::legend::general_alignment::bottomright);

        matplot::title("Rational approximations of sin");    
        matplot::save("plot0sinpade.png");
    }

    // Plot polynomial approximations of sin(x)
    {
        auto x1 = matplot::linspace(-0.5, 0.5, 1000);
        
        std::vector<double> y0;
        for (auto& x : x1) y0.push_back(Ath::Math::foldArgument(x));
        
        std::vector<double> y1;
        for (auto& x : x1) y1.push_back(Ath::Math::sin2pi5(x));

        std::vector<double> y1_error;
        for (auto [x, y] : std::views::zip(x1, y1)) y1_error.push_back((y - std::sin(Ath::Math::tau<double> * x)) * 1e4);

        std::vector<double> y2;
        for (auto& x : x1) y2.push_back(Ath::Math::sin2pi7(x));

        std::vector<double> y2_error;
        for (auto [x, y] : std::views::zip(x1, y2)) y2_error.push_back((y - std::sin(Ath::Math::tau<double> * x)) * 1e6);

        matplot::figure();
        matplot::hold(matplot::on);

        plot(x1, y0, "Math::foldArgument(x)");
        plot(x1, y1, "Math::sin2pi5(x)");
        plot(x1, y1_error, "Error * 1e4");
        plot(x1, y2, "Math::sin2pi7(x)");
        plot(x1, y2_error, "Error * 1e6");

        auto lg = matplot::legend();
        lg->location(matplot::legend::general_alignment::bottomright);

        matplot::title("Polynomial approximations of sin");    
        matplot::save("plot1sinpoly.png");
    }

    // Benchmark sines
    {
        auto run = [&](auto fn)
        {
            constexpr size_t iterations = 1e8;
            constexpr size_t size = 1e3;

            std::array<double, 1000> x;
            for (size_t i = 0; i < size; i++)
            x[i] = (double(i) / double(size));

            auto t0 = std::chrono::high_resolution_clock::now();

            double acc = 0.0;
            for (size_t i = 0; i < iterations; ++i)
                acc += fn(x[i%1000]);

            auto t1 = std::chrono::high_resolution_clock::now();
            sink = acc;

            std::chrono::duration<double, std::milli> ms = t1 - t0;
            return ms.count();
        };

        std::cout << "Benchmark 1e8 calls to sine" << std::endl;
        auto t_std = run([](double v){ return std::sin(v); });
        auto t_sin = run([](double v){ return Ath::Math::sin(v); });
        auto t_sin2pi5 = run([](double v){ return Ath::Math::sin2pi5(v); });
        auto t_sin2pi7 = run([](double v){ return Ath::Math::sin2pi7(v); });
        auto t_sin2pi9 = run([](double v){ return Ath::Math::sin2pi9(v); });
        
        std::vector t = {t_std, t_sin, t_sin2pi5, t_sin2pi7, t_sin2pi9};
        matplot::figure();
        matplot::bar(t);
        matplot::gca()->x_axis().ticklabels({"std", "sin", "sin2pi5", "sin2pi7", "sin2pi9"});

        matplot::title("Execution time of 1e8 calls to sine, ms");    
        matplot::save("plot2sinbench.png");
    }

    // math/Special.h
    {
        auto x1 = matplot::linspace(-1, 1, 1000);
        std::vector<double> y1;
        for (auto& x : x1) y1.push_back(Ath::Math::lanczos1(x));

        auto x2 = matplot::linspace(-2, 2, 1000);
        std::vector<double> y2;
        for (auto& x : x2) y2.push_back(Ath::Math::lanczos2(x));

        auto x3 = matplot::linspace(-3, 3, 1000);
        std::vector<double> y3;
        for (auto& x : x3) y3.push_back(Ath::Math::lanczos3(x));

        matplot::figure();
        matplot::hold(matplot::on);

        plot(x1, y1, "Math::lanczos1");
        plot(x2, y2, "Math::lanczos2");
        plot(x3, y3, "Math::lanczos3");

        auto lg = matplot::legend();
        lg->location(matplot::legend::general_alignment::bottomright);

        matplot::grid(matplot::on);
        matplot::title("Lanczos kernels");    
        matplot::save("plot3lanczos.png");
    }

    return 0;
}