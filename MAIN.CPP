#include "c:/_OPENGL_/gl.h"
#include "c:/_STL_/imguigl.hpp"
#include "c:/_STL_/array.hpp"
#include "c:/_STL_/filedlg.hpp"
#include <stdio.h>
#include <math.h>

#define XRES 640
#define YRES 480

#define MINSPRITEWIDTH 1
#define MAXSPRITEWIDTH 64
#define MINSPRITEHEIGHT 1
#define MAXSPRITEHEIGHT 64
#define MINSPRITEZOOM 1.0
#define MAXSPRITEZOOM 10
#define MAXPALETTECOLORS 256
#define MAXPALETTESLOTS 8
#define TOOLSIZEMIN 1
#define TOOLSIZEMAX 20.0
#define MOUSESPEEDMIN 0.25
#define MOUSESPEEDMAX 2.0

bool askForQuit = false;
bool quit = false;

extern double mouseX, mouseY;
extern double mouseDeltaX, mouseDeltaY;
extern double mouseSpeed;
extern int mouseButtons;
extern int currentKey;
double fps;

unsigned int defaultPal[256]={
     0xff000000,
     0xff111111,
     0xff222222,
     0xff333333,
     0xff444444,
     0xff555555,
     0xff666666,
     0xff777777,
     0xff888888,
     0xff999999,
     0xffaaaaaa,
     0xffbbbbbb,
     0xffcccccc,
     0xffdddddd,
     0xffeeeeee,
     0xffffffff,
     0xff7f7f00,
     0xffbfbf3f,
     0xffffff00,
     0xffffffbf,
     0xffff8181,
     0xffff0000,
     0xffbf3f3f,
     0xff7f0000,
     0xff500f0f,
     0xff7f007f,
     0xffbf3fbf,
     0xfff500f5,
     0xffff81fd,
     0xffcbc0ff,
     0xff8181ff,
     0xff0000ff,
     0xff3f3fbf,
     0xff00007f,
     0xff141455,
     0xff003f7f,
     0xff3f7fbf,
     0xff007fff,
     0xff81bfff,
     0xffbfffff,
     0xff00ffff,
     0xff3fbfbf,
     0xff007f7f,
     0xff007f00,
     0xff3fbf3f,
     0xff00ff00,
     0xffafffaf,
     0xffffbf00,
     0xffff7f00,
     0xffc87d4b,
     0xffc0afbc,
     0xff89aacb,
     0xff90a0a6,
     0xff94947e,
     0xff87826e,
     0xff606e7e,
     0xff5f69a0,
     0xff7278c0,
     0xff748ad0,
     0xff7d9be1,
     0xff8caaeb,
     0xff9bb9f5,
     0xffafc8f6,
     0xffd2e1f5,
     0xffff007f,
     0xff3b3b57,
     0xff3c4173,
     0xff55558e,
     0xff7373ab,
     0xff8f8fc7,
     0xffababe3,
     0xffdad2f8,
     0xffabc7e3,
     0xff739ec4,
     0xff57738f,
     0xff3b5773,
     0xff1f2d3b,
     0xff234141,
     0xff3b7373,
     0xff578f8f,
     0xff55a2a2,
     0xff72b5b5,
     0xff8fc7c7,
     0xffabdada,
     0xffc7eded,
     0xffabe3c7,
     0xff8fc7ab,
     0xff55be8e,
     0xff578f73,
     0xff3e7d58,
     0xff325046,
     0xff0f1e19,
     0xff375023,
     0xff3b573b,
     0xff506450,
     0xff49733b,
     0xff578f57,
     0xff73ab73,
     0xff82c064,
     0xff8fc78f,
     0xffa2d8a2,
     0xfffaf8e1,
     0xffcaeeb4,
     0xffc5e3ab,
     0xff8eb487,
     0xff5f7d50,
     0xff46690f,
     0xff232d1e,
     0xff464123,
     0xff73733b,
     0xffabab64,
     0xffc7c78f,
     0xffe3e3ab,
     0xfff1f1c7,
     0xfff0d2be,
     0xffe3c7ab,
     0xffdcb9a8,
     0xffc7ab8f,
     0xffc78f57,
     0xff8f7357,
     0xff73573b,
     0xff2d190f,
     0xff3b1f1f,
     0xff573b3b,
     0xff734949,
     0xff8f5757,
     0xffaa6e73,
     0xffca7676,
     0xffc78f8f,
     0xffe3abab,
     0xfff8dad0,
     0xffffe3e3,
     0xffc78fab,
     0xffc7578f,
     0xff8f5773,
     0xff733b57,
     0xff3c233c,
     0xff463246,
     0xff724072,
     0xff8f578f,
     0xffab57ab,
     0xffab73ab,
     0xffe1aceb,
     0xfff5dcff,
     0xffe3c7e3,
     0xffd2b9e1,
     0xffbea0d7,
     0xffb98fc7,
     0xffa07dc8,
     0xff915ac3,
     0xff37284b,
     0xff231632,
     0xff1e0a28,
     0xff111840,
     0xff001862,
     0xff0a14a5,
     0xff1020da,
     0xff4a52d5,
     0xff0a3cff,
     0xff325af5,
     0xff6262ff,
     0xff31bdf6,
     0xff3ca5ff,
     0xff0f9bd7,
     0xff0a6eda,
     0xff005ab4,
     0xff054ba0,
     0xff14325f,
     0xff0a5053,
     0xff006262,
     0xff5a808c,
     0xff0094ac,
     0xff0ab1b1,
     0xff5ad5e6,
     0xff10d5ff,
     0xff4aeaff,
     0xff41ffc8,
     0xff46f09b,
     0xff19dc96,
     0xff05c873,
     0xff05a86a,
     0xff146e3c,
     0xff053428,
     0xff084620,
     0xff0c5c0c,
     0xff059614,
     0xff0ad70a,
     0xff0ae614,
     0xff73ff7d,
     0xff5af04b,
     0xff14c500,
     0xff50b405,
     0xff4e8c1c,
     0xff323812,
     0xff809812,
     0xff91c406,
     0xff6ade00,
     0xffa8eb2d,
     0xffa5fe3c,
     0xffcdff6a,
     0xffffeb91,
     0xffffe655,
     0xfff0d77d,
     0xffd5de08,
     0xffde9c10,
     0xff5c5a05,
     0xff522c16,
     0xff7d370f,
     0xff9c4a00,
     0xff966432,
     0xfff65200,
     0xffbd6a18,
     0xffdc7823,
     0xffc39d69,
     0xffffa44a,
     0xffffb090,
     0xffffc55a,
     0xfffab9be,
     0xfff06e78,
     0xffff5a4a,
     0xfff64162,
     0xfff53c3c,
     0xffda1c10,
     0xffbd1000,
     0xff941023,
     0xff48210c,
     0xffb01050,
     0xffd01060,
     0xffd23287,
     0xffff419c,
     0xffff62bd,
     0xffff91b9,
     0xffffa5d7,
     0xfffac3d7,
     0xfffcc6f8,
     0xffff73e6,
     0xffff52ff,
     0xffe020da,
     0xffff29bd,
     0xffc510bd,
     0xffbe148c,
     0xff7b185a,
     0xff641464,
     0xff620041,
     0xff460a32,
     0xff371955,
     0xff8219a0,
     0xff7800c8,
     0xffbf50ff,
     0xffc56aff,
     0xffb9a0fa,
     0xff8c3afc,
     0xff781ee6,
     0xff3910bd,
     0xff4d3498,
     0xff371491,
};

enum {  
  LoadSave_None = 0,
  LoadSave_LoadPalette = 1,
  LoadSave_SavePalette = 2,
  LoadSave_LoadSprite = 3,
  LoadSave_SaveSprite = 4,
  LoadSave_LoadSpriteSheet = 5,
  LoadSave_SaveSpriteSheet = 6,
};

int loadSave = LoadSave_None;

unsigned int temp[MAXSPRITEHEIGHT*MAXSPRITEWIDTH];
unsigned int clipBoard[MAXSPRITEHEIGHT*MAXSPRITEWIDTH];
int clipBoardW=0;
int clipBoardH=0;
unsigned int overlay[MAXSPRITEWIDTH*MAXSPRITEHEIGHT] = {0};

class SpriteCanvas {
public:
  int spriteNr;
  float zoom;
  float aspect;
  bool gridOn;
  bool background;
  bool doublePixel;
  SpriteCanvas() {
    spriteNr = 0;
    zoom = 5.f;
    aspect = 4.0/3.0*9.0/16.0;
    gridOn = false;
    background = false;
    doublePixel = false;
  }
};

SpriteCanvas &getCurrentSpriteCanvas();

