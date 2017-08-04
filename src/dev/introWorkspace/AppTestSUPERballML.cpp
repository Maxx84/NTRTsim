/*
 * Copyright © 2017, United States Government, as represented by the
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

/**
 * @file AppTestSUPERballML.cpp
 * @brief Contains the definition function main() for  
 * AppTestSUPERballML which builds a YAML SUPERball 2.0.
 * @author Marc Leroy
 * $Id$
 */

// This application
#include "yamlbuilder/TensegrityModel.h"
//#include "LengthControllerML.h"
#include "HopfControllerML.h"
// This library
#include "core/terrain/tgBoxGround.h"
#include "core/tgModel.h"
#include "core/tgSimulation.h"
#include "core/tgSimViewGraphics.h"
#include "core/tgWorld.h"
// Bullet Physics
#include "LinearMath/btVector3.h"
// The C++ Standard Library
#include <iostream>
#include <string>
#include <vector>

/*
#include <math.h>
#include <boost/array.hpp>
#include <boost/numeric/odeint.hpp>*/
#include <fstream>

//using namespace boost::numeric::odeint;

/*const double sigma = 10.0;
const double R = 28.0;
const double b = 8.0/3.0;*/
/*const double mu=0.7;
const double omega=5.0;
const char *csvpath="/home/tensegribuntu/projects/tg_shared/hopf_results.csv";*/

//typedef boost::array<double,3> state_type3;
//typedef boost::array<double,2> state_type2;

/*void lorenz(const state_type3 &x, state_type3 &dxdt, double t)
{
    dxdt[0] = sigma*(x[1]-x[0]);
    dxdt[1] = R*x[0]-x[1]-x[0]*x[2];
    dxdt[2] = -b*x[2]+x[0]*x[1];
}

void write_lorenz(const state_type3 &x, const double t)
{
    std::cout << t << '\t' << x[0] << '\t' << x[1] << '\t' << x[2] << std::endl;
}*/

/*void hopf(const state_type2 &x, state_type2 &dxdt, double t)
{
    double r=0;

    r=sqrt(x[0]*x[0]+x[1]*x[1]);
    dxdt[0]=(mu-r*r)*x[0]-omega*x[1];
    dxdt[1]=(mu-r*r)*x[1]+omega*x[0];
}*/

/*void write_hopf(const state_type2 &x, const double t)
{
    std::cout << t << '\t' << x[0] << '\t' << x[1] << std::endl;
    ofstream myfile(csvpath);
    myfile.open(csvpath,std::ios_base::app);
    myfile << t << "," << x[0] << "," << x[1] << std::endl;
    myfile.close();
}*/


std::vector<std::string> selectControlledStrings(std::vector<std::string> tagsToControl);


/**
 * The entry point.
 * @param[in] argc the number of command-line arguments
 * @param[in] argv argv[0] is the executable name
 * @param[in] argv argv[1] is the path of the YAML encoded structure
 * @return 0
 */
