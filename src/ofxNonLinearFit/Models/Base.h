#pragma once

#include <cstring>
#include <mutex>
#include <atomic>

namespace ofxNonLinearFit {
	namespace Models {
		///we must pass the child class as a template argument so that
		///we can instantiate a cached test model for use during optimisation
		template<class DataPoint, class Child>
		class Base {
		public:
			typedef double Parameter;
			typedef Parameter * Parameters;
			typedef vector<DataPoint> DataSet;

			//--
			//parts to overload when designing your own model
			//
			virtual unsigned int getParameterCount() const = 0;
			virtual double getResidual(DataPoint) const = 0;
			virtual void evaluate(DataPoint &) const = 0;
			//
			//--

			///override this if you want to pre-cache anything about your model
			virtual void cacheModel() {
			};

			Base() {
				this->parameters = NULL;
			}

			Base(const Base & other) {
				this->parameters = NULL;
				if (other.isReady()) {
					this->setParameters(other.getParameters());
				}
			}

			virtual ~Base() {
				this->clearParameters();
			}

			bool isReady() const {
				return this->parameters != NULL;
			}

			void initialiseParameters() {
				if (!this->isReady()) {
					this->parameters = new Parameter[this->getParameterCount()];
				}
				this->resetParameters();
			}

			virtual void resetParameters() {
				for (int i = 0; i<this->getParameterCount(); i++) {
					this->parameters[i] = 0.0;
				}
			}

			const Parameters & getParameters() const {
				//could throw an exception here if parameters are not set
				return this->parameters;
			}

			void setParameters(const Parameters parameters) {
				if (!this->isReady()) {
					this->parameters = new Parameter[this->getParameterCount()];
				}
				memcpy(this->parameters, parameters, sizeof(Parameter) * this->getParameterCount());
				this->cacheModel();
			}
			
			void setParameters(const double * x) {
				this->setParameters((Parameters) x);
			}

			///override this if you want to loose pre-cached data when model is cleared
			void clearParameters() {
				if (this->isReady()) {
					delete[] this->parameters;
					this->parameters = NULL;
				}
				this->cacheModel();
			}

			virtual double getResidualOnSet(const DataSet & dataSet) const {
				if (this->multiThreaded) {
					atomic<double> residual = 0.0;
					for (const auto & dataPoint : dataSet) {
						auto thisResidual = this->getResidual(dataPoint);

						residual = residual + thisResidual;
					}
					return residual / (double)dataSet.size();
				}
				else {
					double residual = 0.0;
					for (const auto & dataPoint : dataSet) {
						residual += this->getResidual(dataPoint);
					}
					return residual / (double) dataSet.size();
				}
			}
			
			double getResidualOnSet(const void * data) {
				return this->getResidualOnSet(* (DataSet *) data);
			}

			virtual double getResidualOnSet(const Parameters parameters, const DataSet & dataSet) const {
				Child testModel(* (Child *) this);
				testModel.setParameters(parameters);
				return testModel.getResidualOnSet(dataSet);
			}

			void evaluateSet(DataSet & dataSet) const {
				for(auto & dataPoint : dataSet) {
					this->evaluate(dataPoint);
				}
			}

		protected:
			Parameters parameters;
			bool multiThreaded = true;
		};
	}
}