class Sprite {
public:
  int width,height;
  char name[10];
  Array<unsigned int> bitmap;
  GLuint textureId;
  bool mark;
  Sprite() {
    //memset(bitmap,0x00,MAXSPRITEWIDTH*MAXSPRITEHEIGHT*sizeof(unsigned int));
    bitmap.resize(MAXSPRITEWIDTH*MAXSPRITEHEIGHT);
    width = 64;
    height = 64;
    sprintf(name,"noname");
    textureId = 0;
    mark = false;
  }
  void update() {
    mark = true;
  }
  GLuint texture() {
    if (mark) {
      glDeleteTextures(1,&textureId);
      textureId = 0;
    }
    if (textureId == 0) {
      bool doublePixel = getCurrentSpriteCanvas().doublePixel;
      if (doublePixel) {
        for(int y = 0; y < MAXSPRITEHEIGHT; y++) {
          for(int x = 0; x < MAXSPRITEWIDTH; x += 2) {
            bitmap[x+1+y*MAXSPRITEWIDTH] = bitmap[x+y*MAXSPRITEWIDTH];
          }
        }
      }
      glGenTextures(1, &textureId);
      glBindTexture(GL_TEXTURE_2D, textureId);
      bool bg = getCurrentSpriteCanvas().background;
       for (int i = 0; i < MAXSPRITEWIDTH*MAXSPRITEHEIGHT; i++) {
        int r1 = (bitmap[i]>>0) & 255;
        int g1 = (bitmap[i]>>8) & 255;
        int b1 = (bitmap[i]>>16) & 255;
        int a1 = (bitmap[i]>>24) & 255;
        int r2 = (overlay[i]>>0) & 255;
        int g2 = (overlay[i]>>8) & 255;
        int b2 = (overlay[i]>>16) & 255;
        int a2 = (overlay[i]>>24) & 255;
        int r = ((r2 - r1)*a2)/255+r1;
        int g = ((g2 - g1)*a2)/255+g1;
        int b = ((b2 - b1)*a2)/255+b1;
        int a = ((a2 - a1)*a2)/255+a1;
        temp[i] = r|(g<<8)|(b<<16)|(a<<24);
        if (bg && ((temp[i]>>24)<2)) {
          temp[i] = ((((i+i/MAXSPRITEWIDTH) % 3)+1)*0x00202020)|0xff000000;
        }
      }
      glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,MAXSPRITEWIDTH,MAXSPRITEHEIGHT,0,GL_RGBA,GL_UNSIGNED_BYTE,temp);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    }
    return textureId;
  }
};

class Palette {
public:
  int slot;
  int width;
  int height;
  unsigned int slotColors[MAXPALETTESLOTS];
  unsigned int colors[MAXPALETTECOLORS*2]; // *2 because of truncated width/height
  GLuint textureId;
  bool mark;
  Palette() {
    slot = 0;
    textureId = 0;
    width = sqrt(MAXPALETTECOLORS);
    height = (MAXPALETTECOLORS+width-1)/width;
    for (int i = 0; i < MAXPALETTECOLORS; i++) colors[i] = defaultPal[i];
    for (int j = 0; j < MAXPALETTESLOTS; j++) slotColors[j] = colors[j];
    slotColors[0] = colors[15];
    mark = false;
  }
  void update() {
    mark = true;
  }
  GLuint paletteTexture() {
    if (mark) {
      glDeleteTextures(1,&textureId);
      textureId = 0;
    }
    if (textureId == 0) {
      glGenTextures(1, &textureId);
      glBindTexture(GL_TEXTURE_2D, textureId);
      glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,colors);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    return textureId;
  }
};

#define TOOL_PENCIL 1
#define TOOL_FLOODFILL 2
#define TOOL_RECT 3
#define TOOL_CIRCLE 4
#define TOOL_LINE 5
#define TOOL_MOVE 6
#define TOOL_COPY 7
#define TOOL_SCALE 8

class Tool {
public:
  float size;
  float mouseSpeed;
  bool filled;
  int tool;
  Tool() {
    size = 1.f;
    mouseSpeed = 1.f;
    tool = TOOL_PENCIL;
    filled = true;
  }
  char *getName() {
    switch(tool) {
    case TOOL_PENCIL: return "Pencil";
    case TOOL_FLOODFILL: return "FloodFill";
    case TOOL_RECT: return "Rectangle";
    case TOOL_CIRCLE: return "Circle";
    case TOOL_LINE: return "Line";
    case TOOL_MOVE: return "Move";
    case TOOL_COPY: return "Copy";
    case TOOL_SCALE: return "Scale";
    };
    return "<NONE>";
  }
};

class SpriteSheet {
public:
  int xScroll;
};

Array<Sprite> sprites;
SpriteCanvas spriteCanvas;
Palette palette;
Tool tool;
SpriteSheet spriteSheet;

Array<Sprite> sprites_undo;
SpriteCanvas spriteCanvas_undo;
Palette palette_undo;
Tool tool_undo;

void makeUndoPoint() {
  sprites_undo = sprites;
  spriteCanvas_undo = spriteCanvas;
  palette_undo = palette;
  tool_undo = tool;
}


void undo() {
  sprites = sprites_undo;
  spriteCanvas = spriteCanvas_undo;
  palette = palette_undo;
  tool = tool_undo;
  for (int i = 0; i < sprites.size(); i++) sprites[i].update();
}

SpriteSheet &getCurrentSpriteSheet() {
  return spriteSheet;
}

SpriteCanvas &getCurrentSpriteCanvas() {
  return spriteCanvas;
}

Palette &getCurrentPalette() {
  return palette;
}

Tool &getCurrentTool() {
  return tool;
}

Sprite &getCurrentSprite() {
  return sprites[getCurrentSpriteCanvas().spriteNr];
}

template<class T>
T clamp(const T &v, const T &mn, const T &mx) {
  if (v < mn) return mn;
  if (v > mx) return mx;
  return v;
}

void toColorString(char *txt, unsigned int color) {
  sprintf(txt,"#%08x",color);
}

unsigned int fromColorString(char *txt) {
  unsigned int color = 0;
  for (int i = 0; i < 10; i++) {
    if (txt[i]>='0'&&txt[i]<='9') {color=color*16+txt[i]-'0';}
    if (txt[i]>='a'&&txt[i]<='f') {color=color*16+txt[i]-'a'+10;}
    if (txt[i]>='A'&&txt[i]<='F') {color=color*16+txt[i]-'A'+10;}
  }
  return color;
}

bool toFile(const char *fileName, void *data, int dataSize) {
  FILE *out = fopen(fileName,"wb");
  if (out == NULL) return false;
  fwrite(data,1,dataSize,out);
  fclose(out);
  return true;
}

bool fromFile(const char *fileName, void *data, int dataSize) {
  FILE *in = fopen(fileName,"rb");
  if (in == NULL) return false;
  fread(data,1,dataSize,in);
  fclose(in);
  return true;
}

bool saveSprite(const char *fileName) {
  FILE *out = fopen(fileName,"wb");
  if (out == NULL) return false;
  unsigned char w = getCurrentSprite().width;
  unsigned char h = getCurrentSprite().height;
  char s1 = 'S';
  char s2 = 'P';
  fwrite(&s1,1,1,out);
  fwrite(&s2,1,1,out);
  fwrite(&w,1,1,out);
  fwrite(&h,1,1,out);
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      fwrite(&getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH],1,4,out);
    }
  }
  fclose(out);
  return true;
}

bool loadSprite(const char *fileName) {
  FILE *in = fopen(fileName,"rb");
  if (in == NULL) return false;
  char s1,s2;
  unsigned char w,h;
  fread(&s1,1,1,in);
  fread(&s2,1,1,in);
  if (s1 != 'S' || s2 != 'P') return false;
  fread(&w,1,1,in);
  fread(&h,1,1,in);
  if (w > MAXSPRITEWIDTH) return false;
  if (h > MAXSPRITEHEIGHT) return false;
  getCurrentSprite().width = w;
  getCurrentSprite().height = h;
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      fread(&getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH],1,4,in);
    }
  }
  fclose(in);
  getCurrentSprite().update();
  return true;
}

bool saveSpriteSheet(const char *fileName) {
  FILE *out = fopen(fileName,"wb");
  if (out == NULL) return false;
  char s1 = 'S';
  char s2 = 'S';
  fwrite(&s1,1,1,out);
  fwrite(&s2,1,1,out);
  unsigned short c = sprites.size();
  fwrite(&c,1,2,out);
  for (int i = 0; i < c; i++) {
    for (int j = 0; j < 10; j++) {
      char c = sprites[i].name[j];
      fwrite(&c,1,1,out);
    }
    unsigned int *data = &sprites[i].bitmap[0];
    unsigned char w = sprites[i].width;
    unsigned char h = sprites[i].height;
    char s1 = 'S';
    char s2 = 'P';
    fwrite(&s1,1,1,out);
    fwrite(&s2,1,1,out);
    fwrite(&w,1,1,out);
    fwrite(&h,1,1,out);
    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        fwrite(&sprites[i].bitmap[x+y*MAXSPRITEWIDTH],1,4,out);
      }
    }
  }
  return true;
}

bool loadSpriteSheet(const char *fileName) {
  FILE *in = fopen(fileName,"rb");
  if (in == NULL) return false;
  char s1,s2;
  unsigned char w,h;
  fread(&s1,1,1,in);
  fread(&s2,1,1,in);
  if (s1 != 'S' || s2 != 'S') return false;
  unsigned short count=0;
  fread(&count,1,2,in);
  sprites.clear();
  for (int i = 0; i < count; i++) {
    sprites.push_back(Sprite());
    for (int j = 0; j < 10; j++) {
      char c;
      fread(&c,1,1,in);
      sprites[i].name[j] = c;
    }
    char s1,s2;
    unsigned char w,h;
    fread(&s1,1,1,in);
    fread(&s2,1,1,in);
    if (s1 != 'S' || s2 != 'P') return false;
    fread(&w,1,1,in);
    fread(&h,1,1,in);
    if (w > MAXSPRITEWIDTH) return false;
    if (h > MAXSPRITEHEIGHT) return false;
    sprites[i].width = w;
    sprites[i].height = h;
    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        fread(&sprites[i].bitmap[x+y*MAXSPRITEWIDTH],1,4,in);
      }
    }
    sprites[i].update();
    sprites[i].texture();
  }
  fclose(in);
  return true;
}