int main(int argc, char** argv)
{
    // For this YAML parser app, need to check that an argument path was
    // passed in.
    if (argv[1] == NULL)
    {
      throw std::invalid_argument("No arguments passed in to the application. You need to specify which YAML file you wouldd like to build.");
    }
  
    // create the ground and world. Specify ground rotation in radians
    const double yaw = 0.0;
    const double pitch = 0.0; //0.0;
    const double roll = 0.0; //0.0;
    const tgBoxGround::Config groundConfig(btVector3(yaw, pitch, roll));
    // the world will delete this
    tgBoxGround* ground = new tgBoxGround(groundConfig);

    const tgWorld::Config config(98.1); // gravity, dm/sec^2
    tgWorld world(config, ground);

    // create the view
    //const double timestep_physics = 0.0001; // seconds
    const double timestep_physics = 0.001;
    const double timestep_graphics = 1.f/60.f; // seconds
    tgSimViewGraphics view(world, timestep_physics, timestep_graphics);

    // create the simulation
    tgSimulation simulation(view);

    // create the models with their controllers and add the models to the simulation
    // This constructor for TensegrityModel takes the 'debugging' flag as the
    // second argument.
    TensegrityModel* const myModel = new TensegrityModel(argv[1],false);

    // Attach a controller to the model, if desired.
    // This is a controller that interacts with a generic TensegrityModel as
    // built by the TensegrityModel file.>

    // Parameters for the LengthControllerYAML are specified in that .h file,
    // repeated here:
    double startTime = 2.0;
    double minLength = 0.2; //0.7;
    double rate = 10.25; //0.25
    std::vector<std::string> tagsToControl;

    tagsToControl = selectControlledStrings(tagsToControl);

    // Create the controller
    HopfControllerML* const myController = new HopfControllerML(startTime, minLength, rate, tagsToControl);
    
    // Attach the controller to the model
    myModel->attach(myController);

    // Add the model to the world
    simulation.addModel(myModel);

    /*state_type3 x = {10.0,1.0,1.0};
    integrate(lorenz,x,0.0,15.0,0.1,write_lorenz);*/ 

    /*ofstream myfile(csvpath);
    myfile.open(csvpath);
    myfile.close();
    state_type2 x_h2d = {0.25,0.25};
    integrate(hopf,x_h2d,0.0,50.0,0.1,write_hopf);*/
    
    /*std::ofstream myfile("/home/tensegribuntu/projects/tg_shared/test_file.csv");
    //myfile.open("/home/tensegribuntu/projects/tg_shared/test_file.csv",std::ios_base::app);
    myfile << 123 << "," << 456 << "," << 789 << std::endl;
    //myfile.flush();
    myfile.close();
    myfile.open("/home/tensegribuntu/projects/tg_shared/test_file.csv",std::ios_base::app);
    myfile << 12 << "," << 34 << "," << 56 << std::endl;
    //myfile.flush();
    myfile.close();*/

    simulation.run();

    // teardown is handled by delete
    return 0;
}


/**
 * Defines which strings will be controlled
 * (/!\ order sensitive as will be read by the controller in array format)
 * @param[in] tagsToControl empty string vector
 * @return tagsToControl string vector containing strings to be controlled
 */
