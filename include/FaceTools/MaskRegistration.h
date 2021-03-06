/************************************************************************
 * Copyright (C) 2021 SIS Research Ltd & Richard Palmer
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

#ifndef FACE_TOOLS_MASK_REGISTRATION_H
#define FACE_TOOLS_MASK_REGISTRATION_H

#include "FaceTypes.h"
#include <r3d/KDTree.h>
#include <QReadWriteLock>

namespace FaceTools {

class FaceTools_EXPORT MaskRegistration
{
public:
    // Returns true if the provided filepath points to a valid model and asynchronous loading started
    // OR if the mask is already loaded because the given path already points to the mask.
    // Calling this function with a filepath that is different to the current mask (even
    // if the given path isn't valid) will call unsetMask to unload any existing mask.
    static bool setMask( const QString&);

    // If a mask is already set, this unloads it.
    static void unsetMask();

    // Returns true iff the mask is loaded. Returns false if the model is
    // currently in the middle of being loaded.
    static bool maskLoaded();

    // Returns the currently set mask path.
    static QString maskPath();

    // Returns the hash of the mask mesh.
    static size_t maskHash();

    // Return the position of the given landmark on the given mask
    // mesh according to its expected barycentric position.
    static Vec3f maskLandmarkPosition( const r3d::Mesh&, int id, FaceSide);

    struct MaskData
    {
        MaskData();
        const FM *mask;
        QString path;   // Filepath
        size_t hash;    // Mesh hash

        // Landmarks for a mask are stored as three facial lateral
        // sets keyed by landmark ID. The positions are stored as
        // barycentric coordinates for the associated face.
        std::unordered_map<int, std::pair<int, Vec3f> > lmksL;
        std::unordered_map<int, std::pair<int, Vec3f> > lmksM;
        std::unordered_map<int, std::pair<int, Vec3f> > lmksR;

        std::unordered_map<int, int> oppVtxs;  // Map of laterally opposite vertex IDs
        IntSet medialVtxs;      // Medial (centreline) vertices
        IntSet q0, q1, q2, q3;  // Quadrant vertices (top left, top right, bottom right, bottom left)
        Vec3f centre;   // Centre taken from just the medial vertices
        float radius;   // Radius taken from just the medial vertices
    };  // end struct

    using MaskPtr = std::shared_ptr<const MaskData>;

    // Return the read locked mask data which is released
    // when all shared copies of the returned pointer die.
    static MaskPtr maskData();

    // Register the currently set mask against the given model and return it.
    // The model must have first been brought into *reasonable* rigid alignment with the mask.
    static r3d::Mesh::Ptr registerMask( const r3d::KDTree &target);

    // Given a deformed version of the loaded mask, run procrustes superimposition
    // on it and return its transform from the currently loaded mask.
    static Mat4f calcMaskAlignment( const r3d::Mesh&);

private:
    static MaskData s_mask;
    static QReadWriteLock s_lock;
};  // end class

}   // end namespace

#endif
