#include "../lib/NLopt/include/nlopt.h"
#include "../lib/NLopt/include/nlopt.hpp"

#include <vector>

using namespace std;

namespace ofxNonLinearFit {
	class Algorithm {
	public:
		enum Domain {
			Global = 1,
			Gradient = 2
		};

		Algorithm(nlopt::algorithm);
		virtual nlopt_algorithm getCType() const = 0;
		virtual nlopt::algorithm getCPPType() const = 0;
		virtual Domain getDomain() const = 0;

		string toString() const;
		static vector<Algorithm> List(bool global, bool local);
		static vector<Algorithm> List();
	};
}