#include "RigidBody.h"

namespace ofxNonLinearFit {
	namespace Models {
#pragma mark overrides
		//----------
		unsigned int RigidBody::getParameterCount() const {
			return 6;
		}

		//----------
		double RigidBody::getResidual(DataPoint dataPoint) const {
			return (this->evaluate(dataPoint.x) - dataPoint.xdash).lengthSquared();
		}

		//----------
		void RigidBody::evaluate(DataPoint & dataPoint) const {
			dataPoint.xdash = this->evaluate(dataPoint.x);
		}

		//----------
		void RigidBody::setParameters(const Parameters & parameters) {
			Model::setParameters(parameters);
			
			const auto translate = ofMatrix4x4::newTranslationMatrix(parameters[0], parameters[1], parameters[2]);
			
			const auto eulerAngles = ofVec3f(parameters[3], parameters[4], parameters[5]);
			const auto quaternion = ofQuaternion(eulerAngles.y, ofVec3f(0, 1, 0), eulerAngles.x, ofVec3f(1, 0, 0), eulerAngles.z, ofVec3f(0, 0, 1));
			const auto rotate = ofMatrix4x4::newRotationMatrix(quaternion);

			this->transform = rotate * translate;
		}

		//----------
		void RigidBody::clearParameters() {
			Model::clearParameters();

			//clear back to identity
			this->transform = ofMatrix4x4();
		}

#pragma mark utility functions
		//----------
		void RigidBody::setTransform(const ofMatrix4x4 & transform) {
			const auto translation = transform.getTranslation();
			const auto rotationQuaternion = transform.getRotate();
			const auto rotationEuler = rotationQuaternion.getEuler();

			this->setTransform(translation, rotationEuler);
		}

		//----------
		void RigidBody::setTransform(const ofVec3f & translation, const ofVec3f & rotationEuler) {
			Parameter parameters[] = { translation.x, translation.y, translation.z, rotationEuler.x, rotationEuler.y, rotationEuler.z };
			this->setParameters(parameters); // should also in turn set the transform by rebuilding it from the Parameters
		}

		//----------
		ofVec3f RigidBody::evaluate(const ofVec3f & x) const {
			return x * this->transform;
		}
	}
}