#include <RSGL/RSGL.hpp>
#include <iostream>
#include "../include/package.hpp"
#include <memory>

RSGL::window win("RSGL Apps",{500,500,950,600},{50,50,50});
bool running=true; 

std::vector<std::string> split(std::string string, std::string split, std::string split2=""){
      int start=0; int end=string.find(split); std::vector<std::string> output;
      while (end != -1){
            output.insert(output.end(), string.substr(start, end-start) );
            start = end+split.size(); end = string.find(split,start);
      } output.insert(output.end(), string.substr(start, end-start) ); 
      if (split2.size()){
            std::vector<std::string> output2;
            for (int i=0; i < output.size(); i++){
                  int start=0; int end=string.find(split2);
                  while (end != -1){
                        output2.insert(output2.end(), output.at(i).substr(start, end-start) );
                        start = end+split.size(); end = output.at(i).find(split2,start);
                  } output2.insert(output2.end(), output.at(i).substr(start, end-start) ); 
            } return output2;
      } return output;
}

std::string exec(const char* cmd){
    std::array<char,128> buffer;
    std::string result;
    std::unique_ptr<FILE,decltype(&pclose)> pipe(popen(cmd,"r"),pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(),buffer.size(), pipe.get()) != NULL) result += buffer.data();
    return result;
}

std::string RSGLAppPath="/usr/share/RSGLApps/";
std::string font = RSGLAppPath+"fonts/DejaVuSans.ttf";
RSGL::rect scrollBar = {935,0,15,40}; int scrollY=0; bool grabbed=false; int oldMouse; RSGL::color scrollColor={0,0,0,20};
void drawImage(std::string file,RSGL::rect r){RSGL::drawImage(file,r);}
void reloadApps(){std::string cmd="ls "+RSGLAppPath+"apps"; std::string app=exec(cmd.c_str()); appDir=split(app," ","\n");}




int main(){ reloadApps();
      while(running){
            win.checkEvents();
            scrollBar.y=scrollY*(600/packages.size());
            switch(win.event.type){
                 case RSGL::quit: running=false; break;
                 case RSGL::MouseButtonPressed:
                        if (win.event.button == RSGL::mouseScrollUp){ if (scrollY) scrollY--; }
                        else if (win.event.button == RSGL::mouseScrollDown){ if (scrollY  < packages.size()-4 ) scrollY++;}
                        else if (win.event.button == 1 && RSGL::RectCollidePoint(scrollBar,{win.event.x,win.event.y})){ oldMouse=win.event.y; grabbed=true;  }
                        break; 
                 case RSGL::MouseButtonReleased:  if (win.event.button == 1) grabbed=false; break;
                 case RSGL::MousePosChanged:
                        if (grabbed){
                              if (oldMouse < win.event.y) if (scrollY < packages.size()-4)   scrollY++; 
                              else if (oldMouse > win.event.y) if (scrollY) scrollY--;
                              oldMouse = win.event.y;
                        }
                 default: break;
            }
            if (RSGL::RectCollidePoint(scrollBar,{win.event.x,win.event.y})) scrollColor.a=255; else scrollColor.a=20;
            for (int i=0; i < packages.size(); i++){
                  RSGL::drawRect({100,i*120 + 50 - (120*scrollY),800,110},{100,100,100}); RSGL::drawRect({100,i*120 + 50 - (120*scrollY),800,110},{0,0,0},false);
                  drawImage(RSGLAppPath+packages.at(i).logo.c_str(),{120,i*120 + 55 - (120*scrollY) , 100,100}); RSGL::drawRect({120,i*120 + 55 - (120*scrollY), 100,100},{0,0,0},false);
                  RSGL::drawText(packages.at(i).name,{250,i*120 + 55 - (120*scrollY), 25},font.c_str(),{0,0,0});
                  std::vector<std::string> texts = split( packages.at(i).discription,"\n");
                  for (int j=0; j < texts.size(); j++) RSGL::drawText(texts.at(j),{250,i*120 + 85 + j*15 - (120*scrollY), 15},font.c_str(),{0,0,0});
                  RSGL::drawText(packages.at(i).author,{250,i*120 + 145 - (120*scrollY), 12},font.c_str(),{0,0,0});
                  RSGL::drawText("size : " + packages.at(i).size,{500,i*120 + 145 - (120*scrollY), 12},font.c_str(),{0,0,0});
                  RSGL::drawCircle({800,i*120 + 85 - (120*scrollY) , 50},{150,0,0});
                  if (hasPackage(packages.at(i))){ 
                        RSGL::drawCircle({715,i*120 + 85 - (120*scrollY) , 50},{150,0,0});
                        for (int j=0; j < 2; j++){
                              RSGL::drawLine({710 + j*30 ,i*120 + 120 - (120*scrollY)},{720+ j*30,i*120 + 130 - (120*scrollY)},{0,0,0});
                              RSGL::drawLine({720+ j*30,i*120 + 130 - (120*scrollY)},{725+ j*30,i*120 + 120 - (120*scrollY)},{0,0,0});
                              RSGL::drawLine({720+ j*30,i*120 + 130 - (120*scrollY)},{720+ j*30,i*120 + 80 - (120*scrollY)},{0,0,0});
                        }
                        RSGL::drawText("X",{810,i*120 + 80 - (120*scrollY) , 65},font.c_str(),{0,0,0});
                        RSGL::drawText("Delete",{800,i*170 + 145 - (140*scrollY) , 15},font.c_str(),{0,0,0});
                        RSGL::drawText("Update",{715,i*170 + 145 - (140*scrollY) , 15},font.c_str(),{0,0,0});
                  }
                  else{
                        RSGL::drawRect({800,i*120 + 120 - (120*scrollY),50,17},{0,0,0},false);
                        RSGL::drawRect({800,i*120 + 120 - (120*scrollY),50,1},{150,0,0},false);
                        RSGL::drawLine({803,i*120 + 120 - (120*scrollY)},{825,i*120 + 130 - (120*scrollY)},{0,0,0});
                        RSGL::drawLine({825,i*120 + 130 - (120*scrollY)},{845,i*120 + 120 - (120*scrollY)},{0,0,0});
                        RSGL::drawLine({825,i*120 + 130 - (120*scrollY)},{825,i*120 + 80 - (120*scrollY)},{0,0,0});
                        RSGL::drawText("Download / Install",{765,i*170 + 145 - (140*scrollY) , 15},font.c_str(),{0,0,0});
                  }          


                  if (win.event.type == RSGL::MouseButtonReleased){
                        if (win.event.button == 1){
                              if (hasPackage(packages.at(i)) && RSGL::CircleCollidePoint({800,i*120 + 85 - (120*scrollY) , 50},{win.event.x,win.event.y})) removePackage(packages.at(i));
                              else if (!hasPackage(packages.at(i)) && RSGL::CircleCollidePoint({800,i*120 + 85 - (120*scrollY) , 50},{win.event.x,win.event.y})) installPackage(packages.at(i));
                              else if (hasPackage(packages.at(i)) && RSGL::CircleCollidePoint({715,i*120 + 85 - (120*scrollY) , 50},{win.event.x,win.event.y})) updatePackage(packages.at(i));
                        }
                  }  
            } 
            RSGL::drawRect(scrollBar,scrollColor);
            win.clear();
      } win.close();
}