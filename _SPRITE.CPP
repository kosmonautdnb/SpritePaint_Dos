// -----
// _sprite.exe (c)2025 by Stefan Mader (MIT-License)
// -----
#define _CRT_SECURE_NO_WARNINGS
#include "types.hpp"

#ifndef NOT_DOS
#include "gl.h"
#include <i86.h>
#include "array.hpp"
#include "hashmap.hpp"
#include "hashmap.hpp"
#include "imguigl.hpp"
#define ImTextureID void *
#define spArray Array
#define spInsert(__k__,__v__,__p__) insert(__v__,__p__)
#define spErase(__k__,__b__,__l__) erase(__b__,__l__)
#define spHashMap HashMap
#define spHas(__s__,__v__) has(__v__)
#else
#include <imgui/imgui.h>
#include <imgui/examples/opengl3_example/imgui_impl_glfw_gl3.h >
#include <GLFW/glfw3.h>
#define GL_CLAMP_TO_BORDER GL_REPEAT
#define GL_CLAMP_TO_EDGE GL_REPEAT
bool glVesa(int width, int height, int bpp);
double glSeconds();
void glRefresh();
void glDone();
void ImGuiNewFrame();
void InitImGui(int width, int height);
#include <vector>
#define spArray std::vector
#define spInsert(__k__,__v__,__p__) insert(__k__.begin()+__p__,__v__)
#define spErase(__k__,__b__,__l__) erase(__k__.begin()+__b__,__k__.begin()+__b__+__l__)
#include <map>
#define spHashMap std::map
#define spHas(__s__,__v__) find(__v__)!=(__s__).end()
#endif

#include "filedlg.hpp"
#include "image.hpp"
#include "psdimage.hpp"
#include <stdio.h>
#include <math.h>

#ifndef NOT_DOS
#define XRES 640
#define YRES 480
#define MONITORASPECT (16.0/9.0) // (4.0/3.0)
#else
#define XRES 1280
#define YRES 720
#define MONITORASPECT (1*((double)XRES/YRES))
#endif

/*
#define VIDEO
#define VIDEOPATH "C:\\frames\\%04d.jpg"
int videoFrame=0;
bool recordVideo=false;
*/

#define MINSPRITEWIDTH 1
#define MINSPRITEHEIGHT 1
#define MAXSPRITEWIDTH 128
#define MAXSPRITEHEIGHT 128
#define MINSPRITEZOOM 1.0
#define MAXSPRITEZOOM 20
#define MAXPALETTECOLORS 256
#define MAXPALETTESLOTS 8
#define TOOLSIZEMIN 1
#define TOOLSIZEMAX 20.0
#define MOUSESPEEDMIN 0.25
#define MOUSESPEEDMAX 2.0

extern double mouseX, mouseY;
extern double mouseSpeed;
extern int mouseButtons;              
#define _WIDTH(__x__) ((__x__)*XRES/640)
#define _HEIGHT(__y__) ((__y__)*YRES/480)
bool initGraphics() {
  return glVesa(XRES,YRES,32);
}
void refreshGraphics() {
  glRefresh();
}
void closeGraphics() {
  glDone();
}
double timeInSeconds() {
  return glSeconds();
}

unsigned int defaultPal[]={
     0x00000000,
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
     0xff371491, // not taken because of transparent color at start
};
#define M_PI 3.14159265358979323846

bool askForQuit = false;
bool quit = false;
bool someError = false;

enum {  
  LoadSave_None = 0,
  LoadSave_LoadPalette = 1,
  LoadSave_SavePalette = 2,
  LoadSave_LoadSprite = 3,
  LoadSave_SaveSprite = 4,
  LoadSave_LoadSpriteSheet = 5,
  LoadSave_SaveSpriteSheet = 6,
  LoadSave_ImportSprite = 7,
  LoadSave_ExportSprite = 8,
  LoadSave_ImportPalette = 9,
  LoadSave_ExportPalette = 10,
  LoadSave_ImportSpriteAtlas = 11,
  LoadSave_ExportSprites = 12,
  LoadSave_ImportSpritesCounter = 13,
  LoadSave_ExportSpritesCounter = 14,
  LoadSave_ImportPSDLayers = 15,
  LoadSave_ImportAnimatedGif = 16,
  LoadSave_LoadMapSimpleLayer = 17,
  LoadSave_SaveMapSimpleLayer = 18,
};

int loadSave = LoadSave_None;

enum {
  SUBSET_SPRITEPAINTER = 0,
  SUBSET_MAPPAINTER = 1,
};

int activeSubset = SUBSET_SPRITEPAINTER;

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
  bool showAlpha;
  bool paintMirrored;
  bool mirroredMode;
  SpriteCanvas() {
    spriteNr = 0;
    zoom = 5.f;
    aspect = (double)XRES/YRES/MONITORASPECT;
    gridOn = false;
    background = false;
    doublePixel = false;
    showAlpha = false;
    paintMirrored = false;
    mirroredMode = false;
  }
};

SpriteCanvas &getCurrentSpriteCanvas();

uint16_t getUniqueId();

void updateMap();

