#include "ofMain.h"
#include "RunwayML.h"

class ofApp : public ofBaseApp {
    ofImage originalImage;
    RunwayML::DensePose densepose;
    RunwayML::OpenPifPafPose openPifPafPose;
    RunwayML::YOLOv3 yolo_v3;
    void setup() {
        originalImage.load("resources/rzm_group.jpeg");
        densepose.load("resources/DensePose.json");
        openPifPafPose.load("resources/OpenPifPaf-Pose.json");
        yolo_v3.load("resources/YOLOv3.json");
    }
    void update() {
        
    }
    void draw() {
        ofSetColor(255, 255);
        switch(selected) {
            case Selected::DensePose:
                densepose.image.draw(0.0f, 0.0f);
                break;
            case Selected::OpenPifPafPose:
                originalImage.draw(0.0f, 0.0f);
                openPifPafPose.drawBoundingBox(0.0f, 0.0f);
                openPifPafPose.drawKeypoints(0.0f, 0.0f);
                break;
            case Selected::OpenPifPafPoseImage:
                openPifPafPose.image.draw(0.0f, 0.0f);
                break;
            case Selected::YOLOv3:
                originalImage.draw(0.0f, 0.0f);
                yolo_v3.draw(0.0f, 0.0f, originalImage.getWidth(), originalImage.getHeight());
                break;
        }
    }
    void exit() {
        
    }
    
    void keyPressed(int key) {
        switch(key) {
            case '1':
                selected = Selected::DensePose;
                break;
            case '2':
                selected = Selected::OpenPifPafPose;
                break;
            case '3':
                selected = Selected::OpenPifPafPoseImage;
                break;
            case '4':
                selected = Selected::YOLOv3;
                break;
        }
    }
    void keyReleased(int key) {}
    void mouseMoved(int x, int y) {}
    void mouseDragged(int x, int y, int button) {}
    void mousePressed(int x, int y, int button) {}
    void mouseReleased(int x, int y, int button) {}
    void mouseEntered(int x, int y) {}
    void mouseExited(int x, int y) {}
    void windowResized(int w, int h) {}
    void dragEvent(ofDragInfo dragInfo) {}
    void gotMessage(ofMessage msg) {}
    
    enum class Selected {
        DensePose,
        OpenPifPafPose,
        OpenPifPafPoseImage,
        YOLOv3
    };
    Selected selected{Selected::DensePose};
};

int main() {
    ofSetupOpenGL(1280, 720, OF_WINDOW);
    ofRunApp(new ofApp());

}
