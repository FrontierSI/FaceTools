/************************************************************************
 * Copyright (C) 2017 Richard Palmer
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

#ifndef FACE_TOOLS_MODEL_ENTRY_EXIT_INTERACTOR_H
#define FACE_TOOLS_MODEL_ENTRY_EXIT_INTERACTOR_H

/**
 * Deals with messages relating to position of the mouse cursor as it moves over a face model.
 * Handy for actions needing to enable/disable themselves depending upon where the mouse is.
 * This interactor should only be attached to FaceModelViewer types (not base ModelViewer
 * types) - since it requires FaceModelViewer::attached.
 */

#include "ModelViewerInteractor.h"
#include <FaceControl.h>

namespace FaceTools {
class FaceModelViewer;

namespace Interactor {

class FaceTools_EXPORT ModelEntryExitInteractor : public ModelViewerInteractor
{ Q_OBJECT
public:
    ModelEntryExitInteractor();

    inline FaceControl* model() const { return _mnow;} // Model cursor is currently over (NULL if none).
    inline int landmark() const { return _lnow;}       // Landmark cursor is over (-1 if none).

    bool isLeftDown() const { return _ldown;}

signals:
    void onEnterModel( FaceControl*) const;
    void onLeaveModel( FaceControl*) const;

    // Enter and leave signals for landmarks will only be emitted if the given
    // FaceControl currently has a LandmarksVisualisation applied to its view.
    void onEnterLandmark( FaceControl*, int);
    void onLeaveLandmark( FaceControl*, int);

    void onLeftDown();
    void onLeftDrag();
    void onLeftUp();

protected:
    void onAttached() override;
    void onDetached() override;

private:
    FaceModelViewer* _viewer;
    FaceControl* _mnow;
    int _lnow;
    bool _ldown;

    void testLeaveLandmark( FaceControl*, int);
    void testLeaveModel();

    bool mouseLeave( const QPoint&) override;
    bool mouseEnter( const QPoint&) override;
    bool mouseMove( const QPoint&) override;
    bool middleDrag( const QPoint&) override;
    bool rightDrag( const QPoint&) override;
    bool leftDrag( const QPoint&) override;
    bool leftButtonDown( const QPoint&) override;
    bool leftButtonUp( const QPoint&) override;
    bool rightButtonDown( const QPoint&) override;

    bool testPoint( const QPoint&);
};  // end class

}   // end namespace
}   // end namespace

typedef FaceTools::Interactor::ModelEntryExitInteractor MEEI;

#endif