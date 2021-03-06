#ifndef CLOTH_H
#define CLOTH_H

#include "Mass_spring_system.h"

using namespace Eigen;
class Cloth
{
public:
    Cloth(unsigned int grid_width, unsigned int grid_height, float cloth_y_position, float mass, Mass_spring_system* body_, int locked_particle_selection);
    ~Cloth();
    Cloth(){ M = NULL; }

    void integrateImplicit(const float &dt, const float &ks);
    void updateData();
    void implicit_initialization();

private:
    Mass_spring_system *body_;
    float cloth_particle_mass;

    SparseMatrix<float>* M;
    VectorXf x;
    VectorXf v;
    VectorXf f;

    bool implicit_integration_data_initialized;

    bool locked_line(int j);
    bool locked_border(int i, int j, int max1);
};

#endif // CLOTH_H