class Sprite {
public:
  int width,height;
  char name[10];
  spArray<unsigned int> bitmap;
  GLuint textureId;
  bool mark;
  bool mirrorPaintX;
  bool mirrorPaintY;
  int mirrorXPos;
  int mirrorYPos;
  Sprite() {
    bitmap.resize(MAXSPRITEWIDTH*MAXSPRITEHEIGHT);
    memset(&bitmap[0],0x00,MAXSPRITEWIDTH*MAXSPRITEHEIGHT*sizeof(unsigned int));
    width = 64;
    height = 64;
    sprintf(name,"No name");
    getId() = getUniqueId();
    textureId = 0;
    mark = false;
    mirrorPaintX=false;
    mirrorPaintY=false;
    mirrorXPos = 32;
    mirrorYPos = 32;
  }
  uint16_t &getId() {return *((uint16_t*)&name[8]);}
  void update() {
    mark = true;
    updateMap();
  }
  GLuint texture() {
    if (mark) {
      glDeleteTextures(1,&textureId);
      textureId = 0;
      mark=false;
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
      bool alpha = getCurrentSpriteCanvas().showAlpha;
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
          temp[i] = (((((i%MAXSPRITEWIDTH)+i/MAXSPRITEWIDTH) % 3)+1)*0x00202020)|0xff000000;
        }
        if (alpha) {
          temp[i] = a|(a<<8)|(a<<16)|0xff000000;
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
    width = 16;//sqrt(MAXPALETTECOLORS);
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
      mark = false;
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

#define MODAL_NONE 0
#define MODAL_RESIZESPRITE 1
#define MODAL_COPYSPRITES 2
#define MODAL_DELETESPRITES 3
unsigned int modalMode = MODAL_NONE;


#define TOOL_NONE 0
#define TOOL_PENCIL 1
#define TOOL_FLOODFILL 2
#define TOOL_RECT 3
#define TOOL_CIRCLE 4
#define TOOL_LINE 5
#define TOOL_MOVE 6
#define TOOL_COPY 7
#define TOOL_SCALE 8
#define TOOL_ROTATE 9
#define TOOL_RECOLOR 10
#define TOOL_SETALPHA 11

class Tool {
public:
  float size;
  float angle;
  bool place;
  bool special;
  float x,y,w,h;
  float mouseSpeed;
  bool filled;
  int tool;
  unsigned int color0;
  unsigned int color1;
  int alpha;
  Tool() {
    size = 1.f;
    angle = 0.f;
    place = false;
    special = false;
    x = 0;
    y = 0;
    w = 1;
    h = 1;
    mouseSpeed = 1.f;
    tool = TOOL_PENCIL;
    filled = true;
    color0 = 0xff000000;
    color1 = 0xff000000;
    alpha = 0;
  }
  const char *getName() {
    switch(tool) {
    case TOOL_PENCIL: return "Pencil";
    case TOOL_FLOODFILL: return "FloodFill";
    case TOOL_RECT: return "Rectangle";
    case TOOL_CIRCLE: return "Circle";
    case TOOL_LINE: return "Line";
    case TOOL_MOVE: return "Move";
    case TOOL_COPY: return "Copy";
    case TOOL_SCALE: return "Scale";
    case TOOL_ROTATE: return "Transform";
    case TOOL_RECOLOR: return "ReColor";
    case TOOL_SETALPHA: return "SetAlpha";
    };
    return "<NONE>";
  }
};

class SpriteSheet {
public:
  int xScroll;
  bool halveSize;
  SpriteSheet() {
    xScroll = 0;
    halveSize = false;
  }
};

class Animation {
public:
  float speed;
  int frame0;
  int frameN;
  bool playing;
  double playPos;
  int playedFrame;
  bool flipping;
  Animation() {
    speed = 8;
    frame0 = 0;
    frameN = 0;
    playing = false;
    playPos = 0;
    playedFrame=0;
    flipping = false;
  }
};

class TestBox {
public:
  int width,height;
  spArray<unsigned int> spriteIndices;
  TestBox() {
    width = 2;
    height = 2;
    spriteIndices.resize(width*height);
    for (int i = 0; i < (int)spriteIndices.size(); i++) spriteIndices[i] = 0;
  }
};

spArray<Sprite> sprites;
SpriteCanvas spriteCanvas;
Palette palette;
Tool tool;
SpriteSheet spriteSheet;
Animation animation;
TestBox testBox;

spArray<Sprite> sprites_undo;
SpriteCanvas spriteCanvas_undo;
Palette palette_undo;
Tool tool_undo;
Animation animation_undo;
TestBox testBox_undo;

uint16_t getUniqueId() {
  uint16_t uniqueId=0;
  bool found = false;
  do {
    int a = ((int)glSeconds())&0xffff;
    int b = (rand()+rand()*256)&0xffff;
    uniqueId = (a+b)&0xffff;
    if (uniqueId==0) 
      continue;
    found=false;
    for (int i = 0; i < (int)sprites.size(); i++) {
      if (sprites[i].getId() == uniqueId) {
        found = true;
        break;
      }
    }
  } while(found);
  return uniqueId;
}

void makeUndoPoint() {
  sprites_undo = sprites;
  spriteCanvas_undo = spriteCanvas;
  palette_undo = palette;
  tool_undo = tool;
  animation_undo = animation;
  testBox_undo = testBox;
}

void undo() {
  sprites = sprites_undo;
  spriteCanvas = spriteCanvas_undo;
  palette = palette_undo;
  tool = tool_undo;
  animation = animation_undo;
  testBox = testBox_undo;
  for (int i = 0; i < (int)sprites.size(); i++) sprites[i].update();
  palette.update();
}

int makeScreenShot = 0;
void doScreenShot(const char *fileName) {
#ifndef NOT_DOS
  RGBAImage img;
  img.width = XRES;
  img.height = YRES;
  img.data = glFrameBuffer;
  img = img.getResized(img.height*MONITORASPECT,img.height);
  for (int i = 0; i < img.width*img.height;i++) img.data[i]|=0xff000000;
  if (!img.savePNG(fileName)) someError = true;
  delete[] img.data;
#endif
}

void render() {
  glClearColor(0.1f,0.2f,0.3f,1);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui::Render();
  if (makeScreenShot > 0) {
    makeScreenShot--;
    if (makeScreenShot==0) {
      doScreenShot("scrnshot.png");
    }
  }
#ifdef VIDEO
  if (recordVideo) {
    char buffer[1024];
    sprintf(buffer,VIDEOPATH,videoFrame);
    videoFrame++;
    RGBAImage img;
    img.width = glFrameBufferWidth;
    img.height = glFrameBufferHeight;
    img.data = glFrameBuffer;
    img.saveJPEG(buffer);
  }
#endif
}

Animation &getCurrentAnimation() {return animation;}
TestBox &getCurrentTestBox() {return testBox;}

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

template<class T>
T sign(const T &v) {
  return (v<0?-1:(v>0?1:0));
}

void updateAll() {
  for (int i = 0; i < (int)sprites.size(); i++) {sprites[i].update();sprites[i].texture();}
  getCurrentPalette().update();
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
  if (out == NULL) {someError = true; return false;}
  fwrite(data,1,dataSize,out);
  fclose(out);
  return true;
}

bool fromFile(const char *fileName, void *data, int dataSize) {
  FILE *in = fopen(fileName,"rb");
  if (in == NULL) {someError = true; return false;}
  fread(data,1,dataSize,in);
  fclose(in);
  return true;
}

bool savePalette(const char *fileName) {
  return toFile(fileName,getCurrentPalette().colors,MAXPALETTECOLORS*4);
}

bool loadPalette(const char *fileName) {
  FILE *in = fopen(fileName,"rb");
  if (in == NULL) {someError = true; return false;}
  fseek(in,0,SEEK_END);
  int fileSize = ftell(in);
  fseek(in,0,SEEK_SET);
  if (fileSize>MAXPALETTECOLORS*4) {fclose(in);someError = true; return false;}
  memset(getCurrentPalette().colors,0,MAXPALETTECOLORS*4);
  fread(getCurrentPalette().colors,1,fileSize,in);
  getCurrentPalette().update();
  fclose(in);
  return true;
}

bool saveSprite(const char *fileName) {
  FILE *out = fopen(fileName,"wb");
  if (out == NULL) {someError = true; return false;}
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
  if (in == NULL) {someError = true; return false;}
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

bool importSprite(const char *fileName) {
  RGBAImage img = RGBAImage::fromFile(fileName);
  if (img.data == NULL)  {someError = true; return false;}
  if (img.width > MAXSPRITEWIDTH) {someError = true; return false;}
  if (img.height > MAXSPRITEHEIGHT) {someError = true; return false;}
  getCurrentSprite().width = img.width;
  getCurrentSprite().height = img.height;
  for (int y = 0; y < img.height; y++) {
    for (int x = 0; x < img.width; x++) {
      getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH] = img.data[x+y*img.width];
    }
  }
  getCurrentSprite().update();
  getCurrentSprite().texture();
  getCurrentTool().tool = TOOL_NONE;
  delete[] img.data;
  return true;
}

bool exportSprite(const char *fileName) {
  RGBAImage img;
  img.width = getCurrentSprite().width;
  img.height = getCurrentSprite().height;
  img.data = new unsigned int[img.width*img.height];
  for (int y = 0; y < img.height; y++) {
      for (int x = 0; x < img.width; x++) {
          img.data[x + y * img.width] = getCurrentSprite().bitmap[x + y * MAXSPRITEWIDTH];
      }
  }
  String fName = String(fileName);
  if (fName.find(".")<0) fName += ".png";
  if (!img.savePNG(fName.c_str())) someError = true;
  delete img.data;
  return !someError;
}

bool importPalette(const char *fileName) {
  RGBAImage img = RGBAImage::fromFile(fileName);
  if (img.data == NULL) {someError = true; return false;}
  int index = 0;
  for (int y = 0; y < img.height; y++) {
    for (int x = 0; x < img.width; x++) {
      unsigned int c = img.data[x+y*img.width];
      bool found = false;
      for (int i = 0; i < index; i++) {
        if (getCurrentPalette().colors[i]==c) {found = true;break;}
      }
      if (!found) {
        getCurrentPalette().colors[index] = c;
        index++;
        if (index >= MAXPALETTECOLORS)
          break;
      }
    }
    if (index >= MAXPALETTECOLORS)
      break;
  }
  delete[] img.data;
  getCurrentPalette().update();
  return true;
}

bool exportPalette(const char *fileName) {
  String fName = String(fileName);
  if (fName.find(".")<0) fName += ".png";
  RGBAImage img;
  Palette &p = getCurrentPalette();
  const int PIXMUL = 8;
  img.width = p.width * PIXMUL;
  img.height = p.height * PIXMUL;
  img.data = new unsigned int[img.width*img.height];
  memset(img.data,0,img.width*img.height*sizeof(unsigned int));
  for (int y = 0; y < img.height; y++) {
    for (int x = 0; x < img.width; x++) {
      int rx = x / PIXMUL;
      int ry = y / PIXMUL;
      int index = rx + ry * p.width;
      if (index < MAXPALETTECOLORS)
        img.data[x+y*img.width] = p.colors[index];
    }
  }
  if (!img.savePNG(fName.c_str()))  someError = true;
  delete[] img.data;
  return !someError;
}

bool saveSpriteSheet(const char *fileName) {
  FILE *out = fopen(fileName,"wb");
  if (out == NULL) {someError = true; return false;}
  char s1 = 'S';
  char s2 = 'S';
  fwrite(&s1,1,1,out);
  fwrite(&s2,1,1,out);
  unsigned short c = (unsigned short)sprites.size();
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
  fclose(out);
  return true;
}

bool loadSpriteSheet(const char *fileName) {
  FILE *in = fopen(fileName,"rb");
  if (in == NULL) {someError = true; return false;}
  char s1,s2;
  fread(&s1,1,1,in);
  fread(&s2,1,1,in);
  if (s1 != 'S' || s2 != 'S') {someError = true; return false;}
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
    if (s1 != 'S' || s2 != 'P') {someError = true; return false;}
    fread(&w,1,1,in);
    fread(&h,1,1,in);
    if (w > MAXSPRITEWIDTH) {someError = true; return false;}
    if (h > MAXSPRITEHEIGHT) {someError = true; return false;}
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

unsigned int quantize(unsigned int c) {
  unsigned int *cols = getCurrentPalette().colors;
  int r0 = c & 255;
  int g0 = (c>>8) & 255;
  int b0 = (c>>16) & 255;
  int a0 = (c>>24) & 255;
  double lastD = 256*256*5;
  for (int i = 0; i < MAXPALETTECOLORS; i++) {
    unsigned int c2 = cols[i];
    int r1 = c2 & 255;
    int g1 = (c2>>8) & 255;
    int b1 = (c2>>16) & 255;
    int a1 = (c2>>24) & 255;
    double dr = (r1-r0)*0.25;
    double dg = (g1-g0)*0.6;
    double db = (b1-b0)*0.15;
    double da = (a1-a0);
    double d = dr*dr+dg*dg+db*db+da*da;
    if (d < lastD) {
      lastD = d;
      c = c2;
    }
  }
  return c;
}

void quantizeAll() {
  for (int i = 0; i < (int)sprites.size(); i++) {
    for (int y = 0; y < MAXSPRITEHEIGHT; y++) {
      for (int x = 0; x < MAXSPRITEWIDTH; x++) {
        unsigned int c = sprites[i].bitmap[x+y*MAXSPRITEWIDTH];
        sprites[i].bitmap[x+y*MAXSPRITEWIDTH] = quantize(c);
      }
    }
    sprites[i].update();
    sprites[i].texture();
  }
}

double round2(double v) {
  return floor(v - 0.5);
}

void newWorkSpace() {
  sprites.clear();
  sprites.push_back(Sprite());
  sprites[0].update();
  sprites[0].texture();
  getCurrentSpriteCanvas() = SpriteCanvas();
  getCurrentTool() = Tool();
  getCurrentPalette() = Palette();
  getCurrentSpriteSheet() = SpriteSheet();
  getCurrentAnimation() = Animation();
  getCurrentTestBox() = TestBox();
  makeUndoPoint();
}

int coordX = 0;
int coordY = 0;
int mouseDelay = 0;
bool showAbout = false;
#define MOUSEDELAY 5
double fps=0;

void displayMenuBar() {
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10,1));
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg,ImVec4(0xff/255.f,0xff/255.f,0xff/255.f,0xff/255.f));
    ImGui::PushStyleColor(ImGuiCol_Text,ImVec4(0x20/255.f,0x20/255.f,0x20/255.f,0xff/255.f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg,ImVec4(0xff/255.f,0xff/255.f,0xff/255.f,0xff/255.f));
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New")) newWorkSpace();
        ImGui::Separator();
        if (ImGui::MenuItem("Import palette [png]")) loadSave = LoadSave_ImportPalette;
        if (ImGui::MenuItem("Export palette [png]")) loadSave = LoadSave_ExportPalette;
        if (ImGui::MenuItem("Load palette")) loadSave = LoadSave_LoadPalette;
        if (ImGui::MenuItem("Save palette")) loadSave = LoadSave_SavePalette;
        ImGui::Separator();
        if (ImGui::MenuItem("Import sprite [png]")) loadSave = LoadSave_ImportSprite;
        if (ImGui::MenuItem("Export sprite [png]")) loadSave = LoadSave_ExportSprite;
        if (ImGui::MenuItem("Load sprite")) loadSave = LoadSave_LoadSprite;
        if (ImGui::MenuItem("Save sprite")) loadSave = LoadSave_SaveSprite;
        ImGui::Separator();
        if (ImGui::MenuItem("Load sprite sheet")) loadSave = LoadSave_LoadSpriteSheet;
        if (ImGui::MenuItem("Save sprite sheet")) loadSave = LoadSave_SaveSpriteSheet;
        ImGui::Separator();
#ifndef NOT_DOS
        if (ImGui::MenuItem("Save screenshot")) makeScreenShot = 2;
#endif
#ifdef VIDEO
        if (ImGui::MenuItem("Record video")) {videoFrame=0;recordVideo=true;}
#endif
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
        if (ImGui::MenuItem("Copy sprite")) {
          for (int y = 0; y < getCurrentSprite().height; y++) {
            for (int x = 0; x < getCurrentSprite().width; x++) {
              clipBoard[x+y*MAXSPRITEWIDTH]=getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH];
            }
          }        
          clipBoardW = getCurrentSprite().width;
          clipBoardH = getCurrentSprite().height;
        }
        if (ImGui::MenuItem("Paste sprite")) {
          getCurrentSprite().width = clipBoardW;
          getCurrentSprite().height = clipBoardH;
          for (int y = 0; y < getCurrentSprite().height; y++) {
            for (int x = 0; x < getCurrentSprite().width; x++) {
              getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH]=clipBoard[x+y*MAXSPRITEWIDTH];
            }
          }        
          getCurrentSprite().update();
       }
        if (ImGui::MenuItem("Clear sprite")) {
          unsigned int col = 0x00;
          for (int y = 0; y < MAXSPRITEHEIGHT; y++) {
            for (int x = 0; x < MAXSPRITEWIDTH; x++) {
              getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH] = col;
            }
          }        
          getCurrentSprite().update();
        }
        if (ImGui::MenuItem("Fill sprite")) {
          unsigned int col = getCurrentPalette().slotColors[getCurrentPalette().slot];
          for (int y = 0; y < MAXSPRITEHEIGHT; y++) {
            for (int x = 0; x < MAXSPRITEWIDTH; x++) {
              getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH] = col;
            }
          }        
          getCurrentSprite().update();
        }
        if (ImGui::MenuItem("Flip sprite horizontal")) {
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
        if (ImGui::MenuItem("Flip sprite vertical")) {
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
        if (ImGui::MenuItem("Rotate sprite 90 degrees")) {
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
        if (ImGui::MenuItem("Resize sprite")) {
          modalMode=MODAL_RESIZESPRITE;
        }        
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Sprites")) {
        if (ImGui::MenuItem("Import sprite atlas [png]")) loadSave = LoadSave_ImportSpriteAtlas;
        if (ImGui::MenuItem("Export sprite sheet [png]")) loadSave = LoadSave_ExportSprites;
        ImGui::Separator();
        if (ImGui::MenuItem("Import from animated [gif]")) loadSave = LoadSave_ImportAnimatedGif;
        ImGui::Separator();
        if (ImGui::MenuItem("Import sprites [pngs/counter]")) loadSave = LoadSave_ImportSpritesCounter;
        if (ImGui::MenuItem("Export sprites [pngs/counter]")) loadSave = LoadSave_ExportSpritesCounter;
        ImGui::Separator();
        if (ImGui::MenuItem("Import layers [psd](experimental)")) loadSave = LoadSave_ImportPSDLayers;
        ImGui::Separator();
        if (ImGui::MenuItem("Palettize colors of all sprites")) quantizeAll();
        ImGui::Separator();
        bool &db = getCurrentSpriteCanvas().doublePixel;
        if (db && ImGui::MenuItem("Turn double pixels off##DoublePixelsOnOff1")) {
          db = !db;
          mouseDelay=MOUSEDELAY;
        } else                                                                
        if ((!db) && ImGui::MenuItem("Turn double pixels on##DoublePixelsOnOff2")) {
          db = !db;
          mouseDelay=MOUSEDELAY;
        }
        bool &mr = getCurrentSpriteCanvas().mirroredMode;
        if (mr && ImGui::MenuItem("Turn mirrored mode off##MirroredModeOnOff1")) {
          mr = !mr;
          mouseDelay=MOUSEDELAY;
        } else                                                                
        if ((!mr) && ImGui::MenuItem("Turn mirrored mode on##MirroredModeOnOff2")) {
          mr = !mr;
          mouseDelay=MOUSEDELAY;
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Delete sprites")) {modalMode=MODAL_DELETESPRITES; mouseDelay=MOUSEDELAY;}
        if (ImGui::MenuItem("Copy sprites")) {modalMode=MODAL_COPYSPRITES; mouseDelay=MOUSEDELAY;}
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Tools")) {
        if (ImGui::MenuItem("Map painter (under construction)")) {activeSubset = SUBSET_MAPPAINTER;}
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Info")) {
        if (ImGui::MenuItem("About")) {showAbout = true; mouseDelay=MOUSEDELAY;}
        ImGui::EndMenu();
      }
#ifndef NOT_DOS
      ImGui::Text("\t\t\t%02d|%02d %d|%d %s  FPS/%d",coordX,coordY,getCurrentSpriteCanvas().spriteNr,(int)sprites.size(),currentTimeString().c_str(),(int)fps);
#else
      ImGui::Text("\t\t\t%02d|%02d %d|%d", coordX, coordY,getCurrentSpriteCanvas().spriteNr, (int)sprites.size());
#endif

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
    ImGui::SetNextWindowPos(ImVec2((XRES-_WIDTH(50))/2,(YRES-_HEIGHT(40))/2));
    ImGui::Begin("Quit?",NULL,ImVec2(_WIDTH(50),_HEIGHT(40)),-1,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoCollapse);
    if(ImGui::Button("Yes") || ImGui::IsKeyDown('y') || ImGui::IsKeyDown(ImGui::GetIO().KeyMap[ImGuiKey_Enter])) {
      ImGui::GetIO().KeysDown['y'] = 0;
      ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Enter]] = 0;
      quit = true;
    }
    ImGui::SameLine();
    if(ImGui::Button("No") || ImGui::IsKeyDown('n') || ImGui::IsKeyDown(ImGui::GetIO().KeyMap[ImGuiKey_Escape])) {
      ImGui::GetIO().KeysDown['n'] = 0;
      ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Escape]] = 0;
      mouseDelay = MOUSEDELAY;
      askForQuit = false;
    }
    ImGui::End();
  } else {
    if (ImGui::IsKeyDown(ImGui::GetIO().KeyMap[ImGuiKey_Escape])) {
      askForQuit = true;
      ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Escape]] = 0;
    }
  }
}

int doublePixel(int mx) {
  if (getCurrentSpriteCanvas().doublePixel)
    mx = mx & (~1);
  return mx;
}

void plot(unsigned int *dest, int x, int y, unsigned int col) {
  int w = getCurrentSprite().width;
  int h = getCurrentSprite().height;
  double toolSize = getCurrentTool().size*0.5;

  for (int y2 = (int)floor(y - toolSize-1); y2 <= (int)floor(y + toolSize+1); y2++) {
    for (int x2 = (int)floor(x - toolSize-1); x2 <= (int)floor(x + toolSize+1); x2++) {
      double dx = doublePixel(x2 - x);
      double dy = y2 - y;
      double d = sqrt(dx*dx+dy*dy);
      if (d <= toolSize) {
        if (x2 >= 0 && y2 >= 0 && x2 < w && y2 < h) {
          if (getCurrentSpriteCanvas().doublePixel) {
            dest[doublePixel(x2)+y2*MAXSPRITEWIDTH] = col;
            dest[doublePixel(x2)+1+y2*MAXSPRITEWIDTH] = col;
          } else {
            dest[x2+y2*MAXSPRITEWIDTH] = col;
          }
        }
      }
    }
  }
}

unsigned int currentPixelColor;

void colorBehindPixel(int mx, int my) {
  int w = getCurrentSprite().width;
  int h = getCurrentSprite().height;
  if (mx >= 0 && mx < w && my >= 0 && my < h) {
    unsigned int col = getCurrentSprite().bitmap[mx+my*MAXSPRITEWIDTH];
    currentPixelColor = col;
  }
}

int colIdx = -1;
bool colorInputInit = true;

void handleColorGet(int mx, int my) {
  int w = getCurrentSprite().width;
  int h = getCurrentSprite().height;
  if (mx >= 0 && mx < w && my >= 0 && my < h) {
    unsigned int col = getCurrentSprite().bitmap[mx+my*MAXSPRITEWIDTH];
    if (mouseButtons & 2) {
      getCurrentPalette().slot = 0;
      getCurrentPalette().slotColors[getCurrentPalette().slot] = col;
      colorInputInit = true;
    }
  }
}

int toolWas = 0;
int mouseButtonsWas = 0;

int doublePixel3(int mx) {
  if (getCurrentSpriteCanvas().doublePixel)
    mx = (mx & (~1))+1;
  return mx;
}

void handleToolPencil(int mx, int my) {
  handleColorGet(mx,my);
  static int lastMx = mx;
  static int lastMy = my;
  if (toolWas != TOOL_PENCIL || (mouseButtons & 2)) {
    lastMx = mx;
    lastMy = my;
  }
  int w = getCurrentSprite().width;
  int h = getCurrentSprite().height;
  if (mx >= 0 && mx < w && my >= 0 && my < h && lastMx >= 0 && lastMx < w && lastMy >= 0 && lastMy < h) {
    mx = doublePixel(mx);

    memset(overlay,0,sizeof(overlay));
    unsigned int col = getCurrentPalette().slotColors[getCurrentPalette().slot];
    plot(overlay,mx,my,col|0xff000000);
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
        int kx = doublePixel((int)floor(mx+dx*i));
        int ky = (int)floor(my+dy*i);
        plot(&getCurrentSprite().bitmap[0],kx,ky,col);
      }
      getCurrentSprite().update();
    }
  }
  lastMx = mx;
  lastMy = my;
}

