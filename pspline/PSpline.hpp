#ifndef DEF_PSPLINE
#define DEF_PSPLINE

#include "Lapack.hpp"
#include "omp.h"

class PSpline{
	public:
		PSpline(unsigned int const& k);
		void set(unsigned int const& N=0);

		void add_data(Vector<double> const& ci, double const& yi);
		void add_data(unsigned int const& i, Vector<double> const& c, double const& y);
		void compute_weights();
		double extrapolate(Vector<double> const& x) const;

	private:
		std::vector<Vector<double> > c_;
		std::vector<double> y_;
		Vector<double> weights_;
		unsigned int basis_;
		unsigned int dim_;
		unsigned int N_;
		double (PSpline::*phi_)(double const&) const;
		bool select_basis();

		double phi1(double const& r) const { return r; }
		double phi2(double const& r) const { return r*(r<1?log(pow(r,r)):r*log(r)); }
		double phi3(double const& r) const { return r*r*r; }
		double phi4(double const& r) const { return r*r*r*(r<1?log(pow(r,r)):r*log(r)); }
		double phi5(double const& r) const { return r*r*r*r*r; }
		double phi6(double const& r) const { return r*r*r*r*r*(r<1?log(pow(r,r)):r*log(r)); }
		double phi7(double const& r) const { return (r<1.0?std::pow(1-r,2):0.0); }
};
#endif
