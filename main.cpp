#include <SFML/Graphics.hpp>
#include<bits/stdc++.h>

double clipping,viewAngle, horView,clippingS;

const double PI = 3.14159265358979323;

struct coordinate{
    double x,y,z;
};

struct square{
    coordinate a,b,c,d;
    int colorR,colorG,colorB;
    double cameraDistance;
};

void samePol(double& mit, double& mivel){
    if(mivel < 0){
        mit = -1 * std::abs(mit);
    }
    else{
        mit = std::abs(mit);
    }
}

double toDeg(double rad){
    return (rad/PI)*180;
}

double distance(coordinate a, coordinate b, bool take_square){
    double dis = std::pow(a.x-b.x,2) + std::pow(a.y-b.y,2) + std::pow(a.z-b.z,2);
    if(!take_square) return dis;
    return std::sqrt(dis);
}

double getAngle(coordinate p1, coordinate p2, coordinate p3){
    double a,b,c;
    a = distance(p1,p2,true);
    b = distance(p2,p3,true);
    c = distance(p1,p3,true);
    if(std::abs(a+b-c) < 0.0001){
        return PI - 0.0001;
    }
    if(std::abs(a-b+c) < 0.0001 || std::abs(b+c-a) < 0.0001){
        return 0.00002;
    }
    double angle;
    //angle = std::acos((a*( (b*b-cS)/(2*a*a) +1/2))/b);
    angle = std::acos((a*a+b*b-c*c)/(2*a*b));
    return angle;
}

bool canSee(coordinate& point, coordinate& camera, coordinate& lookAt){
    if(distance(point,camera,false) < clippingS) return false;
    if(getAngle(point,camera,lookAt) > PI / 2 - 0.001) return false;
    return true;
}

coordinate addC(coordinate a, coordinate b){
    coordinate ret;
    ret.x = a.x + b.x;
    ret.y = a.y + b.y;
    ret.z = a.z + b.z;
    return ret;
}

