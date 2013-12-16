//=============================================================================
//
//   Exercise code for the lecture
//   "Advanced Computer Graphics"
//
//   Adapted from Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) 2013 LGG, epfl
//
//   DO NOT REDISTRIBUTE
//=============================================================================


//== INCLUDES =================================================================

#include "Mass_spring_system.h"
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <gl/glut.h>
#endif

//== IMPLEMENTATION ========================================================== 


void Mass_spring_system::clear()
{
    particles.clear();
    springs.clear();
    triangles.clear();
}


//-----------------------------------------------------------------------------


void Mass_spring_system::add_particle(vec3 position, vec3 velocity, float mass, bool locked)
{
    Particle p(position, velocity, mass, locked);
    p.index = particles.size();
    particles.push_back( p );
}

void Mass_spring_system::add_particle(vec2 position, vec2 velocity, float mass, bool locked)
{
    Particle p(vec3(position[0], position[1]), vec3(velocity[0], velocity[1]), mass, locked);
    particles.push_back( p );
}
//-----------------------------------------------------------------------------


void Mass_spring_system::add_spring(unsigned int i0, unsigned int i1, bool breakable)
{
    assert(i0 < particles.size());
    assert(i1 < particles.size());
    springs.push_back( Spring(particles[i0], particles[i1], breakable ));
}


//-----------------------------------------------------------------------------


void Mass_spring_system::add_triangle(unsigned int i0, unsigned int i1, unsigned int i2)
{
    assert(i0 < particles.size());
    assert(i1 < particles.size());
    assert(i2 < particles.size());
    triangles.push_back( Triangle(particles[i0], particles[i1], particles[i2]) );
}


//-----------------------------------------------------------------------------

