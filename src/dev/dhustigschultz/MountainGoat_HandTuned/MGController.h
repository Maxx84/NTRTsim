/*
 * Copyright © 2012, United States Government, as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All rights reserved.
 * 
 * The NASA Tensegrity Robotics Toolkit (NTRT) v1 platform is licensed
 * under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0.
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
*/

#ifndef MG_CONTROLLER_H
#define MG_CONTROLLER_H

/**
 * @file MGController.h
 * @brief Implementing a hand-tuned controller for a quadruped based roughly on the Flemons BigPuppy model.
 * @author Dawn Hustig-Schultz
 * @date Aug 2016
 * @version 1.0.0
 * $Id$
 */

//this library
#include "core/tgObserver.h"

#include "sensors/tgDataObserver.h"

#include "dev/dhustigschultz/MountainGoat_Anchored/MountainGoatAnchored.h"
#include "dev/dhustigschultz/BigPuppy_SpineOnly_Stats/BaseQuadModelLearning.h"

//Forward Declarations
class MountainGoatAnchored;
class BaseQuadModelLearning;

class MGController : public tgObserver<BaseQuadModelLearning>
{
public:

    //I really don't think I need a preferred length here, since there will be many different lengths, decided in onSetup()
    MGController(double timestep);

    ~MGController() {}

    virtual void onSetup(BaseQuadModelLearning& subject);
    
    virtual void onStep(BaseQuadModelLearning& subject, double dt);

    virtual void onTeardown(BaseQuadModelLearning& subject);

private:
      
    double m_totalTime;
    double dt;

    //May need functions for setting target lengths of different muscle groups...
    virtual void moveAllMotors(BaseQuadModelLearning& subject, double dt);

    //For setting muscle lengths:

    void setLeftThighTargetLength(BaseQuadModelLearning& subject, double dt);
    void setRearLeftLegToAbdomenTargetLength(BaseQuadModelLearning& subject, double dt);
    void setLeftHipTargetLength(BaseQuadModelLearning& subject, double dt);

    //For setting muscle lengths in spine
    void setSpineTargetLength(BaseQuadModelLearning& subject, double dt);

    //For the rest of the hip muscles (right hip and shoulder and left shoulder):
    void setOtherHipsTargetLength(BaseQuadModelLearning& subject, double dt);

    //For the rest of the legs:
    void setOtherLegsTargetLength(BaseQuadModelLearning& subject, double dt);
};
#endif //MG_CONTROLLER_H