coordinate getOnScreenPos(coordinate& C, coordinate& E, coordinate& P, coordinate& lookR, coordinate& lookL){
    coordinate N,F,ret;

    if(E.x < 0){
        E.y = E.y * -1;

    }

    double D = 0 - (P.x + E.y*P.y + E.z*P.z);

    double Eys_Ezs = std::pow(E.y,2) + std::pow(E.z,2);

    N.x = (C.x * Eys_Ezs - C.y * E.y - C.z * E.z - D) / (1 + Eys_Ezs);
    N.y = N.x * E.y - C.x * E.y + C.y;
    N.z = N.x * E.z - C.x * E.z + C.z;

    F.z = N.z + 10;
    F.y = (C.y/E.y - (E.z*F.z + D + C.x) / (E.x)) / (1/E.y + E.y/E.x);
    F.x = 0 - (E.y * F.y + E.z * F.z + D);

    double r = std::tan(getAngle(addC(C,E), C, P)) * (1/std::tan(horView/2)) * (1920/2);

    /*F.x = N.x;
    F.y = N.y;*/
    double alfa = getAngle(F,N,P);

    //std::cout<<"first: "<<(getAngle(addC(C,E), C, P)/PI) * 180<<" beta: "<<(alfa/PI)*180<<std::endl;

    //std::cout<<"point pos:"<<toDeg(alfa)<<" "<<r<<std::endl;

    if(distance(P,lookL,false) > distance(P,lookR,false)){
        alfa = 2*PI - alfa;
    }

    ret.z = 0;
    if(0<=alfa && PI/2 >= alfa){
        ret.x = 1080/2 - r*std::cos(alfa);
        ret.y = 1920/2 - r*std::sin(alfa);
    }
    else if(alfa>PI/2 && alfa<= PI){
        alfa = alfa - (PI/2);
        ret.x = 1080/2 + r*std::sin(alfa);
        ret.y = 1920/2 - r*std::cos(alfa);
    }
    else if(alfa>PI && alfa <= (PI*3)/2){
        alfa = alfa - PI;
        ret.x = 1080/2 + r*std::cos(alfa);
        ret.y = 1920/2 + r*std::sin(alfa);
    }
    else if(alfa > (PI*3)/2 && alfa <= PI*2){
        alfa = alfa - (PI*3)/2;
        ret.x = 1080/2 - r*std::sin(alfa);
        ret.y = 1920/2 + r*std::cos(alfa);
    }
    else{
        std::cout<<"NEM JO, ALFA NEM 0 ES 2PI KOZOTT VAN, HANEM:"<<alfa<<std::endl;
    }
    /*double x2,y2;
    x2 = std::cos(alfa) * r;
    x2 = std::abs(x2);
    y2 = std::sin(alfa) * r;
    y2 = std::abs(y2);
    if(alfa < PI){
        ret.y = 1920/2 - y2;
    }
    else{
        ret.y = 1920/2 + y2;
    }

    if(alfa > PI/2 and alfa < (PI*3)/2){
        ret.x = 1080/2 + x2;
    }
    else{
        ret.x = 1080/2 - x2;
    }*/
   /* std::cout<<"N: "<<N.x<<" "<<N.y<<" "<<N.z;
    std::cout<<"   F: "<<F.x<<" "<<F.y<<" "<<F.z<<std::endl;*/
    return ret;

    /*
    c = camera
    e = lookAt
    p = point


    coordinate ret;
    ret.z = 0;
    double a,b,c;
    a = lookAt.x - camera.x;
    b = lookAt.y - camera.y;
    c = lookAt.z - camera.z;
    if(a < 0.0002) a = 0.0002;
    if(b < 0.0002) b = 0.0002;
    if(c < 0.0002) c = 0.0002;
    b = b/a;
    c = c/a;
    a = 1;
    double D;
    D = 0-(point.x + b*point.y + c*point.z);
    double x,y,z;
    x = (b*b*camera.x + c*c*camera.x - b*camera.y - c*camera.z - D) / (1 + b*b + c*c);
    y = b*x + camera.y - b*camera.x;
    z = c*x + camera.z - c*camera.x;
    double xv,yv,zv;
    zv = z + 1;
    yv = ((y - camera.y)*(D + c*zv + camera.x) + (camera.x - x)*camera.y) / (camera.x + b*camera.y - x - b*y);
    xv = 0 - (D + c*zv + b*yv);
    coordinate xyz,xyzv;
    xyz.x = x;
    xyz.y = y;
    xyz.z = z;
    xyzv.x = xv;
    xyzv.y = yv;
    xyzv.z = zv;
    double angDis,angRot;
    angDis = getAngle(point, camera, xyz);
    angRot = getAngle(xyzv, xyz, point);
    double distFromCenter;
    distFromCenter = (angDis/maxViewAngle) * std::sqrt(std::pow(1920/2,2) + std::pow(1080/2,2));
    double x2,y2;
    x2 = std::cos(angRot) * distFromCenter;
    x2 = std::abs(x2);
    y2 = std::sin(angRot) * distFromCenter;
    y2 = std::abs(y2);
    if(angRot < PI){
        ret.y = 1920/2 - y2;
    }
    else{
        ret.y = 1920/2 + y2;
    }

    if(angRot > PI/2 and angRot < (PI*3)/2){
        ret.x = 1080/2 + x2;
    }
    else{
        ret.x = 1080/2 - x2;
    }
    return ret;*/
}

bool inB(double& x, double& y){
    if(0<x && 1080>x && 0<y && 1920>y)return true;
    return false;

}

void drawSquare(sf::RenderWindow& window, square& s, coordinate& camera, coordinate& lookAt, coordinate& lookAtR, coordinate& lookAtL){
    bool shouldDraw = false;
    if(canSee(s.a,camera,lookAt)) shouldDraw = true;
    if(canSee(s.b,camera,lookAt)) shouldDraw = true;
    if(canSee(s.c,camera,lookAt)) shouldDraw = true;
    if(canSee(s.d,camera,lookAt)) shouldDraw = true;
    if(!shouldDraw)return;
    sf::ConvexShape convex;
    convex.setFillColor(sf::Color(s.colorR,s.colorG,s.colorB));
    // resize it to 5 points
    convex.setPointCount(4);

    // define the points
    bool inBound = false;
    coordinate show;
    show = getOnScreenPos(camera,lookAt,s.a,lookAtR,lookAtL);
    if(inB(show.x,show.y))inBound = true;
    convex.setPoint(0, sf::Vector2f(show.y, show.x));
    show = getOnScreenPos(camera,lookAt,s.b,lookAtR,lookAtL);
    if(inB(show.x,show.y))inBound = true;
    convex.setPoint(1, sf::Vector2f(show.y, show.x));
    show = getOnScreenPos(camera,lookAt,s.c,lookAtR,lookAtL);
    if(inB(show.x,show.y))inBound = true;
    convex.setPoint(2, sf::Vector2f(show.y, show.x));
    show = getOnScreenPos(camera,lookAt,s.d,lookAtR,lookAtL);
    if(inB(show.x,show.y))inBound = true;
    convex.setPoint(3, sf::Vector2f(show.y, show.x));
    if(inBound)window.draw(convex);
}

