#include "Cloth.h"
#include "Eigen/IterativeLinearSolvers"
#include <QDebug>

Cloth::Cloth(unsigned int grid_width, unsigned int grid_height, float cloth_y_position, float mass, Mass_spring_system *body_, int locked_particle_selection)
    :body_(body_), M(NULL)
    , cloth_particle_mass(mass), implicit_integration_data_initialized(false)
{
    bool breakable = true;

    double minX = -0.5;
    double minZ = -0.5;
    double maxX = 0.5;
    double maxZ = 0.5;

    double rangeX = maxX - minX;
    double rangeZ = maxZ - minZ;

    double stepX = rangeX / grid_width;
    double stepZ = rangeZ / grid_height;

    body_->clear();
    body_->clear();


    // add the particles
    double x = minX;
    double z = minZ;
    for(double zCounter(0); zCounter < grid_height; ++zCounter) {
        for(double xCounter(0); xCounter < grid_width; ++xCounter) {

            switch(locked_particle_selection) {
            case 0:
            {
                body_->add_particle( vec3(x, cloth_y_position, z),
                                     vec3(), cloth_particle_mass, false);
                break;
            }
            case 1:
            {
                body_->add_particle( vec3(x, cloth_y_position, z),
                                     vec3(), cloth_particle_mass, locked_line(zCounter));
                break;
            }
            case 2: {
                body_->add_particle( vec3(x, cloth_y_position, z),
                                     vec3(), cloth_particle_mass, locked_border(xCounter, zCounter, grid_width));
                break;
            }
            }

            x += stepX;
        }

        z += stepZ;
        x = minX;
    }

    //http://graphics.stanford.edu/courses/cs468-02-winter/Papers/Rigidcloth.pdf

    // add the spring
    // "structural spring"
    for(unsigned int i = 0; i < grid_height; ++i) {
        for(unsigned int j = 0; j < grid_width - 1; j++) {
            int particle_index = j + i * grid_width;
            int next_particle_index = particle_index + 1;

            body_->add_spring(particle_index, next_particle_index, breakable);
        }
    }

   for(unsigned int i = 0; i < grid_width; ++i) {
        for(unsigned int j = 0; j < grid_height - 1; j++) {
            int particle_index = i + j * grid_width;
            int next_particle_index = i + (j + 1) * grid_width;

            body_->add_spring(particle_index, next_particle_index, breakable);
        }
    }

    //diagonal
    // "shear spring"
    for(unsigned int i = 0; i < grid_height - 1; ++i) {
        for(unsigned int j = 0; j < grid_width - 1; ++j) {
            body_->add_spring(
                        i + j  * grid_height,
                        (i + 1) + (j + 1) * grid_height, breakable );
            body_->add_spring(
                        (i + 1) + j  * grid_height,
                        i + (j + 1) * grid_height, breakable );
        }
    }

    // "flexion spring"
    for(unsigned int i = 0; i < grid_height; ++i) {
        for(unsigned int j = 0; j < grid_width - 2; j++) {
            int particle_index = j + i * grid_width;
            int next_particle_index = particle_index + 2;

            body_->add_spring(particle_index, next_particle_index, breakable);
        }
    }

    for(unsigned int i = 0; i < grid_width; ++i) {
        for(unsigned int j = 0; j < grid_height - 2; j++) {
            int particle_index = i + j * grid_width;
            int next_particle_index = i + (j + 2) * grid_width;

            body_->add_spring(particle_index, next_particle_index, breakable);
        }
    }



    // create the triangle used for the futur rendering
    for(unsigned int h = 0; h < grid_height - 1; ++h) {
        for(unsigned int w = 0; w < grid_width - 1; w++) {

            int currentIndex = w + grid_width * h;
            int neighbor1 = w + 1 + grid_width * h;
            int neighbor2 = w + grid_width * (h + 1);
            int neighbor3 = w + 1 + grid_width * (h + 1);

            Particle& p1 = body_->particles[currentIndex];
            Particle& p2 = body_->particles[neighbor1];
            Particle& p3 = body_->particles[neighbor2];
            Particle& p4 = body_->particles[neighbor3];


            unsigned int size = body_->springs.size();

            //p1 - p2
            //   \  |
            //     p4
            Triangle_R triangle1(p1, p2, p4);

            //p1
            // | \
            //p3 - p4
            Triangle_R triangle2(p1, p4, p3);

            for(unsigned int i(0); i < size; ++i) {
                Spring& spring = body_->springs[i];
                Particle* sp0 = spring.particle0;
                Particle* sp1 = spring.particle1;

                if((sp0 == &p1 && sp1 == &p2) || (sp1 == &p1 && sp0 == &p2)) {
                   triangle1.spring01 = &spring;
                } else if((sp0 == &p1 && sp1 == &p4) || (sp1 == &p1 && sp0 == &p4)) {
                    triangle1.spring02 = &spring;
                    triangle2.spring01 = &spring;
                } else if((sp0 == &p2 && sp1 == &p4) || (sp1 == &p2 && sp0 == &p4)) {
                    triangle1.spring12 = &spring;
                } else if((sp0 == &p1 && sp1 == &p3) || (sp1 == &p1 && sp0 == &p3)) {
                    triangle2.spring02 = &spring;
                } else if((sp0 == &p4 && sp1 == &p3) || (sp1 == &p4 && sp0 == &p3)) {
                    triangle2.spring12 = &spring;
                }

            }

            body_->triangles_r.push_back(triangle1);
            body_->triangles_r.push_back(triangle2);
        }
    }
}

