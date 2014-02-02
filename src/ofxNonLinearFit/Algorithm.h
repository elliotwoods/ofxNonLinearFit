#include "../lib/NLopt/include/nlopt.h"
#include "../lib/NLopt/include/nlopt.hpp"

#include <vector>

using namespace std;

namespace ofxNonLinearFit {
	class Algorithm {
	public:
		///LSB denotes if gradient is required
		///Bit 2 denotes if local
		enum Domain {
			GlobalGradientless = 0,
			GlobalGradient = 1,
			LocalGradientless = 2,
			LocalGradient = 3,
			Other = 4
		};

		virtual nlopt_algorithm getCType() const;
		virtual nlopt::algorithm getCPPType() const;
		virtual Domain getDomain() const;

		string toString() const;
		static vector<Algorithm> List(Domain = LocalGradientless); ///<List all algorithms within a given domain
		static vector<Algorithm> ListAll();
	protected:
		Algorithm(nlopt::algorithm, Domain);
		const nlopt::algorithm algorithm;
		const Domain domain;
	};
}