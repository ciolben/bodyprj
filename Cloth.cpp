#include "Cloth.h"

Cloth::Cloth(unsigned int grid_width, unsigned int grid_height, float cloth_y_position, Mass_spring_system *body_)
    :body_(body_)
    , cloth_particle_mass(0.001f)
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

    // add the particles
    double x = minX;
    double z = minZ;
    for(double zCounter(0); zCounter < grid_height; ++zCounter) {
        for(double xCounter(0); xCounter < grid_width; ++xCounter) {
            body_->add_particle( vec3(x, cloth_y_position, z),
                                vec3(), cloth_particle_mass, /*false*/ zCounter == 0 );
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
    for(unsigned int h = 1; h < grid_height - 2; ++h) {
        for(unsigned int w = 1; w < grid_width - 2; w++) {

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
            //      |
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
