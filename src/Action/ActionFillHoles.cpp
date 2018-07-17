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

#include <ActionFillHoles.h>
#include <FaceModelViewer.h>
#include <FaceControl.h>
#include <FaceModel.h>
#include <ObjModelHoleFiller.h>
#include <algorithm>
using FaceTools::Action::ActionFillHoles;
using FaceTools::Action::ChangeEventSet;
using FaceTools::Action::FaceAction;
using FaceTools::FaceControlSet;
using FaceTools::FaceControl;
using FaceTools::FaceModel;


ActionFillHoles::ActionFillHoles( const QString& dn, const QIcon& ico, QProgressBar* pb)
    : FaceAction(dn, ico)
{
    if ( pb)
        setAsync(true, QTools::QProgressUpdater::create(pb));
}   // end ctor


bool ActionFillHoles::testReady( const FaceControl* fc)
{
    const FaceModel* fm = fc->data();
    fm->lockForRead();
    const bool rval = fm->info()->boundaries().size() > 1;
    fm->unlock();
    return rval;
}   // end testReady


bool ActionFillHoles::doAction( FaceControlSet& rset)
{
    assert(rset.size() == 1);
    FaceControl* fc = rset.first();
    FaceModel* fm = fc->data();

    fm->lockForWrite();

    RFeatures::ObjModelInfo::Ptr fmi = fm->info();
    RFeatures::ObjModelHoleFiller hfiller( fmi->model());

    bool filledHole = true;
    while ( filledHole)
    {
        filledHole = false;
        int nc = (int)fmi->components().size();
        for ( int c = 0; c < nc; ++c)
        {
            const IntSet* bidxs = fmi->components().cboundaries(c);
            if ( bidxs == nullptr) // Cannot fill holes on a component without boundaries
                continue;

            IntSet hbs = *bidxs;   // Copy out the boundary indices for the component
            hbs.erase( fmi->components().lboundary(c));    // Erase the longest boundary (which is likely the outer boundary)

            for ( int i : hbs)
            {
                const std::list<int>& blist = fmi->boundaries().boundary(i);
                IntSet newPolys;
                hfiller.fillHole( blist, &newPolys);
                std::cerr << " Filled hole (boundary " << i << ") on component " << (c+1) << " of " << nc
                          << " with " << newPolys.size() << " polygons" << std::endl;
                filledHole = true;
            }   // end for
        }   // end for

        if ( filledHole)
            fmi->reset();
    }   // end while

    fm->update(fmi);
    fm->unlock();
    return true;
}   // end doAction
