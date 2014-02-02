#include "Algorithm.h"

namespace ofxNonLinearFit {
	//----------
	Algorithm::Algorithm(nlopt::algorithm algorithm, Domain domain) : 
		algorithm(algorithm), domain(domain) {
	}

	//----------
	Algorithm & Algorithm::operator=(const Algorithm & other) {
		(* this) = Algorithm(other.getCPPType(), other.getDomain());
		return * this;
	}

	//----------
	nlopt_algorithm Algorithm::getCType() const {
		return nlopt_algorithm(this->algorithm);
	}

	//----------
	nlopt::algorithm Algorithm::getCPPType() const {
		return this->algorithm;
	}

	//----------
	Algorithm::Domain Algorithm::getDomain() const {
		return this->domain;
	}

	//----------
	string Algorithm::toString() const {
		return string(nlopt::algorithm_name(this->getCPPType()));
	}

	//----------
	vector<Algorithm> Algorithm::List(Domain domain) {
		auto listAll = Algorithm::ListAll();
		vector<Algorithm> listDomain;

		for(auto item : listAll) {
			if(item.getDomain() == domain) {
				listDomain.push_back(item);
			}
		}

		return listDomain;
	}

	//----------
	vector<Algorithm> Algorithm::ListAll() {
		vector<Algorithm> list;

		using namespace nlopt;

		nlopt::algorithm globalGradientless[] = {
			GN_DIRECT,
			GN_DIRECT_L,
			GN_DIRECT_L_RAND,
			GN_DIRECT_NOSCAL,
			GN_DIRECT_L_NOSCAL,
			GN_DIRECT_L_RAND_NOSCAL,
			GN_ORIG_DIRECT,
			GN_ORIG_DIRECT_L,
			GN_ESCH,
			GN_ISRES
		};
		
		/*
		nlopt::algorithm globalGradient[] = {
			// GD_STOGO, NOT COMPILED
			// GD_STOGO_RAND NOT COMPILED
		};
		*/

		nlopt::algorithm localGradientless[] = {
			LN_COBYLA,
			LN_NEWUOA,
			LN_NEWUOA_BOUND,
			LN_NELDERMEAD,
			LN_SBPLX,
			LN_AUGLAG,
			LN_AUGLAG_EQ,
			LN_BOBYQA,
			LN_PRAXIS
		};

		nlopt::algorithm localGradient[] = {
			// LD_LBFGS_NOCEDAL, NOT COMPILED
			LD_LBFGS,
			LD_VAR1,
			LD_VAR2,
			LD_TNEWTON,
			LD_TNEWTON_RESTART,
			LD_TNEWTON_PRECOND,
			LD_TNEWTON_PRECOND_RESTART,
			LD_MMA,
			LD_SLSQP,
			LD_CCSAQ,
			LD_AUGLAG,
			LD_AUGLAG_EQ
		};
		
		for(auto algorithm : globalGradientless) {
			list.push_back(Algorithm(algorithm, GlobalGradientless));
		}
		
		/* NO COMPILED ALGORITHMS
		for(auto algorithm : globalGradient) {
			list.push_back(Algorithm(algorithm, GlobalGradient));
		}
		*/

		for(auto algorithm : localGradient) {
			list.push_back(Algorithm(algorithm, LocalGradient));
		}

		for(auto algorithm : localGradientless) {
			list.push_back(Algorithm(algorithm, LocalGradientless));
		}

		return list;
	}
}