#pragma once
#include "../util.hpp"
#include "date.hpp"
#include <unordered_map>

namespace ephemeris {

	struct elements {

		union {
			double semi_major_axis;
			double a;
		};

		union {
			double eccentricity;
			double e;
		};

		union {
			double inclination;
			double i;
		};

		union {
			double mean_longitude;
			double L;
		};

		union {
			double longitude_perihelion;
			double w;
		};

		union {
			double longitude_ascending_node;
			double O;
		};

		elements() {
			this->a = 0;
			this->e = 0;
			this->i = 0;
			this->L = 0;
			this->w = 0;
			this->O = 0;
		}

		elements(double a, double e, double i, double L, double w, double O) {
			this->a = a;
			this->e = e;
			this->i = i;
			this->L = L;
			this->w = w;
			this->O = O;
		}

		template <typename T>
		elements& operator * (const T& right) {

			this->a *= right;
			this->e *= right;
			this->i *= right;
			this->L *= right;
			this->L *= right;
			this->w *= right;
			this->O *= right;

			return *this;
		}

		elements& operator * (const elements& right) {

			this->a *= right.a;
			this->e *= right.e;
			this->i *= right.i;
			this->L *= right.L;
			this->w *= right.w;
			this->O *= right.O;

			return *this;
		}

		elements& operator + (const elements& right) {

			this->a += right.a;
			this->e += right.e;
			this->i += right.i;
			this->L += right.L;
			this->w += right.w;
			this->O += right.O;

			return *this;
		}
	};

	class elements_table {

	private:

		std::unordered_map<std::string, elements> table;
		inline static elements_table* instance;

	public:

		elements_table() {

			table = std::unordered_map<std::string, elements>();
			instance = this;
		}

		void load_table(const std::string& path, const date& _date) {

			std::string content = util::read_file(path);
			std::vector<std::string> lines = util::split_str(content, '\n');

			for (auto& curr : lines) {

				std::vector<std::string> el = util::split_str(curr, ';');
				
				elements au;
				elements au_cent;

				std::string name = el[0];
				double sma = std::stod(el[1]);
				double sma_cent = std::stod(el[2]);
				double ecc = std::stod(el[3]);
				double ecc_cent = std::stod(el[4]);
				double inc = std::stod(el[5]);
				double inc_cent = std::stod(el[6]);
				double m_long = std::stod(el[7]);
				double m_long_cent = std::stod(el[8]);
				double long_peri = std::stod(el[9]);
				double long_peri_cent = std::stod(el[10]);
				double long_asc = std::stod(el[11]);
				double long_asc_cent = std::stod(el[12]);

				au = elements(sma, ecc, inc, m_long, long_peri, long_asc);
				au_cent = elements(sma_cent, ecc_cent, inc_cent, m_long_cent, long_peri_cent, long_asc_cent);

				table[name] = au + au_cent * _date.bessel_epoch();
			}
		}

		static elements_table* get() {
			return elements_table::instance;
		}

		elements& operator [] (const std::string& name) noexcept(false) {

			if (table.find(name) != table.end()) {

				return table[name];
			}

			else {
				throw std::exception("could not find entry");
			}
		}
	};
}