void handleToolFloodFill(int mx, int my) {
  handleColorGet(mx,my);
  int w = getCurrentSprite().width;
  int h = getCurrentSprite().height;
  if (mx >= 0 && mx < w && my >= 0 && my < h) {
    mx = doublePixel(mx);

    memset(overlay,0,sizeof(overlay));
    unsigned int col = getCurrentPalette().slotColors[getCurrentPalette().slot];
    plot(overlay,mx,my,col|0xff000000);
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
      int secondX = doublePixel3(mx);
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
        int secondX = doublePixel3(mx);
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
        for (int x = startX; x <= endX+1; x++) { //+1 because of doublePixel pixel 2
          if (x < MAXSPRITEWIDTH) {
            for (int y = startY; y <= endY; y++) {
              double fx = (double)(doublePixel(x - startX)+0.5)/(endX-startX+1)*2.0-1.0;
              double fy = (double)(y - startY+0.5)/(endY-startY+1)*2.0-1.0;
              double d = sqrt(fx*fx+fy*fy);
              if(d<=1.0)
                overlay[x+y*MAXSPRITEWIDTH]=col;
            }
          }
        }
      } else {
        int k = 1024;
        for (int i = 0; i < k; i++) {
          double fk = (double)i / k * 2.0 * M_PI;
          int x = (int)floor((sin(fk)*0.5+0.5)*(endX-startX)+startX+0.5);
          int y = (int)floor((cos(fk)*0.5+0.5)*(endY-startY)+startY+0.5);
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
          for (int x = startX; x <= endX+1; x++) { //+1 because of doublePixel pixel 2
            if (x < MAXSPRITEWIDTH) {
              for (int y = startY; y <= endY; y++) {
                double fx = (double)(doublePixel(x - startX)+0.5)/(endX-startX+1)*2.0-1.0;
                double fy = (double)(y - startY+0.5)/(endY-startY+1)*2.0-1.0;
                double d = sqrt(fx*fx+fy*fy);
                if(d<=1.0)
                  getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH]=col;
              }
            }
          }
        } else {
          unsigned int *k2 = &getCurrentSprite().bitmap[0];
          int k = 1024;
          for (int i = 0; i < k; i++) {
            double fk = (double)i / k * 2.0 * M_PI;
            int x = (int)floor((sin(fk)*0.5+0.5)*(endX-startX)+startX+0.5);
            int y = (int)floor((cos(fk)*0.5+0.5)*(endY-startY)+startY+0.5);
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
        int x = doublePixel((int)floor(fk*(endX-startX)+startX+0.5));
        int y = (int)floor(fk*(endY-startY)+startY+0.5);
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
          int x = doublePixel((int)floor(fk*(endX-startX)+startX+0.5));
          int y = (int)floor(fk*(endY-startY)+startY+0.5);
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
    if (mode==2)
    {
      undo();
    }
    mode = 0;
  }
  if (mx >= 0 && mx < w && my >= 0 && my < h) {
    mx = doublePixel(mx);

    if (mouseButtons & 1) {
      if (mode == 0) {
        firstX = doublePixel(mx);
        firstY = my;
        mode++;
      }
      if (mode >= 2) {
        mode = 3;
        return;
      }
      int secondX = doublePixel(mx);
      int secondY = my;
      int startX = firstX;
      int startY = firstY;
      int endX = secondX;
      int endY = secondY;
      if (startX>endX) {int t = startX; startX = doublePixel(endX); endX = t;}
      if (startY>endY) {int t = startY; startY = endY; endY = t;}
      memset(overlay,0,sizeof(overlay));
      unsigned int col = 0x7fffffff;
      for (int x = startX; x <= doublePixel3(endX); x++) {
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
        makeUndoPoint();
        int secondX = doublePixel(mx);
        int secondY = my;
        int startX = firstX;
        int startY = firstY;
        int endX = secondX;
        int endY = secondY;
        if (startX>endX) {int t = startX; startX = endX; endX = t;}
        if (startY>endY) {int t = startY; startY = endY; endY = t;}
        unsigned int *k2 = &getCurrentSprite().bitmap[0];
        clipBoardW = doublePixel3(endX-startX)+1;
        clipBoardH = doublePixel3(endY-startY)+1;
        for (int y = startY; y <= endY; y++) {
          for (int x = startX; x <= doublePixel3(endX); x++) {
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
        firstX = doublePixel(mx);
        firstY = my;
        mode++;
      }
      if (mode >= 2) {
        mode = 3;
        return;
      }
      int secondX = doublePixel(mx);
      int secondY = my;
      int startX = firstX;
      int startY = firstY;
      int endX = secondX;
      int endY = secondY;
      if (startX>endX) {int t = startX; startX = endX; endX = t;}
      if (startY>endY) {int t = startY; startY = endY; endY = t;}
      memset(overlay,0,sizeof(overlay));
      unsigned int col = 0x7fffffff;
        for (int x = startX; x <= doublePixel3(endX); x++) {
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
        int secondX = doublePixel(mx);
        int secondY = my;
        int startX = firstX;
        int startY = firstY;
        int endX = secondX;
        int endY = secondY;
        if (startX>endX) {int t = startX; startX = endX; endX = t;}
        if (startY>endY) {int t = startY; startY = endY; endY = t;}
        unsigned int *k2 = &getCurrentSprite().bitmap[0];
        clipBoardW = doublePixel3(endX-startX)+1;
        clipBoardH = doublePixel3(endY-startY)+1;
        for (int y = startY; y <= endY; y++) {
          for (int x = startX; x <= doublePixel3(endX); x++) {
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

void alpha(unsigned int &dest, unsigned int s) {
  int r0 = (dest>>0)&255;
  int g0 = (dest>>8)&255;
  int b0 = (dest>>16)&255;
  int a0 = (dest>>24)&255;
  int r1 = (s>>0)&255;
  int g1 = (s>>8)&255;
  int b1 = (s>>16)&255;
  int a1 = (s>>24)&255;
  int r = (r1-r0)*a1/255+r0;
  int g = (g1-g0)*a1/255+g0;
  int b = (b1-b0)*a1/255+b0;
  int a = (a1-a0)*a1/255+a0;
  dest=r|(g<<8)|(b<<16)|(a<<24);
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
        firstX = doublePixel(mx);
        firstY = my;
        mode++;
      }
      if (mode >= 2) {
        mode = 3;
        return;
      }
      int secondX = doublePixel(mx);
      int secondY = my;
      int startX = firstX;
      int startY = firstY;
      int endX = secondX;
      int endY = secondY;
      if (startX>endX) {int t = startX; startX = endX; endX = t;}
      if (startY>endY) {int t = startY; startY = endY; endY = t;}
      memset(overlay,0,sizeof(overlay));
      unsigned int col = 0x7fffffff;
      for (int x = startX; x <= doublePixel3(endX); x++) {
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
        int secondX = doublePixel(mx);
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
          for (int x = startX; x <= doublePixel3(endX); x++) {
            clipBoard[x-startX+(y-startY)*MAXSPRITEWIDTH] = k2[x+y*MAXSPRITEWIDTH];
            k2[x+y*MAXSPRITEWIDTH] = 0x00000000;
          }
        }
        getCurrentSprite().update();
        getCurrentSprite().texture();
      }
      if (mode == 2) {
        int secondX = doublePixel(mx);
        int secondY = my;
        int startX = firstX;
        int startY = firstY;
        int endX = secondX;
        int endY = secondY;
        if (startX>endX) {int t = startX; startX = endX; endX = t;}
        if (startY>endY) {int t = startY; startY = endY; endY = t;}

        memset(overlay,0,sizeof(overlay));
        for (int y = startY; y <= endY; y++) {
          for (int x = startX; x <= doublePixel3(endX); x++) {
            int x2 = (x - startX)*clipBoardW/(endX-startX+1);
            int y2 = (y - startY)*clipBoardH/(endY-startY+1);
            if (x >= 0 && x < w && y >= 0 && y < h)
              alpha(overlay[doublePixel(x)+y*MAXSPRITEWIDTH],clipBoard[x2+y2*MAXSPRITEWIDTH]);
              if (getCurrentSpriteCanvas().doublePixel)
                alpha(overlay[doublePixel3(x)+y*MAXSPRITEWIDTH],clipBoard[x2+y2*MAXSPRITEWIDTH]);
          }
        }
        getCurrentSprite().update();
        getCurrentSprite().texture();
        memset(overlay,0,sizeof(overlay));
      }
      if (mode == 3) {
        makeUndoPoint();
        int secondX = doublePixel(mx);
        int secondY = my;
        int startX = firstX;
        int startY = firstY;
        int endX = secondX;
        int endY = secondY;
        if (startX>endX) {int t = startX; startX = endX; endX = t;}
        if (startY>endY) {int t = startY; startY = endY; endY = t;}
        unsigned int *k2 = &getCurrentSprite().bitmap[0];
        for (int y = startY; y <= endY; y++) {
          for (int x = startX; x <= doublePixel3(endX); x++) {
            int x2 = (x - startX)*clipBoardW/(endX-startX+1);
            int y2 = (y - startY)*clipBoardH/(endY-startY+1);
            if (x >= 0 && x < w && y >= 0 && y < h)
              alpha(k2[doublePixel(x)+y*MAXSPRITEWIDTH],clipBoard[x2+y2*MAXSPRITEWIDTH]);
              if (getCurrentSpriteCanvas().doublePixel)
                alpha(k2[doublePixel3(x)+y*MAXSPRITEWIDTH],clipBoard[x2+y2*MAXSPRITEWIDTH]);
          }
        }
        getCurrentSprite().update();
        mode = 0;
      }
    }
  }
}

void handleToolSetAlpha(int mx, int my) {
  int w = getCurrentSprite().width;
  int h = getCurrentSprite().height;
  static int lastMx = mx;
  static int lastMy = my;
  if (toolWas != TOOL_SETALPHA || (mouseButtons & 2)) {
    lastMx = mx;
    lastMy = my;
  }
  if (mx >= 0 && mx < w && my >= 0 && my < h) {
    mx = doublePixel(mx);

    memset(overlay,0,sizeof(overlay));
    overlay[mx+my*MAXSPRITEWIDTH] = 0xffffffff;
    getCurrentSprite().update();
    getCurrentSprite().texture();
    memset(overlay,0,sizeof(overlay));

    if (mouseButtons & 2) {
      getCurrentTool().alpha = getCurrentSprite().bitmap[mx+my*MAXSPRITEWIDTH]>>24;
    }

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
        int kx = doublePixel((int)floor(mx+dx*i));
        int ky = (int)floor(my+dy*i);
        if (kx >= 0&&ky>=0&&kx<w&&ky<h)
          getCurrentSprite().bitmap[kx+ky*MAXSPRITEWIDTH] = (getCurrentSprite().bitmap[kx+ky*MAXSPRITEWIDTH] & 0x00ffffff)|(getCurrentTool().alpha<<24);
      }
      getCurrentSprite().update();
    }
  }
  lastMx = mx;
  lastMy = my;
}


void handleToolRotate(int mx, int my) { // handleToolTransform
  int w = getCurrentSprite().width;
  int h = getCurrentSprite().height;
  static int firstX;
  static int firstY;
  static int mode;
  static double centerX=0,centerY=0;
  if (toolWas != TOOL_ROTATE || (mouseButtons & 2)) {
    if (mode==2) {
      undo();
    }
    mode = 0;
    getCurrentTool().special = false;
  }
  if (mx >= 0 && mx < w && my >= 0 && my < h) {
    mx = doublePixel(mx);

    if (mouseButtons & 1) {
      if (mode == 0) {
        firstX = mx;
        firstY = my;
        mode++;
      }
      if (mode == 2) {
        mode = 0;
        return;
      }
      int secondX = doublePixel(mx);
      int secondY = my;
      int startX = firstX;
      int startY = firstY;
      int endX = secondX;
      int endY = secondY;
      if (startX>endX) {int t = startX; startX = endX; endX = t;}
      if (startY>endY) {int t = startY; startY = endY; endY = t;}
      memset(overlay,0,sizeof(overlay));
      unsigned int col = 0x7fffffff;
      for (int x = startX; x <= doublePixel3(endX); x++) {
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
        makeUndoPoint();
        int secondX = doublePixel(mx);
        int secondY = my;
        int startX = firstX;
        int startY = firstY;
        int endX = secondX;
        int endY = secondY;
        if (startX>endX) {int t = startX; startX = endX; endX = t;}
        if (startY>endY) {int t = startY; startY = endY; endY = t;}
        unsigned int *k2 = &getCurrentSprite().bitmap[0];
        clipBoardW = doublePixel3(endX-startX)+1;
        clipBoardH = doublePixel3(endY-startY)+1;
        centerX = (endX+startX)*0.5;
        centerY = (endY+startY)*0.5;
        for (int y = startY; y <= endY; y++) {
          for (int x = startX; x <= doublePixel3(endX); x++) {
            clipBoard[x-startX+(y-startY)*MAXSPRITEWIDTH] = k2[x+y*MAXSPRITEWIDTH];
            k2[x+y*MAXSPRITEWIDTH] = 0x00000000;
          }
        }
        getCurrentSprite().update();
        getCurrentSprite().texture();
      }
    }
  }
  if (mode == 2) {
    getCurrentTool().special = true;
    memset(overlay,0,sizeof(overlay));
    double ang = getCurrentTool().angle*2.0*M_PI/360.0;
    for (int y = 0; y < getCurrentSprite().height; y++) {
      for (int x = 0; x < getCurrentSprite().width; x++) {
        double dx = floor(x - centerX+0.5);
        double dy = floor(y - centerY+0.5);
        dx -= getCurrentTool().x;
        dy -= getCurrentTool().y;
        double d = sqrt(dx*dx+dy*dy);
        double a = atan2(dx,dy);
        double nx = sin(a+ang)*d+(clipBoardW-1)/2;
        double ny = cos(a+ang)*d+(clipBoardH-1)/2;
        nx /= getCurrentTool().w;
        ny /= getCurrentTool().h;
        int ax = (int)floor(nx+0.5);
        int ay = (int)floor(ny+0.5);
        if (ax>=0&&ay>=0&&ax<clipBoardW&&ay<clipBoardH) {
          overlay[doublePixel(x)+y*MAXSPRITEWIDTH]=clipBoard[ax+ay*MAXSPRITEWIDTH];
          overlay[doublePixel3(x)+y*MAXSPRITEWIDTH]=clipBoard[ax+ay*MAXSPRITEWIDTH];
        }
      }
    }
    if (getCurrentTool().place) {
      getCurrentTool().special = false;
      getCurrentTool().place = false;
      mode=0;
      unsigned int *k2 = &getCurrentSprite().bitmap[0];
      for (int y = 0; y < getCurrentSprite().height; y++) {
        for (int x = 0; x < getCurrentSprite().width; x++) {
          double dx = floor(x - centerX+0.5);
          double dy = floor(y - centerY+0.5);
          dx -= getCurrentTool().x;
          dy -= getCurrentTool().y;
          double d = sqrt(dx*dx+dy*dy);
          double a = atan2(dx,dy);
          double nx = sin(a+ang)*d+(clipBoardW-1)/2;
          double ny = cos(a+ang)*d+(clipBoardH-1)/2;
          nx /= getCurrentTool().w;
          ny /= getCurrentTool().h;
          int ax = (int)floor(nx+0.5);
          int ay = (int)floor(ny+0.5);
          if (ax>=0&&ay>=0&&ax<clipBoardW&&ay<clipBoardH) {
            alpha(k2[doublePixel(x)+y*MAXSPRITEWIDTH],clipBoard[ax+ay*MAXSPRITEWIDTH]);
            if (getCurrentSpriteCanvas().doublePixel)
              alpha(k2[doublePixel3(x)+y*MAXSPRITEWIDTH],clipBoard[ax+ay*MAXSPRITEWIDTH]);
          }
        }
      }
    }
    getCurrentSprite().update();
    getCurrentSprite().texture();
    memset(overlay,0,sizeof(overlay));
  }
}

void handleTool(int mx, int my) {
  colorBehindPixel(mx,my);
  switch(getCurrentTool().tool) {
  case TOOL_PENCIL: handleToolPencil(mx,my); break;
  case TOOL_FLOODFILL: handleToolFloodFill(mx,my); break;
  case TOOL_RECT: handleToolRect(mx,my); break;
  case TOOL_CIRCLE: handleToolCircle(mx,my); break;
  case TOOL_LINE: handleToolLine(mx,my); break;
  case TOOL_MOVE: handleToolMove(mx,my); break;
  case TOOL_COPY: handleToolCopy(mx,my); break;
  case TOOL_SCALE: handleToolScale(mx,my); break;
  case TOOL_ROTATE: handleToolRotate(mx,my); break;
  case TOOL_RECOLOR:   handleColorGet(mx,my); break;
  case TOOL_SETALPHA:   handleToolSetAlpha(mx,my); break;
  }
  toolWas = getCurrentTool().tool;
  mouseButtonsWas = mouseButtons;
}

int lastMx = 0;
int lastMy = 0;
spArray<unsigned int> before;
void displaySpriteEditField() {
  ImGui::SetNextWindowPos(ImVec2(_WIDTH(180),_HEIGHT(20)));
  coordX = 0;
  coordY = 0;
  if (lastMx>=0&&lastMy>=0&&lastMx<getCurrentSprite().width&&lastMy<getCurrentSprite().height) {coordX = lastMx; coordY = lastMy;}
  ImGui::Begin(String(("Sprite Editor /")+String(getCurrentTool().getName())+"##SpriteEditor").c_str(),NULL,ImVec2(_WIDTH(385),_HEIGHT(385)),-1.f,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove);
  ImGui::PushItemWidth(80);
  ImGui::InputInt("Nr", &getCurrentSpriteCanvas().spriteNr);
  getCurrentSpriteCanvas().spriteNr = clamp(getCurrentSpriteCanvas().spriteNr,0,(int)sprites.size()-1);
  ImGui::SameLine();
  ImGui::Text("  ");  
  ImGui::SameLine();
  ImGui::InputInt("Width",&getCurrentSprite().width);
  getCurrentSprite().width = clamp(getCurrentSprite().width,MINSPRITEWIDTH,MAXSPRITEWIDTH);
  ImGui::SameLine();
  ImGui::InputInt("Height",&getCurrentSprite().height);
  getCurrentSprite().height = clamp(getCurrentSprite().height,MINSPRITEHEIGHT,MAXSPRITEHEIGHT);
  ImGui::InputFloat("Zoom",&getCurrentSpriteCanvas().zoom,1.0);
  ImGui::SameLine();
  ImGui::PushItemWidth(52);
  ImGui::InputText("##Name123", getCurrentSprite().name,8);
  ImGui::PopItemWidth();
  ImGui::SameLine();
  ImGui::Text("#%04x",getCurrentSprite().getId());
  ImGui::SameLine();
  ImGui::Checkbox("Grid",&getCurrentSpriteCanvas().gridOn);
  ImGui::SameLine();
  if (ImGui::Checkbox("BG",&getCurrentSpriteCanvas().background)) {updateAll();}
  ImGui::SameLine();
  if (ImGui::Checkbox("Alpha",&getCurrentSpriteCanvas().showAlpha)) {updateAll();}
  if (ImGui::IsKeyDown('+')) {getCurrentSpriteCanvas().zoom += 1.0; ImGui::GetIO().KeysDown['+'] = 0;}
  if (ImGui::IsKeyDown('-')) {getCurrentSpriteCanvas().zoom -= 1.0; ImGui::GetIO().KeysDown['-'] = 0;}
  getCurrentSpriteCanvas().zoom = clamp(getCurrentSpriteCanvas().zoom,(float)MINSPRITEZOOM,(float)MAXSPRITEZOOM);
  ImGui::PopItemWidth();
  ImVec2 windowSize = ImGui::GetWindowSize();
  ImVec2 maxTex = ImVec2((float)getCurrentSprite().width/MAXSPRITEWIDTH,(float)getCurrentSprite().height/MAXSPRITEHEIGHT);
  ImVec2 cp0 = ImGui::GetCursorScreenPos();
  ImVec2 size = ImVec2(getCurrentSprite().width*getCurrentSpriteCanvas().zoom*getCurrentSpriteCanvas().aspect,getCurrentSprite().height*getCurrentSpriteCanvas().zoom);
  ImVec2 cp1 = ImVec2(cp0.x+size.x,cp0.y+size.y);
  ImVec2 size2 = ImVec2((float)getCurrentSprite().width,(float)getCurrentSprite().height/getCurrentSpriteCanvas().aspect);
  ImGui::Image((ImTextureID)((intptr_t)getCurrentSprite().texture()), size ,ImVec2(0,0),maxTex);
  ImGui::SameLine();
  ImVec2 k = ImVec2(size2.x*2,size2.y*2);
  k.x += 16;
  k.y += 16*6;
  ImGui::BeginChild("Tileable",k);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2(0,0));
    ImGui::Image((ImTextureID)((intptr_t)getCurrentSprite().texture()), size2 ,ImVec2(0,0),maxTex);
    ImGui::SameLine();
    ImGui::Image((ImTextureID)((intptr_t)getCurrentSprite().texture()), size2 ,ImVec2(0,0),maxTex);
    ImGui::Image((ImTextureID)((intptr_t)getCurrentSprite().texture()), size2 ,ImVec2(0,0),maxTex);
    ImGui::SameLine();
    ImGui::Image((ImTextureID)((intptr_t)getCurrentSprite().texture()), size2 ,ImVec2(0,0),maxTex);
    ImGui::PopStyleVar();
    if (getCurrentSpriteCanvas().mirroredMode) {
      ImGui::Checkbox("Mirror",&getCurrentSpriteCanvas().paintMirrored);
      if (getCurrentSpriteCanvas().paintMirrored) {
        ImGui::Checkbox("Mirror X##MirrorX88",&getCurrentSprite().mirrorPaintX);
        if (getCurrentSprite().mirrorPaintX) ImGui::InputInt("X##CenterX88",&getCurrentSprite().mirrorXPos);
        ImGui::Checkbox("Mirror Y##MirrorY88",&getCurrentSprite().mirrorPaintY);
        if (getCurrentSprite().mirrorPaintY) ImGui::InputInt("Y##CenterY88",&getCurrentSprite().mirrorYPos);
      }
      getCurrentSprite().mirrorXPos=clamp(doublePixel(getCurrentSprite().mirrorXPos),1,getCurrentSprite().width-2);
      getCurrentSprite().mirrorYPos=clamp(getCurrentSprite().mirrorYPos,1,getCurrentSprite().height-2);
    } else {
      getCurrentSpriteCanvas().paintMirrored = false;
    }
  ImGui::EndChild();
  ImDrawList *l = ImGui::GetWindowDrawList();
  l->AddRect(cp0,cp1,0xff202020,0);
  if (getCurrentSpriteCanvas().gridOn) {
    int w = getCurrentSprite().width;
    int h = getCurrentSprite().height;
    int add = 1;
    if (getCurrentSpriteCanvas().doublePixel) add = 2;
    for (int x = add; x < w; x+=add) {
      int xp = (int)floor(x * (cp1.x-cp0.x) / w + cp0.x);
      l->AddLine(ImVec2((float)xp,cp0.y),ImVec2((float)xp,cp1.y),0x40ffffff);
    }
    for (int y = 1; y < h; y++) {
      int yp = (int)floor(y * (cp1.y-cp0.y) / h + cp0.y);
      l->AddLine(ImVec2(cp0.x,(float)yp),ImVec2(cp1.x,(float)yp),0x40ffffff);
    }
  }
  if (getCurrentSprite().mirrorPaintX && getCurrentSpriteCanvas().paintMirrored) {
    int w = getCurrentSprite().width;
    int h = getCurrentSprite().height;
    int xp = (int)floor(getCurrentSprite().mirrorXPos * (cp1.x-cp0.x) / w + cp0.x);
    l->AddLine(ImVec2((float)xp,cp0.y),ImVec2((float)xp,cp1.y),0x8000ffff);
  }
  if (getCurrentSprite().mirrorPaintY && getCurrentSpriteCanvas().paintMirrored) {
    int w = getCurrentSprite().width;
    int h = getCurrentSprite().height;
    int yp = (int)floor(getCurrentSprite().mirrorYPos * (cp1.y-cp0.y) / h + cp0.y);
    l->AddLine(ImVec2(cp0.x,(float)yp),ImVec2(cp1.x,(float)yp),0x8000ffff);
  } 
  ImGui::End();
  int toolMousePosX = (int)round2((double)(mouseX-cp0.x)*getCurrentSprite().width/(cp1.x-cp0.x));
  int toolMousePosY = (int)round2((double)(mouseY-cp0.y)*getCurrentSprite().height/(cp1.y-cp0.y));
  lastMx = toolMousePosX;
  lastMy = toolMousePosY;
  before = getCurrentSprite().bitmap;

  if (mouseDelay<=0) {
    mouseDelay=0;
    handleTool(toolMousePosX,toolMousePosY);
  } else {mouseDelay--;}

  if (getCurrentSpriteCanvas().paintMirrored) {
    if (getCurrentSprite().mirrorPaintX) {
      for (int y = 0; y < getCurrentSprite().height; y++) {
        for (int x = 0; x < getCurrentSprite().width; x++) {
          unsigned int c0 = getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH];
          unsigned int c1 = before[x+y*MAXSPRITEWIDTH];
          if (c1 != c0) {
            int x2 = -1;
            if (toolMousePosX<getCurrentSprite().mirrorXPos && x<getCurrentSprite().mirrorXPos) 
              x2 = (getCurrentSprite().mirrorXPos-x-1)+getCurrentSprite().mirrorXPos;
            if (toolMousePosX>=getCurrentSprite().mirrorXPos && x>=getCurrentSprite().mirrorXPos) 
              x2 = getCurrentSprite().mirrorXPos-(x+1-getCurrentSprite().mirrorXPos);
            if (x2>=0&&x2<getCurrentSprite().width) {
              getCurrentSprite().bitmap[x2+y*MAXSPRITEWIDTH]=c0;
              getCurrentSprite().update();
            }
          }
        }
      }
    }
    if (getCurrentSprite().mirrorPaintY) {
      for (int y = 0; y < getCurrentSprite().height; y++) {
        for (int x = 0; x < getCurrentSprite().width; x++) {
          unsigned int c0 = getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH];
          unsigned int c1 = before[x+y*MAXSPRITEWIDTH];
          if (c1 != c0) {
            int y2 = -1;
            if (toolMousePosY<getCurrentSprite().mirrorYPos && y<getCurrentSprite().mirrorYPos) 
              y2 = (getCurrentSprite().mirrorYPos-y-1)+getCurrentSprite().mirrorYPos;
            if (toolMousePosY>=getCurrentSprite().mirrorYPos && y>=getCurrentSprite().mirrorYPos) 
              y2 = getCurrentSprite().mirrorYPos-(y+1-getCurrentSprite().mirrorYPos);
            if (y2>=0&&y2<getCurrentSprite().height) {
              getCurrentSprite().bitmap[x+y2*MAXSPRITEWIDTH]=c0;
              getCurrentSprite().update();
            }
          }
        }
      }
    }
  }

}

