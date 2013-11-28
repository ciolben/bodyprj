#ifndef CLOTH_H
#define CLOTH_H

#include "Mass_spring_system.h"

class Cloth
{
public:
    Cloth(unsigned int grid_width, unsigned int grid_height, float cloth_z_position, Mass_spring_system* body_);

private:
    Mass_spring_system *body_;
    float cloth_particle_mass;
};

#endif // CLOTH_H
