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

#ifndef FACE_TOOLS_VIS_SURFACE_METRICS_MAPPER_H
#define FACE_TOOLS_VIS_SURFACE_METRICS_MAPPER_H

#include <FaceTypes.h>
#include <FaceModel.h>
#include <FaceViewSet.h>
#include <SurfaceMapper.h>  // RVTK
#include <ScalarColourRangeMapper.h>    // QTools

namespace FaceTools { namespace Vis {

class FaceTools_EXPORT SurfaceMetricsMapper
{
public:
    using Ptr = std::shared_ptr<SurfaceMetricsMapper>;

    // label:   name of the data range mapped to actors,
    // mapp:    true if this mapper maps polygons; false if maps vertices
    // dims:    the metric dimension (1 for scalar, >1 for vector)
    // minv:    the minimum metric magnitude allowed
    // maxv:    the maximum metric magnitute allowed
    SurfaceMetricsMapper( const std::string& label, bool mapp, size_t dims, float minv, float maxv);
    virtual ~SurfaceMetricsMapper();

    const std::string& label() const { return _smapper->label();}

    // Returns true iff this is a mapping of scalars to colours.
    bool isScalarMapping() const;

    float minRange() const { return _minr;} // Min range initially same as min visible
    float maxRange() const { return _maxr;} // Max range initially same as max visible
    float minVisible() const { return _minv;}
    float maxVisible() const { return _maxv;}

    void setVisibleRange( float,float);    // Scalar visible values or vector magnitudes.

    // If this is a scalar colour mapping, return the number of colours used to map the surface of actors.
    // Only returns > 1 if this is a scalar mapping. Returns 1 if a vector mapping. Never returns less than 1.
    size_t numColours() const;
    void setNumColours( size_t);

    // Colour functions only do anything if this is a scalar visualisation.
    QColor minColour() const;
    QColor maxColour() const;
    void setMinColour( const QColor&);
    void setMaxColour( const QColor&);

    // Returns lookup table if scalar mapping otherwise null.
    vtkLookupTable* scalarLookupTable();

    void rebuild(); // Causes actor surfaces to be remapped

    // Returns true to indicate mapping success.
    bool mapMetrics( FV*);

    // Add/remove FaceViews that are currently receiving surface updates from this mapper.
    void remove( FV*);
    void add( FV*);

protected:
    // Must be overridden to return true if visualisation mapping available for model.
    // Note that at the very least, the model must have sequential IDs.
    virtual bool isAvailable( const FM* fm) const { return fm && fm->model().hasSequentialIds();}

    // Purge this object of old surface mapping data triggered by the given event.
    // Returns true if purged, false if not.
    virtual bool purge( const FM*, Action::Event) { return true;}

    virtual float metric( int objid, size_t k=0) = 0;

    // Must be overridden to return true in order for visualisation mapping to proceed.
    virtual bool init( const FM*) { return false;}

    // Called when metric mapping complete and before mapMetrics returns true.
    virtual void done( const FM*) {}

    friend class SurfaceVisualisation;

private:
    RVTK::SurfaceMapper::CPtr _smapper;
    const float _minr;
    const float _maxr;
    float _minv, _maxv;
    QTools::ScalarColourRangeMapper _cmapper;
    FVS _fvs;
    void _refreshActorSurface( vtkActor*);
};  // end class

}}   // end namespaces

#endif