void displayColorPicker() {
  static bool isInit = true; if (isInit) ImGui::SetNextWindowPos(ImVec2(_WIDTH(0),_HEIGHT(330))); isInit = false;
  ImGui::Begin("Palette",NULL,ImVec2(_WIDTH(180),_HEIGHT(150)),-1,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove);
  ImVec2 windowSize = ImGui::GetWindowSize();
  ImVec2 windowPos = ImGui::GetWindowPos();
  ImVec2 cp0 = ImGui::GetCursorScreenPos();
  int w = (int)floor(windowSize.x-6);
  int h = (int)floor(windowSize.y-60);
  ImGui::Image((ImTextureID)((intptr_t)getCurrentPalette().paletteTexture()), ImVec2((float)w,(float)h));
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
      if (col == currentPixelColor) {
        l->AddRect(xy0,xy1,0xff0000ff,0);
        xy0.x += 1;
        xy0.y += 1;
        xy1.x -= 1;
        xy1.y -= 1;
        l->AddRect(xy0,xy1,0xff000000,0);
        colIdx = x+y*getCurrentPalette().width;
        xy0.x -= 1;
        xy0.y -= 1;
        xy1.x += 1;
        xy1.y += 1;
      }
      if (col == getCurrentPalette().slotColors[getCurrentPalette().slot]) {
        l->AddRect(xy0,xy1,0xffffffff,0);
        xy0.x += 1;
        xy0.y += 1;
        xy1.x -= 1;
        xy1.y -= 1;
        l->AddRect(xy0,xy1,0xff000000,0);
        colIdx = x+y*getCurrentPalette().width;
        xy0.x -= 1;
        xy0.y -= 1;
        xy1.x += 1;
        xy1.y += 1;
      }
    }
  }
  ImGui::PushItemWidth(80);
  if (ImGui::InputText("Color",colorInput,10) && (colIdx>=0)) {
    unsigned int color = fromColorString(colorInput);
    getCurrentPalette().colors[colIdx] = color;
    getCurrentPalette().slotColors[getCurrentPalette().slot] = color;
    getCurrentPalette().update();
  }
  ImGui::SameLine();
  ImGui::Text("#%02x",colIdx);
  ImGui::PopItemWidth();
  for (int i = 0; i < MAXPALETTESLOTS; i++) {
    ImVec2 cp = ImGui::GetCursorScreenPos();
    ImVec2 cp2 = ImGui::GetCursorScreenPos(); cp2.x += (windowSize.x-3) / MAXPALETTESLOTS - 2; cp2.y += windowSize.y-(cp2.y-windowPos.y);
    cp.x += (windowSize.x-3) / MAXPALETTESLOTS * i;
    cp2.x += (windowSize.x-3) / MAXPALETTESLOTS * i;
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

bool importSpriteAtlas(const char *fileName) {
  RGBAImage img = RGBAImage::fromFile(fileName);
  if (img.data == NULL) {someError = true; return false;}

  unsigned int textureId;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img.width,img.height,0,GL_RGBA,GL_UNSIGNED_BYTE,img.data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  bool finished = false;
  bool first = true;
  int borderLeft = 0;
  int borderRight = 0;
  int borderTop = 0;
  int borderBottom = 0;
  int sizeX = getCurrentSprite().width;
  int sizeY = getCurrentSprite().height;
  int distanceX = 0;
  int distanceY = 0;
  int firstSpriteNr = 0;
  int lastSpriteNr = 0;
  int spriteNr = 0;
  int displaySprite = 0;
  float zoom = 1.0;
  float zoom2 = 1.0;
  float zoom3 = 1.0;
  int maxSpritesPerLine = 1000;

  do
  {
    ImGuiNewFrame();
    if (first) ImGui::SetNextWindowPos(ImVec2(_WIDTH(0),_HEIGHT(0)));
    first = false;

    ImGui::Begin((String("Image:")+String((int)floor(img.width*zoom3))+"x"+String((int)floor(img.height*zoom3))+"##Image10").c_str());
    ImGui::PushItemWidth(64);
    ImGui::InputFloat("Zoom##ZoomSpa",&zoom,0.5);
    ImGui::PopItemWidth();
    zoom = clamp(zoom,0.1f,16.0f);
    ImVec2 cp0 = ImGui::GetCursorScreenPos();
    ImVec2 cp1 = ImGui::GetCursorScreenPos();
    double aspect = getCurrentSpriteCanvas().aspect;
    ImGui::Image((ImTextureID)((intptr_t)textureId),ImVec2((float)(img.width*zoom*zoom3),(float)(img.height*zoom/aspect*zoom3)));
    cp1.x += (float)(img.width*zoom);
    cp1.y += (float)(img.height*zoom/aspect);
    ImDrawList *l = ImGui::GetWindowDrawList();
    int oneLine = (int)floor(img.width*zoom3)-borderLeft-borderRight+distanceX;
    int spritesOnOneLine=-1;
    if (sizeX+distanceX > 0) {
      spritesOnOneLine=(oneLine)/(sizeX+distanceX);
      if (spritesOnOneLine > 0) {    
        if (spritesOnOneLine>maxSpritesPerLine) spritesOnOneLine=maxSpritesPerLine;
        for (int sp = firstSpriteNr; sp <= lastSpriteNr; sp++) {
          int lineX = sp % spritesOnOneLine;
          int lineY = sp / spritesOnOneLine;
          int x0 = borderLeft+lineX*(sizeX+distanceX);
          int y0 = borderTop+lineY*(sizeY+distanceY);
          int x1 = x0 + sizeX;
          int y1 = y0 + sizeY;
          ImVec2 cd0,cd1;
          cd0.x = x0*(cp1.x-cp0.x)/img.width+cp0.x;
          cd0.y = y0*(cp1.y-cp0.y)/img.height+cp0.y;
          cd1.x = x1*(cp1.x-cp0.x)/img.width+cp0.x;
          cd1.y = y1*(cp1.y-cp0.y)/img.height+cp0.y;
          l->AddRect(cd0,cd1,0xff0000ff,0);
        }
      }
    }
    ImGui::End();

    ImGui::Begin("Sprite");
    ImGui::PushItemWidth(64);
    ImGui::InputFloat("Zoom##ZoomSpr",&zoom2,0.5);
    zoom2 = clamp(zoom2,0.1f,16.0f);
    ImGui::InputInt("SpriteNr",&displaySprite);
    displaySprite = clamp(displaySprite,firstSpriteNr,lastSpriteNr);
    if (spritesOnOneLine > 0) {
      int lineX = displaySprite % spritesOnOneLine;
      int lineY = displaySprite / spritesOnOneLine;
      int x0 = borderLeft+lineX*(sizeX+distanceX);
      int y0 = borderTop+lineY*(sizeY+distanceY);
      int x1 = x0 + sizeX;
      int y1 = y0 + sizeY;
      ImVec2 ce0 = ImVec2((float)(x0/floor(img.width*zoom3)),(float)(y0/floor(img.height*zoom3)));
      ImVec2 ce1 = ImVec2((float)(x1/floor(img.width*zoom3)),(float)(y1/floor(img.height*zoom3)));
      ImVec2 cp0 = ImGui::GetCursorScreenPos();
      ImVec2 cp1 = ImGui::GetCursorScreenPos();
      cp1.x += sizeX*zoom2;
      cp1.y += (float)(sizeY*zoom2/aspect);
      ImGui::Image((ImTextureID)((intptr_t)textureId),ImVec2((float)(sizeX*zoom2),(float)(sizeY*zoom2/aspect)),ce0,ce1);
      ImDrawList *l = ImGui::GetWindowDrawList();
      l->AddRect(cp0,cp1,0xff0000ff,0);
    }
    ImGui::PopItemWidth();
    ImGui::End();
  
    ImGui::SetNextWindowFocus();
    ImGui::Begin("Sprite Atlas");
    ImGui::PushItemWidth(70);
    ImGui::Separator();
    ImGui::InputInt("InsertToSpriteNr",&spriteNr);
    ImGui::Separator();
    ImGui::InputFloat("ZoomSourcePicture",&zoom3,0.25);
    zoom3 = clamp(zoom3,0.025f,16.0f);
    ImGui::Separator();
    ImGui::InputInt("FirstSpriteNr",&firstSpriteNr);
    ImGui::InputInt("LastSpriteNr",&lastSpriteNr);
    ImGui::InputInt("MaxSpritesLine",&maxSpritesPerLine);
    if (maxSpritesPerLine<1) maxSpritesPerLine=1;
    ImGui::Separator();
    ImGui::InputInt("BoderLeft",&borderLeft);
    ImGui::InputInt("BoderTop",&borderTop);
    ImGui::InputInt("BoderRight",&borderRight);
    ImGui::InputInt("BoderBottom",&borderBottom);
    ImGui::InputInt("SizeX",&sizeX);
    ImGui::InputInt("SizeY",&sizeY);
    sizeX = clamp(sizeX,1,MAXSPRITEWIDTH);
    sizeY = clamp(sizeY,1,MAXSPRITEHEIGHT);
    ImGui::InputInt("DistanceX",&distanceX);
    ImGui::InputInt("DistanceY",&distanceY);
    if (firstSpriteNr<0) firstSpriteNr = 0;
    if (lastSpriteNr<firstSpriteNr) lastSpriteNr = firstSpriteNr;
    ImGui::PopItemWidth();
    if (ImGui::Button("Cancel##Cancel10")||ImGui::IsKeyDown(ImGui::GetIO().KeyMap[ImGuiKey_Escape])) {
      ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Escape]] = 0;
      finished = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Proceed##Proceed10")) {
      if (spritesOnOneLine > 0) {
        int toInsert = spriteNr;
        for (int nr = firstSpriteNr; nr <= lastSpriteNr; nr++) {
          int lineX = nr % spritesOnOneLine;
          int lineY = nr / spritesOnOneLine;
          int x0 = borderLeft+lineX*(sizeX+distanceX);
          int y0 = borderTop+lineY*(sizeY+distanceY);
          int x1 = x0 + sizeX;
          int y1 = y0 + sizeY;
          Sprite k;
          k.width = sizeX;
          k.height = sizeY;
          for (int y = 0; y < sizeY; y++) {
            for (int x = 0; x < sizeX; x++) {
              int rx = (x1-x0)*x/sizeX+x0;
              int ry = (y1-y0)*y/sizeY+y0;
              rx = (int)floor(rx/zoom3);
              ry = (int)floor(ry/zoom3);
              if (rx >= 0&&ry>=0&&rx<img.width&&ry<img.height) {
                k.bitmap[x+y*MAXSPRITEWIDTH]=img.data[rx+ry*img.width];
              }
            }
          }
          k.update();
          k.texture();
          sprites.spInsert(sprites,k,toInsert);
          toInsert++;
        }
        finished = true;
      } else {
        finished = false;
      }
    }
    ImGui::End();
    render();
    refreshGraphics();
  } while(!finished);

  delete[] img.data;
  glDeleteTextures(1, &textureId);

  return true;
}

