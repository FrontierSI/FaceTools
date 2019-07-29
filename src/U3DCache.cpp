/************************************************************************
 * Copyright (C) 2019 Spatial Information Systems Research Limited
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

#include <U3DCache.h>
#include <FaceModel.h>
#include <U3DExporter.h>
#include <QTemporaryFile>
#include <cassert>
using FaceTools::U3DCache;
using FaceTools::FM;

QTemporaryDir U3DCache::_tmpdir;
QReadWriteLock U3DCache::_cacheLock;
std::unordered_set<const FM*> U3DCache::_cache;


U3DCache::Filepath U3DCache::u3dfilepath( const FM* fm)
{
    QString fname;
    _cacheLock.lockForRead();
    if ( _cache.count(fm) > 0)
        fname = makeFilePath(fm);
    return Filepath( new QString( fname), []( QString* s){ delete s; _cacheLock.unlock();});
}   // end lock


bool U3DCache::isAvailable()
{
    bool avail = false;
    if ( _cacheLock.tryLockForRead())
    {
        avail = RModelIO::U3DExporter::isAvailable();
        _cacheLock.unlock();
    }   // end if
    return avail;
}   // end isAvailable


void U3DCache::refresh( const FM* fm, bool med9)
{
    // Copy out model and ensure in standard position:
    fm->lockForRead();
    RFeatures::ObjModel::Ptr model = fm->model().deepCopy( true);    // Share materials
    const cv::Vec3f centre = fm->centre();
    const RFeatures::Orientation on = fm->orientation();
    fm->unlock();

    model->setTransformMatrix( on.asMatrix(centre).inv());
    model->fixTransformMatrix();
    RModelIO::U3DExporter xptr( true, med9);
    QTemporaryFile tfile( QDir::tempPath() + "/XXXXXX.u3d");
    if ( tfile.open())
    {
        std::cerr << QString( "Exporting U3D model to '%1'").arg( tfile.fileName()).toStdString() << std::endl;
        if ( xptr.save( *model, tfile.fileName().toStdString()))
        {
            // Copy U3D model exported to the temporary location to the cache location
            const QString cacheFileName = makeFilePath(fm);
            std::cerr << QString("Copying U3D model to cache location at '%1'").arg(cacheFileName).toStdString() << std::endl;
            _cacheLock.lockForWrite();
            QFile::copy( tfile.fileName(), cacheFileName);
            _cache.insert(fm);
            _cacheLock.unlock();
            std::cerr << "Finished updating U3D cache" << std::endl;
            QFile::remove( tfile.fileName());
        }   // end if
        else
            std::cerr << "Unable to save to U3D format!" << std::endl;
    }   // end if
    else
        std::cerr << "Couldn't open temporary file for exporting U3D model!" << std::endl;
}   // end refresh


void U3DCache::purge( const FM* fm)
{
    _cacheLock.lockForWrite();
    if ( _cache.count(fm) > 0)
    {
        QFile::remove( makeFilePath(fm));
        _cache.erase(fm);
    }   // end if
    _cacheLock.unlock();
}   // end purge


// private
QString U3DCache::makeFilePath( const FM* fm)
{
    QString fname;
    QTextStream os(&fname);
    os << "obj_" << hex << fm << ".u3d";
    fname = _tmpdir.filePath( fname);   // The filename to save to
    return fname;
}   // end makeFilePath
