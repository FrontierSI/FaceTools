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

#include <ActionSaveAsFaceModel.h>
#include <FaceControl.h>
#include <QFileDialog>
#include <QMessageBox>
#include <boost/filesystem.hpp>
#include <algorithm>
#include <cassert>
using FaceTools::Action::ActionSaveAsFaceModel;
using FaceTools::Action::ChangeEventSet;
using FaceTools::Action::FaceAction;
using FaceTools::FileIO::FaceModelManager;
using FaceTools::FaceControlSet;
using FaceTools::FaceModel;


ActionSaveAsFaceModel::ActionSaveAsFaceModel( const QString& dn, const QIcon& ico, FaceModelManager* fmm, QWidget *parent)
    : FaceAction( dn, ico),
      _fmm(fmm), _parent(parent)
{
    setAsync(true);
}   // end ctor


bool ActionSaveAsFaceModel::doBeforeAction( FaceControlSet& fset)
{
    assert(fset.size() == 1);
    FaceModel* fm = fset.first()->data();

    // Make default save filename have the preferred extension
    std::string filename = _fmm->filepath( fm);
    boost::filesystem::path outpath( filename);
    const QString parentDir = outpath.parent_path().string().c_str();
    const QString dsuff = _fmm->fileFormats().preferredExt();
    filename = outpath.replace_extension( dsuff.toStdString()).string();

    QFileDialog fileDialog( _parent);
    fileDialog.setWindowTitle( tr("Save model as..."));
    fileDialog.setFileMode( QFileDialog::AnyFile);
    fileDialog.setNameFilters( _fmm->fileFormats().createExportFilters().split(";;"));
    fileDialog.setDirectory( parentDir);    // Default save directory is last save location for model
    fileDialog.setDefaultSuffix( dsuff);
    fileDialog.selectFile( filename.c_str());
    fileDialog.setAcceptMode( QFileDialog::AcceptSave);
    //fileDialog.setOption( QFileDialog::DontUseNativeDialog);

    bool tryagain = true;
    while ( tryagain)
    {
        tryagain = false;
        QStringList fnames;
        if ( fileDialog.exec())
            fnames = fileDialog.selectedFiles();

        _filename = "";
        if ( !fnames.empty())
            _filename = fnames.first().toStdString();

        // Display a warning if the model has meta-data and the selected format is not XML based.
        if ( !_filename.empty() && fm->hasMetaData() && !_fmm->isPreferredFileFormat( _filename))
        {
            QString msg = tr("Landmark and other meta-data will not be saved using the chosen file format! Continue saving?");
            if ( QMessageBox::No == QMessageBox::warning( _parent, tr("Incomplete save!"), msg, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
            {
                _filename = "";
                tryagain = true;
            }   // end if
        }   // end if
    }   // end while

    return !_filename.empty();
}   // end doBeforeAction


bool ActionSaveAsFaceModel::doAction( FaceControlSet& fset)
{
    assert(fset.size() == 1);
    assert( !_filename.empty());
    return _fmm->write( fset.first()->data(), &_filename); // Save by specifying new filename
}   // end doAction


void ActionSaveAsFaceModel::doAfterAction( ChangeEventSet&, const FaceControlSet &fcs, bool success)
{
    if (success)
        emit onSavedAs( fcs.first());
    else
    {
        QString msg( ("\nUnable to save to \"" + _filename + "\"!").c_str());
        msg.append( ("\n" + _fmm->error()).c_str());
        QMessageBox::critical( _parent, tr("Unable to save file!"), tr(msg.toStdString().c_str()));
    }   // end else
    _filename = "";
}   // end doAfterAction
