//
// Created by Ghosh, Anirban on 4/27/22.
//

#ifndef CODE_MED_H
#define CODE_MED_H

#include "algorithm"
#include "vector"
#include <QtWidgets>
#include <QApplication>
#include <QLabel>
#include <QString>
#include <QTranslator>
#include <CGAL/spatial_sort.h>

#include "CGALComponents-1.h"

static std::default_random_engine randGen((std::random_device())());

//void drawConvexHullUsingQT(const std::vector<Point> &P,const Circle C,const bool labels) {
//
//    const double pointSize = 2; // adjust this value to change the size of the points
//    /***************************************************/
//    QPicture pi;
//    QPainter canvas(&pi);
//    canvas.setRenderHint(QPainter::Antialiasing);
//    //canvas.setFont(QFont("Times", 12));
//    // DO NOT TOUCH THESE Qt STATEMENTS!!!
//    /***************************************************/
//
//    unsigned id = 0;
//    for ( Point p : P ) {
////        canvas.setBrush(Qt::black);
//
//        canvas.drawEllipse(QPointF(p.x(), p.y()), pointSize, pointSize);
//        if(labels) {
//            QPointF textPos(p.x() + 4.0, p.y() + 4.0);
//            canvas.drawText(textPos, QString(std::to_string(id).c_str()));
//        }
//        id++;
//    }
//
//    //circle drawing
//
//    double circleRadius = sqrt(C.squared_radius());
//    canvas.drawEllipse(QPointF(C.center().x(),C.center().y()),circleRadius,circleRadius);
//
////    std::cout<<canvas.opacity();
//
//    /*********************************************/
//    canvas.end();
//    auto l = new QLabel;
//    l->setStyleSheet("QLabel { background-color : white; color : black; }");
//    l->setPicture(pi);
//    l->setWindowTitle("Polygon Triangulation");
//    l->show();
//    // l->showMaximized();
//    QApplication::exec();
//    // DO NOT TOUCH THESE Qt STATEMENTS!!!
//    /***************************************************/
//}

const double INF = 1e18;

bool isCoveredby(const Point &p, const Circle &C)
{
    if( CGAL::squared_distance(p,C.center() ) > C.squared_radius() + 0.001 ) return false;
    return true;
}

bool isCoveredby(const std::vector<Point> &P, const Circle &C) {
    for(const Point &p : P)
        if( CGAL::squared_distance(p,C.center() ) > C.squared_radius() + .001 )
            return false;
    return true;
}

Circle miniDiskNaive(std::vector<Point> &P) {
    // complete this part

    unsigned pointSize = P.size();
    Circle D{ { 0, 0 }, INF };

    for (unsigned i = 0; i < pointSize; i++) {
        for (unsigned j = i + 1; j < pointSize; j++) {
            Circle circleObject(P[i], P[j]);

            if (circleObject.squared_radius() < D.squared_radius() && isCoveredby(P,circleObject))
                D = circleObject;
        }
    }

    for (unsigned i = 0; i < pointSize; i++) {
        for (unsigned j = i + 1; j < pointSize; j++) {
            for (unsigned k = j + 1; k < pointSize; k++) {
                Circle circleObject(P[i], P[j], P[k]);
                if (circleObject.squared_radius() < D.squared_radius() && isCoveredby(P,circleObject))
                    D = circleObject;
            }
        }
    }
    return D;
}


Circle MiniDiskWith2Points(const std::vector<Point> &P, const Point &q1, const Point &q2)
{
    Circle D0(q1,q2);
    std::vector<Circle> disk;
    disk.emplace_back(D0);
    for( unsigned k = 0; k<P.size(); k++ )
    {
        Point pk = P.at(k);
        if(!isCoveredby(pk,D0)){
            Circle circleObject(q1,q2,pk);
            D0 = circleObject;
        }
    }
     return D0;
}


