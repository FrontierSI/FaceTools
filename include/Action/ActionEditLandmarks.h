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

#ifndef FACE_TOOLS_ACTION_EDIT_LANDMARKS_H
#define FACE_TOOLS_ACTION_EDIT_LANDMARKS_H

#include "ActionVisualise.h"
#include <LandmarksVisualisation.h>
#include <LandmarksInteractor.h>
#include <QStatusBar>

namespace FaceTools {
namespace Action {

class FaceTools_EXPORT ActionEditLandmarks : public ActionVisualise
{ Q_OBJECT
public:
    ActionEditLandmarks( const QString&, const QIcon&, FEEI*, QStatusBar* sb=nullptr);
    ~ActionEditLandmarks() override;

    Interactor::LandmarksInteractor* interactor() override { return _interactor;}

private slots:
    void doAfterAction( ChangeEventSet&, const FaceControlSet&, bool) override;
    void doOnEditedLandmark( const FaceControl*);

private:
    Vis::LandmarksVisualisation *_vis;
    Interactor::LandmarksInteractor *_interactor;
    QStatusBar *_sbar;
};  // end class

}   // end namespace
}   // end namespace

#endif