spHashMap<int,String> currentPaths;

RGBAImage loadMyFormats(const char *fileName) {
  RGBAImage r;
  FILE *in = fopen(fileName,"rb");
  if (in == NULL) {someError = true;return r;} //img.data is NULL
  unsigned char s1,s2;
  fread(&s1,1,1,in);
  fread(&s2,1,1,in);
  if (s1 == 'S' && s2 == 'P') {
    unsigned char w,h;
    fread(&w,1,1,in);
    fread(&h,1,1,in);
    r.width = w;
    r.height = h;
    r.data = new unsigned int[w*h];
    fread(r.data,1,w*h*4,in);
  }
  if (s1 == 'S' && s2 == 'S') {
    unsigned short spriteCount;
    fread(&spriteCount,1,2,in);
    if (spriteCount <= 0) return r;
    int spriteWidthHeight = 32;
    int textureWidth = 512;
    int spritesPerLine = textureWidth/spriteWidthHeight;
    int w2 = spriteWidthHeight*spriteCount;
    if (w2 > textureWidth) w2 = textureWidth;
    int h2 = spriteWidthHeight*((spriteCount+spritesPerLine-1)/spritesPerLine);
    r.width = w2;
    r.height = h2;
    r.data = new unsigned int[w2*h2];
    memset(r.data,0,w2*h2*4);
    for (int i = 0; i < spriteCount; i++) {
      char name[10];
      fread(name,1,10,in);
      unsigned char s1,s2;
      fread(&s1,1,1,in);
      fread(&s2,1,1,in);
      if (s1 == 'S' && s2 == 'P') {
        unsigned char w,h;
        fread(&w,1,1,in);
        fread(&h,1,1,in);
        unsigned int *data = new unsigned int[w*h];
        fread(data,1,w*h*4,in);
        for (int y = 0; y < spriteWidthHeight; y++) {
          for (int x = 0; x < spriteWidthHeight; x++) {
            int rx = x + (i % spritesPerLine)*spriteWidthHeight;
            int ry = y + (i / spritesPerLine)*spriteWidthHeight;
            int cx = x * w / spriteWidthHeight;
            int cy = y * h / spriteWidthHeight;
            if (rx>=0&&ry>=0&&rx<w2&&ry<h2&&cx>=0&&cy>=0&&cx<w&&cy<h) {
              r.data[rx+ry*w2]=data[cx+cy*w];
            }
          }
        }
        delete[] data;
      }
    }
  }
  fclose(in);
  return r;
}

void exportSprites(const char *fileName) {
  String fName = String(fileName);
  if (fName.find(".")<0) fName += ".png";

  int currentWidth = 0;
  int currentHeight = 0;
  {for(int i = 0; i < (int)sprites.size(); i++) {
    currentWidth += sprites[i].width;
    if (currentWidth > 1024) {currentWidth = 1024; break;}
  }}

  int currentX = 0;
  int currentY = 0;
  int lastMaxHeight = 0;
  int lastLineY = 0;
  {for(int i = 0; i < (int)sprites.size(); i++) {
    currentX += sprites[i].width;
    if (sprites[i].height>lastMaxHeight) lastMaxHeight = sprites[i].height;
    if (currentX > 1024) {
      currentX = sprites[i].width;
      currentY += lastMaxHeight;
      lastLineY = currentY;
      lastMaxHeight = sprites[i].height;
    }
  }}
  currentHeight = lastLineY + lastMaxHeight;
  unsigned int *data = new unsigned int[currentWidth*currentHeight];
  RGBAImage img(currentWidth,currentHeight,data);
  memset(data,0,currentWidth*currentHeight*4);

  currentX = 0;
  currentY = 0;
  lastMaxHeight = 0;
  {for(int i = 0; i < (int)sprites.size(); i++) {
    int paintCurrentX = currentX;
    currentX += sprites[i].width;
    if (sprites[i].height>lastMaxHeight) lastMaxHeight = sprites[i].height;
    if (currentX > 1024) {
      paintCurrentX = 0;
      currentX = sprites[i].width;
      currentY += lastMaxHeight;
      lastMaxHeight = sprites[i].height;
    }
    for (int y = 0; y < sprites[i].height; y++) {
      for (int x = 0; x < sprites[i].width; x++) {
        int dx = x + paintCurrentX;
        int dy = y + currentY;
        if (dx>=0&&dx<currentWidth&&dy>=0&&dy<currentHeight) {
          data[dx+dy*currentWidth]=sprites[i].bitmap[x+y*MAXSPRITEWIDTH];
        }
      }
    }
  }}
  if (!img.savePNG(fName.c_str())) someError = true;
  delete[] data;
}

bool importSpritesCounter(const char *fileName) {
  String path = getPath(String(fileName));
  String name = getName(String(fileName));
  int firstCounterPos=-1;
  for (int i = (int)name.size()-1; i >= 0; i--) {
    char c = name[i];
    if (c >= '0' && c <= '9')
      firstCounterPos = i;
    else
      break;
  }
  if (firstCounterPos == -1) {someError = true; return false;}
  if (firstCounterPos < (int)name.size()-4) {someError = true; return false;}
  int a = atoi(&name[firstCounterPos]);
  static int destSprite = 0;
  int firstSprite = a;
  int lastSprite = a;   

  bool finished = false;
  bool first = true;
  do
  {
    ImGuiNewFrame();
    if (first) ImGui::SetNextWindowPos(ImVec2(_WIDTH(0),_HEIGHT(0)));
    first = false;
    ImGui::SetNextWindowFocus();
    ImGui::Begin("Import Sprites##ImportSprites66");
    ImGui::PushItemWidth(64);
    ImGui::InputInt("InsertTo##InsertTo66", &destSprite,0);
    ImGui::InputInt("sprite0##Sprite066", &firstSprite,0);
    ImGui::InputInt("spriteN##SpriteN66", &lastSprite,0);
    ImGui::PopItemWidth();
    destSprite = clamp(destSprite,0,(int)sprites.size());
    firstSprite = clamp(firstSprite,0,9999);
    lastSprite = clamp(lastSprite,firstSprite,9999);
    if (ImGui::Button("Cancel##Cancel66")||ImGui::IsKeyDown(ImGui::GetIO().KeyMap[ImGuiKey_Escape])) {
      ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Escape]] = 0;
      finished = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Proceed##Proceed66")) {
      spArray<Sprite> ins;
      for (int i = firstSprite; i <= lastSprite; i++) {
        char buffer[10];
        sprintf(buffer,"%04d",i);
        String fName = path + name.substr(0,firstCounterPos) + String(buffer)+".png";
        RGBAImage img = RGBAImage::fromFile(fName.c_str());
        if (img.data == NULL) {someError = true; break;}
        Sprite spr;
        spr.width = img.width;
        spr.height = img.height;
        for (int y = 0; y < spr.height; y++) {
          for (int x = 0; x < spr.width; x++) {
            spr.bitmap[x+y*MAXSPRITEWIDTH] = img.data[x+y*img.width];
          }
        }
        ins.push_back(spr);
      }
      if (!someError) {
        if (destSprite<(int)sprites.size()) {
          for (int i = lastSprite-firstSprite; i>= 0; i--) {
              sprites.spInsert(sprites,ins[i],destSprite);
              sprites[destSprite].update();
              sprites[destSprite].texture();
          }
        } else {
          for (int i = 0; i <= lastSprite-firstSprite; i++) {
              sprites.push_back(ins[i]);
              sprites.back().update();
              sprites.back().texture();
          }
        }
      }
      finished = true;
    }             
    ImGui::End();
    render();
  
    refreshGraphics();
  } while(!finished);
  return true;
}

