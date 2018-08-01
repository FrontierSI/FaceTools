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

#include <ActionCloseFaceModels.h>
#include <QMessageBox>
#include <boost/filesystem.hpp>
#include <algorithm>
using FaceTools::Action::FaceAction;
using FaceTools::Action::ActionCloseFaceModels;
using FaceTools::FileIO::FaceModelManager;
using FaceTools::FaceControlSet;
using FaceTools::FaceModel;


ActionCloseFaceModels::ActionCloseFaceModels( const QString& dname, const QIcon& ico, const QKeySequence& keys,
                                              FaceModelManager* fmm, QWidget* pw)
    : FaceAction( dname, ico, keys), _fmm(fmm), _parent(pw)
{
}   // end ctor


bool ActionCloseFaceModels::doBeforeAction( FaceControlSet& fcs)
{
    FaceModelSet fms = fcs.models();
    for ( FaceModel* fm : fms)
    {
        bool inPreferredFormat = _fmm->hasPreferredFileFormat(fm);

        // If FaceModel hasn't been saved and the user doesn't want to close it (after prompting), remove from action set.
        bool doclose = false;
        if ( fm->isSaved() && ( inPreferredFormat || !fm->hasMetaData()))
            doclose = true;
        else
        {
            const std::string fname = boost::filesystem::path( _fmm->filepath(fm)).filename().string();
            QString msg = tr( ("Model \"" + fname + "\" has unsaved changes! Close anyway?").c_str());
            if ( !inPreferredFormat)
                msg = tr("Model not saved in preferred file format (.3df); close anyway?");

            if ( QMessageBox::Yes == QMessageBox::question( _parent, tr("Unsaved changes!"), msg,
                                                QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
            {
                doclose = true;
            }   // end if
        }   // end if

        if ( !doclose)
            fcs.erase(fm);
    }   // end for
    return !fcs.empty();
}   // end doBeforeAction