std::vector<std::string> selectControlledStrings(std::vector<std::string> tagsToControl)
{
    // See the threeBarModel.ML file to see where "vertical_string" is used.
    /*tagsToControl.push_back("SUPERball_string16");
    tagsToControl.push_back("SUPERball_string24");  
    tagsToControl.push_back("SUPERball_string3");  
    tagsToControl.push_back("SUPERball_string4");
    tagsToControl.push_back("SUPERball_string11");  
    tagsToControl.push_back("SUPERball_string12");  
    //tagsToControl.push_back("SUPERball_string12");
    //tagsToControl.push_back("SUPERball_string12");
    tagsToControl.push_back("SUPERball_string1");  
    tagsToControl.push_back("SUPERball_string2");  
    tagsToControl.push_back("SUPERball_string5");  
    tagsToControl.push_back("SUPERball_string6");  
    tagsToControl.push_back("SUPERball_string7");  
    tagsToControl.push_back("SUPERball_string8");  
    tagsToControl.push_back("SUPERball_string9");  
    tagsToControl.push_back("SUPERball_string10");
    tagsToControl.push_back("SUPERball_string13");
    tagsToControl.push_back("SUPERball_string14");
    tagsToControl.push_back("SUPERball_string15");
    tagsToControl.push_back("SUPERball_string17");
    tagsToControl.push_back("SUPERball_string18");
    tagsToControl.push_back("SUPERball_string19");
    tagsToControl.push_back("SUPERball_string20");
    tagsToControl.push_back("SUPERball_string21");
    tagsToControl.push_back("SUPERball_string22");
    tagsToControl.push_back("SUPERball_string23");*/

/*    tagsToControl.push_back("SUPERball_string5");
    tagsToControl.push_back("SUPERball_string13");
    tagsToControl.push_back("SUPERball_string21");
    tagsToControl.push_back("SUPERball_string22");
    
    tagsToControl.push_back("SUPERball_string2");
    tagsToControl.push_back("SUPERball_string10");
    tagsToControl.push_back("SUPERball_string19");
    tagsToControl.push_back("SUPERball_string20");

    tagsToControl.push_back("SUPERball_string3");
    tagsToControl.push_back("SUPERball_string7");
    tagsToControl.push_back("SUPERball_string17");
    tagsToControl.push_back("SUPERball_string21");
    

    tagsToControl.push_back("SUPERball_string6");
    tagsToControl.push_back("SUPERball_string14");
    tagsToControl.push_back("SUPERball_string23");
    tagsToControl.push_back("SUPERball_string24");  
    
    tagsToControl.push_back("SUPERball_string1");
    tagsToControl.push_back("SUPERball_string9");
    tagsToControl.push_back("SUPERball_string17");
    tagsToControl.push_back("SUPERball_string18");
    
    tagsToControl.push_back("SUPERball_string12");
    tagsToControl.push_back("SUPERball_string16");
    tagsToControl.push_back("SUPERball_string20");
    tagsToControl.push_back("SUPERball_string24");*/

    /*tagsToControl.push_back("SUPERball_string1");
    tagsToControl.push_back("SUPERball_string3");
    tagsToControl.push_back("SUPERball_string5");
    tagsToControl.push_back("SUPERball_string7");
    tagsToControl.push_back("SUPERball_string9");
    tagsToControl.push_back("SUPERball_string11");
    tagsToControl.push_back("SUPERball_string13");
    tagsToControl.push_back("SUPERball_string15");
    tagsToControl.push_back("SUPERball_string17");
    tagsToControl.push_back("SUPERball_string19");
    tagsToControl.push_back("SUPERball_string21");
    tagsToControl.push_back("SUPERball_string23");
    
    tagsToControl.push_back("SUPERball_string2");
    tagsToControl.push_back("SUPERball_string4");
    tagsToControl.push_back("SUPERball_string6");
    tagsToControl.push_back("SUPERball_string8");
    tagsToControl.push_back("SUPERball_string10");
    tagsToControl.push_back("SUPERball_string12");
    tagsToControl.push_back("SUPERball_string14");
    tagsToControl.push_back("SUPERball_string16");
    tagsToControl.push_back("SUPERball_string18");
    tagsToControl.push_back("SUPERball_string20");
    tagsToControl.push_back("SUPERball_string22");
    tagsToControl.push_back("SUPERball_string24");*/
    

    tagsToControl.push_back("SUPERball_string1");
    tagsToControl.push_back("SUPERball_string3");
    tagsToControl.push_back("SUPERball_string17");
    tagsToControl.push_back("SUPERball_string10");
    tagsToControl.push_back("SUPERball_string12");
    tagsToControl.push_back("SUPERball_string20");
    tagsToControl.push_back("SUPERball_string13");
    tagsToControl.push_back("SUPERball_string15");
    tagsToControl.push_back("SUPERball_string22");
    tagsToControl.push_back("SUPERball_string6");
    tagsToControl.push_back("SUPERball_string8");
    tagsToControl.push_back("SUPERball_string23");
    
    tagsToControl.push_back("SUPERball_string9");
    tagsToControl.push_back("SUPERball_string11");
    tagsToControl.push_back("SUPERball_string18");
    tagsToControl.push_back("SUPERball_string2");
    tagsToControl.push_back("SUPERball_string4");
    tagsToControl.push_back("SUPERball_string19");
    tagsToControl.push_back("SUPERball_string5");
    tagsToControl.push_back("SUPERball_string7");
    tagsToControl.push_back("SUPERball_string21");
    tagsToControl.push_back("SUPERball_string14");
    tagsToControl.push_back("SUPERball_string16");
    tagsToControl.push_back("SUPERball_string24");

    //tagsToControl.push_back("SUPERball_string5");
    //tagsToControl.push_back("SUPERball_string14");
    /*tagsToControl.push_back("SUPERball_string1");
    tagsToControl.push_back("SUPERball_string4");
    tagsToControl.push_back("SUPERball_string10");
    tagsToControl.push_back("SUPERball_string11");

    tagsToControl.push_back("SUPERball_string17");
    tagsToControl.push_back("SUPERball_string2");
    tagsToControl.push_back("SUPERball_string20");
    tagsToControl.push_back("SUPERball_string9");

    tagsToControl.push_back("SUPERball_string3");
    tagsToControl.push_back("SUPERball_string19");
    tagsToControl.push_back("SUPERball_string12");
    tagsToControl.push_back("SUPERball_string18");*/
    return tagsToControl;
}