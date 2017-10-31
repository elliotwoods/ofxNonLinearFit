#pragma once

#include "ofVec3f.h"
#include "ofMatrix4x4.h"

#include "Base.h"

namespace ofxNonLinearFit {
	namespace Models {
		struct RigidBodyDataPoint {
			ofVec3f x;
			ofVec3f xdash;
		};

		class RigidBody : public Base<RigidBodyDataPoint, RigidBody> {
		public:
			typedef RigidBodyDataPoint DataPoint;

			unsigned int getParameterCount() const override;
			void getResidual(DataPoint, double & residual, double * gradient) const override;
			void evaluate(DataPoint &) const override;

			/// Override this if you would like to interpret the 6 pose parameters in a non-default way
			virtual void cacheModel() override;

			void setTransform(const ofVec3f & translation, const ofVec3f & rotationEuler);
			const ofMatrix4x4 & getCachedTransform() const;
			ofVec3f evaluate(const ofVec3f &) const;

		protected:
			ofMatrix4x4 transform;
		};
	}
}