bool exportSpritesCounter(const char *fileName) {
  String path = getPath(String(fileName));
  String name = getName(String(fileName));
  if ((int)name.size()>4) name.resize(4);
  RGBAImage img;
  for(int i = 0; i < (int)sprites.size(); i++) {
    img.width = sprites[i].width;
    img.height = sprites[i].height;
    img.data = new unsigned int[img.width*img.height];
    for(int y = 0; y < img.height; y++) {
      for(int x = 0; x < img.width; x++) {
        img.data[x+y*img.width] = sprites[i].bitmap[x+y*MAXSPRITEWIDTH];
      }
    }
    char buffer[10];
    sprintf(buffer,"%04d",i);
    String fileName2 = path+name+String(buffer)+".png";
    if (!img.savePNG(fileName2.c_str())) {
      someError = true;
      delete[] img.data;
      return false;
    }
    delete[] img.data;
  }
  return true;
}

bool bgPixels = false;
unsigned int swizzleRGBA(int x, int y, unsigned int t) {
  unsigned int t2 = (t & 0xff00ff00)|((t>>16)&0x000000ff)|((t<<16)&0x00ff0000);
  if (bgPixels) {
    unsigned int a = (t >> 24) & 255;
    int r0 = t2 & 255;
    int g0 = (t2>>8) & 255;
    int b0 = (t2>>16) & 255;
    int z = (((x + y)%3)+1)*0x20;
    r0 = (r0-z)*a/255+z;
    g0 = (g0-z)*a/255+z;
    b0 = (b0-z)*a/255+z;
    return 0xff000000|(r0)|(g0<<8)|(b0<<16);
  }
  return t2;
}

bool importPSDLayers(const char *fileName) {
  PSDImage img;
  if (!img.Load(fileName)) {someError = true;return false;}
  if (img.NumLayers()==0) {someError = true;return false;}
  int imageWidth = img.header.Columns;
  int imageHeight = img.header.Rows;
  if (imageWidth > MAXSPRITEWIDTH) {someError = true;return false;}
  if (imageHeight > MAXSPRITEHEIGHT) {someError = true;return false;}

  float zoom = 3.0;
  int destSprite = 0;
  int layer = 0;
  int layer0 = 0;
  int layerN = img.NumLayers()-1;
  bool doublePixel = getCurrentSpriteCanvas().doublePixel;
  bool finished = false;
  bool first = true;
  unsigned int textureId = 0;
  bgPixels = false;
  do
  {
    ImGuiNewFrame();

    ImGui::Begin(("Preview "+String(imageWidth)+"x"+String(imageHeight)+"##Preview77").c_str());
    ImGui::PushItemWidth(64);
    ImGui::InputFloat("Zoom##Zoom77",&zoom,0.5);
    ImGui::InputInt("Layer##Layer77",&layer,1);
    ImGui::SameLine();
    ImGui::Checkbox("BG##BG77",&bgPixels);
    ImGui::PopItemWidth();
    zoom = clamp(zoom,0.1f,16.0f);
    layer = clamp(layer,0,img.NumLayers()-1);

    int ofsX,ofsY,sizX,sizY;
    unsigned int *imagedata=(unsigned int*)img.GetLayerData(layer, sizX, sizY, ofsX, ofsY);
    unsigned int *data = new unsigned int[imageWidth*imageHeight];
    memset(data,0,imageWidth*imageHeight*4);
    for (int y = 0; y < sizY; y++) {
      for (int x = 0; x < sizX; x++) {
        int xp = ofsX + x;
        int yp = ofsY + y;
        if (xp<imageWidth&&yp<imageHeight&&xp>=0&&yp>=0)
          data[xp+yp*imageWidth]=swizzleRGBA(x,y,imagedata[x+y*sizX]);
      }
    }
    if (textureId != 0) glDeleteTextures(1,&textureId);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,imageWidth,imageHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    delete[] data;

    ImVec2 dp=ImVec2(imageWidth*zoom*(doublePixel ? 2 : 1)*getCurrentSpriteCanvas().aspect,imageHeight*zoom);
    ImVec2 cp0 = ImGui::GetCursorScreenPos();
    ImVec2 cp1 = cp0;
    cp0.x += dp.x;
    cp0.y += dp.y;
    ImGui::Image((ImTextureID)((intptr_t)textureId),dp);
    ImDrawList *l = ImGui::GetWindowDrawList();
    l->AddRect(cp0,cp1,0xff202020,0);
    ImGui::End();

    if (first) ImGui::SetNextWindowPos(ImVec2(_WIDTH(0),_HEIGHT(0)));
    first = false;
    ImGui::SetNextWindowFocus();
    ImGui::Begin("Import Layers[psd]##ImportLayers77");
    ImGui::PushItemWidth(64);
    ImGui::InputInt("InsertTo##InsertTo77", &destSprite,0);
    ImGui::Separator();
    ImGui::Text("Last layer:%d",img.NumLayers()-1);
    ImGui::InputInt("Layer0##Layer077", &layer0,0);
    ImGui::InputInt("LayerN##LayerN77", &layerN,0);
    ImGui::Separator();
    ImGui::Checkbox("DoublePixel##DoublePixel77)",&doublePixel);
    ImGui::PopItemWidth();
    destSprite = clamp(destSprite,0,(int)sprites.size());
    layer0 = clamp(layer0,0,img.NumLayers()-1);
    layerN = clamp(layerN,layer0,img.NumLayers()-1);
    if (ImGui::Button("Cancel##Cancel77")||ImGui::IsKeyDown(ImGui::GetIO().KeyMap[ImGuiKey_Escape])) {
      ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Escape]] = 0;
      finished = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Proceed##Proceed77")) {
      spArray<Sprite> ins;
      for(int i = layer0; i <= layerN; i++) {
        int ofsX,ofsY,sizX,sizY;
        unsigned int *imagedata=(unsigned int*)img.GetLayerData(i, sizX, sizY, ofsX, ofsY);
        Sprite k;
        k.width = imageWidth*(doublePixel?2:1);
        k.height = imageHeight;
        for (int y = 0; y < sizY; y++) {
          for (int x = 0; x < sizX*(doublePixel?2:1); x++) {
            int xp = ofsX*(doublePixel?2:1) + x;
            int yp = ofsY + y;
            if (xp<k.width&&yp<k.height&&xp>=0&&yp>=0)
              k.bitmap[xp+yp*MAXSPRITEWIDTH]=swizzleRGBA(x,y,imagedata[x/(doublePixel?2:1)+y*sizX]);
          }
        }
        ins.push_back(k);
      }
      if (destSprite<(int)sprites.size()) {
        for (int i = layerN-layer0; i>= 0; i--) {
            sprites.spInsert(sprites,ins[i],destSprite);
            sprites[destSprite].update();
            sprites[destSprite].texture();
        }
      } else {
        for (int i = 0; i <= layerN-layer0; i++) {
            sprites.push_back(ins[i]);
            sprites.back().update();
            sprites.back().texture();
        }
      }
      finished = true;
    }             
    ImGui::End();
    render();
  
    refreshGraphics();
  } while(!finished);

   if (textureId != 0) glDeleteTextures(1,&textureId);

  return true;
}

bool importAnimatedGif(const char *fileName) {

  FILE *in = fopen(fileName,"rb");
  if (in == NULL) {someError = true; return false;}
  fseek(in,0,SEEK_END);
  int64_t fileSize = ftell(in);
  fseek(in,0,SEEK_SET);
  unsigned char *arr = new unsigned char[fileSize];
  fread(arr,1,fileSize,in);
  fclose(in);                                                
  int xg,yg,zg,compg;
  unsigned char *gifData = stbi_load_gif_from_memory(arr, fileSize, NULL, &xg, &yg, &zg, &compg, 4);
  delete[] arr;
  if (gifData==NULL) {someError = true; return false;}
  if (xg <= 0 || xg > MAXSPRITEWIDTH) {delete[] gifData; someError = true; return false;}
  if (yg <= 0 || yg > MAXSPRITEHEIGHT) {delete[] gifData; someError = true; return false;}
  if (zg <= 0) {delete[] gifData; someError = true; return false;}

  float zoom = 3.0;
  int frame = 0;
  int frameWidth = xg;
  int frameHeight = yg;
  int frameCount = zg;
  int destSprite = 0;
  int layer0 = 0;
  int layerN = frameCount-1;
  bool finished = false;
  bool first = true;
  unsigned int textureId = 0;
  bgPixels = false;
  do
  {
    ImGuiNewFrame();

    ImGui::Begin(("Preview "+String(frameWidth)+"x"+String(frameHeight)+"##Preview177").c_str());
    ImGui::PushItemWidth(64);
    ImGui::InputFloat("Zoom##Zoom177",&zoom,0.5);
    ImGui::InputInt("Frame##Layer177",&frame,1);
    ImGui::PopItemWidth();
    zoom = clamp(zoom,0.1f,16.0f);
    frame = clamp(frame,0,frameCount-1);      
    unsigned int *frameData=(unsigned int*)&gifData[frameWidth*frameHeight*4*frame];
    if (textureId != 0) glDeleteTextures(1,&textureId);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,frameWidth,frameHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,frameData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    ImVec2 dp=ImVec2(frameWidth*zoom*getCurrentSpriteCanvas().aspect,frameHeight*zoom);
    ImVec2 cp0 = ImGui::GetCursorScreenPos();
    ImVec2 cp1 = cp0;
    cp0.x += dp.x;
    cp0.y += dp.y;
    ImGui::Image((ImTextureID)((intptr_t)textureId),dp);
    ImDrawList *l = ImGui::GetWindowDrawList();
    l->AddRect(cp0,cp1,0xff202020,0);
    ImGui::End();

    if (first) ImGui::SetNextWindowPos(ImVec2(_WIDTH(0),_HEIGHT(0)));
    first = false;
    ImGui::SetNextWindowFocus();
    ImGui::Begin("Import Animated Gif##ImportAnimatedGif177");
    ImGui::PushItemWidth(64);
    ImGui::InputInt("InsertTo##InsertTo177", &destSprite,0);
    ImGui::Separator();
    ImGui::Text("Last Frame:%d",frameCount-1);
    ImGui::InputInt("Frame0##Layer0177", &layer0,0);
    ImGui::InputInt("FrameN##LayerN177", &layerN,0);
    ImGui::Separator();
    ImGui::PopItemWidth();
    destSprite = clamp(destSprite,0,(int)sprites.size());
    layer0 = clamp(layer0,0,frameCount-1);
    layerN = clamp(layerN,layer0,frameCount-1);
    if (ImGui::Button("Cancel##Cancel177")||ImGui::IsKeyDown(ImGui::GetIO().KeyMap[ImGuiKey_Escape])) {
      ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Escape]] = 0;
      finished = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Proceed##Proceed177")) {
      spArray<Sprite> ins;
      for(int i = layer0; i <= layerN; i++) {
        Sprite k;
        k.width = frameWidth;
        k.height = frameHeight;
        unsigned int *l = (unsigned int*)&gifData[frameWidth*frameHeight*4*i];
        for (int y = 0; y < frameHeight; y++) {
          for (int x = 0; x < frameWidth; x++) {
            k.bitmap[x+y*MAXSPRITEWIDTH]=l[x+y*frameWidth];
          }
        }
        ins.push_back(k);
      }
      if (destSprite<(int)sprites.size()) {
        for (int i = layerN-layer0; i>= 0; i--) {
            sprites.spInsert(sprites,ins[i],destSprite);
            sprites[destSprite].update();
            sprites[destSprite].texture();
        }
      } else {
        for (int i = 0; i <= layerN-layer0; i++) {
            sprites.push_back(ins[i]);
            sprites.back().update();
            sprites.back().texture();
        }
      }
      finished = true;
    }             
    ImGui::End();
    render();
  
    refreshGraphics();
  } while(!finished);

  delete[] gifData;
  if (textureId != 0) glDeleteTextures(1,&textureId);

  return true;
}

unsigned int previewPictureId=0;
int previewPictureWidth=1;
int previewPictureHeight=1;
void loadPreviewPicture(const char *fileName) {
  if (previewPictureId!=0) {
    glDeleteTextures(1,&previewPictureId);
    previewPictureId=0;
  }
  if (fileName == NULL) return;
  RGBAImage img = RGBAImage::fromFile(fileName);
  if (img.data == NULL) {
    img = loadMyFormats(fileName);
    if (img.data == NULL) return;
  }
  glGenTextures(1, &previewPictureId);
  glBindTexture(GL_TEXTURE_2D, previewPictureId);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img.width,img.height,0,GL_RGBA,GL_UNSIGNED_BYTE,img.data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  previewPictureWidth=img.width;
  previewPictureHeight=img.height;

  delete[] img.data;
}

bool saveMapSimpleLayer(const char *fname);
bool loadMapSimpleLayer(const char *fname);