double round2(double v) {
  return floor(v - 0.5);
}

void displayMenuBar() {
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10,1));
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg,ImVec4(0xff/255.f,0xff/255.f,0xff/255.f,0xff/255.f));
    ImGui::PushStyleColor(ImGuiCol_Text,ImVec4(0x20/255.f,0x20/255.f,0x20/255.f,0xff/255.f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg,ImVec4(0xff/255.f,0xff/255.f,0xff/255.f,0xff/255.f));
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Load palette")) loadSave = LoadSave_LoadPalette;
        if (ImGui::MenuItem("Save palette")) loadSave = LoadSave_SavePalette;
        ImGui::Separator();
        if (ImGui::MenuItem("Load sprite")) loadSave = LoadSave_LoadSprite;
        if (ImGui::MenuItem("Save sprite")) loadSave = LoadSave_SaveSprite;
        ImGui::Separator();
        if (ImGui::MenuItem("Load sprite sheet")) loadSave = LoadSave_LoadSpriteSheet;
        if (ImGui::MenuItem("Save sprite sheet")) loadSave = LoadSave_SaveSpriteSheet;
        ImGui::Separator();
        bool &db = getCurrentSpriteCanvas().doublePixel;
        if (db && ImGui::MenuItem("DoublePixels off##DoublePixelsOnOff")) {
          db = !db;
        } else
        if ((!db) && ImGui::MenuItem("DoublePixels on##DoublePixelsOnOff")) {
          db = !db;
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Quit")) {
          askForQuit = true;
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Edit")) {
        if (ImGui::MenuItem("Undo")) {
          undo();
        }
        if (ImGui::MenuItem("Clear")) {
          unsigned int col = 0x00;
          for (int y = 0; y < MAXSPRITEHEIGHT; y++) {
            for (int x = 0; x < MAXSPRITEWIDTH; x++) {
              getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH] = col;
            }
          }        
          getCurrentSprite().update();
        }
        if (ImGui::MenuItem("Fill")) {
          unsigned int col = getCurrentPalette().slotColors[getCurrentPalette().slot];
          for (int y = 0; y < MAXSPRITEHEIGHT; y++) {
            for (int x = 0; x < MAXSPRITEWIDTH; x++) {
              getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH] = col;
            }
          }        
          getCurrentSprite().update();
        }
        if (ImGui::MenuItem("Flip Horizontal")) {
          int y;
          for (y = 0; y < MAXSPRITEHEIGHT; y++) {
            for (int x = 0; x < getCurrentSprite().width; x++) {
              temp[getCurrentSprite().width-1-x+y*MAXSPRITEWIDTH]=getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH];
            }
          }        
          for (y = 0; y < MAXSPRITEHEIGHT; y++) {
            for (int x = 0; x < getCurrentSprite().width; x++) {
              getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH]=temp[x+y*MAXSPRITEWIDTH];
            }
          }        
          getCurrentSprite().update();
        }
        if (ImGui::MenuItem("Flip Vertical")) {
          int y;
          for (y = 0; y < getCurrentSprite().height; y++) {
            for (int x = 0; x < MAXSPRITEWIDTH; x++) {
              temp[x+(getCurrentSprite().height-1-y)*MAXSPRITEWIDTH]=getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH];
            }
          }        
          for (y = 0; y < getCurrentSprite().height; y++) {
            for (int x = 0; x < MAXSPRITEWIDTH; x++) {
              getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH]=temp[x+y*MAXSPRITEWIDTH];
            }
          }        
          getCurrentSprite().update();
        }
        if (ImGui::MenuItem("Rotate 90 Degrees")) {
          int y;
          for (y = 0; y < getCurrentSprite().height; y++) {
            for (int x = 0; x < getCurrentSprite().width; x++) {
              temp[y+x*MAXSPRITEWIDTH]=getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH];
            }
          }        
          for (y = 0; y < MAXSPRITEHEIGHT; y++) {
            for (int x = 0; x < MAXSPRITEWIDTH; x++) {
              getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH]=temp[x+y*MAXSPRITEWIDTH];
            }
          }        
          // und noch ein Flip Horizontal
          for (y = 0; y < MAXSPRITEHEIGHT; y++) {
            for (int x = 0; x < getCurrentSprite().width; x++) {
              temp[getCurrentSprite().width-1-x+y*MAXSPRITEWIDTH]=getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH];
            }
          }        
          for (y = 0; y < MAXSPRITEHEIGHT; y++) {
            for (int x = 0; x < getCurrentSprite().width; x++) {
              getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH]=temp[x+y*MAXSPRITEWIDTH];
            }
          }        
          getCurrentSprite().update();
        }
        ImGui::EndMenu();
      }
      ImGui::Text("  || %s || FPS:%d",currentTimeString().c_str(),(int)fps);
      ImGui::EndMainMenuBar();
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

void displayQuitBox() {
  if (askForQuit) {
    ImGui::SetNextWindowFocus();
    ImGui::SetNextWindowPos(ImVec2((XRES-50)/2,(YRES-40)/2));
    ImGui::Begin("Quit?",NULL,ImVec2(50,40),-1,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoCollapse);
    if(ImGui::Button("Yes") || currentKey == 'y' || currentKey == GL_VK_ENTER) {
      quit = true;
    }
    ImGui::SameLine();
    if(ImGui::Button("No") || currentKey == 'n' || currentKey == GL_VK_ESCAPE) {
      askForQuit = false;
    }
    ImGui::End();
  } else {
    if (currentKey == GL_VK_ESCAPE) askForQuit = true;
  }
}


void plot(unsigned int *dest, int x, int y, unsigned int col) {
  int w = getCurrentSprite().width;
  int h = getCurrentSprite().height;
  double toolSize = getCurrentTool().size;
  for (double radius = 0; radius < toolSize*0.5; radius += 0.5) {
    double u = 4 * radius * 3.1415927;
    for (double angle = 0; angle < 2.f * 3.1415927; angle += 2.f * 3.1415927/u) {
      int nx = sin(angle)*radius+x;
      int ny = cos(angle)*radius+y;
      if (nx >= 0 && ny >= 0 && nx < w && ny < h) {
        dest[nx+ny*w] = col;
      }
    }
  }
}

void handleColorGet(int mx, int my) {
  int w = getCurrentSprite().width;
  int h = getCurrentSprite().height;
  if (mx >= 0 && mx < w && my >= 0 && my < h) {
    if (mouseButtons & 2) {
      unsigned int col = getCurrentSprite().bitmap[mx+my*w];
      getCurrentPalette().slot = 0;
      getCurrentPalette().slotColors[getCurrentPalette().slot] = col;
    }
  }
}

int toolWas = 0;
int mouseButtonsWas = 0;

int doublePixel(int mx) {
  if (getCurrentSpriteCanvas().doublePixel)
    mx = mx & (~1);
  return mx;
}

int doublePixel2(int mx) {
  if (getCurrentSpriteCanvas().doublePixel)
    mx = (mx & (~1))-1;
  return mx;
}

void handleToolPencil(int mx, int my) {
  handleColorGet(mx,my);
  static int lastMx = mx;
  static int lastMy = my;
  int w = getCurrentSprite().width;
  int h = getCurrentSprite().height;
  if (mx >= 0 && mx < w && my >= 0 && my < h && lastMx >= 0 && lastMx < w && lastMy >= 0 && lastMy < h) {
    mx = doublePixel(mx);

    memset(overlay,0,sizeof(overlay));
    unsigned int col = getCurrentPalette().slotColors[getCurrentPalette().slot];
    plot(overlay,mx,my,col);
    getCurrentSprite().update();
    getCurrentSprite().texture();
    memset(overlay,0,sizeof(overlay));

    if ((!(mouseButtonsWas & 1)) && (mouseButtons & 1)) makeUndoPoint();

    if (mouseButtons & 1) {
      double dx = lastMx - mx;
      double dy = lastMy - my;
      double d = fabs(dx);
      if (fabs(dy)>d) d = fabs(dy);
      if (d < 1) d = 1;
      dx /= d;
      dy /= d;
      for (int i = 0; i < d+1; i++) {
        int kx = mx+dx*i;
        int ky = my+dy*i;
        plot(&getCurrentSprite().bitmap[0],kx,ky,col);
      }
      getCurrentSprite().update();
    }
  }
  lastMx = mx;
  lastMy = my;
}