void calculateView(coordinate& lookAt, double vertic, double horiz){

    if(0<=vertic && PI/2 >= vertic){
        lookAt.x = 1;
        lookAt.y = std::tan(vertic);
    }
    else if(vertic>PI/2 && vertic<= PI){
        vertic = vertic - (PI/2);
        lookAt.x = -1;
        lookAt.y = 1/std::tan(vertic);
    }
    else if(vertic>PI && vertic <= (PI*3)/2){
        vertic = vertic - PI;
        lookAt.x = -1;
        lookAt.y = std::tan(vertic) * -1;
    }
    else if(vertic > (PI*3)/2 && vertic <= PI*2){
        vertic = vertic - (PI*3)/2;
        lookAt.x = 1;
        lookAt.y = 1/std::tan(vertic) * -1;
    }
    else{
        std::cout<<"NEM JO, VERTIC NEM 0 ES 2PI KOZOTT VAN, HANEM:"<<vertic<<std::endl;
    }

    /*lookAt.x = 1;
    if(PI/2 < vertic && vertic < (3*PI)/2){
        lookAt.x = -1;
    }
    lookAt.y = std::tan(vertic);*/

    lookAt.z = std::tan(horiz) * std::sqrt(1 + lookAt.y*lookAt.y);
}

double near(double mi, double mihez){
    if(std::abs(mi-mihez) < 0.0001){
        return mihez - 0.0005;
    }
    return mi;
}

void setVerHot(double& vertic, double& horiz){
if(vertic < 0.0001)vertic = 2*PI - 0.0005;
        if(vertic > 2*PI - 0.0001) vertic = 0.0005;
        vertic = near(vertic,PI/2);
        vertic = near(vertic,PI);
        vertic = near(vertic,(3 * PI)/2);

        if(horiz < (PI/2 - 0.0001) * -1) horiz = (PI/2 - 0.0005) * -1;
        if(horiz > PI/2 - 0.0001) horiz = (PI/2 - 0.0005);
        horiz = near(horiz , 0);

}

void toGomb(coordinate& l, coordinate c){
    double A,B,C;
    A = std::sqrt( 1/( 1 + std::pow(l.y,2) / std::pow(l.x,2) + std::pow(l.z,2) / std::pow(l.x,2) ) );
    B = A*l.y/l.x;
    C = A*l.z/l.x;
    samePol(A,l.x);
    samePol(B,l.y);
    samePol(C,l.z);
    l.x = A;
    l.y = B;
    l.z = C;
    l = addC(l,c);
}

