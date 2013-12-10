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

#include "Mass_spring_viewer.h"
#define M_PI       3.14159265358979323846
#include <sstream>

#include "Cloth.h"

//== IMPLEMENTATION ==========================================================


Mass_spring_viewer::Mass_spring_viewer(const qglviewer::Camera *camera)
    : camera(camera)
    , m_objects(std::vector<Object3D*>())
{
    integration_         = Verlet;
    collisions_          = Impulse_based;
    external_force_      = Gravitation;
    animate_             = false;
    area_forces_         = true;
    show_forces_         = false;

    particle_radius_     = 0.03;

    particle_mass_       = 0.1;
    damping_             = 0.1;
    collision_stiffness_ = 1000.0;
    spring_stiffness_    = 1000.0;
    spring_damping_      = 1.0;
    area_stiffness_      = 100000.0;

    mouse_spring_.active = false;

    animate_     = true;

    selected_ = -1;

    camera_gravitation = false;

    cloth_simulation = false;

    cloth_width = 30;
    cloth_height = 30;

    cloth_show_particles = false;
}

Mass_spring_viewer::~Mass_spring_viewer()
{
    //delete object3ds
    for(unsigned i = 0; i < m_objects.size(); ++i) {
        delete m_objects[i];
    }
}

//-----------------------------------------------------------------------------

void Mass_spring_viewer::printInfo(const QString& text) {
    std::cout << "Info : " << text.toStdString() << std::endl;
    emit newInfo(text);
}

void Mass_spring_viewer::drawWithNames()
{
    body_.draw(particle_radius_, show_forces_);
}

void Mass_spring_viewer::setSelected(int selected)
{
    m_oldDepth = 0.9f;
    selected_ = selected;
    std::cerr << selected << std::endl;
}

