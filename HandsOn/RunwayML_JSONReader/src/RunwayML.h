//
//  RunwayML.h
//  RunwayML_JSONReader
//
//  Created by 2bit on 2020/01/13.
//

#ifndef RunwayML_h
#define RunwayML_h

#include "ofMain.h"
#include "Base64.h"

namespace RunwayML {
    struct DensePose {
        void load(const std::string &path) {
            ofJson json = ofLoadJson(path);
            filename = json[0]["filename"];
            std::string base64 = json[0]["output"];
            ofStringReplace(base64, "data:image/png;base64,", "");
            std::vector<std::uint8_t> png_binary;
            auto res = macaron::Base64::Decode(base64, png_binary);
            ofLogNotice() << res << " " << png_binary.size();
            if(0 < png_binary.size()) {
                ofBuffer buf{(char *)png_binary.data(), png_binary.size()};
                image.load(buf);
            }
        }
        
        std::string filename;
        ofImage image;
    };

    struct OpenPifPafPose {
        struct Keypoint {
            float x;
            float y;
            float a;
            bool found() const {
                return x != 0.0f || y != 0.0f || a != 0.0f;
            }
        };
        struct Object {
            ofRectangle boundingBox;
            std::vector<Keypoint> keypoints;
        };

        void load(const std::string &path) {
            ofJson json = ofLoadJson(path);
            filename = json[0]["filename"];
            std::string base64 = json[0]["image"];
            ofStringReplace(base64, "data:image/png;base64,", "");
            std::vector<std::uint8_t> png_binary;
            auto res = macaron::Base64::Decode(base64, png_binary);
            ofLogNotice() << res << " " << png_binary.size();
            if(0 < png_binary.size()) {
                ofBuffer buf{(char *)png_binary.data(), png_binary.size()};
                image.load(buf);
            }
            ofJson &&os = ofJson::parse(json[0]["keypoints"].get<std::string>());
            ofLogNotice() << os.dump(2);
            for(auto &&o : os) {
                Object obj;
                
                auto &&bbox = o["bbox"];
                obj.boundingBox.x = bbox[0];
                obj.boundingBox.y = bbox[1];
                obj.boundingBox.width  = bbox[2];
                obj.boundingBox.height = bbox[3];
                
                auto &&ks = o["keypoints"];
                for(auto i = 0; i < ks.size(); i += 3) {
                    Keypoint k;
                    k.x = ks[i + 0];
                    k.y = ks[i + 1];
                    k.a = ks[i + 2];
                    obj.keypoints.push_back(k);
                }
                
                objects.emplace_back(std::move(obj));
            }
        }

        void drawBoundingBox(float x, float y, int index = -1) {
            ofPushMatrix();
            ofPushStyle();
            ofNoFill();
            ofTranslate(x, y);
            if(index == -1) {
                for(const auto &obj : objects) {
                    ofDrawRectangle(obj.boundingBox);
                }
            } else {
                ofDrawRectangle(objects[index].boundingBox);
            }
            ofPopStyle();
            ofPopMatrix();
        }
        void drawKeypoints(float x, float y, int index = -1) {
            ofPushMatrix();
            ofTranslate(x, y);
            if(index == -1) {
                for(const auto &obj : objects) {
                    for(const auto &k : obj.keypoints) {
                        ofDrawCircle(k.x, k.y, 5);
                    }
                }
            } else {
                for(const auto &k : objects[index].keypoints) {
                    ofDrawCircle(k.x, k.y, 5);
                }
            }
            ofPopMatrix();
        }
        
        std::string filename;
        ofImage image;
        std::vector<Object> objects;
    };
    
    struct YOLOv3 {
        void load(const std::string &path) {
            ofJson json = ofLoadJson(path);
            filename = json[0]["filename"];
            auto &&bboxes = json[0]["bboxes"];
            auto &&classes = json[0]["classes"];
            auto &&scores = json[0]["scores"];
            results.resize(scores.size());
            for(auto i = 0; i < results.size(); ++i) {
                results[i].boundingBox.x = bboxes[i][0];
                results[i].boundingBox.y = bboxes[i][1];
                results[i].boundingBox.width  = bboxes[i][2];
                results[i].boundingBox.width  -= results[i].boundingBox.x;
                results[i].boundingBox.height = bboxes[i][3];
                results[i].boundingBox.height -= results[i].boundingBox.y;
                
                results[i].label = classes[i];
                
                results[i].score = scores[i];
            }
        }
        
        void draw(float x, float y, float w, float h, int index = -1) {
            ofPushMatrix();
            ofPushStyle();
            ofTranslate(x, y);
            if(index == -1) {
                for(const auto &res : results) {
                    ofNoFill();
                    ofSetColor(255, 0, 0);
                    ofDrawRectangle(res.boundingBox.x * w,
                                    res.boundingBox.y * h,
                                    res.boundingBox.width * w,
                                    res.boundingBox.height * h);
                    ofFill();
                    ofSetColor(255);
                    ofDrawBitmapStringHighlight(res.label + " " + ofToString(res.score, 3), res.boundingBox.x * w, res.boundingBox.y * h);
                }
            }
            ofPopStyle();
            ofPopMatrix();
        }
        
        std::string filename;
        struct Result {
            ofRectangle boundingBox;
            std::string label;
            float score;
        };
        std::vector<Result> results;
    };
};

#endif /* RunwayML_h */