void handleToolFloodFill(int mx, int my) {
  int w = getCurrentSprite().width;
  int h = getCurrentSprite().height;
  if (mx >= 0 && mx < w && my >= 0 && my < h) {
    mx = doublePixel(mx);

    memset(overlay,0,sizeof(overlay));
    unsigned int col = getCurrentPalette().slotColors[getCurrentPalette().slot];
    plot(overlay,mx,my,col);
    getCurrentSprite().update();
    getCurrentSprite().texture();
    memset(overlay,0,sizeof(overlay));

    if ((!(mouseButtonsWas & 1)) && (mouseButtons & 1)) makeUndoPoint();

    if (mouseButtons & 1) {
      unsigned int toFill = getCurrentSprite().bitmap[mx+my*MAXSPRITEWIDTH];
      unsigned int fillColor = 0x00fefefe;
      getCurrentSprite().bitmap[mx+my*MAXSPRITEWIDTH] = fillColor;
      bool toFillOn;
      do {
        toFillOn = false;
        for (int y = 0; y < h; y++) {
          for (int x = 0; x < w; x++) {
            unsigned int colHere = getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH];
            if (colHere == fillColor) {
              if(x+1<w&&getCurrentSprite().bitmap[x+1+y*MAXSPRITEWIDTH]==toFill) {toFillOn=true;getCurrentSprite().bitmap[x+1+y*MAXSPRITEWIDTH]=fillColor;}
              if(x-1>=0&&getCurrentSprite().bitmap[x-1+y*MAXSPRITEWIDTH]==toFill) {toFillOn=true;getCurrentSprite().bitmap[x-1+y*MAXSPRITEWIDTH]=fillColor;}
              if(y+1<h&&getCurrentSprite().bitmap[x+(y+1)*MAXSPRITEWIDTH]==toFill) {toFillOn=true;getCurrentSprite().bitmap[x+(y+1)*MAXSPRITEWIDTH]=fillColor;}
              if(y-1>=0&&getCurrentSprite().bitmap[x+(y-1)*MAXSPRITEWIDTH]==toFill) {toFillOn=true;getCurrentSprite().bitmap[x+(y-1)*MAXSPRITEWIDTH]=fillColor;}
            }
          }
        }
      } while(toFillOn);
      unsigned int newColor = getCurrentPalette().slotColors[getCurrentPalette().slot];
      for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
          unsigned int colHere = getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH];
          if (colHere == fillColor) {
            getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH]=newColor;
          }
        }
      }
    }
  }
}

void handleToolRect(int mx, int my) {
  int w = getCurrentSprite().width;
  int h = getCurrentSprite().height;
  static int firstX;
  static int firstY;
  static int mode;
  if (toolWas != TOOL_RECT || (mouseButtons & 2)) {
    mode = 0;
  }
  if (mx >= 0 && mx < w && my >= 0 && my < h) {
    mx = doublePixel(mx);

    if (mouseButtons & 1) {
      if (mode == 0) {
        firstX = mx;
        firstY = my;
        mode++;
      }
      int secondX = mx;
      int secondY = my;
      int startX = firstX;
      int startY = firstY;
      int endX = secondX;
      int endY = secondY;
      if (startX>endX) {int t = startX; startX = endX; endX = t;}
      if (startY>endY) {int t = startY; startY = endY; endY = t;}
      memset(overlay,0,sizeof(overlay));
      unsigned int col = getCurrentPalette().slotColors[getCurrentPalette().slot];
      col |= 0xff000000;
      col &= 0x7fffffff;
      if (getCurrentTool().filled) {
        for (int x = startX; x <= endX; x++) {
          for (int y = startY; y <= endY; y++) {
              overlay[x+y*MAXSPRITEWIDTH]=col;
          }
        }
      } else {
        for (int x = startX; x <= endX; x++) {
          plot(overlay,x,startY,col);
          plot(overlay,x,endY,col);
        }
        for (int y = startY; y <= endY; y++) {
          plot(overlay,startX,y,col);
          plot(overlay,endX,y,col);
        }
      }
      getCurrentSprite().update();
      getCurrentSprite().texture();
      memset(overlay,0,sizeof(overlay));
    } else {
      if (mode == 1) {
        makeUndoPoint();
        mode = 0;
        unsigned int col = getCurrentPalette().slotColors[getCurrentPalette().slot];
        int secondX = mx;
        int secondY = my;
        int startX = firstX;
        int startY = firstY;
        int endX = secondX;
        int endY = secondY;
        if (startX>endX) {int t = startX; startX = endX; endX = t;}
        if (startY>endY) {int t = startY; startY = endY; endY = t;}
        if (getCurrentTool().filled) {
          for (int x = startX; x <= endX; x++) {
            for (int y = startY; y <= endY; y++) {
                getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH]=col;
            }
          }
        } else {
          unsigned int *k = &getCurrentSprite().bitmap[0];
          for (int x = startX; x <= endX; x++) {
            plot(k,x,startY,col);
            plot(k,x,endY,col);
          }
          for (int y = startY; y <= endY; y++) {
            plot(k,startX,y,col);
            plot(k,endX,y,col);
          }
        }
        getCurrentSprite().update();
      }
    }
  }
}

void handleToolCircle(int mx, int my) {
  int w = getCurrentSprite().width;
  int h = getCurrentSprite().height;
  static int firstX;
  static int firstY;
  static int mode;
  if (toolWas != TOOL_CIRCLE || (mouseButtons & 2)) {
    mode = 0;
  }
  if (mx >= 0 && mx < w && my >= 0 && my < h) {
    mx = doublePixel(mx);

    if (mouseButtons & 1) {
      if (mode == 0) {
        firstX = mx;
        firstY = my;
        mode++;
      }
      int secondX = mx;
      int secondY = my;
      int startX = firstX;
      int startY = firstY;
      int endX = secondX;
      int endY = secondY;
      if (startX>endX) {int t = startX; startX = endX; endX = t;}
      if (startY>endY) {int t = startY; startY = endY; endY = t;}
      memset(overlay,0,sizeof(overlay));
      unsigned int col = getCurrentPalette().slotColors[getCurrentPalette().slot];
      col |= 0xff000000;
      col &= 0x7fffffff;
      if (getCurrentTool().filled) {
        for (int x = startX; x <= endX; x++) {
          for (int y = startY; y <= endY; y++) {
            double fx = (double)(x - startX+0.5)/(endX-startX+1)*2.0-1.0;
            double fy = (double)(y - startY+0.5)/(endY-startY+1)*2.0-1.0;
            double d = sqrt(fx*fx+fy*fy);
            if(d<=1.0)
              overlay[x+y*MAXSPRITEWIDTH]=col;
          }
        }
      } else {
        int k = 1024;
        for (int i = 0; i < k; i++) {
          double fk = (double)i / k * 2.f * 3.1415927;
          int x = sin(fk)*(endX-startX)+startX+0.5;
          int y = cos(fk)*(endY-startY)+startY+0.5;
          plot(overlay,x,y,col);
        }
      }
      getCurrentSprite().update();
      getCurrentSprite().texture();
      memset(overlay,0,sizeof(overlay));
    } else {
      if (mode == 1) {
        makeUndoPoint();
        mode = 0;
        unsigned int col = getCurrentPalette().slotColors[getCurrentPalette().slot];
        int secondX = mx;
        int secondY = my;
        int startX = firstX;
        int startY = firstY;
        int endX = secondX;
        int endY = secondY;
        if (startX>endX) {int t = startX; startX = endX; endX = t;}
        if (startY>endY) {int t = startY; startY = endY; endY = t;}
        if (getCurrentTool().filled) {
          for (int x = startX; x <= endX; x++) {
            for (int y = startY; y <= endY; y++) {
              double fx = (double)(x - startX+0.5)/(endX-startX+1)*2.0-1.0;
              double fy = (double)(y - startY+0.5)/(endY-startY+1)*2.0-1.0;
              double d = sqrt(fx*fx+fy*fy);
              if(d<=1.0)
                getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH]=col;
            }
          }
        } else {
          unsigned int *k2 = &getCurrentSprite().bitmap[0];
          int k = 1024;
          for (int i = 0; i < k; i++) {
            double fk = (double)i / k * 2.f * 3.1415927;
            int x = sin(fk)*(endX-startX)+startX+0.5;
            int y = cos(fk)*(endY-startY)+startY+0.5;
            plot(k2,x,y,col);
          }
        }
        getCurrentSprite().update();
      }
    }
  }
}

void handleToolLine(int mx, int my) {
  int w = getCurrentSprite().width;
  int h = getCurrentSprite().height;
  static int firstX;
  static int firstY;
  static int mode;
  if (toolWas != TOOL_LINE || (mouseButtons & 2)) {
    mode = 0;
  }
  if (mx >= 0 && mx < w && my >= 0 && my < h) {
    mx = doublePixel(mx);

    if (mouseButtons & 1) {
      if (mode == 0) {
        firstX = mx;
        firstY = my;
        mode++;
      }
      int secondX = mx;
      int secondY = my;
      int startX = firstX;
      int startY = firstY;
      int endX = secondX;
      int endY = secondY;
      memset(overlay,0,sizeof(overlay));
      unsigned int col = getCurrentPalette().slotColors[getCurrentPalette().slot];
      col |= 0xff000000;
      col &= 0x7fffffff;
      int k = 1024;
      for (int i = 0; i < k; i++) {
        double fk = (double)i / k;
        int x = fk*(endX-startX)+startX+0.5;
        int y = fk*(endY-startY)+startY+0.5;
        plot(overlay,x,y,col);
      }
      getCurrentSprite().update();
      getCurrentSprite().texture();
      memset(overlay,0,sizeof(overlay));
    } else {
      if (mode == 1) {
        mode = 0;
        makeUndoPoint();
        unsigned int col = getCurrentPalette().slotColors[getCurrentPalette().slot];
        int secondX = mx;
        int secondY = my;
        int startX = firstX;
        int startY = firstY;
        int endX = secondX;
        int endY = secondY;
        unsigned int *k2 = &getCurrentSprite().bitmap[0];
        int k = 1024;
        for (int i = 0; i < k; i++) {
          double fk = (double)i / k;
          int x = fk*(endX-startX)+startX+0.5;
          int y = fk*(endY-startY)+startY+0.5;
          plot(k2,x,y,col);
        }
        getCurrentSprite().update();
      }
    }
  }
}

