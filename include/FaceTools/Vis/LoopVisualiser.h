/************************************************************************
 * Copyright (C) 2020 SIS Research Ltd & Richard Palmer
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

#ifndef FACE_TOOLS_VIS_LOOP_VISUALISER_H
#define FACE_TOOLS_VIS_LOOP_VISUALISER_H

#include "MetricVisualiser.h"
#include "LoopView.h"

namespace FaceTools { namespace Vis {

class FaceTools_EXPORT LoopVisualiser : public MetricVisualiser
{
public:
    const char* name() const override { return "LoopVisualiser (virtual)";}
    bool belongs( const vtkProp*, const FV*) const override;
    bool isVisible( const FV*) const override;
    void syncWithViewTransform( const FV*) override;

    void setHighlighted( const FV*, bool) override; // virtual from MetricVisualiser

protected:
    //void doApply( const FV*) override;
    //void doRefresh( const FV*) override;
    void doPurge( const FV*) override;
    void doSetVisible( const FV*, bool) override;

    std::unordered_map<const FV*, std::vector<LoopView*> > _views;
};  // end class

}}   // end namespaces

#endif