void displayLoadSave() {
  if (loadSave == LoadSave_None)
    return;

  bool isLoading = true;
  switch(loadSave) {
  case LoadSave_SaveMapSimpleLayer:
  case LoadSave_ExportSpritesCounter:
  case LoadSave_ExportSprites:
  case LoadSave_ExportPalette:
  case LoadSave_ExportSprite:
  case LoadSave_SaveSprite:
  case LoadSave_SaveSpriteSheet:
  case LoadSave_SavePalette: isLoading = false; break;
  }

  FileDialog *f = new FileDialog();
  f->open(isLoading,"*.*\0\0",loadPreviewPicture);
  if (currentPaths.spHas(currentPaths,loadSave))
    f->setCurrentPath(currentPaths[loadSave]);

  bool finished;
  bool first = true;
  bool first2 = true;
  float zoom4 = 1.f;

  ImGui::PushStyleColor(ImGuiCol_WindowBg,ImVec4(0,0,0,1.0));

  do
  {
    ImGuiNewFrame();
      
    if (previewPictureId != 0) {
      ImVec2 cv2 = ImVec2((float)previewPictureWidth,(float)previewPictureHeight);
      cv2.x *= zoom4;
      cv2.y *= zoom4/getCurrentSpriteCanvas().aspect;
      ImVec2 cv1 = cv2;
      cv2.x += 64;
      cv2.y += 64;
      ImGui::SetNextWindowSize(cv2);
      if (first2) ImGui::SetNextWindowPos(ImVec2(_WIDTH(0),_HEIGHT(0)));
      first2=false;
      ImGui::Begin(("Preview "+String(previewPictureWidth)+"x"+String(previewPictureHeight)+"##Preview44").c_str());
      ImGui::PushItemWidth(64);
      ImGui::InputFloat("Zoom##Zoom4",&zoom4,0.5);
      ImGui::PopItemWidth();
      zoom4 = clamp(zoom4,0.1f,32.f);
      ImGui::Image((ImTextureID)((intptr_t)previewPictureId),cv1);
      ImGui::End();
    }

    if (first) ImGui::SetNextWindowPos(ImVec2(_WIDTH(20),_HEIGHT(20)));
    first = false;
    ImGui::SetNextWindowFocus();
    finished = f->display();

    render();

    refreshGraphics();
  } while(!finished);

  ImGui::PopStyleColor();
  ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Escape]] = 0;
  
  currentPaths[loadSave] = f->getCurrentPath();

  String fname = f->getSelectedFileName();
  if (!fname.empty()) {
    switch(loadSave) {
    case LoadSave_SavePalette: savePalette(fname.c_str()); break;
    case LoadSave_LoadPalette: loadPalette(fname.c_str()); break;
    case LoadSave_SaveSprite: {saveSprite(fname.c_str());} break;
    case LoadSave_LoadSprite: {loadSprite(fname.c_str());} break;
    case LoadSave_SaveSpriteSheet: {saveSpriteSheet(fname.c_str());} break;
    case LoadSave_LoadSpriteSheet: {loadSpriteSheet(fname.c_str());} break;
    case LoadSave_ImportSprite: {importSprite(fname.c_str());} break;
    case LoadSave_ExportSprite: {exportSprite(fname.c_str());} break;
    case LoadSave_ImportPalette: {importPalette(fname.c_str());} break;
    case LoadSave_ExportPalette: {exportPalette(fname.c_str());} break;
    case LoadSave_ImportSpriteAtlas: {importSpriteAtlas(fname.c_str());} break;
    case LoadSave_ExportSprites: {exportSprites(fname.c_str());} break;
    case LoadSave_ImportSpritesCounter: {importSpritesCounter(fname.c_str());} break;
    case LoadSave_ExportSpritesCounter: {exportSpritesCounter(fname.c_str());} break;
    case LoadSave_ImportPSDLayers: {importPSDLayers(fname.c_str());} break;
    case LoadSave_ImportAnimatedGif: {importAnimatedGif(fname.c_str());} break;
    case LoadSave_SaveMapSimpleLayer: {saveMapSimpleLayer(fname.c_str());} break;
    case LoadSave_LoadMapSimpleLayer: {loadMapSimpleLayer(fname.c_str());} break;
    }
  }

  updateAll();
  ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Escape]] = 0; // clear escape key, just in case it was used for "cancel" before

  loadSave = LoadSave_None;
  loadPreviewPicture(NULL);
  delete f;
}

void buttonActive(int toolType) {
  if (getCurrentTool().tool == toolType)
    ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(0.5f,0.6f,0.9f,1));
  else
    ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(0.2f,0.3f,0.4f,1));
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
  static bool isInit = true; if (isInit) ImGui::SetNextWindowPos(ImVec2(_WIDTH(0),_HEIGHT(20))); isInit = false;
  ImGui::Begin("Tools", NULL, ImVec2(_WIDTH(180),_HEIGHT(310)),-1,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove);
  ImGui::PushItemWidth(70);
#ifndef NOT_DOS
  ImGui::SliderFloat("Sensitivity", &getCurrentTool().mouseSpeed, MOUSESPEEDMIN, MOUSESPEEDMAX);
#endif
  ImGui::PopItemWidth();
  ImGui::PushItemWidth(70);
  ImGui::Separator();
  ImGui::Text(getCurrentTool().getName());
  int t = getCurrentTool().tool;
  if (t==TOOL_PENCIL||t==TOOL_RECT||t==TOOL_CIRCLE||t==TOOL_LINE) {
    ImGui::SliderFloat("Pixels", &getCurrentTool().size, TOOLSIZEMIN, TOOLSIZEMAX);
    ImGui::InputFloat("Pixels##Pixels2a", &getCurrentTool().size, 0.5);
  }
  if (t==TOOL_ROTATE) {
    if (getCurrentTool().special) {
      ImGui::InputFloat("Angle",&getCurrentTool().angle,10.f); 
      ImGui::SameLine();
      if (ImGui::Button("Place")) { 
        getCurrentTool().place=true;
      }
      ImGui::InputFloat("x##x11",&getCurrentTool().x,1.0f);
      ImGui::SameLine();
      ImGui::InputFloat("y##y11",&getCurrentTool().y,1.0f);
      ImGui::InputFloat("w##w11",&getCurrentTool().w,0.05f);
      ImGui::SameLine();
      ImGui::InputFloat("h##h11",&getCurrentTool().h,0.05f);
      getCurrentTool().h = clamp(getCurrentTool().h,0.001f,128.f);
      getCurrentTool().w = clamp(getCurrentTool().w,0.001f,128.f);
    }
  }
  if (t == TOOL_RECT) {ImGui::Checkbox("Filled##Filled1",&getCurrentTool().filled);}
  if (t == TOOL_CIRCLE) {ImGui::Checkbox("Filled##Filled2",&getCurrentTool().filled);}
  if (t == TOOL_MOVE || t == TOOL_COPY) {
    if (ImGui::Button("FlipX##FlipX1")||ImGui::IsKeyDown('x')) {ImGui::GetIO().KeysDown['x'] = 0; flipClipBoardX();}
    ImGui::SameLine(); 
    if (ImGui::Button("FlipY##FlipY1")||ImGui::IsKeyDown('y')) {ImGui::GetIO().KeysDown['y'] = 0;flipClipBoardY();}
  }
  if (t == TOOL_RECOLOR) {
    if (ImGui::Button("Colorize")) {
      for (int y = 0; y < getCurrentSprite().height; y++) {
        for (int x = 0; x < getCurrentSprite().width; x++) { 
          unsigned int c = getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH];
          if (c == getCurrentTool().color0)
            getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH] = getCurrentTool().color1;
        }
      }
      getCurrentSprite().update();
      getCurrentSprite().texture();
    }
    ImGui::SameLine();
    if (ImGui::Button("On all sprites")) {
      for (int i = 0; i < (int)sprites.size(); i++) {
        Sprite &a = sprites[i];
        for (int y = 0; y < a.height; y++) {
          for (int x = 0; x < a.width; x++) { 
            unsigned int c = a.bitmap[x+y*MAXSPRITEWIDTH];
            if (c == getCurrentTool().color0)
              a.bitmap[x+y*MAXSPRITEWIDTH] = getCurrentTool().color1;
          }
        }
      }
      updateAll();
    }
    ImDrawList *l = ImGui::GetWindowDrawList();
    ImVec2 cp0 = ImGui::GetCursorScreenPos();
    ImVec2 b10 = cp0;
    b10.x += 4;
    b10.y += 14;
    ImVec2 b11 = b10;
    b11.x += 30;
    b11.y += 20;

    ImVec2 b20 = b10;
    b20.x += 35;
    ImVec2 b21 = b20;
    b21.x += 30;
    b21.y += 20;
    ImGui::Text("Source/Dest");    
    l->AddRectFilled(b10,b11,getCurrentTool().color0);
    l->AddRect(b10,b11,0xffffffff);
    l->AddRectFilled(b20,b21,getCurrentTool().color1);
    l->AddRect(b20,b21,0xffffffff);
    if (ImGui::IsMouseHoveringRect(b10,b11) && (mouseButtons & 1)) {
      getCurrentTool().color0 = getCurrentPalette().slotColors[getCurrentPalette().slot];
    }
    if (ImGui::IsMouseHoveringRect(b20,b21) && (mouseButtons & 1)) {
      getCurrentTool().color1 = getCurrentPalette().slotColors[getCurrentPalette().slot];
    }
  }
  if (t == TOOL_SETALPHA) {
    ImGui::InputInt("Alpha",&getCurrentTool().alpha);
    getCurrentTool().alpha = clamp(getCurrentTool().alpha,0,255);
  }
  mouseSpeed = getCurrentTool().mouseSpeed;
  ImGui::PopItemWidth();
  ImGui::SetCursorPosY(118);
  ImGui::Separator();
  if (!getCurrentTool().special) {
    buttonActive(TOOL_PENCIL); if (ImGui::Button("Pencil")) getCurrentTool().tool = TOOL_PENCIL; buttonActiveEnd();
    buttonActive(TOOL_FLOODFILL); if (ImGui::Button("Fill")) getCurrentTool().tool = TOOL_FLOODFILL; buttonActiveEnd();
    buttonActive(TOOL_RECT); if (ImGui::Button("Rect")) getCurrentTool().tool = TOOL_RECT; buttonActiveEnd();
    buttonActive(TOOL_CIRCLE); if (ImGui::Button("Circle")) getCurrentTool().tool = TOOL_CIRCLE; buttonActiveEnd();
    buttonActive(TOOL_LINE); if (ImGui::Button("Line")) getCurrentTool().tool = TOOL_LINE; buttonActiveEnd();
    buttonActive(TOOL_MOVE); if (ImGui::Button("Move")) getCurrentTool().tool = TOOL_MOVE; buttonActiveEnd();
    buttonActive(TOOL_COPY); if (ImGui::Button("Copy")) getCurrentTool().tool = TOOL_COPY; buttonActiveEnd();
    buttonActive(TOOL_SCALE); if (ImGui::Button("Scale")) getCurrentTool().tool = TOOL_SCALE; buttonActiveEnd();
    ImGui::SameLine();
    buttonActive(TOOL_ROTATE); if (ImGui::Button("Transform")) {Tool &t = getCurrentTool(); t.tool = TOOL_ROTATE;t.angle=0.f;t.place=false;t.special=false;t.x=0;t.y=0;t.w=1;t.h=1;} buttonActiveEnd();
    buttonActive(TOOL_RECOLOR); if (ImGui::Button("Recolor")) {Tool &t = getCurrentTool(); t.tool = TOOL_RECOLOR; t.color0 = getCurrentPalette().slotColors[getCurrentPalette().slot]; t.color1 = getCurrentPalette().slotColors[getCurrentPalette().slot];} buttonActiveEnd();
    ImGui::SameLine();
    buttonActive(TOOL_SETALPHA); if (ImGui::Button("PixelAlpha")) {Tool &t = getCurrentTool(); t.tool = TOOL_SETALPHA;} buttonActiveEnd();
  }
  ImGui::End();
}