void handleToolMove(int mx, int my) {
  int w = getCurrentSprite().width;
  int h = getCurrentSprite().height;
  static int firstX;
  static int firstY;
  static int mode;
  if (toolWas != TOOL_MOVE || (mouseButtons & 2)) {
    mode = 0;
  }
  if (mx >= 0 && mx < w && my >= 0 && my < h) {
    mx = doublePixel(mx);

    if (mouseButtons & 1) {
      if (mode == 0) {
        firstX = mx;
        firstY = my;
        mode++;
      }
      if (mode >= 2) {
        mode = 3;
        return;
      }
      int secondX = doublePixel2(mx);
      int secondY = my;
      int startX = firstX;
      int startY = firstY;
      int endX = secondX;
      int endY = secondY;
      if (startX>endX) {int t = startX; startX = endX; endX = t;}
      if (startY>endY) {int t = startY; startY = endY; endY = t;}
      memset(overlay,0,sizeof(overlay));
      unsigned int col = 0x7fffffff;
      if (getCurrentTool().filled) {
        for (int x = startX; x <= endX; x++) {
          for (int y = startY; y <= endY; y++) {
              overlay[x+y*MAXSPRITEWIDTH]=col;
          }
        }
      } else {
        for (int x = startX; x <= endX; x++) {
          plot(overlay,x,startY,col);
          plot(overlay,x,endY,col);
        }
        for (int y = startY; y <= endY; y++) {
          plot(overlay,startX,y,col);
          plot(overlay,endX,y,col);
        }
      }
      getCurrentSprite().update();
      getCurrentSprite().texture();
      memset(overlay,0,sizeof(overlay));
    } else {
      if (mode == 1) {
        mode = 2;
        int secondX = doublePixel2(mx);
        int secondY = my;
        int startX = firstX;
        int startY = firstY;
        int endX = secondX;
        int endY = secondY;
        if (startX>endX) {int t = startX; startX = endX; endX = t;}
        if (startY>endY) {int t = startY; startY = endY; endY = t;}
        unsigned int *k2 = &getCurrentSprite().bitmap[0];
        clipBoardW = endX-startX+1;
        clipBoardH = endY-startY+1;
        for (int y = startY; y <= endY; y++) {
          for (int x = startX; x <= endX; x++) {
            clipBoard[x-startX+(y-startY)*MAXSPRITEWIDTH] = k2[x+y*MAXSPRITEWIDTH];
            k2[x+y*MAXSPRITEWIDTH] = 0x00000000;
          }
        }
        getCurrentSprite().update();
        getCurrentSprite().texture();
      }
      if (mode == 2) {
        memset(overlay,0,sizeof(overlay));
        for (int y = 0; y < clipBoardH; y++) {
          for (int x = 0; x < clipBoardW; x++) {
            int rx = x + mx - doublePixel(clipBoardW/2);
            int ry = y + my - clipBoardH/2;
            if (rx >= 0 && rx < w && ry >= 0 && ry < h)
              overlay[rx+ry*MAXSPRITEWIDTH]=clipBoard[x+y*MAXSPRITEWIDTH];
          }
        }
        getCurrentSprite().update();
        getCurrentSprite().texture();
        memset(overlay,0,sizeof(overlay));
      }
      if (mode == 3) {
        makeUndoPoint();
        unsigned int *k2 = &getCurrentSprite().bitmap[0];
        for (int y = 0; y < clipBoardH; y++) {
          for (int x = 0; x < clipBoardW; x++) {
            int rx = x + mx - doublePixel(clipBoardW/2);
            int ry = y + my - clipBoardH/2;
            if (rx >= 0 && rx < w && ry >= 0 && ry < h) {
              int r1 = (clipBoard[x+y*MAXSPRITEWIDTH]) & 0xff;
              int g1 = (clipBoard[x+y*MAXSPRITEWIDTH]>>8) & 0xff;
              int b1 = (clipBoard[x+y*MAXSPRITEWIDTH]>>16) & 0xff;
              int a1 = (clipBoard[x+y*MAXSPRITEWIDTH]>>24) & 0xff;
              int r2 = (k2[rx+ry*MAXSPRITEWIDTH]) & 0xff;
              int g2 = (k2[rx+ry*MAXSPRITEWIDTH]>>8) & 0xff;
              int b2 = (k2[rx+ry*MAXSPRITEWIDTH]>>16) & 0xff;
              int a2 = (k2[rx+ry*MAXSPRITEWIDTH]>>24) & 0xff;
              int r = ((r1-r2)*a1)/255+r2;
              int g = ((g1-g2)*a1)/255+g2;
              int b = ((b1-b2)*a1)/255+b2;
              int a = ((a1-a2)*a1)/255+a2;
              k2[rx+ry*MAXSPRITEWIDTH]=r|(g<<8)|(b<<16)|(a<<24);
            }
          }
        }
        getCurrentSprite().update();
        mode = 0;
      }
    }
  }
}

void handleToolCopy(int mx, int my) {
  int w = getCurrentSprite().width;
  int h = getCurrentSprite().height;
  static int firstX;
  static int firstY;
  static int mode;
  if (toolWas != TOOL_COPY || (mouseButtons & 2)) {
    mode = 0;
  }
  if (mx >= 0 && mx < w && my >= 0 && my < h) {
    mx = doublePixel(mx);

    if (mouseButtons & 1) {
      if (mode == 0) {
        firstX = mx;
        firstY = my;
        mode++;
      }
      if (mode >= 2) {
        mode = 3;
        return;
      }
      int secondX = doublePixel2(mx);
      int secondY = my;
      int startX = firstX;
      int startY = firstY;
      int endX = secondX;
      int endY = secondY;
      if (startX>endX) {int t = startX; startX = endX; endX = t;}
      if (startY>endY) {int t = startY; startY = endY; endY = t;}
      memset(overlay,0,sizeof(overlay));
      unsigned int col = 0x7fffffff;
      if (getCurrentTool().filled) {
        for (int x = startX; x <= endX; x++) {
          for (int y = startY; y <= endY; y++) {
              overlay[x+y*MAXSPRITEWIDTH]=col;
          }
        }
      } else {
        for (int x = startX; x <= endX; x++) {
          plot(overlay,x,startY,col);
          plot(overlay,x,endY,col);
        }
        for (int y = startY; y <= endY; y++) {
          plot(overlay,startX,y,col);
          plot(overlay,endX,y,col);
        }
      }
      getCurrentSprite().update();
      getCurrentSprite().texture();
      memset(overlay,0,sizeof(overlay));
    } else {
      if (mode == 1) {
        mode = 2;
        int secondX = doublePixel2(mx);
        int secondY = my;
        int startX = firstX;
        int startY = firstY;
        int endX = secondX;
        int endY = secondY;
        if (startX>endX) {int t = startX; startX = endX; endX = t;}
        if (startY>endY) {int t = startY; startY = endY; endY = t;}
        unsigned int *k2 = &getCurrentSprite().bitmap[0];
        clipBoardW = endX-startX+1;
        clipBoardH = endY-startY+1;
        for (int y = startY; y <= endY; y++) {
          for (int x = startX; x <= endX; x++) {
            clipBoard[x-startX+(y-startY)*MAXSPRITEWIDTH] = k2[x+y*MAXSPRITEWIDTH];
            //k2[x+y*MAXSPRITEWIDTH] = 0x00000000;
          }
        }
        getCurrentSprite().update();
        getCurrentSprite().texture();
      }
      if (mode == 2) {
        memset(overlay,0,sizeof(overlay));
        for (int y = 0; y < clipBoardH; y++) {
          for (int x = 0; x < clipBoardW; x++) {
            int rx = x + mx - doublePixel(clipBoardW/2);
            int ry = y + my - clipBoardH/2;
            if (rx >= 0 && rx < w && ry >= 0 && ry < h)
              overlay[rx+ry*MAXSPRITEWIDTH]=clipBoard[x+y*MAXSPRITEWIDTH];
          }
        }
        getCurrentSprite().update();
        getCurrentSprite().texture();
        memset(overlay,0,sizeof(overlay));
      }
      if (mode == 3) {
        makeUndoPoint();
        unsigned int *k2 = &getCurrentSprite().bitmap[0];
        for (int y = 0; y < clipBoardH; y++) {
          for (int x = 0; x < clipBoardW; x++) {
            int rx = x + mx - doublePixel(clipBoardW/2);
            int ry = y + my - clipBoardH/2;
            if (rx >= 0 && rx < w && ry >= 0 && ry < h) {
              int r1 = (clipBoard[x+y*MAXSPRITEWIDTH]) & 0xff;
              int g1 = (clipBoard[x+y*MAXSPRITEWIDTH]>>8) & 0xff;
              int b1 = (clipBoard[x+y*MAXSPRITEWIDTH]>>16) & 0xff;
              int a1 = (clipBoard[x+y*MAXSPRITEWIDTH]>>24) & 0xff;
              int r2 = (k2[rx+ry*MAXSPRITEWIDTH]) & 0xff;
              int g2 = (k2[rx+ry*MAXSPRITEWIDTH]>>8) & 0xff;
              int b2 = (k2[rx+ry*MAXSPRITEWIDTH]>>16) & 0xff;
              int a2 = (k2[rx+ry*MAXSPRITEWIDTH]>>24) & 0xff;
              int r = ((r1-r2)*a1)/255+r2;
              int g = ((g1-g2)*a1)/255+g2;
              int b = ((b1-b2)*a1)/255+b2;
              int a = ((a1-a2)*a1)/255+a2;
              k2[rx+ry*MAXSPRITEWIDTH]=r|(g<<8)|(b<<16)|(a<<24);
            }
          }
        }
        getCurrentSprite().update();
        mode = 2;
      }
    }
  }
}

