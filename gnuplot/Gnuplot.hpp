#ifndef GNUPLOT
#define GNUPLOT

#include "IOFiles.hpp"
#include "Linux.hpp"

class Gnuplot{
	public:
		/*!Constructor for the creation of a .gp file in path/filename*/
		Gnuplot(std::string const&  path, std::string const& filename);
		/*!Default destructor*/
		~Gnuplot() = default;
		/*{Forbidden*/
		Gnuplot() = delete;
		Gnuplot(Gnuplot const&) = delete;
		Gnuplot(Gnuplot&&) = delete;
		Gnuplot& operator=(Gnuplot);
		/*}*/

		void multiplot();

		void title(std::string const& title);

		void range(std::string const& axis, std::string const& a, std::string const& b);
		void range(std::string const& axis, double const& a, double const& b);
		void range(std::string const& axis, double const& a, std::string const& b);
		void range(std::string const& axis, std::string const& a, double const& b);
		void range(std::string const& axis, std::string const& s);
		void range(std::string const& axis);

		void margin(std::string const& l, std::string const& r, std::string const& t, std::string const& b);
		void margin(double const& l, double const& r, double const& t, double const& b);

		void tics(std::string const& axis, std::string const& t);
		void tics(std::string const& axis, double const& t);
		void tics(std::string const& axis);

		void label(std::string const& axis, std::string const& l, std::string const& options="");
		void label(std::string const& axis);

		void key(std::string const& option);

		void operator=(std::string const& s);
		void operator+=(std::string const& s);

		void save_file();
		void create_image(bool const& silent, std::string const& format="");

	private:
		std::string path_;		//!< path of the .gp, .png and .pdf files
		std::string filename_;	//!< filename (without the extension)
		std::string plot_;		//!< text of the .gp file
		bool multiplot_;
};
#endif
