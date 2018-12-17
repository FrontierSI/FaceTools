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

#include <ActionDeletePath.h>
#include <FaceModel.h>
#include <FaceTools.h>
#include <algorithm>
#include <cassert>
using FaceTools::Action::FaceAction;
using FaceTools::Action::ActionEditPaths;
using FaceTools::Action::ActionDeletePath;
using FaceTools::Action::EventSet;
using FaceTools::Interactor::PathSetInteractor;
using FaceTools::Vis::PathSetVisualisation;
using FaceTools::FVS;
using FaceTools::Vis::FV;
using FaceTools::FM;


ActionDeletePath::ActionDeletePath( const QString& dn, const QIcon& ico, ActionEditPaths* e)
    : FaceAction( dn, ico),
      _vis( static_cast<PathSetVisualisation*>(e->visualisation())),
      _interactor( e->interactor())
{
}   // end ctor


bool ActionDeletePath::testEnabled( const QPoint*) const
{
    const FV* fv = _interactor->hoverModel();
    return isReady(fv) && _interactor->hoverPathId() >= 0;
}   // end testEnabled


bool ActionDeletePath::doAction( FVS& fvs, const QPoint&)
{
    FV* fv = fvs.first();
    assert(fv);
    assert( _interactor->hoverModel() == fv);
    fvs.clear();

    FM* fm = fv->data();
    const int pid = _interactor->hoverPathId();
    if ( pid >= 0)
    {
        //fm->lockForWrite();
        fm->paths()->removePath(pid);
        fm->setSaved(false);
        //fm->unlock();
        fvs.insert( fm);
        _interactor->doOnLeaveHandle( fv);
        _vis->refresh(fm);
    }   // end if

    return !fvs.empty();
}   // end doAction
