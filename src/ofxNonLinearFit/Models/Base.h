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
			virtual void getResidual(DataPoint, double & residual, double * gradient) const = 0;
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
			}

			virtual void getResidualOnSet(const DataSet & dataSet, double & residual, double * gradient) {
				residual = 0.0;

				this->cacheModel();

				if (gradient) {
					for (int i = 0; i < this->getParameterCount(); i++) {
						gradient[i] = 0.0;
					}
				}

				for (const auto & dataPoint : dataSet) {
					double dataPointResidual = 0.0;

					if (gradient) {
						double * dataPointGradient = new double[this->getParameterCount()];
						this->getResidual(dataPoint, dataPointResidual, dataPointGradient);

						for (int i = 0; i < this->getParameterCount(); i++) {
							gradient[i] += dataPointGradient[i];
						}
						delete[] dataPointGradient;
					}
					else {
						this->getResidual(dataPoint, dataPointResidual, NULL);
					}
					residual += dataPointResidual;
				}
			}
			
			void getResidualOnSet(const void * data, double & residual, double * gradient) {
				this->getResidualOnSet(* (DataSet *) data, residual, gradient);
			}

			virtual void getResidualOnSet(const Parameters parameters, const DataSet & dataSet, double & residual, double * gradient) const {
				Child testModel(* (Child *) this);
				testModel.setParameters(parameters);
				testModel.getResidualOnSet(dataSet, residual, gradient);
			}

			void evaluateSet(DataSet & dataSet) const {
				for(auto & dataPoint : dataSet) {
					this->evaluate(dataPoint);
				}
			}

		protected:
			Parameters parameters;
		};
	}
}