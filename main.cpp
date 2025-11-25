// damped_verlet.cpp
// Damped Harmonic Oscillator using position-Verlet with velocity-estimate for damping
// Produces CSV file with columns: t, x, v, energy
//
// Compile:
//    g++ -O2 -std=c++17 -o damped_verlet damped_verlet.cpp
//
// Run (examples):
//    ./damped_verlet            # runs default parameters and writes CSV
//    ./damped_verlet 1.0 1.0 0.2 0.01 50.0 1.0 0.0
//
// Arguments (optional):
//    m k b dt tmax x0 v0
// Defaults: m=1.0 k=1.0 b=0.2 dt=0.01 tmax=50 x0=1.0 v0=0.0

#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <string>
#include <sstream>

int main(int argc, char** argv) {
    // --- default parameters ---
    double m = 1.0;
    double k = 1.0;
    double b = 0.2;
    double dt = 0.01;
    double tmax = 50.0;
    double x0 = 1.0;
    double v0 = 0.0;

    if (argc >= 2) m = std::stod(argv[1]);
    if (argc >= 3) k = std::stod(argv[2]);
    if (argc >= 4) b = std::stod(argv[3]);
    if (argc >= 5) dt = std::stod(argv[4]);
    if (argc >= 6) tmax = std::stod(argv[5]);
    if (argc >= 7) x0 = std::stod(argv[6]);
    if (argc >= 8) v0 = std::stod(argv[7]);

    // Determine regime
    double discr = b*b - 4.0*m*k;
    std::string regime;
    if (discr < 0) regime = "underdamped";
    else if (std::abs(discr) < 1e-12) regime = "critically damped";
    else regime = "overdamped";

    // Output file name:
    std::ostringstream fname;
    fname << "damped_b" << std::fixed << std::setprecision(2) << b
          << "_dt" << dt << ".csv";
    std::string filename = fname.str();

    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Error opening output file: " << filename << "\n";
        return 1;
    }

    // Header
    out << "# Damped harmonic oscillator (m=" << m << ", k=" << k << ", b=" << b
        << ", dt=" << dt << ", x0=" << x0 << ", v0=" << v0 << ", regime=" << regime << ")\n";
    out << "t,x,v,energy\n";

    // Initial acceleration at t=0: a0 = (-k*x0 - b*v0)/m
    double a0 = (-k * x0 - b * v0) / m;

    // Compute "previous" position x_-1 using backward Taylor:
    // x_{-1} = x0 - v0*dt + 0.5*a0*dt^2
    double x_prev = x0 - v0*dt + 0.5 * a0 * dt * dt;
    double x = x0;
    double v = v0;

    // initial energy
    auto energy = [&](double x_, double v_){
        return 0.5 * m * v_ * v_ + 0.5 * k * x_ * x_;
    };

    double t = 0.0;
    out << std::fixed << std::setprecision(8);
    out << t << "," << x << "," << v << "," << energy(x,v) << "\n";

    // Time-stepping loop
    int nsteps = static_cast<int>(std::ceil(tmax / dt));
    for (int n = 0; n < nsteps; ++n) {
        // estimate velocity at time n:
        double v_est = (x - x_prev) / dt;

        // acceleration a_n
        double a = (-k * x - b * v_est) / m;

        // Verlet update for next position
        double x_next = 2.0 * x - x_prev + a * dt * dt;

        // estimate velocity at next time using central difference (optional)
        double v_next = (x_next - x) / dt;

        t += dt;
        // write step n+1 (t)
        out << t << "," << x_next << "," << v_next << "," << energy(x_next, v_next) << "\n";

        // shift variables for next iteration
        x_prev = x;
        x = x_next;
        v = v_next;
    }

    out.close();
    std::cout << "Simulation finished. Output written to: " << filename << "\n";
    std::cout << "Regime detected: " << regime << " (b^2 - 4*m*k = " << discr << ")\n";
    std::cout << "Suggested plotting: x(t), phase space (x vs v), energy(t).\n";
    std::cout << "For different regimes try b = 0.2 (underdamped), 2.0 (critical/over depending on m,k), 5.0 (overdamped).\n";
    std::cout << "If you want a smaller dt, re-run with dt as the 4th command-line argument.\n";
    return 0;
}