void displaySpriteSheet() {
  static bool isInit = true; if (isInit) ImGui::SetNextWindowPos(ImVec2(_WIDTH(180),_HEIGHT(405))); isInit = false;
  ImGui::Begin("Sprites", NULL, ImVec2(_WIDTH(460),_HEIGHT(75)),-1,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2(0,0));
  ImGui::BeginChild("SheetConfig",ImVec2(90,55));
  ImGui::PushItemWidth(80);
  ImGui::InputInt("Nr",&getCurrentSpriteSheet().xScroll);
  getCurrentSpriteSheet().xScroll = clamp(getCurrentSpriteSheet().xScroll,0,(int)sprites.size()-1);
  if (ImGui::Button("Insert")) {
    sprites.spInsert(sprites,Sprite(),getCurrentSpriteCanvas().spriteNr);
    int i = getCurrentSpriteCanvas().spriteNr;
    sprites[i].update();
    sprites[i].texture();
  }
  ImGui::SameLine();
  ImGui::Text(" ");
  ImGui::SameLine();
  ImGui::Checkbox("x2",&getCurrentSpriteSheet().halveSize);
  if (!sprites.empty()) {
    if (ImGui::Button("Erase")) {
      if ((int)sprites.size()>1) {
        sprites.spErase(sprites,getCurrentSpriteCanvas().spriteNr,1);
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
  
  ImVec2 k0 = ImGui::GetCursorPos();
  k0.x += 93;

  bool doubleLinePainted=false;
  for (int i = getCurrentSpriteSheet().xScroll; i < (int)sprites.size(); i++) {
    int wh=52;
    if (getCurrentSpriteSheet().halveSize) wh/=2;
    ImGui::SameLine();
    ImVec2 cp0 = ImGui::GetCursorScreenPos();
    if (cp0.x > XRES - wh) {
      if (doubleLinePainted) 
        break;
      if (getCurrentSpriteSheet().halveSize) {
        ImGui::Text(""); // remove same line;
        ImGui::SetCursorPosX(k0.x);
        cp0 = ImGui::GetCursorScreenPos();
        doubleLinePainted=true;
      } else {
        break;
      }
    }
    ImVec2 cpx = ImGui::GetCursorPos();
    ImVec2 cpx2 = cpx;
    cpx2.x-=1;
    cpx2.y-=3;
    ImGui::SetCursorPos(cpx2);
    ImGui::PushStyleColor(ImGuiCol_Text,ImVec4(0,1,1,1));
    ImGui::Text("%d",i);
    ImGui::PopStyleColor();
    ImGui::SetCursorPos(cpx);
    int w = sprites[i].width;
    int h = sprites[i].height;
    ImVec2 size = ImVec2(wh*getCurrentSpriteCanvas().aspect,(float)wh);
    ImVec2 cp1 = ImVec2(cp0.x+size.x,cp0.y+size.y);
    ImVec2 maxTex = ImVec2((float)w/MAXSPRITEWIDTH,(float)h/MAXSPRITEHEIGHT);
    ImGui::Image((ImTextureID)((intptr_t)sprites[i].textureId),size,ImVec2(0,0),maxTex);
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
  a = (float)fmod(a+360.f,360.f);
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
  int ri = (int)floor(r*255.f);
  int gi = (int)floor(g*255.f);
  int bi = (int)floor(b*255.f);
  if (ri<0) ri = 0;
  if (gi<0) gi = 0;    
  if (bi<0) bi = 0;
  if (ri>255) ri = 255;
  if (gi>255) gi = 255;    
  if (bi>255) bi = 255;
  ri = (int)floor(ri*l);
  gi = (int)floor(gi*l);
  bi = (int)floor(bi*l);
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

  static bool isInit = true; if (isInit) ImGui::SetNextWindowPos(ImVec2(_WIDTH(565),_HEIGHT(20))); isInit = false;
  ImGui::Begin("ColorWheel", NULL, ImVec2(_WIDTH(75),_HEIGHT(120)),-1,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoTitleBar);
  ImVec2 cp1 = ImGui::GetCursorScreenPos();
  ImVec2 cp2 = cp1;
  cp2.x += COLORWHEELXRES;
  cp2.y += COLORWHEELYRES;
  ImGui::Image((ImTextureID)((intptr_t)colorWheelTextureId),ImVec2(COLORWHEELXRES,COLORWHEELYRES));
  static char text[10];
  static unsigned int col;
  if (ImGui::IsMouseHoveringRect(cp1,cp2) && (mouseButtons & 1)) {
    col = hsl(colorWheelHue,(float)(mouseX-cp1.x)/COLORWHEELXRES,(float)(mouseY-cp1.y)/COLORWHEELYRES);
    toColorString(text,col);
  }
  ImVec2 cp0 = ImGui::GetCursorScreenPos();
  ImGui::Image((ImTextureID)((intptr_t)hueLineTextureId),ImVec2(COLORWHEELXRES,COLORWHEELHUEY));
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
  cp0.x += 30;
  cp2.x += COLORWHEELXRES;
  cp2.y += 14;
  l->AddRectFilled(cp0,cp2,col);
  ImGui::Text(text);
  if (colIdx >= 0 && colIdx < MAXPALETTECOLORS && ImGui::Button("Set")) {
    getCurrentPalette().colors[colIdx] = fromColorString(text);
    getCurrentPalette().update();
  }
  ImGui::End();
}

int playedAt=-1;

void displayAnimation() {
  static bool isInit = true; if (isInit) ImGui::SetNextWindowPos(ImVec2(_WIDTH(565),_HEIGHT(140))); isInit = false;
  ImGui::Begin("Anim", NULL, ImVec2(_WIDTH(75),_HEIGHT(180)),-1,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove);
  ImGui::Text("Speed/FPS");
  ImGui::InputFloat("##Speed",&getCurrentAnimation().speed);
  ImGui::Text("Frame0");
  ImGui::InputInt("##Frame0",&getCurrentAnimation().frame0,0);
  ImGui::Text("FrameN");
  ImGui::InputInt("##FrameN",&getCurrentAnimation().frameN,0);
  getCurrentAnimation().frame0 = clamp(getCurrentAnimation().frame0,0,(int)sprites.size()-1);
  getCurrentAnimation().frameN = clamp(getCurrentAnimation().frameN,0,(int)sprites.size()-1);
  if (!getCurrentAnimation().playing) {
    if (ImGui::Button("Play##Play1")) {
      if (playedAt!=-1) getCurrentSpriteCanvas().spriteNr=playedAt;
      getCurrentSpriteCanvas().spriteNr=clamp(getCurrentSpriteCanvas().spriteNr,0,(int)sprites.size()-1);
      playedAt=getCurrentSpriteCanvas().spriteNr;
      getCurrentAnimation().playPos = 0;
      getCurrentAnimation().playing = true;
    }
    ImGui::Checkbox("Bounce##Flip1",&getCurrentAnimation().flipping);
  } else {
    if (ImGui::Button("Stop##Stop1")) {
      getCurrentSpriteCanvas().spriteNr=playedAt;
      getCurrentSpriteCanvas().spriteNr=clamp(getCurrentSpriteCanvas().spriteNr,0,(int)sprites.size()-1);
      playedAt=-1;
      getCurrentAnimation().playPos = 0;
      getCurrentAnimation().playing = false;
    }
  }
  if (getCurrentAnimation().playing) {
    int delta = getCurrentAnimation().frameN-getCurrentAnimation().frame0;
    int d;
    if (!getCurrentAnimation().flipping) {
      d = (((int)floor(getCurrentAnimation().playPos))%(abs(delta)+1))*sign(delta);
    } else {
      int a = abs(delta)*2;
      int d2 = (((int)floor(getCurrentAnimation().playPos))%a);
      if (d2 <= abs(delta)) 
        d = d2;
      else
        d = abs(delta)-(d2-abs(delta));
      d *= sign(delta);
    }
    getCurrentAnimation().playedFrame=d+getCurrentAnimation().frame0;
    ImGui::Text("Frame:%d",(int)getCurrentAnimation().playedFrame);
  }
  ImGui::End();
}

void displayTestBox() {
  static bool isInit = true; if (isInit) ImGui::SetNextWindowPos(ImVec2(_WIDTH(565),_HEIGHT(320))); isInit = false;
  ImGui::Begin("TestBox", NULL, ImVec2(_WIDTH(75),_HEIGHT(85)),-1,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoTitleBar);
  int width = 50;
  int height = (int)floor(width/getCurrentSpriteCanvas().aspect);
  ImVec2 cv = ImVec2((float)width/getCurrentTestBox().width,(float)height/getCurrentTestBox().height);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));
  int over=-1;
  for (int y = 0; y < getCurrentTestBox().height; y++) {
    for (int x = 0; x < getCurrentTestBox().width; x++) {
      if (x != 0) ImGui::SameLine();
      int spriteNr = clamp((int)getCurrentTestBox().spriteIndices[x+y*getCurrentTestBox().width],0,(int)sprites.size()-1);
      ImVec2 c2 = ImVec2((float)sprites[spriteNr].width/MAXSPRITEWIDTH,(float)sprites[spriteNr].height/MAXSPRITEHEIGHT);
      ImVec2 sp0 = ImGui::GetCursorScreenPos();
      ImVec2 sp1 = sp0;
      sp1.x += cv.x;
      sp1.y += cv.y;
      ImGui::Image((ImTextureID)((intptr_t)sprites[spriteNr].textureId),cv,ImVec2(0,0),c2);
      if (ImGui::IsMouseHoveringRect(sp0,sp1)) {
        over = spriteNr;
        ImDrawList *l = ImGui::GetWindowDrawList();
        l->AddRect(sp0,sp1,0xff0000ff,0);
        if (mouseButtons & 1) {
          getCurrentTestBox().spriteIndices[x+y*getCurrentTestBox().width]=getCurrentSpriteCanvas().spriteNr;
        }
      }
    }
  }
  if (over>=0)
   ImGui::Text("Sprite:%d",over);
  ImGui::PopStyleVar();
  ImGui::End();
}

void displayResizeSprite() {
  bool finished = false;
  bool first = true;
  static int newSizeX = getCurrentSprite().width;
  static int newSizeY = getCurrentSprite().height;
  do
  {
    ImGuiNewFrame();
    if (first) ImGui::SetNextWindowPos(ImVec2(_WIDTH(0),_HEIGHT(0)));
    first = false;
    ImGui::Begin("New Size##NewSize11");
    ImGui::Text("Old Size:%dx%d",getCurrentSprite().width,getCurrentSprite().height);
    ImGui::PushItemWidth(64);
    ImGui::InputInt("Width", &newSizeX,0);
    ImGui::InputInt("Height", &newSizeY,0);
    ImGui::PopItemWidth();
    newSizeX = clamp(newSizeX,1,MAXSPRITEWIDTH);
    newSizeY = clamp(newSizeY,1,MAXSPRITEHEIGHT);
    if (ImGui::Button("Cancel##Cancel11")||ImGui::IsKeyDown(ImGui::GetIO().KeyMap[ImGuiKey_Escape])) {
      ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Escape]] = 0;
      finished = true;
    }             
    ImGui::SameLine();
    if (ImGui::Button("Proceed##Proceed11")) {
      {
        for (int y = 0; y < MAXSPRITEHEIGHT; y++) {
          for (int x = 0; x < MAXSPRITEWIDTH; x++) {
            temp[x+y*MAXSPRITEWIDTH] = getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH];
          }
        }
      }
      {
        for (int y = 0; y < newSizeY; y++) {
          for (int x = 0; x < newSizeX; x++) {
           int rx = x * getCurrentSprite().width/newSizeX;
           int ry = y * getCurrentSprite().height/newSizeY;
           getCurrentSprite().bitmap[x+y*MAXSPRITEWIDTH] = temp[rx+ry*MAXSPRITEWIDTH];
          }
        }
        getCurrentSprite().width = newSizeX;
        getCurrentSprite().height = newSizeY;
        getCurrentSprite().update();
        getCurrentSprite().texture();
      }
      finished = true;
    }             
    ImGui::End();
    render();
  
    refreshGraphics();
  } while(!finished);
  modalMode = MODAL_NONE;
}

void displayDeleteSprites() {
  bool finished = false;
  bool first = true;
  int deleteStartSprite = 0;
  int deleteEndSprite = 0;
  do
  {
    ImGuiNewFrame();
    if (first) ImGui::SetNextWindowPos(ImVec2(_WIDTH(0),_HEIGHT(0)));
    first = false;
    ImGui::Begin("Delete Sprites##DeleteSprites22");
    ImGui::PushItemWidth(64);
    ImGui::InputInt("Sprite0##Sprite022", &deleteStartSprite,0);
    ImGui::InputInt("SpriteN##SpriteN22", &deleteEndSprite,0);
    ImGui::PopItemWidth();
    deleteStartSprite = clamp(deleteStartSprite,0,(int)sprites.size()-1);
    deleteEndSprite = clamp(deleteEndSprite,deleteStartSprite,(int)sprites.size()-1);
    if (ImGui::Button("Cancel##Cancel22")||ImGui::IsKeyDown(ImGui::GetIO().KeyMap[ImGuiKey_Escape])) {
      ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Escape]] = 0;
      finished = true;
    }             
    ImGui::SameLine();
    if (ImGui::Button("Proceed##Proceed22")) {
      sprites.spErase(sprites,deleteStartSprite,deleteEndSprite-deleteStartSprite+1);
      finished = true;
    }             
    ImGui::End();
    render();
  
    refreshGraphics();
  } while(!finished);
  modalMode = MODAL_NONE;
}

void displayCopySprites() {
  bool finished = false;
  bool first = true;
  int copyStartSprite = 0;
  int copyEndSprite = 0;
  int spriteNr = 0;
  do
  {
    ImGuiNewFrame();
    if (first) ImGui::SetNextWindowPos(ImVec2(_WIDTH(0),_HEIGHT(0)));
    first = false;
    ImGui::Begin("Copy Sprites##CopySprites33");
    ImGui::PushItemWidth(64);
    ImGui::InputInt("InsertTo##InsertTo33", &spriteNr,0);
    ImGui::InputInt("Sprite0##Sprite033", &copyStartSprite,0);
    ImGui::InputInt("SpriteN##SpriteN33", &copyEndSprite,0);
    ImGui::PopItemWidth();
    copyStartSprite = clamp(copyStartSprite,0,(int)sprites.size()-1);
    copyEndSprite = clamp(copyEndSprite,copyStartSprite,(int)sprites.size()-1);
    spriteNr = clamp(spriteNr,0,(int)sprites.size());
    if (ImGui::Button("Cancel##Cancel33")||ImGui::IsKeyDown(ImGui::GetIO().KeyMap[ImGuiKey_Escape])) {
      ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Escape]] = 0;
      finished = true;
    }             
    ImGui::SameLine();
    if (ImGui::Button("Proceed##Proceed33")) {
      if (spriteNr==(int)sprites.size()) {
        for(int i = copyStartSprite; i <= copyEndSprite; i++) {
          sprites[i].update();
          sprites.push_back(sprites[i]);
        }
      } else {
        spArray<Sprite> k;
        {for(int i = copyEndSprite; i>= copyStartSprite; i--) {
          sprites[i].update();
          k.push_back(sprites[i]);
        }}
        {for(int i = 0; i< copyEndSprite-copyStartSprite+1; i++) {
          sprites.spInsert(sprites,k[i],spriteNr);
        }}
      }
      finished = true;
    }             
    ImGui::End();
    render();
  
    refreshGraphics();
  } while(!finished);
  modalMode = MODAL_NONE;
}

void displaySomeError() {
  bool finished = false;
  bool first = true;
  do
  {
    ImGuiNewFrame();
    if (first) ImGui::SetNextWindowPos(ImVec2(_WIDTH(0),_HEIGHT(0)));
    first = false;
    ImGui::Begin("Some Error##SomeError55");
    ImGui::Text("!!Some Error happened!!\nThis may be a file write/read error,\nor something other that\ndidn't go well.\nMaybe sprite size too big?\nMaximum Size=%dx%d",MAXSPRITEWIDTH,MAXSPRITEHEIGHT);
    if (ImGui::Button("Ok##Proceed55")) finished = true;
    ImGui::End();
    render();
    refreshGraphics();  
  } while(!finished);
  someError = false;
}

struct meminfo {
    unsigned LargestBlockAvail;
    unsigned MaxUnlockedPage;
    unsigned LargestLockablePage;
    unsigned LinAddrSpace;
    unsigned NumFreePagesAvail;
    unsigned NumPhysicalPagesFree;
    unsigned TotalPhysicalPages;
    unsigned FreeLinAddrSpace;
    unsigned SizeOfPageFile;
    unsigned Reserved[3];
} MemInfo;

void displayAbout() {

#ifndef NOT_DOS
  union REGS regs;
  struct SREGS sregs;
  regs.x.eax = 0x00000500;
  memset( &sregs, 0, sizeof(sregs) );
  sregs.es = FP_SEG( &MemInfo );
  regs.x.edi = FP_OFF( &MemInfo );
  int386x( 0x31, &regs, &regs, &sregs );
#else
  memset(&MemInfo, 0, sizeof(MemInfo));
#endif

  bool finished = false;
  bool first = true;
  do
  {
    ImGuiNewFrame();
    if (first) ImGui::SetNextWindowPos(ImVec2(_WIDTH(0),_HEIGHT(0)));
    first = false;
    ImGui::Begin("About##ShowAbout99");
    ImGui::Text("_sprite.exe\n(c)2025 by Stefan Mader\nUses a converted ImGui.\n\nFree memory peak:%dKB",MemInfo.LargestBlockAvail/1024);
    if (ImGui::Button("Ok##Proceed99")) finished = true;
    ImGui::End();
    render();
    refreshGraphics();    
  } while(!finished);
  showAbout = false;
}

#include "_map.cpp" // :cpp include:

int main(int argc, char **argv) {
  if (!initGraphics()) {
    printf("No Vesa Mode %dx%d with 32Bit found.\n",XRES,YRES);
    exit(0);
  }
  InitImGui(XRES, YRES);
  newWorkSpace();

  //ImGui::GetStyle().WindowRounding = 0.f;
  
  do
  {
    ImGuiNewFrame();

    static double lastFPSClock = timeInSeconds();
    double fpsClock = timeInSeconds();
    static int fpsCounter=0;
    fpsCounter++;
    if (fpsClock-lastFPSClock>1.0) {
      lastFPSClock = fpsClock;
      fps = fpsCounter;
      fpsCounter = 0;
    }

    static double lastFrameSecond = timeInSeconds();
    double second = timeInSeconds();
    if (getCurrentAnimation().playing) {
      getCurrentAnimation().playPos += (second-lastFrameSecond)*getCurrentAnimation().speed;
      getCurrentSpriteCanvas().spriteNr = getCurrentAnimation().playedFrame;
      getCurrentSpriteCanvas().spriteNr = clamp(getCurrentSpriteCanvas().spriteNr,0,(int)sprites.size()-1);
    }
    lastFrameSecond = second;

    if (activeSubset == SUBSET_SPRITEPAINTER) {
      displayMenuBar();
      displaySpriteEditField();
      displayToolWindow();
      displayColorPicker();
      displayColorWheel();
      displayAnimation();
      displayTestBox();
      displaySpriteSheet();
    } else
    if (activeSubset == SUBSET_MAPPAINTER) {
      display_mapPainter();
    }

    if (showAbout)
      displayAbout();
    displayLoadSave();
    if (modalMode == MODAL_RESIZESPRITE) 
      displayResizeSprite();
    if (modalMode == MODAL_DELETESPRITES) 
      displayDeleteSprites();
    if (modalMode == MODAL_COPYSPRITES) 
      displayCopySprites();
    displayQuitBox();      

    if (someError) displaySomeError();
    render();

    refreshGraphics();
  } while(!quit);
  
  saveSpriteSheet("-backup-.ss");

  closeGraphics();

  return 0;
}