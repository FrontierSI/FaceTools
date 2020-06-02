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

#include <Interactor/PropHandler.h>
#include <Action/ModelSelector.h>
using FaceTools::Interactor::PropHandler;

PropHandler::PropHandler()
{
    const SelectNotifier* sn = Action::ModelSelector::selector();
    connect( sn, &SelectNotifier::onEnterProp, this, &PropHandler::doEnterProp);
    connect( sn, &SelectNotifier::onLeaveProp, this, &PropHandler::doLeaveProp);
}   // end ctor