void handleToolScale(int mx, int my) {
  int w = getCurrentSprite().width;
  int h = getCurrentSprite().height;
  static int firstX;
  static int firstY;
  static int mode;
  if (toolWas != TOOL_SCALE || (mouseButtons & 2)) {
    mode = 0;
  }
  if (mx >= 0 && mx < w && my >= 0 && my < h) {
    mx = doublePixel(mx);

    if (mouseButtons & 1) {
      if (mode == 0) {
        firstX = mx;
        firstY = my;
        mode++;
      }
      if (mode >= 2) {
        mode = 3;
        return;
      }
      int secondX = doublePixel2(mx);
      int secondY = my;
      int startX = firstX;
      int startY = firstY;
      int endX = secondX;
      int endY = secondY;
      if (startX>endX) {int t = startX; startX = endX; endX = t;}
      if (startY>endY) {int t = startY; startY = endY; endY = t;}
      memset(overlay,0,sizeof(overlay));
      unsigned int col = 0x7fffffff;
      for (int x = startX; x <= endX; x++) {
        for (int y = startY; y <= endY; y++) {
            overlay[x+y*MAXSPRITEWIDTH]=col;
        }
      }
      getCurrentSprite().update();
      getCurrentSprite().texture();
      memset(overlay,0,sizeof(overlay));
    } else {
      if (mode == 1) {
        mode = 2;
        int secondX = doublePixel2(mx);
        int secondY = my;
        int startX = firstX;
        int startY = firstY;
        int endX = secondX;
        int endY = secondY;
        if (startX>endX) {int t = startX; startX = endX; endX = t;}
        if (startY>endY) {int t = startY; startY = endY; endY = t;}
        unsigned int *k2 = &getCurrentSprite().bitmap[0];
        clipBoardW = endX-startX+1;
        clipBoardH = endY-startY+1;
        for (int y = startY; y <= endY; y++) {
          for (int x = startX; x <= endX; x++) {
            clipBoard[x-startX+(y-startY)*MAXSPRITEWIDTH] = k2[x+y*MAXSPRITEWIDTH]|0xff000000;
            k2[x+y*MAXSPRITEWIDTH] = 0x00000000;
          }
        }
        getCurrentSprite().update();
        getCurrentSprite().texture();
      }
      if (mode == 2) {
        int secondX = doublePixel2(mx);
        int secondY = my;
        int startX = firstX;
        int startY = firstY;
        int endX = secondX;
        int endY = secondY;
        if (startX>endX) {int t = startX; startX = endX; endX = t;}
        if (startY>endY) {int t = startY; startY = endY; endY = t;}

        memset(overlay,0,sizeof(overlay));
        for (int y = startY; y <= endY; y++) {
          for (int x = startX; x <= endX; x++) {
            int x2 = (x - startX)*clipBoardW/(endX-startX+1);
            int y2 = (y - startY)*clipBoardH/(endY-startY+1);
            if (x >= 0 && x < w && y >= 0 && y < h)
              overlay[x+y*MAXSPRITEWIDTH]=clipBoard[x2+y2*MAXSPRITEWIDTH];
          }
        }
        getCurrentSprite().update();
        getCurrentSprite().texture();
        memset(overlay,0,sizeof(overlay));
      }
      if (mode == 3) {
        makeUndoPoint();
        int secondX = doublePixel2(mx);
        int secondY = my;
        int startX = firstX;
        int startY = firstY;
        int endX = secondX;
        int endY = secondY;
        if (startX>endX) {int t = startX; startX = endX; endX = t;}
        if (startY>endY) {int t = startY; startY = endY; endY = t;}
        unsigned int *k2 = &getCurrentSprite().bitmap[0];
        for (int y = startY; y <= endY; y++) {
          for (int x = startX; x <= endX; x++) {
            int x2 = (x - startX)*clipBoardW/(endX-startX+1);
            int y2 = (y - startY)*clipBoardH/(endY-startY+1);
            if (x >= 0 && x < w && y >= 0 && y < h)
              k2[x+y*MAXSPRITEWIDTH]=clipBoard[x2+y2*MAXSPRITEWIDTH];
          }
        }
        getCurrentSprite().update();
        mode = 0;
      }
    }
  }
}

void handleTool(int mx, int my) {
  switch(getCurrentTool().tool) {
  case TOOL_PENCIL: handleToolPencil(mx,my); break;
  case TOOL_FLOODFILL: handleToolFloodFill(mx,my); break;
  case TOOL_RECT: handleToolRect(mx,my); break;
  case TOOL_CIRCLE: handleToolCircle(mx,my); break;
  case TOOL_LINE: handleToolLine(mx,my); break;
  case TOOL_MOVE: handleToolMove(mx,my); break;
  case TOOL_COPY: handleToolCopy(mx,my); break;
  case TOOL_SCALE: handleToolScale(mx,my); break;
  }
  toolWas = getCurrentTool().tool;
  mouseButtonsWas = mouseButtons;
}

void displaySpriteEditField() {
  ImGui::SetNextWindowPos(ImVec2(180,20));
  ImGui::Begin(String(("Sprite Editor /")+String(getCurrentTool().getName())+"##SpriteEditor").c_str(),NULL,ImVec2(385,385),-1.f,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove);
  ImGui::PushItemWidth(80);
  ImGui::InputInt("Nr", &getCurrentSpriteCanvas().spriteNr);
  getCurrentSpriteCanvas().spriteNr = clamp(getCurrentSpriteCanvas().spriteNr,0,(int)sprites.size()-1);
  ImGui::SameLine();
  ImGui::InputText("Name", getCurrentSprite().name,10);
  ImGui::SameLine();
  ImGui::Checkbox("Grid",&getCurrentSpriteCanvas().gridOn);
  ImGui::SameLine();
  if (ImGui::Checkbox("BG",&getCurrentSpriteCanvas().background)) getCurrentSprite().update();
  ImGui::InputInt("Width",&getCurrentSprite().width);
  getCurrentSprite().width = clamp(getCurrentSprite().width,MINSPRITEWIDTH,MAXSPRITEWIDTH);
  ImGui::SameLine();
  ImGui::InputInt("Height",&getCurrentSprite().height);
  getCurrentSprite().height = clamp(getCurrentSprite().height,MINSPRITEHEIGHT,MAXSPRITEHEIGHT);
  ImGui::SameLine();
  ImGui::InputFloat("Zoom",&getCurrentSpriteCanvas().zoom,1.0);
  if (currentKey == '+') getCurrentSpriteCanvas().zoom += 1.0;
  if (currentKey == '-') getCurrentSpriteCanvas().zoom -= 1.0;
  getCurrentSpriteCanvas().zoom = clamp(getCurrentSpriteCanvas().zoom,(float)MINSPRITEZOOM,(float)MAXSPRITEZOOM);
  ImGui::PopItemWidth();
  ImVec2 windowSize = ImGui::GetWindowSize();
  ImVec2 maxTex = ImVec2((float)getCurrentSprite().width/MAXSPRITEWIDTH,(float)getCurrentSprite().height/MAXSPRITEHEIGHT);
  ImVec2 cp0 = ImGui::GetCursorScreenPos();
  ImVec2 size = ImVec2(getCurrentSprite().width*getCurrentSpriteCanvas().zoom*getCurrentSpriteCanvas().aspect,getCurrentSprite().height*getCurrentSpriteCanvas().zoom);
  ImVec2 cp1 = ImVec2(cp0.x+size.x,cp0.y+size.y);
  ImVec2 size2 = ImVec2(getCurrentSprite().width,getCurrentSprite().height/getCurrentSpriteCanvas().aspect);
  ImGui::Image((void*)getCurrentSprite().texture(), size ,ImVec2(0,0),maxTex);
  ImGui::SameLine();
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2(0,0));
  ImGui::BeginChild("Tileable",ImVec2(size2.x*2,size2.y*2));
    ImGui::Image((void*)getCurrentSprite().texture(), size2 ,ImVec2(0,0),maxTex);
    ImGui::SameLine();
    ImGui::Image((void*)getCurrentSprite().texture(), size2 ,ImVec2(0,0),maxTex);
    ImGui::Image((void*)getCurrentSprite().texture(), size2 ,ImVec2(0,0),maxTex);
    ImGui::SameLine();
    ImGui::Image((void*)getCurrentSprite().texture(), size2 ,ImVec2(0,0),maxTex);
  ImGui::EndChild();
  ImGui::PopStyleVar();
  ImDrawList *l = ImGui::GetWindowDrawList();
  l->AddRect(cp0,cp1,0xff202020,0);
  if (getCurrentSpriteCanvas().gridOn) {
    int w = getCurrentSprite().width;
    int h = getCurrentSprite().height;
    for (int x = 1; x < w; x++) {
      int xp = x * (cp1.x-cp0.x) / w + cp0.x;
      l->AddLine(ImVec2(xp,cp0.y),ImVec2(xp,cp1.y),0x40ffffff);
    }
    for (int y = 1; y < h; y++) {
      int yp = y * (cp1.y-cp0.y) / h + cp0.y;
      l->AddLine(ImVec2(cp0.x,yp),ImVec2(cp1.x,yp),0x40ffffff);
    }
  }
  ImGui::End();
  int toolMousePosX = round2((double)(mouseX-cp0.x)*getCurrentSprite().width/(cp1.x-cp0.x));
  int toolMousePosY = round2((double)(mouseY-cp0.y)*getCurrentSprite().height/(cp1.y-cp0.y));
  handleTool(toolMousePosX,toolMousePosY);
}

