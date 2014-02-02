#pragma once

#include "ofVec3f.h"
#include "ofMatrix4x4.h"

#include "../Model.h"

namespace ofxNonLinearFit {
	namespace Models {
		struct RigidBodyDataPoint {
			ofVec3f x;
			ofVec3f xdash;
		};

		class RigidBody : public Model<RigidBodyDataPoint, RigidBody> {
		public:
			typedef RigidBodyDataPoint DataPoint;

			unsigned int getParameterCount() const override;
			double getResidual(DataPoint) const override;
			void evaluate(DataPoint &) const override;

			void setParameters(const Parameters &) override;
			void clearParameters() override;

			void setTransform(const ofMatrix4x4 &);
			void setTransform(const ofVec3f & translation, const ofVec3f & rotationEuler);
			ofVec3f evaluate(const ofVec3f &) const;

		protected:
			ofMatrix4x4 transform;
		};
	}
}