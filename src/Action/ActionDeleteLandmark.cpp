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

#include <ActionDeleteLandmark.h>
#include <FaceModel.h>
#include <FaceTools.h>
#include <cassert>
using FaceTools::Action::FaceAction;
using FaceTools::Action::ActionDeleteLandmark;
using FaceTools::Action::ActionEditLandmarks;
using FaceTools::Action::EventSet;
using FaceTools::Interactor::LandmarksInteractor;
using FaceTools::FVS;
using FaceTools::Vis::FV;
using FaceTools::FM;


ActionDeleteLandmark::ActionDeleteLandmark( const QString& dn, const QIcon& ico, ActionEditLandmarks* e)
    : FaceAction( dn, ico), _editor(e)
{
}   // end ctor


bool ActionDeleteLandmark::testEnabled( const QPoint*) const
{
    bool enabled = false;
    assert(_editor);
    if ( _editor->isChecked())
    {
        const FV* fv = _editor->interactor()->hoverModel();
        enabled = isReady( fv) && _editor->interactor()->hoverId() >= 0;
    }   // end if
    return enabled;
}   // end testEnabled


bool ActionDeleteLandmark::doAction( FVS& fvs, const QPoint&)
{
    assert(_editor);
    FV* fv = fvs.first();
    assert( _editor->interactor()->hoverModel() == fv);
    fvs.clear();

    FM* fm = fv->data();
    //fm->lockForWrite();

    const int id = _editor->interactor()->hoverId();
    const bool remok = fm->landmarks()->erase(id);
    assert(remok);
    fm->setSaved(false);
    //fm->unlock();
    
    fvs.insert( fm);
    return !fvs.empty();
}   // end doAction
