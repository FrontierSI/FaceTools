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

#include <Vis/OutlinesVisualisation.h>
#include <FaceModelViewer.h>
#include <FaceModel.h>
#include <rimg/Colour.h>
using FaceTools::Vis::OutlinesVisualisation;
using FaceTools::Vis::FV;


OutlinesVisualisation::OutlinesVisualisation() : _midx(-1) {}


bool OutlinesVisualisation::isVisible( const FV* fv) const
{
    return _views.count(fv) > 0
        && !_views.at(fv).empty()
        && _views.at(fv).at(0).isVisible();
}   // end isVisible


void OutlinesVisualisation::syncTransform( const FV* fv)
{
    for ( LoopView &lv : _views[fv])
        lv.pokeTransform( fv->transformMatrix());
}   // end syncTransform


namespace {
std::vector<const r3d::Vec3f*> getVertices( const std::list<int> &blist, const r3d::Mesh &mesh)
{
    std::vector<const r3d::Vec3f*> vtxs;
    for ( int vidx : blist)
        vtxs.push_back( &mesh.uvtx(vidx));
    return vtxs;
}   // end getVertices
}   // end namespace


/*
void OutlinesVisualisation::refresh( FV *fv)
{
    const FM *fm = fv->data();
    const r3d::Mesh &mesh = fm->mesh();
    const r3d::Manifolds &manifolds = fm->manifolds();
    size_t nloops = 0;
    for ( size_t i = 0; i < manifolds.count(); ++i)
        nloops += manifolds[int(i)].boundaries().count();

    std::vector<LoopView>& views = _views[fv];
    views.resize(nloops);

    rlib::Random rng( 5); // Randomize based on model

    double r, g, b;
    int k = 0;
    for ( int i = 0; i < int(manifolds.count()); ++i)
    {
        getColour( rng, r, g, b); // a different colour for each set of boundaries per manifold
        const r3d::Boundaries& bnds = manifolds[i].boundaries();
        for ( int j = 0; j < int(bnds.count()); ++j)
        {
            LoopView &lv = views[k++];
            lv.setColour( r, g, b, 0.99);
            lv.setLineWidth( 3.0);
            lv.update( getVertices( bnds.boundary(j), mesh));
        }   // end for
    }   // end for
}   // end refresh
*/

void OutlinesVisualisation::setManifoldIndex( int midx) { _midx = midx;}


void OutlinesVisualisation::refresh( FV *fv)
{
    const FM *fm = fv->data();
    const r3d::Mesh &mesh = fm->mesh();
    const r3d::Manifolds &manifolds = fm->manifolds();
    assert( _midx < int(manifolds.count()));
    const r3d::Boundaries& bnds = manifolds[_midx].boundaries();

    std::vector<LoopView>& views = _views[fv];
    // Note here that bnds.count could be zero since the manifold may not have any boudaries!
    views.resize(bnds.count());
    for ( int i = 0; i < int(bnds.count()); ++i)
    {
        LoopView &lv = views[i];
        lv.setLineWidth( 3.0);
        const rimg::Colour col = rimg::Colour::random( rimg::Colour( 0.2, 0.2, 0.2), rimg::Colour( 0.9, 0.9, 0.9));
        lv.setColour( col[0], col[1], col[2], 0.99);
        lv.update( getVertices( bnds.boundary(i), mesh));
    }   // end for
}   // end refresh


void OutlinesVisualisation::purge( const FV* fv) { _views.erase(fv);}


void OutlinesVisualisation::purgeAll() { _views.clear();}


void OutlinesVisualisation::setVisible( FV* fv, bool v)
{
    for ( LoopView &lv : _views[fv])
        lv.setVisible( v, fv->viewer());
}   // end setVisible
