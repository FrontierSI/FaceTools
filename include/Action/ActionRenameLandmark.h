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

#ifndef FACE_TOOLS_ACTION_RENAME_LANDMARK_H
#define FACE_TOOLS_ACTION_RENAME_LANDMARK_H

#include "ActionEditLandmarks.h"

namespace FaceTools {
namespace Action {

class FaceTools_EXPORT ActionRenameLandmark : public FaceAction
{ Q_OBJECT
public:
    ActionRenameLandmark( const QString& dname, const QIcon& ico, ActionEditLandmarks*, QWidget *parent=nullptr);

private slots:
    bool testEnabled( const QPoint* mc=nullptr) const override;
    bool doAction( FVS&, const QPoint&) override;
    void doAfterAction( EventSet& cs, const FVS&, bool) override { cs.insert(LANDMARKS_CHANGE);}

private:
    ActionEditLandmarks *_editor;
    QWidget *_parent;
};  // end class

}   // end namespace
}   // end namespace

#endif
