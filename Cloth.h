#ifndef CLOTH_H
#define CLOTH_H

#include "Mass_spring_system.h"

using namespace Eigen;
class Cloth
{
public:
    Cloth(unsigned int grid_width, unsigned int grid_height, float cloth_y_position, Mass_spring_system* body_);

    Cloth(){}

    void integrateImplicit(const float &dt, const float &ks);
    void updateForces();

private:
    Mass_spring_system *body_;
    float cloth_particle_mass;

    MatrixXf M;
    VectorXf x;
    VectorXf v;
    VectorXf f;
};

#endif // CLOTH_H