int main()
{

    /*coordinate aa,bb,cc;
    aa.x = -6.96;
    aa.y = 10.33;
    aa.z = 6;
    bb.x = 6.67;
    bb.y = 6.99;
    bb.z = -2.33;
    cc.x = -8.21;
    cc.y = -22.51;
    cc.z = 0.94;

    std::cout<<(getAngle(aa,bb,cc)/PI)*180;*/

    clipping = 0.1;
    horView = PI/2;
    viewAngle = PI/2;

    coordinate camera,lookAt, lookAtR,lookAtL;
    double vertic,horiz;

    vertic = 0.01;
    horiz = 0.01;

    camera.x = 0;
    camera.y = 0;
    camera.z = 0;

    clippingS = std::pow(clipping,2);

    coordinate p;
    p.x = 4;
    p.y = 2;
    p.z = 1;
    lookAt.x = 1;
    lookAt.y = -0.2;
    lookAt.z = 0.1;
    //coordinate rett = getOnScreenPos(camera,lookAt,p);



    // create the window
    sf::RenderWindow window(sf::VideoMode(1920 , 1080), "My window", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(1000);
    // run the program as long as the window is open

    std::vector<square>squares;

    int side = 20;

    for(int i = side*-1; i<=side ; i++){
        for(int j= side*-1;j<=side;j++){
            square n;
            if((i+j)%2 == 0){
                n.colorR = 0;
                n.colorG = 0;
                n.colorB = 200;
            }
            else{
                n.colorR = 0;
                n.colorG = 200;
                n.colorB = 0;
            }
            double depth = -40;
            n.a.z = depth;
            n.b.z = depth;
            n.c.z = depth;
            n.d.z = depth;
            n.a.x = i;
            n.b.x = i;
            n.c.x = i+1;
            n.d.x = i+1;
            n.a.y = j;
            n.b.y = j+1;
            n.c.y = j+1;
            n.d.y = j;
            squares.push_back(n);
        }
    }
    //korona.tavoktatas.bme.hu.tksoft.dev     1.15.2
    square a,b,c;
    a.a.x=4;
    a.b.x=4;
    a.c.x=4;
    a.d.x=4;
    a.a.z=1;
    a.b.z=1;
    a.c.z=0;
    a.d.z=0;
    a.a.y=2;
    a.b.y=3;
    a.c.y=3;
    a.d.y=2;

    c.a.x=4;
    c.b.x=4;
    c.c.x=4;
    c.d.x=4;
    c.a.z=1;
    c.b.z=1;
    c.c.z=2;
    c.d.z=2;
    c.a.y=2;
    c.b.y=3;
    c.c.y=3;
    c.d.y=2;

    b.a.x=-4;
    b.b.x=-4;
    b.c.x=-4;
    b.d.x=-4;
    b.a.z=-1;
    b.b.z=-1;
    b.c.z=-0;
    b.d.z=-0;
    b.a.y=-2;
    b.b.y=-3;
    b.c.y=-3;
    b.d.y=-2;

    a.colorR = 0;
    a.colorG = 0;
    a.colorB = 200;

    b.colorR = 200;
    b.colorG = 0;
    b.colorB = 0;

    c.colorR = 0;
    c.colorG = 200;
    c.colorB = 0;

    window.setMouseCursorVisible(false);

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);


        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            window.setMouseCursorVisible(true);
            window.close();
        }


        sf::Vector2i localPosition = sf::Mouse::getPosition(window);
        sf::Mouse::setPosition(sf::Vector2i(1920/2, 1080/2), window);



        double downs = 0.0003;

        vertic+= (localPosition.x - 1920/2) * downs;
        horiz-= (localPosition.y - 1080/2) * downs;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
            vertic += 0.001;
            //std::cout<<"vertic: "<<vertic<<std::endl;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            vertic -= 0.001;
            //std::cout<<"vertic: "<<vertic<<std::endl;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            horiz += 0.001;
            //std::cout<<"horiz: "<<horiz<<std::endl;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            horiz -= 0.001;
            //std::cout<<"horiz: "<<horiz<<std::endl;
        }

        sf::CircleShape circle(10.f);

        circle.setFillColor(sf::Color(100, 250, 50));

        coordinate tt;
        tt.x = 10;
        tt.y = 10;
        tt.z = 0;

        setVerHot(vertic,horiz);
        double verR,verL;
        verR = vertic + 0.1;
        verL = vertic - 0.1;

        setVerHot(verR,horiz);
        setVerHot(verL,horiz);

        calculateView(lookAt,vertic,horiz);
        calculateView(lookAtR,verR,horiz);
        calculateView(lookAtL,verL,horiz);

        toGomb(lookAtR, camera);
        toGomb(lookAtL, camera);

        /*if(canSee(tt,camera,lookAt)){
            coordinate nPos = getOnScreenPos(camera,lookAt,tt, lookAtR,lookAtL);
            circle.setPosition(nPos.y,nPos.x);
            window.draw(circle);
        }*/






        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
            std::cout<<"alf: "<<toDeg(vertic)<<", beta: "<<toDeg(horiz)<<std::endl;
            std::cout<<"lookAt: "<<lookAt.x<<", "<<lookAt.y<<", "<<lookAt.z<<std::endl;
        }
        //std::cout<<"alf: "<<toDeg(vertic)<<", beta: "<<toDeg(horiz)<<std::endl;
        drawSquare(window,a,camera,lookAt, lookAtR,lookAtL);
        drawSquare(window,b,camera,lookAt, lookAtR,lookAtL);
        drawSquare(window,c,camera,lookAt, lookAtR,lookAtL);
        for(int i=0;i<squares.size();i++){
            drawSquare(window,squares[i],camera,lookAt,lookAtR,lookAtL);
        }


        // create an empty shape
/*sf::ConvexShape convex;

// resize it to 5 points
convex.setPointCount(5);

// define the points
convex.setPoint(0, sf::Vector2f(0.f, 0.f));
convex.setPoint(1, sf::Vector2f(150.f, 10.f));
convex.setPoint(2, sf::Vector2f(120.f, 90.f));
convex.setPoint(3, sf::Vector2f(30.f, 100.f));
convex.setPoint(4, sf::Vector2f(0.f, 50.f));
window.draw(convex);*/
        // end the current frame
        window.display();
    }

    return 0;
}