void Mass_spring_system::compute_normals(int width, int height)
{
    for(int h = 1; h < height - 1; ++h)
    {
        for(int w = 1; w < width - 1; ++w) {
//                n1 - n2 - n3
//                n4 - cr - n5
//                n6 - n7 - n8

            int currentIndex = w + width * h;
            int neighbor5 = w + 1 + width * h;
            int neighbor2 = w + width * (h + 1);
            int neighbor4 = w - 1 + width * h;
            int neighbor7 = w + width * (h - 1);

            Particle& cr = particles[currentIndex];
            const Particle& p2 = particles[neighbor2];
            const Particle& p4 = particles[neighbor4];
            const Particle& p5 = particles[neighbor5];
            const Particle& p7 = particles[neighbor7];

            vec3 crp5 = p5.position - cr.position;
            vec3 crp2 = p2.position - cr.position;
            vec3 normal1 = crp5^crp2;

            vec3 crp4 = p4.position - cr.position;
            vec3 normal2 = crp4^crp2;

            vec3 crp7 = p7.position - cr.position;
            vec3 normal3 = crp7^crp4;

            vec3 normal4 = crp5^crp7;

            vec3 normal_av = normal1 + normal2 + normal3 + normal4;

            cr.normal = normalize(normal_av);
        }
    }


    for(int h = 1; h < height - 1; ++h) {
        //                n1
        //                cr - n2
        //                n3

        int currentIndex = width * h;
        int neighbor2 = 1 + width * h;
        int neighbor1 = width * (h + 1);
        int neighbor3 = width * (h - 1);

        Particle& cr = particles[currentIndex];
        const Particle& p2 = particles[neighbor2];
        const Particle& p1 = particles[neighbor1];
        const Particle& p3 = particles[neighbor3];

        vec3 crp1 = p1.position - cr.position;
        vec3 crp2 = p2.position - cr.position;
        vec3 normal1 = crp1^crp2;

        vec3 crp3 = p3.position - cr.position;
        vec3 normal2 = crp2^crp3;

        vec3 normal_av = normal1 + normal2;

        cr.normal = normalize(normal_av);


        //                n4
        //           n5 - cr2
        //                n6
        currentIndex = width-1 + width * h;
        int neighbor5 = width - 2 + width * h;
        int neighbor4 = width-1 + width * (h + 1);
        int neighbor6 = width-1 + width * (h - 1);

        Particle& cr2 = particles[currentIndex];
        const Particle& p5 = particles[neighbor5];
        const Particle& p4 = particles[neighbor4];
        const Particle& p6 = particles[neighbor6];

        vec3 crp4 = p4.position - cr2.position;
        vec3 crp5 = p5.position - cr2.position;
        normal1 = crp5^crp4;

        vec3 crp6 = p6.position - cr2.position;
        normal2 = crp6^crp5;

        normal_av = normal1 + normal2;

        cr2.normal = normalize(normal_av);
    }

    for(int w = 1; w < width - 1; ++w) {
        //           n1 - cr - n3
        //                n2


        int currentIndex = w;
        int neighbor1 = w - 1;
        int neighbor2 = w + width;
        int neighbor3 = w + 1;

        Particle& cr = particles[currentIndex];
        const Particle& p2 = particles[neighbor2];
        const Particle& p1 = particles[neighbor1];
        const Particle& p3 = particles[neighbor3];

        vec3 crp1 = p1.position - cr.position;
        vec3 crp2 = p2.position - cr.position;
        vec3 normal1 = crp1^crp2;

        vec3 crp3 = p3.position - cr.position;
        vec3 normal2 = crp2^crp3;

        vec3 normal_av = normal1 + normal2;

        cr.normal = normalize(normal_av);


        //                n5
        //           n4 - cr2 - n6
        currentIndex = w + width * (height-1);
        int neighbor5 = w + width * (height-2);
        int neighbor4 = w-1 + width * (height-1);
        int neighbor6 = w+1 + width * (height-1);

        Particle& cr2 = particles[currentIndex];
        const Particle& p5 = particles[neighbor5];
        const Particle& p4 = particles[neighbor4];
        const Particle& p6 = particles[neighbor6];

        vec3 crp4 = p4.position - cr2.position;
        vec3 crp5 = p5.position - cr2.position;
        normal1 = crp5^crp4;

        vec3 crp6 = p6.position - cr2.position;
        normal2 = crp6^crp5;

        normal_av = normal1 + normal2;

        cr2.normal = normalize(normal_av);
    }

    // the 4 border

    //
    //           cr - n1 .. n3 - cr2
    //           n2              n4
    //           .               .
    //           .               .
    //           n5              n7
    //          cr3 - n6 .. n8 - cr4

    int currentIndex = 0;
    int neighbor1 = 1;
    int neighbor2 = width;

    Particle& cr = particles[currentIndex];
    const Particle& p1 = particles[neighbor1];
    const Particle& p2 = particles[neighbor2];

    vec3 crp1 = p1.position - cr.position;
    vec3 crp2 = p2.position - cr.position;
    vec3 normal = crp2^crp1;

    cr.normal = normalize(normal);


    currentIndex = width - 1;
    int neighbor3 = width - 2;
    int neighbor4 = width - 1 + width;

    Particle& cr2 = particles[currentIndex];
    const Particle& p3 = particles[neighbor3];
    const Particle& p4 = particles[neighbor4];

    vec3 crp3 = p3.position - cr2.position;
    vec3 crp4 = p4.position - cr2.position;
    normal = crp3^crp4;

    cr2.normal = normalize(normal);


    currentIndex = width * (height - 1);
    int neighbor5 = width * (height - 2);
    int neighbor6 = 1 + width * (height - 1);

    Particle& cr3 = particles[currentIndex];
    const Particle& p5 = particles[neighbor5];
    const Particle& p6 = particles[neighbor6];

    vec3 crp5 = p5.position - cr3.position;
    vec3 crp6 = p6.position - cr3.position;
    normal = crp6^crp5;

    cr3.normal = normalize(normal);


    currentIndex = width - 1 + width * (height - 1);
    int neighbor7 = width - 1 + width * (height - 2);
    int neighbor8 = width - 2 + width * (height - 1);

    Particle& cr4 = particles[currentIndex];
    const Particle& p7 = particles[neighbor7];
    const Particle& p8 = particles[neighbor8];

    vec3 crp7 = p7.position - cr4.position;
    vec3 crp8 = p8.position - cr4.position;
    normal = crp7^crp8;

    cr4.normal = normalize(normal);

}

