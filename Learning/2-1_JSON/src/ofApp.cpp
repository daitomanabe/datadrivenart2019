#include "ofMain.h"
#include "ScreenConsole.h"

class ofApp : public ofBaseApp {
    std::size_t version{0};
    ScreenConsole console{100};
    ofColor color;
    ofColor backgroundColor;
    
    void loadJson() {
        ofJson json = ofLoadJson("./example.json");
        
        if(json["int"].is_number()) {
            int i = json["int"];
            console("int") << i;
        } else {
            console("int") << "type is wrong?";
        }
        
        if(json["int"].is_number()) {
            double d = json["double"];
            console("double") << d;
        } else {
            console("double") << "type is wrong?";
        }
        
        if(json["bool"].is_boolean()) {
            bool b = json["bool"];
            console("bool") << b;
        } else {
            console("bool") << "type is wrong?";
        }
        
        if(json["string"].is_string()) {
            std::string str = json["string"];
            console("string") << str;
        } else {
            console("string") << "type is wrong?";
        }
        
        if(json["array"].is_array()) {
            std::vector<ofJson> array = json["array"];
            for(int i = 0; i < array.size(); ++i) {
                double v = array[i];
                console("array::value") << v;
            }
        } else {
            console("array") << "type is wrong?";
        }
        
        if(json["object"].is_object()) {
            std::map<std::string, ofJson> object = json["object"];
            // 明示的な型指定での値取得
            console("object::key1") << object["key1"].get<double>();
            console("object::key2") << object["key2"].get<double>();
            console("object::key3") << object["key3"].get<double>();
        } else {
            console("object") << "type is wrong?";
        }
    }
    
    void loadSetting() {
        ofJson json = ofLoadJson("setting.json");
        
        color.r = json["color"]["r"];
        color.g = json["color"]["g"];
        color.b = json["color"]["b"];

        backgroundColor.r = json["backgroundColor"]["r"];
        backgroundColor.g = json["backgroundColor"]["g"];
        backgroundColor.b = json["backgroundColor"]["b"];
    }
    
public:
    void setup() {
        loadJson();
        loadSetting();
    }
    void update() {
        
    }
    void draw() {
        ofSetColor(color);
        ofSetBackgroundColor(backgroundColor);
        console.print(20, 20);
    }
    void exit() {
        
    }
    
    void keyPressed(int key) {
        if(key == ' ') {
            ofJson json;
            json["frameNum"] = ofGetFrameNum();
            json["version"] = version++;
            json["logX"] = ofGetMouseX();
            json["logY"] = ofGetMouseY();
            
            console.clear();
            console.push(json.dump(2));
        }
        else if(key == 'r') {
            loadSetting();
        }
    }
};

int main() {
    ofSetupOpenGL(1280, 720, OF_WINDOW);
    ofRunApp(new ofApp());
}
