#include "../lib/NLopt/include/nlopt.h"
#include "../lib/NLopt/include/nlopt.hpp"

#include "Models/Base.h"
#include "Algorithm.h"

#include "ofLog.h"

namespace ofxNonLinearFit {
	template<class Model>
	class Fit {
	public:
		Fit(Algorithm algorithm) :
			algorithm(algorithm) {
			this->optimiser = nlopt_create(this->algorithm.getCType(), Model().getParameterCount());
			this->clearSettings();
		}

		virtual ~Fit() {
			nlopt_destroy(this->optimiser);
		}

		///set some parameters in the model if you want to use those as starting parameters
		///since C++ language does not have "where" keyword, we can't access Model::DataSet in the
		///function definition, and must use a void * instead.
		///returns residual or throws string
		bool optimise(Model & model, void * const dataSet, double * residual = NULL) { 
			bool localResidual = (!residual);
			if(localResidual) {
				residual = new double;
			}

			if(!model.isReady()) {
				model.initialiseParameters();
			}

			Payload payload = {model, dataSet};
			nlopt_func function = & Fit<Model>::minimiserFunction;
			nlopt_set_min_objective(this->optimiser, function, &payload);
			auto result = nlopt_optimize(this->optimiser, model.getParameters(), residual);

			if (result < 0) {
				ofLogWarning("ofxNonLinearFit") << "Fit failed :" << toString(result);
			}

			if (localResidual) {
				delete residual;
			}

			return result >= 0;
		}

		void clearSettings() {
			nlopt_set_stopval(this->optimiser, 1e-12);
		}

		void forceStop() {
			nlopt_force_stop(this->optimiser);
		}

		const Algorithm & getAlgorithm() const {
			return this->algorithm;
		}

		static string toString(nlopt_result result) {
			switch(result) {
			case 1:
				return "success";
			case 2:
				return "stop value reached";
			case 3:
				return "ftol reached";
			case 4:
				return "xtol reached";
			case 5:
				return "maxeval reached";
			case 6:
				return "max time reached";
			case -1:
				return "failure";
			case -2:
				return "invalid arguments";
			case -3:
				return "out of memory";
			case -4:
				return "roundoff limited";
			case -5:
				return "forced stop";
			}
			return "code not known";
		}
	protected:
		struct Payload {
			Model & model;
			void * const dataSet;
		};

		static double minimiserFunction(unsigned n, const double * x, double * grad, void * data) {
			//get our useful data out
			const Payload & payload = * (Payload *) data;
			auto & model = payload.model;
			const auto & dataSet = * (Model::DataSet *) payload.dataSet;

			//update the model
			auto parameters = (Model::Parameters) x;
			model.setParameters(parameters);

			//calc residual
			return model.getResidualOnSet(dataSet);
		}

		nlopt_opt optimiser;
		Algorithm algorithm;
	};
}