void Mass_spring_system::draw(float particle_radius, bool show_forces, int selected) const
{

    GLfloat specular_term[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat specular_exponant[] = { 100.0 };
    GLfloat light_position[] = { -1.0, 1.0, 1.0, 0.0 };
    GLfloat light_term[] = { 0.8, 0.8, 0.8 };
    GLfloat ambient_term[] = { 0.3, 0.3, 0.3, 1.0 };
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_term);
    glMaterialfv(GL_FRONT, GL_SHININESS, specular_exponant);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_term );
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_term );
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_term);

    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);


    for (unsigned int i=0; i<particles.size(); ++i)
    {
        const Particle& p = particles[i];
        GLfloat ambient_material_term[] = { 0.2, 0.2, 0.2, 1.0 };
        GLfloat diffuse_material_term[] = { 0.2, 0.2, 0.2, 1.0 };

        if (p.locked) {
            glColor3f(0.6, 0.0, 0.0);
            ambient_material_term[0] = 0.6;
            diffuse_material_term[0] = 0.6;
        } else {
            glColor3f(0.0, 0.6, 0.0);
            ambient_material_term[1] = 0.6;
            diffuse_material_term[1] = 0.6;
        }

        if (i == selected) {
            ambient_material_term[0] = 0.6;
            ambient_material_term[1] = 0.6;
            ambient_material_term[2] = 0.6;
            diffuse_material_term[0] = 0.6;
            diffuse_material_term[1] = 0.6;
            diffuse_material_term[2] = 0.6;
        }

        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_material_term);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_material_term);

        glEnable(GL_LIGHTING);
        glPushMatrix();
        glTranslatef( p.position.x, p.position.y, p.position.z);
        glPushName(i); //*********************************************************
        glutSolidSphere( particle_radius + 0.f, 20, 20 );
        glPopName();

        glPopMatrix();
        glDisable(GL_LIGHTING);

        if (show_forces)
        {
            glLineWidth(1.0);
            glBegin(GL_LINES);
            glVertex3f(p.position.x, p.position.y, p.position.z);
            glVertex3f(p.position[0] + 0.05 * p.force[0]
                    , p.position[1] + 0.05 * p.force[1]
                    , p.position[2] + 0.05 * p.force[2]);
            glEnd();
        }
    }
    glDisable(GL_COLOR_MATERIAL);

    // draw springs
    glDisable(GL_LIGHTING);
    glLineWidth(5.0);
    glColor3f(0,0,0);
    glBegin(GL_LINES);
    for (unsigned int i=0; i<springs.size(); ++i)
    {
        if (!springs[i].is_broken) {
            glVertex3fv( springs[i].particle0->position.data() );
            glVertex3fv( springs[i].particle1->position.data() );
        }
    }
    glEnd();

    // draw area constraints
    glDisable(GL_LIGHTING);
    glColor3f(0.8, 1.0, 0.8);
    glBegin(GL_TRIANGLES);
    for (unsigned int i=0; i<triangles.size(); ++i)
    {
        glVertex3fv( triangles[i].particle0->position.data() );
        glVertex3fv( triangles[i].particle1->position.data() );
        glVertex3fv( triangles[i].particle2->position.data() );
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void Mass_spring_system::draw_cloth(int width, int height) const
{
    //tissue values
        GLfloat specular_term[] = { 0.1, 0.1, 0.1, 1.0 };
        GLfloat specular_exponant[] = { 10.0 };
        GLfloat light_position[] = { -1.0, 1.0, 1.0, 0.0 };
        GLfloat light_term[] = { 0.8, 0.8, 0.8 };
        GLfloat ambient_term[] = { 0.3, 0.3, 0.3, 1.0 };
        glShadeModel(GL_SMOOTH);

        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_term);
        glMaterialfv(GL_FRONT, GL_SHININESS, specular_exponant);

        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_term );
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_term );
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_term);


        GLfloat ambient_material_term[] = { 0.1, 0.1, 0.6, /*0.6, 0.1, 0.1,*/ 1.0 };
        GLfloat diffuse_material_term[] = { 0.1, 0.1, 0.6, /*0.6, 0.1, 0.1,*/ 1.0 };

        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_material_term);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_material_term);

        glEnable(GL_LIGHT0);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glBegin( GL_TRIANGLES );

        unsigned int size = triangles_r.size();
        for(unsigned int i(0); i < size; ++i) {
            const Triangle_R& triangle = triangles_r[i];
            const Particle* p1 = triangle.particle0;
            const Particle* p2 = triangle.particle1;
            const Particle* p3 = triangle.particle2;

            if(!triangle.spring01->is_broken
                    && !triangle.spring02->is_broken
                    && !triangle.spring12->is_broken){

                glNormal3f(p1->normal.x, p1->normal.y, p1->normal.z);
                glVertex3f(p1->position.x, p1->position.y, p1->position.z);
                glNormal3f(p2->normal.x, p2->normal.y, p2->normal.z);
                glVertex3f(p2->position.x, p2->position.y, p2->position.z);
                glNormal3f(p3->normal.x, p3->normal.y, p3->normal.z);
                glVertex3f(p3->position.x, p3->position.y, p3->position.z);
            }
        }

        glEnd();
        glDisable(GL_LIGHTING);
        glDisable(GL_COLOR_MATERIAL);
}

//=============================================================================
