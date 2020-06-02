/************************************************************************
 * Copyright (C) 2019 SIS Research Ltd & Richard Palmer
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

#ifndef FACE_TOOLS_FILE_IO_FACE_MODEL_3DS_FILE_HANDLER_H
#define FACE_TOOLS_FILE_IO_FACE_MODEL_3DS_FILE_HANDLER_H

#include "FaceModelFileHandler.h"
#include <r3dio/AssetImporter.h>
#include <r3dio/AssetExporter.h>

namespace FaceTools { namespace FileIO {

class FaceTools_EXPORT FaceModel3DSFileHandler : public FaceModelFileHandler
{
public:
    FaceModel3DSFileHandler();
    QString getFileDescription() const override;
    const std::unordered_set<QString>& getFileExtensions() const override { return _exts;}

    bool canRead() const override { return true;}

    // Disallow writing because 3DS inherently limited to max of 65536 triangles.
    bool canWrite() const override { return false;}
    bool canWriteTextures() const override { return true;}

    QString error() const override { return _err;}

    FM* read( const QString& filepath) override;
    bool write( const FM*, const QString&) override;

private:
    r3dio::AssetImporter _importer;
    r3dio::AssetExporter _exporter;
    std::unordered_set<QString> _exts;
    QString _err;
};  // end class

}}   // end namespace

#endif
