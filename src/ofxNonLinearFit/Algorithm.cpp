#include "Algorithm.h"

namespace ofxNonLinearFit {
	//----------
	string Algorithm::toString() const {
		return string(nlopt::algorithm_name(this->getCPPType()));
	}

	//----------
	vector<Algorithm> Algorithm::List(bool global, bool gradient) {
		auto listAll = Algorithm::List();
		vector<Algorithm> listDomain;

		for(auto item : listAll) {
			const auto domain = item.getDomain();
			if(domain & (Global * global) || (domain & (Gradient * gradient))) {
				listDomain.push_back(item);
			}
		}

		return listDomain;
	}

	//----------
	vector<Algorithm> Algorithm::List() {

	}

}