bool Mass_spring_viewer::moveSelectedParticule(const vec2 &mousePos)
{
    if (selected_ == -1) { return false; }
    Particle& p = body_.particles.at(selected_);

    QPoint pointp(mousePos.data()[0], mousePos.data()[1]);
    bool found;

    float depth;
    glReadPixels(pointp.x(), camera->screenHeight()-1-pointp.y(), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    found = depth < 1.0;
    if (found) {
        //we fix the depth (smooth transitions)
        m_oldDepth = depth;
    }

    qglviewer::Vec point(pointp.x(), pointp.y(), m_oldDepth);
    point = camera->unprojectedCoordinatesOf(point);

    p.position = vec3(point[0], point[1], p.position.z);

    //other way to do it (but there is a jump when clicked)
//    QPoint pixel(mousePos.data()[0], mousePos.data()[1]);
//    qglviewer::Vec v( ((2.0 * pixel.x() / camera->screenWidth()) - 1.0) /** tan(camera->fieldOfView()/2.0) * camera->aspectRatio()*/,
//       ((2.0 * (camera->screenHeight()-pixel.y()) / camera->screenHeight()) - 1.0) /** tan(camera->fieldOfView()/2.0)*/,
//       -1.0 );
//    vec3 projPos(v[0], v[1], p.position.z);
//    p.position = projPos;

    return true;
}

void Mass_spring_viewer::addObject3D(Object3D *object3d)
{
    if(object3d != NULL) {
        m_objects.push_back(object3d);
    }
}

void Mass_spring_viewer::removeObject3D(Object3D *object3d)
{
    if(object3d != NULL) {
        for(std::vector<Object3D*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it) {
            if (*it == object3d) {
                m_objects.erase(it);
                delete object3d;
                break;
            }
        }
    }
}

void Mass_spring_viewer::removeAllObject3D()
{
    for(std::vector<Object3D*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it) {
        if (*it != NULL) {
            delete *it;
        }
    }
    m_objects.clear();
}

bool Mass_spring_viewer::keyboard(QKeyEvent* key)
{
    switch (key->key())
    {
        // setup problem 1
        case Qt::Key_1:
        {
            cloth_simulation = false;
            body_.clear();
            body_.add_particle( vec2(-0.5, -0.5), vec2(14.0, -2.0), particle_mass_, false );
            printInfo("1 particle");
           // glutPostRedisplay();
            break;
        }

        // setup problem 5
        case Qt::Key_2:
        {
            cloth_simulation = false;
            body_.clear();
            for (int i=0; i<100; ++i)
            {
                body_.add_particle( vec3(0.9* cos(i/50.0*M_PI), 0.9*sin(i/50.0*M_PI), 0.9*sin(i/50.0*M_PI)), vec3(-sin(i/50.0*M_PI), cos(i/50.0*M_PI)), particle_mass_, false );
            }
            printInfo("circle");
            //glutPostRedisplay();
            break;
        }

        // setup problem 4
        case Qt::Key_3:
        {
            cloth_simulation = false;
            body_.clear();

            for (int i=0; i<10; ++i)
            {
                body_.add_particle( vec2(i*0.1, 0.8), vec2(0.0, 0.0), particle_mass_, i==0 );
            }

            for (unsigned int i=0; i<9; ++i)
            {
                body_.add_spring(i, i+1, true);
            }

            //glutPostRedisplay();
            break;
        }

        // setup problem 2
        case Qt::Key_4:
        {
            cloth_simulation = false;
            body_.clear();

            body_.add_particle( vec2(-0.1, 0.7), vec2(0.0, 0.0), particle_mass_, false );
            body_.add_particle( vec2( 0.0, 0.6), vec2(0.0, 0.0), particle_mass_, false );
            body_.add_particle( vec2( 0.1, 0.7), vec2(0.0, 0.0), particle_mass_, false );

            body_.add_spring(0, 1);
            body_.add_spring(0, 2);
            body_.add_spring(1, 2);

            body_.add_triangle(0, 1, 2);

            //glutPostRedisplay();
            break;
        }

        // setup problem 3
        case Qt::Key_5:
        {
            cloth_simulation = false;
            body_.clear();

            for (int i=0; i<8; ++i)
            {
                body_.add_particle( vec2(-0.5+0.2*cos(0.25*i*M_PI), -0.5+0.2*sin(0.25*i*M_PI)), vec2(5.0, 5.0), particle_mass_, false );
            }

            body_.add_particle( vec2(-0.5, -0.5), vec2(5.0, 5.0), particle_mass_, false );

            for (unsigned int i=0; i<8; ++i)
            {
                body_.add_spring( i, (i+1) % 8 );
                body_.add_spring(i, 8);
                body_.add_triangle(i, (i+1)%8, 8);
            }

            //glutPostRedisplay();
            break;
        }
        case Qt::Key_6: {
            Cloth cloth(cloth_width, cloth_height, 0.7, &body_);
            cloth_simulation = true;

            break;
        }
        // switch between time integration methods
        case Qt::Key_I:
        {
            switch (integration_)
            {
                case Euler:    integration_ = Midpoint; printInfo("Midpoint"); break;
                case Midpoint: integration_ = Verlet;   printInfo("Verlet"); break;
                case Verlet:   integration_ = Euler;    printInfo("Euler"); break;
            }
            //glutPostRedisplay();
            break;
        }

        // switch between center and gravitation force
        case Qt::Key_F:
        {
            switch (external_force_)
            {
                case None:        external_force_ = Center; printInfo("Center"); break;
                case Center:      external_force_ = Gravitation; printInfo("Gravitation"); break;
                case Gravitation: external_force_ = None;      printInfo("None"); break;
            }
            //glutPostRedisplay();
            break;
        }

        // switch between force-based and impulse-based collisions
        case Qt::Key_C:
        {
            switch (collisions_)
            {
                case Force_based:   collisions_ = Impulse_based; printInfo("Impulse_based"); break;
                case Impulse_based: collisions_ = Force_based;   printInfo("Force_based"); break;
            }
           // glutPostRedisplay();
            break;
        }


        // toggle area forces on/off
        case Qt::Key_A:
        {
            area_forces_ = !area_forces_;
            area_forces_ ? printInfo("Area forces ON") : printInfo("Area forces OFF") ;
            //glutPostRedisplay();
            break;
        }


        // visualization of particle forces on/off
        case Qt::Key_V:
        {
            show_forces_ = !show_forces_;
            show_forces_ ? printInfo("Show forces ON") : printInfo("Show forces OFF") ;
            //glutPostRedisplay();
            break;
        }

        // gravitation based on camera position
        case Qt::Key_M:
        {
            camera_gravitation = !camera_gravitation;
            camera_gravitation ? printInfo("camera gravitation ON") : printInfo("camera gravitation OFF") ;
            break;
        }

        // objects adding, sphere (shift remove)
        case Qt::Key_O:
        {
            if (key->modifiers() == Qt::ShiftModifier) {
                removeAllObject3D();
            } else {
                //for now, just one sphere in the center
                if (m_objects.size() == 1) { break; }
                addObject3D(new Sphere(vec3(0), 0.33f));
            }
            break;
        }

        case Qt::Key_U:
        {
            cloth_show_particles = !cloth_show_particles;
        }

        // let parent class do the work
        default:
        {
//            Viewer_2D::keyboard(key, x, y);
//            break;
        return false;
        }
    }
    return true;
}


//-----------------------------------------------------------------------------


void Mass_spring_viewer::draw()
{
     // draw walls
    glDisable(GL_LIGHTING);
        glLineWidth(1.0);
        glColor3f(0.5,0.5,0.5);

        glBegin(GL_LINE_STRIP);
        glVertex3f(-1.0, 1.0,  -1.0 );
        glVertex3f(-1.0, 1.0,  1.0 );
        glVertex3f(1.0, 1.0,  1.0 );
        glVertex3f(1.0, 1.0,  -1.0 );
        glVertex3f(-1.0, 1.0,  -1.0 );
        glEnd();

        glBegin(GL_LINE_STRIP);
        glVertex3f(-1.0, -1.0,  -1.0 );
        glVertex3f(-1.0, -1.0,  1.0 );
        glVertex3f(1.0, -1.0,  1.0 );
        glVertex3f(1.0, -1.0,  -1.0 );
        glVertex3f(-1.0, -1.0,  -1.0 );
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(-1.0, 1.0,  -1.0 );
        glVertex3f(-1.0, -1.0,  -1.0 );

        glVertex3f(-1.0, 1.0,  1.0 );
        glVertex3f(-1.0, -1.0,  1.0 );

        glVertex3f(1.0, 1.0,  1.0 );
        glVertex3f(1.0, -1.0,  1.0 );

        glVertex3f(1.0, 1.0,  -1.0 );
        glVertex3f(1.0, -1.0,  -1.0 );

        glVertex3f(-1.0, 1.0,  -1.0 );
        glVertex3f(-1.0, -1.0,  -1.0 );
        glEnd();


    // draw mouse spring
    if (mouse_spring_.active)
    {
        glDisable(GL_LIGHTING);
        glLineWidth(5.0);
        glColor3f(1,0,0);
        glBegin(GL_LINES);
        glVertex2fv( body_.particles[mouse_spring_.particle_index].position.data() );
        glVertex2fv( mouse_spring_.mouse_position.data() );
        glEnd();
    }


    // draw particles, springs, triangles
    if (!cloth_simulation || cloth_show_particles) {
        body_.draw(particle_radius_, show_forces_, selected_);
    } else {
        body_.compute_normals(cloth_width, cloth_height);
        body_.draw_cloth(cloth_width, cloth_height);
    }

    //draw object3ds
    for(unsigned i = 0; i < m_objects.size(); ++i) {
        m_objects[i]->draw();
    }

}


//-----------------------------------------------------------------------------


void Mass_spring_viewer::mouse(bool release, int _x, int _y)
{
    // need particles to do interaction
    if (!body_.particles.empty())
    {
        // mouse button release destroys current mouse spring
        if (release)
        {
            mouse_spring_.active = false;
        }

        // mouse button press generates new mouse spring
        else
        {
            // get point under mouse cursor
            //vec2 p = pick(_x, _y);
            vec3 p(_x, _y);

            // find closest particle
            int   pidx = -1;
            float dmin = FLT_MAX;
            for (unsigned int i=0; i<body_.particles.size(); ++i)
            {
                float d = norm(p - body_.particles[i].position);
                if (d < dmin)
                {
                    dmin = d;
                    pidx = i;
                }
            }

            // construct mouse spring
            mouse_spring_.mouse_position = p;
            mouse_spring_.particle_index = pidx;
            mouse_spring_.active = true;
        }
    }

    //glutPostRedisplay();
}


//-----------------------------------------------------------------------------


void Mass_spring_viewer::motion(int _x, int _y)
{
    if (mouse_spring_.active)
    {
        // update mouse positions
        mouse_spring_.mouse_position = vec3(_x, _y); //pick(_x, _y);
    }

    //glutPostRedisplay();
}


//-----------------------------------------------------------------------------


void Mass_spring_viewer::time_integration(float dt)
{
    switch (integration_)
    {
        case Euler:
        {
            /** \todo (Part 1) Implement Euler integration scheme
             \li Hint: compute_forces() computes all forces for the current positions and velocities.
             */

            // compute all forces
            compute_forces();

            // update positions
            for (unsigned int i=0; i<body_.particles.size(); ++i)
                if (!body_.particles[i].locked)
                    body_.particles[i].position += dt * body_.particles[i].velocity;

            // update velocities
            for (unsigned int i=0; i<body_.particles.size(); ++i)
                if (!body_.particles[i].locked)
                    body_.particles[i].velocity += dt * body_.particles[i].force / body_.particles[i].mass;

            break;
        }

        case Midpoint:
        {
            /** \todo (Part 2) Implement the Midpoint time integration scheme
             \li The Particle class has variables position_t and velocity_t to store intermediate values
             \li Hint: compute_forces() computes all forces for the current positions and velocities.
             */
            // store current position and velocity
            for (unsigned int i=0; i<body_.particles.size(); ++i)
            {
                body_.particles[i].position_t = body_.particles[i].position;
                body_.particles[i].velocity_t = body_.particles[i].velocity;
            }

            // compute forces at time t
            compute_forces();

            // update positions to t+h/2
            for (unsigned int i=0; i<body_.particles.size(); ++i)
                if (!body_.particles[i].locked)
                    body_.particles[i].position += 0.5*dt*body_.particles[i].velocity;

            // update velocities to t+h/2
            for (unsigned int i=0; i<body_.particles.size(); ++i)
                if (!body_.particles[i].locked)
                    body_.particles[i].velocity += 0.5*dt*body_.particles[i].force/body_.particles[i].mass;

            // compute forces at time t+h/2
            compute_forces();

            // update positions to t+h
            for (unsigned int i=0; i<body_.particles.size(); ++i)
                if (!body_.particles[i].locked)
                    body_.particles[i].position = body_.particles[i].position_t + dt*body_.particles[i].velocity;

            // update velocities to t+h
            for (unsigned int i=0; i<body_.particles.size(); ++i)
                if (!body_.particles[i].locked)
                    body_.particles[i].velocity = body_.particles[i].velocity_t + dt*body_.particles[i].force/body_.particles[i].mass;

            break;
        }


        case Verlet:
        {
            /** \todo (Part 2) Implement the Verlet time integration scheme
             \li The Particle class has a variable acceleration to remember the previous values
             \li Hint: compute_forces() computes all forces for the current positions and velocities.
             */
            // update positions
            for (unsigned int i=0; i<body_.particles.size(); ++i)
                if (!body_.particles[i].locked)
                    body_.particles[i].position += (dt * body_.particles[i].velocity +
                                                    dt*dt*0.5f * body_.particles[i].acceleration);

            // compute new forces
            compute_forces();

            // update velocities
            for (unsigned int i=0; i<body_.particles.size(); ++i)
                if (!body_.particles[i].locked)
                    body_.particles[i].velocity += dt*0.5 * (body_.particles[i].acceleration +
                                                             body_.particles[i].force / body_.particles[i].mass);

            // remember accelerations
            for (unsigned int i=0; i<body_.particles.size(); ++i)
                body_.particles[i].acceleration = body_.particles[i].force / body_.particles[i].mass;

            break;
        }
    }


    // impulse-based collision handling
    if (collisions_ == Impulse_based)
    {
        impulse_based_collisions();
    }

    // do collision with objects3d
    for(unsigned o = 0; o < m_objects.size(); ++o) {
        for (unsigned int i=0; i < body_.particles.size(); ++i) {
            Particle& p = body_.particles[i];
            m_objects[o]->collisionResponse(p, particle_radius_);
        }
    }

    // test spring state
    for (unsigned int i=0; i<body_.springs.size(); ++i) {
        body_.springs[i].break_test();
    }

    //glutPostRedisplay();
}


//-----------------------------------------------------------------------------


void
Mass_spring_viewer::compute_forces()
{
    // clear forces
    for (unsigned int i=0; i<body_.particles.size(); ++i)
        body_.particles[i].force = vec3(0,0,0);


    /** \todo (Part 1) Implement center force
     */
    if (external_force_ == Center)
    {
        for (unsigned int i=0; i<body_.particles.size(); ++i)
        {
            body_.particles[i].force += (vec3(0.0, 0.0, 0.0) - body_.particles[i].position) * 20.0f;
        }
    }


    /** \todo (Part 1) Implement damping force
     \li The damping coefficient is given as member damping_
     */
    for (unsigned int i=0; i<body_.particles.size(); ++i)
          body_.particles[i].force += damping_ * -body_.particles[i].velocity;


    /** \todo (Part 1) Implement gravitation force
     \li Particle mass available as particle_mass_
     */
    if (external_force_ == Gravitation)
    {
        vec3 gravitation_vector(0.0, -9.81, 0.0);

        if (camera_gravitation) {
            float result[3];
            camera->getWorldCoordinatesOf(gravitation_vector.data(), result);

            gravitation_vector[0] = result[0];
            gravitation_vector[1] = result[1];
            gravitation_vector[2] = result[2];
        }

        for (unsigned int i=0; i<body_.particles.size(); ++i)
            body_.particles[i].force += gravitation_vector * particle_mass_;
    }


    /** \todo (Part 1) Implement force based boundary collisions
     \li Collision coefficient given as collision_stiffness_
     */
    // collision forces
    if (collisions_ == Force_based)
    {
        float planes[6][4] = {
            {  0.0,  1.0, 0.0, 1.0 },
            {  0.0, -1.0, 0.0, 1.0 },
            {  1.0,  0.0, 0.0, 1.0 },
            { -1.0,  0.0, 0.0, 1.0 },
            {  0.0, 0.0, 1.0, 1.0 },
            {  0.0, 0.0, -1.0, 1.0 }
        };

        for (unsigned int i=0; i<body_.particles.size(); ++i)
        {
            const vec3& pos = body_.particles[i].position;
            for (unsigned int p=0; p<6; ++p)
            {
                float dist = pos[0]*planes[p][0] + pos[1]*planes[p][1] + pos[2] * planes[p][2] + planes[p][3];
                dist -= particle_radius_; // account for particle radius
                if (dist < 0.0)
                    body_.particles[i].force += collision_stiffness_ * fabs(dist) * vec3(planes[p][0], planes[p][1], planes[p][2]);
            }
        }
    }


    /** \todo (Part 1) Compute force of the interactive mouse spring
     \li Required coefficients are given as spring_stiffness_ and spring_damping_
     */
    if (mouse_spring_.active)
    {
        Particle& p0 = body_.particles[ mouse_spring_.particle_index ];

        vec3 pos0 = p0.position;
        vec3 pos1 = mouse_spring_.mouse_position;
        vec3    d = pos0 - pos1;
        float   l = norm(d);
        d = normalize(d);

        vec3 f0 = -d * (spring_stiffness_ * l +
                        spring_damping_   * dot(p0.velocity, d));

        body_.particles[mouse_spring_.particle_index].force += f0;
    }


    /** \todo (Part 1) Compute spring forces
     \li Required information about springs in the scene are found in body_.springs
     \li Required coefficients are given as spring_stiffness_ and spring_damping_
     */
    for (unsigned int i=0; i<body_.springs.size(); ++i)
    {
        Spring& spring = body_.springs[i];
        if(!spring.is_broken) {

            Particle& p0 = *spring.particle0;
            Particle& p1 = *spring.particle1;

            vec3 x0 = p0.position;
            vec3 x1 = p1.position;
            vec3  d = normalize(x0 - x1);

            float   l = spring.length();
            float   L = spring.rest_length;

            vec3 f0 = -d * (spring_stiffness_ * (l-L) +
                            spring_damping_   * dot(p0.velocity-p1.velocity, d));
            vec3 f1 = -f0;

            p0.force += f0;
            p1.force += f1;
        }
    }


    /** \todo (Part 2) Compute more forces in part 2 of the exercise: triangle-area forces, binding forces, etc.
     */
    // triangle-area forces
    if (area_forces_)
    {
        for (unsigned int i=0; i<body_.triangles.size(); ++i)
        {
            Triangle& triangle = body_.triangles[i];

            Particle& p0 = *triangle.particle0;
            Particle& p1 = *triangle.particle1;
            Particle& p2 = *triangle.particle2;

            vec3 x0 = p0.position;
            vec3 x1 = p1.position;
            vec3 x2 = p2.position;

            float A = triangle.rest_area;
            float a = triangle.area();

            vec3 f0 = -0.5 * area_stiffness_ * (a-A) * vec3(x1[1]-x2[1], x2[0]-x1[0], 0);
            vec3 f1 = -0.5 * area_stiffness_ * (a-A) * vec3(x2[1]-x0[1], x0[0]-x2[0], 0);
            vec3 f2 = -0.5 * area_stiffness_ * (a-A) * vec3(x0[1]-x1[1], x1[0]-x0[0], 0);

            p0.force += f0;
            p1.force += f1;
            p2.force += f2;
        }
    }
}


//-----------------------------------------------------------------------------


void Mass_spring_viewer::impulse_based_collisions()
{
    //http://mathworld.wolfram.com/HessianNormalForm.html

    /** \todo (Part 2) Handle collisions based on impulses
     */
    // planes for which we compute collisions
    float planes[6][4] = {
        {  0.0,  1.0, 0.0, 1.0 },
        {  0.0, -1.0, 0.0, 1.0 },
        {  1.0,  0.0, 0.0, 1.0 },
        { -1.0,  0.0, 0.0, 1.0 },
        {  0.0, 0.0, 1.0, 1.0 },
        {  0.0, 0.0, -1.0, 1.0 }
    };

    for (unsigned int i=0; i<body_.particles.size(); ++i)
    {
        Particle&    p = body_.particles[i];
        const vec3&  x = p.position;

        for (unsigned int j=0; j<6; ++j)
        {
            // compute distance from plane
            float dist = x[0]*planes[j][0] + x[1]*planes[j][1] + x[2] * planes[j][2] + planes[j][3] - particle_radius_;

            // collision?
            if (dist < 0.0)
            {
                vec3 v  = p.velocity;
                vec3 n  = vec3(planes[j][0], planes[j][1], planes[j][2]);

                // if particle is on the wrong way
                if (dot(v,n) < 0.0)
                {
                    float e = 0.9;
                    float j = -(1.0f+e) * dot(v,n) * p.mass;
                    p.velocity += j / p.mass * n;
                }
            }
        }
    }
}


//=============================================================================