int colIdx = -1;
bool colorInputInit = true;

void displayColorPicker() {
  static bool isInit = true; if (isInit) ImGui::SetNextWindowPos(ImVec2(0,300)); isInit = false;
  ImGui::Begin("Palette",NULL,ImVec2(180,180),-1,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove);
  ImVec2 windowSize = ImGui::GetWindowSize();
  ImVec2 windowPos = ImGui::GetWindowPos();
  ImVec2 cp0 = ImGui::GetCursorScreenPos();
  int w = windowSize.x*0.8;
  int h = windowSize.y*0.6-16;
  ImGui::Image((void*)getCurrentPalette().paletteTexture(), ImVec2(w,h));
  ImDrawList *l = ImGui::GetWindowDrawList();
  static char colorInput[10];
  if (colorInputInit) {
    toColorString(colorInput,getCurrentPalette().slotColors[getCurrentPalette().slot]);
    colorInputInit = false;
  }
  colIdx = -1;
  for (int y = 0; y < getCurrentPalette().height; y++) {
    for (int x = 0; x < getCurrentPalette().width; x++) {
      ImVec2 xy0 = ImVec2(cp0.x + w * x / getCurrentPalette().width,cp0.y + h * y / getCurrentPalette().height);
      ImVec2 xy1 = ImVec2(cp0.x + w * (x+1) / getCurrentPalette().width,cp0.y + h * (y+1) / getCurrentPalette().height);
      unsigned int col = getCurrentPalette().colors[x+y*getCurrentPalette().width];
      if (ImGui::IsMouseHoveringRect(xy0,xy1) && (mouseButtons & 1)) {
        getCurrentPalette().slotColors[getCurrentPalette().slot] = col;
        toColorString(colorInput,col);
      }
      if (col == getCurrentPalette().slotColors[getCurrentPalette().slot]) {
        l->AddRect(xy0,xy1,0xffffffff,0);
        xy0.x += 1;
        xy0.y += 1;
        xy1.x -= 1;
        xy1.y -= 1;
        l->AddRect(xy0,xy1,0xff000000,0);
        colIdx = x+y*getCurrentPalette().width;
      }
    }
  }
  ImGui::PushItemWidth(80);
  if (ImGui::InputText("Color",colorInput,10)) {
    unsigned int color = fromColorString(colorInput);
    getCurrentPalette().colors[colIdx] = color;
    getCurrentPalette().slotColors[getCurrentPalette().slot] = color;
    getCurrentPalette().update();
  }
  ImGui::PopItemWidth();
  for (int i = 0; i < MAXPALETTESLOTS; i++) {
    ImVec2 cp = ImGui::GetCursorScreenPos();
    ImVec2 cp2 = ImGui::GetCursorScreenPos(); cp2.x += 20; cp2.y += 15;
    cp.x += 22 * i;
    cp2.x += 22 * i;
    if (ImGui::IsMouseHoveringRect(cp,cp2) && (mouseButtons & 1)) {
      getCurrentPalette().slot = i;
      toColorString(colorInput,getCurrentPalette().slotColors[i]);
    }
    if (cp2.x > windowSize.x + windowPos.x) break;
    l->AddRectFilled(cp,cp2,getCurrentPalette().slotColors[i],0);
    if (i == getCurrentPalette().slot) {
      l->AddRect(cp,cp2,0xffffffff,0);
      cp.x += 1;
      cp.y += 1;
      cp2.x -= 1;
      cp2.y -= 1;
      l->AddRect(cp,cp2,0xff000000,0);
    }
  }
  ImGui::End();
}

void render() {
  glClearColor(0.1,0.2,0.3,1);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui::Render();
}

void displayLoadSave() {
  if (loadSave == LoadSave_None)
    return;

  bool isLoading = true;
  switch(loadSave) {
  case LoadSave_SaveSprite:
  case LoadSave_SaveSpriteSheet:
  case LoadSave_SavePalette: isLoading = false; break;
  }

  FileDialog *f = new FileDialog();
  f->open(isLoading);

  bool finished;
  bool first = true;
  do
  {
    ImGuiNewFrame();
    if (first) ImGui::SetNextWindowPos(ImVec2(0,0));
    first = false;
    finished = f->display();

    render();

    glRefresh();
  } while(!finished);

  String fname = f->getSelectedFileName();
  if (!fname.empty()) {
    switch(loadSave) {
    case LoadSave_SavePalette: toFile(fname.c_str(),&getCurrentPalette().colors,MAXPALETTECOLORS*4); break;
    case LoadSave_LoadPalette: fromFile(fname.c_str(),&getCurrentPalette().colors,MAXPALETTECOLORS*4); break;
    case LoadSave_SaveSprite: {saveSprite(fname.c_str());} break;
    case LoadSave_LoadSprite: {loadSprite(fname.c_str());} break;
    case LoadSave_SaveSpriteSheet: {saveSpriteSheet(fname.c_str());} break;
    case LoadSave_LoadSpriteSheet: {loadSpriteSheet(fname.c_str());} break;
    }
  }

  loadSave = LoadSave_None;
}

void buttonActive(int toolType) {
  if (getCurrentTool().tool == toolType)
    ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(0.5,0.6,0.9,1));
  else
    ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(0.2,0.3,0.4,1));
}

void buttonActiveEnd() {
  ImGui::PopStyleColor();
}

void flipClipBoardX() {
  {for (int y = 0; y < clipBoardH; y++) {
    for (int x = 0; x < clipBoardW; x++) {
      temp[clipBoardW-1-x+y*MAXSPRITEWIDTH] = clipBoard[x+y*MAXSPRITEWIDTH];
    }
  }}
  {for (int y = 0; y < clipBoardH; y++) {
    for (int x = 0; x < clipBoardW; x++) {
      clipBoard[x+y*MAXSPRITEWIDTH] = temp[x+y*MAXSPRITEWIDTH];
    }
  }}
}

void flipClipBoardY() {
  {for (int y = 0; y < clipBoardH; y++) {
    for (int x = 0; x < clipBoardW; x++) {
      temp[x+(clipBoardH-1-y)*MAXSPRITEWIDTH] = clipBoard[x+y*MAXSPRITEWIDTH];
    }
  }}
  {for (int y = 0; y < clipBoardH; y++) {
    for (int x = 0; x < clipBoardW; x++) {
      clipBoard[x+y*MAXSPRITEWIDTH] = temp[x+y*MAXSPRITEWIDTH];
    }
  }}
}

void displayToolWindow() {
  static bool isInit = true; if (isInit) ImGui::SetNextWindowPos(ImVec2(0,20)); isInit = false;
  ImGui::Begin("Tools", NULL, ImVec2(180,280),-1,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove);
  ImGui::Text(getCurrentTool().getName());
  ImGui::SliderFloat("Size", &getCurrentTool().size, TOOLSIZEMIN, TOOLSIZEMAX);
  ImGui::SliderFloat("Mouse", &getCurrentTool().mouseSpeed, MOUSESPEEDMIN, MOUSESPEEDMAX);
  mouseSpeed = getCurrentTool().mouseSpeed;
  ImGui::Separator();
  buttonActive(TOOL_PENCIL); if (ImGui::Button("Pencil")) getCurrentTool().tool = TOOL_PENCIL; buttonActiveEnd();
  buttonActive(TOOL_FLOODFILL); if (ImGui::Button("Fill")) getCurrentTool().tool = TOOL_FLOODFILL; buttonActiveEnd();
  buttonActive(TOOL_RECT); if (ImGui::Button("Rect")) getCurrentTool().tool = TOOL_RECT; buttonActiveEnd();
  if (getCurrentTool().tool == TOOL_RECT) {ImGui::SameLine();ImGui::Checkbox("Filled##Filled1",&getCurrentTool().filled);}
  buttonActive(TOOL_CIRCLE); if (ImGui::Button("Circle")) getCurrentTool().tool = TOOL_CIRCLE; buttonActiveEnd();
  if (getCurrentTool().tool == TOOL_CIRCLE) {ImGui::SameLine();ImGui::Checkbox("Filled##Filled2",&getCurrentTool().filled);}
  buttonActive(TOOL_LINE); if (ImGui::Button("Line")) getCurrentTool().tool = TOOL_LINE; buttonActiveEnd();
  buttonActive(TOOL_MOVE); if (ImGui::Button("Move")) getCurrentTool().tool = TOOL_MOVE; buttonActiveEnd();
  if (getCurrentTool().tool == TOOL_MOVE) {ImGui::SameLine(); if (ImGui::Button("FlipX##FlipX1")||currentKey=='x') flipClipBoardX();  ImGui::SameLine(); if (ImGui::Button("FlipY##FlipY1")||currentKey=='y') flipClipBoardY();}
  buttonActive(TOOL_COPY); if (ImGui::Button("Copy")) getCurrentTool().tool = TOOL_COPY; buttonActiveEnd();
  if (getCurrentTool().tool == TOOL_COPY) {ImGui::SameLine(); if (ImGui::Button("FlipX##FlipX2")||currentKey=='x') flipClipBoardX();  ImGui::SameLine(); if (ImGui::Button("FlipY##FlipY2")||currentKey=='y') flipClipBoardY();}
  buttonActive(TOOL_SCALE); if (ImGui::Button("Scale")) getCurrentTool().tool = TOOL_SCALE; buttonActiveEnd();
  ImGui::End();
}

