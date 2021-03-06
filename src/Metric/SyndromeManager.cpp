/************************************************************************
 * Copyright (C) 2021 SIS Research Ltd & Richard Palmer
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

#include <Metric/SyndromeManager.h>
#include <MiscFunctions.h>
#include <rlib/FileIO.h>
#include <iostream>
#include <cassert>
using FaceTools::Metric::SyndromeManager;
using FaceTools::Metric::Syndrome;

// Static definitions
IntSet SyndromeManager::_ids;
QStringList SyndromeManager::_codes;
QStringList SyndromeManager::_names;
std::unordered_map<int, Syndrome> SyndromeManager::_syns;
std::unordered_map<int, IntSet> SyndromeManager::_gsyns;
std::unordered_map<int, IntSet> SyndromeManager::_hsyns;
std::unordered_map<QString, int> SyndromeManager::_nsyns;

namespace {
static const IntSet EMPTY_INT_SET;
}   // end namespace


const IntSet& SyndromeManager::hpoSyndromes( int hid)
{
    if ( _hsyns.count(hid) == 0)
        return EMPTY_INT_SET;
    return _hsyns.at(hid);
}   // end hpoSyndromes


const IntSet& SyndromeManager::geneSyndromes( int gid)
{
    if ( _gsyns.count(gid) == 0)
        return EMPTY_INT_SET;
    return _gsyns.at(gid);
}   // end geneSyndromes


Syndrome *SyndromeManager::syndrome( int id) { return _syns.count(id) > 0 ? &_syns.at(id) : nullptr;}

Syndrome *SyndromeManager::syndrome( const QString& nm)
{
    return _nsyns.count(nm) > 0 ? syndrome( _nsyns.at(nm)) : nullptr;
}   // end syndrome


int SyndromeManager::load( const QString& fname)
{
    QTemporaryFile* tmpfile = writeToTempFile(fname);
    if (!tmpfile)
        return 0;

    _ids.clear();
    _codes.clear();
    _names.clear();
    _syns.clear();
    _gsyns.clear();
    _hsyns.clear();
    _nsyns.clear();

    const QString fpath = tmpfile->fileName();
    std::vector<rlib::StringVec> lines;
    int nrecs = rlib::readFlatFile( fpath.toStdString(), lines, IBAR, true/*skip # symbols as well as blank lines*/);
    delete tmpfile;

    if ( nrecs <= 0)
        return nrecs;

    int lrecs = 0;
    bool ok = false;
    for ( size_t i = 0; i < size_t(nrecs); ++i)
    {
        const rlib::StringVec& recs = lines[i];
        int id = QString(recs[0].c_str()).toInt(&ok);
        if ( !ok || id < 0 || _syns.count(id) > 0)
        {
            std::cerr << "[WARN] FaceTools::Metric::SyndromeManager::load: Invalid syndrome id read in!" << std::endl;
            continue;
        }   // end else

        Syndrome& syn = _syns[id];
        syn.setId( id);

        syn.setCode( recs[1].c_str());
        syn.setName( recs[2].c_str());

        // Add associated genes
        QStringList sgids = QString( recs[3].c_str()).split(SC, Qt::SkipEmptyParts);
        for ( const QString& sgid : sgids)
        {
            int gid = sgid.toInt(&ok);
            if ( ok)
            {
                syn.addGene(gid);
                _gsyns[gid].insert(id);
            }   // end if
        }   // end for

        // Add associated HPO terms
        QStringList shids = QString( recs[4].c_str()).split(SC, Qt::SkipEmptyParts);
        for ( const QString& shid : shids)
        {
            int hid = shid.toInt(&ok);
            if ( ok)
            {
                syn.addHPO(hid);
                _hsyns[hid].insert(id);
            }   // end if
        }   // end for

        _ids.insert(id);
        _codes.append(syn.code());
        _names.append(syn.name());
        _nsyns[syn.name()] = id;
        lrecs++;
    }   // end for

    _codes.sort();
    _names.sort();
    return lrecs;
}   // end load


bool SyndromeManager::save( const QString& fname)
{
    QFile file( fname);
    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    for ( int id : ids())
        out << *syndrome(id) << Qt::endl;
    out << Qt::endl;   // Add EoF newline

    file.close();
    return true;
}   // end save