Cloth::~Cloth() {
    if(M != NULL) {
        delete M;
    }
}

bool Cloth::locked_line(int j) {
    return j == 0;
}

bool Cloth::locked_border(int i, int j, int max1) {
    return (j == 0) && (i == 0 || i == max1 - 1);
}

void Cloth::integrateImplicit(const float& dt, const float& ks) {
    int size = body_->particles.size();
    SparseMatrix<float> K(3 * size, 3 * size);
    Matrix3f I = Matrix3f::Identity();

    SparseMatrix<float> A(3 * size, 3 * size);
    VectorXf b = VectorXf(3 * size);

    for(unsigned int i(0); i<body_->springs.size(); ++i ){
        Spring& spring = body_->springs[i];
        Particle* p0 = spring.particle0;
        Particle* p1 = spring.particle1;
        vec3 dxf = p0->position - p1->position;
        int i_id = p1->index;
        int j_id = p0->index;
        Vector3f dx(dxf.data());

        double dist = dx.norm();
        double l = spring.rest_length / dist;

        Matrix3f Kii = ks *
                     (-I + (l * (I -
                                 ((dx * dx.transpose()) / (dist * dist)))));

        for (int k = 0; k < 3; ++k) {
            for (int l = 0; l < 3; ++l) {
                K.coeffRef(i_id * 3 + k,i_id * 3 + l) += Kii(k, l);
                K.coeffRef(j_id * 3 + k,j_id * 3 + l) += Kii(k, l);

                K.coeffRef(j_id * 3 + k,i_id * 3 + l) -= Kii(k, l);
                K.coeffRef(i_id * 3 + k,j_id * 3 + l) -= Kii(k, l);
            }
        }
    }

    //resolve the system
    A = (*M) - ((dt * dt) * K);
    b = ((*M) * v) + (dt * f);

    ConjugateGradient<SparseMatrix<float> > conjGrad;

    conjGrad.compute(A);
    VectorXf v_new = conjGrad.solve(b);

    for (int i(0); i < size; ++i) {
        Particle& p = body_->particles[i];
        int id = p.index;

        if (!p.locked) {
            p.position += dt * vec3(v_new[id * 3], v_new[id * 3 + 1], v_new[id * 3 + 2]);
            p.velocity = vec3(v_new[id * 3], v_new[id * 3 + 1], v_new[id * 3 + 2]);
        }
    }
}

void Cloth::updateData() {
    for (int i(0); i < body_->particles.size(); ++i) {
        Particle& p = body_->particles[i];
        vec3 force = p.force;
        int index = p.index * 3;
        f[index] = force.x;
        f[index+1] = force.y;
        f[index+2] = force.z;
		
		v[index] = p.velocity.x;
		v[index+1] = p.velocity.y;
		v[index+2] = p.velocity.z;
    }
}

void Cloth::implicit_initialization() {
    if(!implicit_integration_data_initialized) {
        implicit_integration_data_initialized = true;

        int nb_particules = body_->particles.size();
        M = new SparseMatrix<float>(3 * nb_particules, 3 * nb_particules);
        for (int i(0); i < 3 * nb_particules; ++i) {
            M->insert(i, i) = cloth_particle_mass;
        }

        this->x = VectorXf(3 * nb_particules);
        this->x = 0.f * this->x;
        for(uint i = 0; i < body_->particles.size(); ++i) {
            float* data = body_->particles[i].position.data();
            this->x[i*3] = data[0];
            this->x[i*3 + 1] = data[1];
            this->x[i*3 + 2] = data[2];
        }

        this->v = VectorXf(3 * nb_particules);
        this->v = 0.0 * v;
        this->f = VectorXf(3 * nb_particules);
        this->f = 0.0 * f;
    }
}