Circle MiniDiskWithPoint(const std::vector<Point> &P, const Point &q)
{
    Circle D1{q,P.at(0)};
    for( unsigned j = 1; j<P.size(); j++ )
    {
        Point p = P.at(j);
        if(!isCoveredby(p,D1))
        {
            std::vector<Point> pointTmp;
            for(unsigned k = 0; k<j; k++ ) {
                pointTmp.emplace_back(P.at(k));
            }
            D1 = MiniDiskWith2Points(pointTmp,P.at(j),q);
        }
    }
    return D1;
}

Circle miniDiskIncremental(std::vector<Point> &P) {

    // complete this part
    std::shuffle(P.begin(),P.end(),randGen);

//    CGAL::spatial_sort(P.begin(),P.end());

    Circle D2(P.at(0),P.at(1));

    for ( unsigned i = 2; i<P.size(); i++ )
    {
        Point p = P.at(i);
        if(!isCoveredby(p,D2))
        {
            std::vector<Point> pointTmp;
            for(unsigned j = 0; j < i; j++ ) {
                pointTmp.emplace_back(P.at(j));
            }
            std::shuffle(pointTmp.begin(),pointTmp.end(),randGen);
//            CGAL::spatial_sort(P.begin(),P.end());
            D2 = MiniDiskWithPoint(pointTmp,p);
        }
    }
    return D2;
}


void tester() {

    std::freopen("a.txt","w",stdout);
    double miniDiskNaiveTime = 0.0, miniDiskIncrementalTime = 0.0;
    const unsigned totalNumberOfTests = 15 * 5;
    unsigned testsPassed = 0,testsFailed = 0;

    unsigned numberOfSamples = 5;
    for(unsigned n = 100; n <= 1500; n = n + 100) {
        double miniDiskNaiveTime = 0.0, miniDiskIncrementalTime = 0.0;
        for(unsigned i = 0; i < numberOfSamples; ++i) {

            std::vector<Point> P, copyOfP;
            generatePointsInsideASquare(n,200,P);

            for(const Point &p : P)
                copyOfP.emplace_back(p);

            CGAL::Timer clock;
            clock.start();
            Circle C1 = miniDiskNaive(P);
            clock.stop();
            miniDiskNaiveTime += clock.time();
            clock.reset();

            clock.start();
            Circle C2 = miniDiskIncremental(copyOfP);
            clock.stop();
            miniDiskIncrementalTime += clock.time();

//            clock.reset();
            if(isCoveredby(P,C1) && isCoveredby(copyOfP,C2) && fabs((C1.center().x()- C2.center().x())) < 0.000000001
            && fabs((C1.center().y()- C2.center().y())) < 0.000000001
            && fabs(C1.squared_radius() - C2.squared_radius())  < 0.0000000001)
            {
                testsPassed++;
            }
            else {
                testsFailed++;
//                std::cout<<((C1.squared_radius() - C2.squared_radius())  < 0.0000000001)<<std::endl;
//                std::cout<<isCoveredby(P,C1)<<std::endl;
//                std::cout<<isCoveredby(copyOfP,C2)<<std::endl;
//                std::cout<<(((C1.center()- C2.center())).y() < .0000000001)<<std::endl;
//                std::cout<<(((C1.center()- C2.center())).x() < .0000000001)<<std::endl;
//                std::cout<<"Naive "<<n<<" "<<C1.center()<<" "<<sqrt(C1.squared_radius()) <<std::endl;
//                std::cout<<"Linear "<<n<<" "<<C2.center()<<" "<<sqrt(C2.squared_radius()) <<std::endl;
            }
        }
        std::cout<<n<<" "<<(double)miniDiskNaiveTime/numberOfSamples<<" "<<(double)miniDiskIncrementalTime/numberOfSamples<<std::endl;
    }

    std::cout << "Tests passed: " << testsPassed << " out of " << totalNumberOfTests << std::endl;
}

#endif //CODE_MED_H
