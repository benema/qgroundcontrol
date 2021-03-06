///*=====================================================================
//
//QGroundControl Open Source Ground Control Station
//
//(c) 2009, 2010 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
//
//This file is part of the QGROUNDCONTROL project
//
//    QGROUNDCONTROL is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    QGROUNDCONTROL is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with QGROUNDCONTROL. If not, see <http://www.gnu.org/licenses/>.
//
//======================================================================*/

/**
 * @file
 *   @brief Definition of the class Pixhawk3DWidget.
 *
 *   @author Lionel Heng <hengli@student.ethz.ch>
 *
 */

#ifndef PIXHAWK3DWIDGET_H
#define PIXHAWK3DWIDGET_H

#include <osgText/Text>

#include "HUDScaleGeode.h"
#include "Imagery.h"
#include "ImageWindowGeode.h"
#include "WaypointGroupNode.h"

#ifdef QGC_LIBFREENECT_ENABLED
#include "Freenect.h"
#endif

#include "Q3DWidget.h"

class UASInterface;

/**
 * @brief A 3D View widget which displays vehicle-centric information.
 **/
class Pixhawk3DWidget : public Q3DWidget
{
    Q_OBJECT

public:
    explicit Pixhawk3DWidget(QWidget* parent = 0);
    ~Pixhawk3DWidget();

public slots:
    void setActiveUAS(UASInterface* uas);

private slots:
    void selectFrame(QString text);
    void showGrid(int state);
    void showTrail(int state);
    void showWaypoints(int state);
    void selectMapSource(int index);
    void selectVehicleModel(int index);
    void recenter(void);
    void toggleFollowCamera(int state);

    void selectTarget(void);
    void insertWaypoint(void);
    void moveWaypoint(void);
    void setWaypoint(void);
    void deleteWaypoint(void);
    void setWaypointAltitude(void);
    void clearAllWaypoints(void);

protected:
    QVector< osg::ref_ptr<osg::Node> > findVehicleModels(void);
    void buildLayout(void);
    virtual void display(void);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);

    UASInterface* uas;

private:
    void getPose(double& x, double& y, double& z,
                 double& roll, double& pitch, double& yaw,
                 QString& utmZone);
    void getPose(double& x, double& y, double& z,
                 double& roll, double& pitch, double& yaw);
    void getPosition(double& x, double& y, double& z,
                     QString& utmZone);
    void getPosition(double& x, double& y, double& z);

    osg::ref_ptr<osg::Geode> createGrid(void);
    osg::ref_ptr<osg::Geode> createTrail(void);
    osg::ref_ptr<Imagery> createMap(void);
    osg::ref_ptr<osg::Geode> createRGBD3D(void);
    osg::ref_ptr<osg::Node> createTarget(void);

    void setupHUD(void);
    void resizeHUD(void);

    void updateHUD(double robotX, double robotY, double robotZ,
                   double robotRoll, double robotPitch, double robotYaw,
                   const QString& utmZone);
    void updateTrail(double robotX, double robotY, double robotZ);
    void updateImagery(double originX, double originY, double originZ,
                       const QString& zone);
    void updateWaypoints(void);
    void updateTarget(double robotX, double robotY);
#ifdef QGC_LIBFREENECT_ENABLED
    void updateRGBD(void);
#endif

    int findWaypoint(int mouseX, int mouseY);
    bool findTarget(int mouseX, int mouseY);
    void showInsertWaypointMenu(const QPoint& cursorPos);
    void showEditWaypointMenu(const QPoint& cursorPos);

    enum Mode {
        DEFAULT_MODE,
        MOVE_WAYPOINT_MODE
    };
    Mode mode;
    int selectedWpIndex;

    bool displayGrid;
    bool displayTrail;
    bool displayImagery;
    bool displayWaypoints;
    bool displayRGBD2D;
    bool displayRGBD3D;
    bool enableRGBDColor;
    bool enableTarget;

    bool followCamera;

    osg::ref_ptr<osg::Vec3dArray> trailVertices;
    QVarLengthArray<osg::Vec3d, 10000> trail;

    osg::ref_ptr<osg::Node> vehicleModel;
    osg::ref_ptr<osg::Geometry> hudBackgroundGeometry;
    osg::ref_ptr<osgText::Text> statusText;
    osg::ref_ptr<HUDScaleGeode> scaleGeode;
    osg::ref_ptr<ImageWindowGeode> rgb2DGeode;
    osg::ref_ptr<ImageWindowGeode> depth2DGeode;
    osg::ref_ptr<osg::Image> rgbImage;
    osg::ref_ptr<osg::Image> depthImage;
    osg::ref_ptr<osg::Geode> gridNode;
    osg::ref_ptr<osg::Geode> trailNode;
    osg::ref_ptr<osg::Geometry> trailGeometry;
    osg::ref_ptr<osg::DrawArrays> trailDrawArrays;
    osg::ref_ptr<Imagery> mapNode;
    osg::ref_ptr<WaypointGroupNode> waypointGroupNode;
    osg::ref_ptr<osg::Node> targetNode;
    osg::ref_ptr<osg::Geode> rgbd3DNode;
#ifdef QGC_LIBFREENECT_ENABLED
    QScopedPointer<Freenect> freenect;
#endif
    bool enableFreenect;

    QVector< osg::ref_ptr<osg::Node> > vehicleModels;

    MAV_FRAME frame;
    osg::Vec2d target;
    double lastRobotX, lastRobotY, lastRobotZ;
};

#endif // PIXHAWK3DWIDGET_H
