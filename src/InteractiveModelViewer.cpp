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

#include <InteractiveModelViewer.h>
#include <ModelViewerVTKInterface.h>
#include <cassert>
using FaceTools::InteractiveModelViewer;

// private
void InteractiveModelViewer::init()
{
    vtkSmartPointer<FaceTools::ModelViewerVTKInterface> vtkInterface( FaceTools::ModelViewerVTKInterface::New());
    _qviewer->setInteractor(vtkInterface);
    _qinterface = vtkInterface->getQtInterface();
    _qviewer->setContextMenuPolicy(Qt::CustomContextMenu);
    connect( _qviewer, &QTools::VtkActorViewer::customContextMenuRequested, this, &InteractiveModelViewer::requestContextMenu);
}   // end init

// public
InteractiveModelViewer::InteractiveModelViewer( QTools::VtkActorViewer* qviewer)
    : FaceTools::ModelViewer(qviewer), _qinterface(NULL)
{
    init();
}   // end ctor


// public
void InteractiveModelViewer::addToLayout( QLayout *layout)
{
    layout->addWidget(_qviewer);
}   // end addToLayout


// public
void InteractiveModelViewer::removeFromLayout( QLayout *layout)
{
    layout->removeWidget(_qviewer);
}   // end removeFromLayout


// public
void InteractiveModelViewer::connectInterface( FaceTools::InteractionInterface* iint) const
{
    assert(iint);
    iint->setInterface(_qinterface);
}   // end connectInterface


// public
const vtkProp* InteractiveModelViewer::getPointedAt() const
{
    return FaceTools::ModelViewer::getPointedAt( getMouseCoords());
}   // end getPointedAt


// public
void InteractiveModelViewer::setCursor( QCursor cursor)
{
    FaceTools::ModelViewer::setCursor(cursor);
}   // end setCursor


// public
void InteractiveModelViewer::setKeyPressHandler( QTools::KeyPressHandler* kph)
{
    _qviewer->setKeyPressHandler(kph);  // Pass through to QVTKWidget
}   // end setKeyPressHandler
