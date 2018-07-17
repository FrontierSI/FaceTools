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

#ifndef FACE_TOOLS_ACTION_TOGGLE_CAMERA_ACTOR_INTERACTION_H
#define FACE_TOOLS_ACTION_TOGGLE_CAMERA_ACTOR_INTERACTION_H

#include "FaceAction.h"
#include <ModelMoveInteractor.h>

namespace FaceTools {
namespace Action {

class FaceTools_EXPORT ActionToggleCameraActorInteraction : public FaceAction
{ Q_OBJECT
public:
    ActionToggleCameraActorInteraction( const QString& dname="Move Models", const QIcon& ico=QIcon());

    Interactor::ModelMoveInteractor* interactor() override { return &_interactor;}

protected slots:
    bool testEnabled() const override { return true;}
    bool doAction( FaceControlSet&) override;
    void doOnAffineChange( const FaceControl*);

private:
    Interactor::ModelMoveInteractor _interactor;
    bool displayDebugStatusProgression() const override { return false;}
};  // end class

}   // end namespace
}   // end namespace

#endif
