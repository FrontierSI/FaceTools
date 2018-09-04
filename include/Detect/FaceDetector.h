/************************************************************************
 * Copyright (C) 2018 Spatial Information Systems Research Limited
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/

#ifndef FACE_TOOLS_DETECT_FACE_DETECTOR_H
#define FACE_TOOLS_DETECT_FACE_DETECTOR_H

#include <LandmarkSet.h>
#include <ObjModelKDTree.h> // RFeatures
#include <Orientation.h>    // RFeatures
#include <vtkActor.h>
#include <string>

namespace FaceTools {
namespace Detect {

class FaceTools_EXPORT FaceDetector
{
public:
    // Initialise both FeaturesDetector and FaceShapeLandmarks2DDetector returning true on success.
    // Initialising of the classifier models must occur using this function in order to use FaceDetector.
    static bool initialise( const std::string& haarCascadesModelDir,
                            const std::string& faceShapeLandmarksDat);
   
    // Make an offscreen face detector for 3D models with a camera orientationDetectRange units from
    // the face model along the +Z axis for detecting initial orientation features (eye centres and
    // nose tip) using the Viola and Jones detector, then with a closer range of landmarksDetectRange
    // for the more detailed detection of landmarks using dlib:
    // "One Millisecond Face Alignment with an Ensemble of Regression Trees" (Vahid Kazemi and
    // Josephine Sullivan, CVPR 2014).
    FaceDetector( float orientationDetectRange=700.0f, float landmarksDetectRange=288.0f);
    virtual ~FaceDetector(){}

    // Find orientation and landmarks on the given model. Returns true iff detection succeeds.
    bool detect( const RFeatures::ObjModelKDTree::Ptr, RFeatures::Orientation&, LandmarkSet::Ptr);

    const std::string& err() const { return _err;} // Error message relating to last call to detect()

private:
    static bool s_initOk;
    float _orng, _drng;
    std::string _err;

    bool cleanUp();
    bool cleanUp( std::string);
    FaceDetector( const FaceDetector&) = delete;
    void operator=( const FaceDetector&) = delete;
};  // end class

}   // end namespace
}   // end namespace

#endif