void displaySpriteSheet() {
  static bool isInit = true; if (isInit) ImGui::SetNextWindowPos(ImVec2(180,405)); isInit = false;
  ImGui::Begin("Sprites", NULL, ImVec2(460,75),-1,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove);

  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2(0,0));
  ImGui::BeginChild("SheetConfig",ImVec2(90,55));
  ImGui::PushItemWidth(80);
  ImGui::InputInt("Nr",&getCurrentSpriteSheet().xScroll);
  getCurrentSpriteSheet().xScroll = clamp(getCurrentSpriteSheet().xScroll,0,(int)sprites.size()-1);
  if (ImGui::Button("Insert")) {
    sprites.insert(Sprite(),getCurrentSpriteCanvas().spriteNr);
    int i = getCurrentSpriteCanvas().spriteNr;
    sprites[i].update();
    sprites[i].texture();
  }
  if (!sprites.empty()) {
    if (ImGui::Button("Erase")) {
      if (sprites.size()>1) {
        sprites.erase(getCurrentSpriteCanvas().spriteNr,1);
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("Append")) {
      sprites.push_back(Sprite());
      sprites.back().update();
      sprites.back().texture();
    }
  }
  ImGui::PopItemWidth();
  ImGui::EndChild();
  ImGui::PopStyleVar();

  for (int i = getCurrentSpriteSheet().xScroll; i < sprites.size(); i++) {
    ImGui::SameLine();
    ImVec2 cp0 = ImGui::GetCursorScreenPos();
    int w = sprites[i].width;
    int h = sprites[i].height;
    ImVec2 size = ImVec2(w*52*4*9/MAXSPRITEWIDTH/3/16,h*52/MAXSPRITEHEIGHT);
    ImVec2 cp1 = ImVec2(cp0.x+size.x,cp0.y+size.y);
    ImVec2 maxTex = ImVec2((float)w/MAXSPRITEWIDTH,(float)h/MAXSPRITEHEIGHT);
    ImGui::Image((void*)sprites[i].textureId,size,ImVec2(0,0),maxTex);
    ImDrawList *l = ImGui::GetWindowDrawList();
    l->AddRect(cp0,cp1,getCurrentSpriteCanvas().spriteNr==i?0xff2020ff:0xff202020,0);
    if (ImGui::IsMouseHoveringRect(cp0,cp1) && (mouseButtons & 1)) {
      getCurrentSpriteCanvas().spriteNr = i;
    }
  }

  ImGui::End();
}

#define COLORWHEELXRES 64
#define COLORWHEELYRES 64
#define COLORWHEELHUEY 10

unsigned int colorWheel[COLORWHEELXRES*COLORWHEELYRES];
unsigned int colorWheelTextureId = 0;
unsigned int hueLine[COLORWHEELXRES*COLORWHEELYRES];
unsigned int hueLineTextureId = 0;
float colorWheelHue = 0;

static float c(float a) {
  a -= 60.f+180.f;
  a = fmod(a+360.f,360.f);
  if (a < 60.f) return a / 60.f;
  if (a < 60.f+120.f) return 1;
  if (a < 240.f) return 1.f-(a-(60.f+120.f))/60.f;
  return 0;
}

static unsigned int hsl(float h,float s,float l) {
  float r = c(h*360);
  float g = c(h*360-120);
  float b = c(h*360-240);
  r = r * s + 1.f-s;
  g = g * s + 1.f-s;
  b = b * s + 1.f-s;
  int ri = r*255.f;
  int gi = g*255.f;
  int bi = b*255.f;
  if (ri<0) ri = 0;
  if (gi<0) gi = 0;    
  if (bi<0) bi = 0;
  if (ri>255) ri = 255;
  if (gi>255) gi = 255;    
  if (bi>255) bi = 255;
  ri *= l;
  gi *= l;
  bi *= l;
  return (ri|(gi<<8)|(bi<<16)|0xff000000);
}

void displayColorWheel() {
  for (int y = 0; y < COLORWHEELYRES; y++) {
    for (int x = 0; x < COLORWHEELXRES; x++) {
      colorWheel[x+y*COLORWHEELXRES] = hsl(colorWheelHue,(float)x/COLORWHEELXRES,(float)y/COLORWHEELYRES);
    }
  }

  if (colorWheelTextureId == 0) 
    glGenTextures(1, &colorWheelTextureId);
  if (hueLineTextureId == 0) {
    glGenTextures(1, &hueLineTextureId);
    for (int y = 0; y < COLORWHEELHUEY; y++) {
      for (int x = 0; x < COLORWHEELXRES; x++) {
        hueLine[x+y*COLORWHEELXRES] = hsl((float)x/COLORWHEELXRES,1.0,1.0);
      }
    }
  }
  glBindTexture(GL_TEXTURE_2D, colorWheelTextureId);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,COLORWHEELXRES,COLORWHEELYRES,0,GL_RGBA,GL_UNSIGNED_BYTE,colorWheel);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glBindTexture(GL_TEXTURE_2D, hueLineTextureId);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,COLORWHEELXRES,COLORWHEELHUEY,0,GL_RGBA,GL_UNSIGNED_BYTE,hueLine);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  static bool isInit = true; if (isInit) ImGui::SetNextWindowPos(ImVec2(565,20)); isInit = false;
  ImGui::Begin("ColorWheel", NULL, ImVec2(75,120),-1,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoTitleBar);
  ImVec2 cp1 = ImGui::GetCursorScreenPos();
  ImVec2 cp2 = cp1;
  cp2.x += COLORWHEELXRES;
  cp2.y += COLORWHEELYRES;
  ImGui::Image((void*)colorWheelTextureId,ImVec2(COLORWHEELXRES,COLORWHEELYRES));
  static char text[10];
  static unsigned int col;
  if (ImGui::IsMouseHoveringRect(cp1,cp2) && (mouseButtons & 1)) {
    col = hsl(colorWheelHue,(float)(mouseX-cp1.x)/COLORWHEELXRES,(float)(mouseY-cp1.y)/COLORWHEELYRES);
    toColorString(text,col);
  }
  ImVec2 cp0 = ImGui::GetCursorScreenPos();
  ImGui::Image((void*)hueLineTextureId,ImVec2(COLORWHEELXRES,COLORWHEELHUEY));
  cp2 = cp0;
  cp2.x += COLORWHEELXRES;
  cp2.y += COLORWHEELYRES;
  if (ImGui::IsMouseHoveringRect(cp0,cp2) && (mouseButtons & 1)) {
    colorWheelHue = (float)(mouseX - cp0.x)/(COLORWHEELXRES);
  }
  ImDrawList *l = ImGui::GetWindowDrawList();
  float hx = cp0.x+COLORWHEELXRES*colorWheelHue;
  l->AddLine(ImVec2(hx,cp0.y-1),ImVec2(hx,cp0.y+COLORWHEELHUEY-1),0xffffffff);
  cp0.y += COLORWHEELHUEY+20;
  cp2 = cp0;
  cp2.x += COLORWHEELXRES;
  cp2.y += 10;
  l->AddRectFilled(cp0,cp2,col);
  ImGui::Text(text);
  ImGui::End();
}

int main(int argc, char **argv) {
  glVesa(XRES,YRES,32);
  InitImGui(XRES, YRES);
  sprites.push_back(Sprite());
  makeUndoPoint();
  
  do
  {
    ImGuiNewFrame();

    static double lastFPSClock = glSeconds();
    double fpsClock = glSeconds();
    static int fpsCounter=0;
    fpsCounter++;
    if (fpsClock-lastFPSClock>1.0) {
      lastFPSClock = fpsClock;
      fps = fpsCounter;
      fpsCounter = 0;
    }

    displaySpriteEditField();
    displayToolWindow();
    displayColorPicker();
    displayColorWheel();
    displaySpriteSheet();
    displayMenuBar();
    displayLoadSave();
    displayQuitBox();      

    render();

    glRefresh();
  } while(!quit);
  glDone();

  return 0;
}