// -----
// _sprite.exe (c)2025 by Stefan Mader (MIT-License)
// -----

#define MAXMAPWIDTH 4096
#define MAXMAPHEIGHT 4096

#define MAPTILESINX _WIDTH(80)
#define MAPTILESINY _HEIGHT(52)
#define MAPTILEWIDTH 32
#define MAPTILEHEIGHT 32

enum {
  MAPMODAL_NONE = 0,
  MAPMODAL_CANVAS = 1,
};

int mapModal = MAPMODAL_NONE;

class MapTile {
public:
  spArray<uint16_t> tileIds;
  MapTile() {
  }
  void clear() {
    tileIds.clear();
  }
};

enum {
  MAPTOOL_PENCIL = 1,
};

class MapTools {
public:
  int currentTool;
  int randomAdd;
  MapTools() {
    currentTool = MAPTOOL_PENCIL;
    randomAdd = 0;
  }
};

MapTools mapTools;

MapTools &getCurrentMapTools() {
  return mapTools;
}

class Map {
public:
  GLuint textureId;
  int scrollX,scrollY;
  int selectedX, selectedY;
  int mouseX, mouseY;
  int lastMouseX, lastMouseY;
  int width,height;
  bool mark;
  spArray<MapTile> tiles;

  Map() {
    textureId = 0;
    selectedX = 0;
    selectedY = 0;
    mouseX = 0;
    mouseY = 0;
    lastMouseX = 0;
    lastMouseY = 0;
    scrollX = 0;
    scrollY = 0;
    width = 32;
    height = 32;
    tiles.resize(width*height);
    mark = false;
  }

  void update() {
    mark = true;
  }

  GLuint texture() {
    if (mark) {
      glDeleteTextures(1,&textureId);
      textureId = 0;
      mark = false;
    }
    if (textureId == 0) {
      spHashMap<int,int> idToSpriteNr;
      for (int i = 0; i < (int)sprites.size(); i++) {
        idToSpriteNr[sprites[i].getId()]=i;
      }
      unsigned int *temp = new unsigned int[MAPTILESINX*MAPTILEWIDTH*MAPTILESINY*MAPTILEHEIGHT];
      memset(temp,0,MAPTILESINX*MAPTILEWIDTH*MAPTILESINY*MAPTILEHEIGHT*4);

      for (int tx = 0; tx < MAPTILESINX; tx++) {
        for (int ty = 0; ty < MAPTILESINY; ty++) {
          int tx2 = tx + scrollX;
          int ty2 = ty + scrollY;
          if (tx2<0||ty2<0||tx2>=width||ty2>=height) {
            for (int my = 0; my < MAPTILEHEIGHT; my++) {
              int y2 = ty*MAPTILEHEIGHT+my;
              for (int mx = 0; mx < MAPTILEWIDTH; mx++) {
                int x2 = tx*MAPTILEWIDTH+mx;
                temp[x2+y2*(MAPTILESINX*MAPTILEWIDTH)] = 0x80101010-(((tx+scrollX)^(ty+scrollY))&3)*0x00020202;
              }
            }
            continue;
          }
          const MapTile &t = tiles[tx2+ty2*width];
          if (t.tileIds.empty()) {
            for (int my = 0; my < MAPTILEHEIGHT; my++) {
              int y2 = ty*MAPTILEHEIGHT+my;
              for (int mx = 0; mx < MAPTILEWIDTH; mx++) {
                int x2 = tx*MAPTILEWIDTH+mx;
                temp[x2+y2*(MAPTILESINX*MAPTILEWIDTH)] = 0x80803060-(((tx+scrollX)^(ty+scrollY))&3)*0x00201020;
              }
            }
            continue;
          }
          int tileId = t.tileIds[0];
          if (!(idToSpriteNr.spHas(idToSpriteNr,tileId))) {
            for (int my = 0; my < MAPTILEHEIGHT; my++) {
              int y2 = ty*MAPTILEHEIGHT+my;
              for (int mx = 0; mx < MAPTILEWIDTH; mx++) {
                int x2 = tx*MAPTILEWIDTH+mx;
                temp[x2+y2*(MAPTILESINX*MAPTILEWIDTH)] = 0xffff00ff;
              }
            }
            continue;
          }
          int spriteNr = idToSpriteNr[tileId];
          spriteNr = clamp(spriteNr,0,(int)sprites.size()-1);
          Sprite &s = sprites[spriteNr];
          for (int my = 0; my < MAPTILEHEIGHT; my++) {
            int y2 = ty*MAPTILEHEIGHT+my;
            int y3 = my * s.height / MAPTILEHEIGHT;
            for (int mx = 0; mx < MAPTILEWIDTH; mx++) {
              int x2 = tx*MAPTILEWIDTH+mx;
              int x3 = mx * s.width / MAPTILEWIDTH;
              temp[x2+y2*(MAPTILESINX*MAPTILEWIDTH)] = s.bitmap[x3+y3*MAXSPRITEWIDTH];
            }
          }
        }
      }
      glGenTextures(1, &textureId);
      glBindTexture(GL_TEXTURE_2D, textureId);
      glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,MAPTILESINX*MAPTILEWIDTH,MAPTILESINY*MAPTILEHEIGHT,0,GL_RGBA,GL_UNSIGNED_BYTE,temp);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
      delete[] temp;
    }
    return textureId;
  }
};

class TileSelect {
public:
  int first;
  int spriteNr;
  TileSelect() {
    first = -1;
    spriteNr = 0;
  }
};

Map map;
TileSelect tileSelect;

Map &getCurrentMap() {
  return map;
}

TileSelect &getCurrentTileSelect() {
  return tileSelect;
}

void updateMap() {
  getCurrentMap().update();
}

void displayMenuBar_mapPainter() {
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10,1));
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg,ImVec4(0xff/255.f,0xff/255.f,0xff/255.f,0xff/255.f));
    ImGui::PushStyleColor(ImGuiCol_Text,ImVec4(0x20/255.f,0x20/255.f,0x20/255.f,0xff/255.f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg,ImVec4(0xff/255.f,0xff/255.f,0xff/255.f,0xff/255.f));
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        ImGui::Separator();
        if (ImGui::MenuItem("Save simple layer")) loadSave = LoadSave_SaveMapSimpleLayer;
        if (ImGui::MenuItem("Load simple layer")) loadSave = LoadSave_LoadMapSimpleLayer;
        ImGui::Separator();
        if (ImGui::MenuItem("Exit map painter")) {
          activeSubset = SUBSET_SPRITEPAINTER;
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Map")) {
        if (ImGui::MenuItem("Re-Canvas")) {
          mapModal = MAPMODAL_CANVAS;
        }
        if (ImGui::MenuItem("Fill")) {
          MapTile toFillWith;
          toFillWith.tileIds.push_back(0);
          MapTile cleared;
          for (int i = 0; i < getCurrentMap().width*getCurrentMap().height; i++) {
            int spriteNr = getCurrentTileSelect().spriteNr;
            spriteNr += (rand() % abs(getCurrentMapTools().randomAdd))*sign(getCurrentMapTools().randomAdd);
            if (spriteNr>=0&&spriteNr<(int)sprites.size()) {
              toFillWith.tileIds[0]=sprites[spriteNr].getId();
              getCurrentMap().tiles[i] = toFillWith;
            } else {
              getCurrentMap().tiles[i] = cleared;
            }
          }
          updateMap();
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Info")) {
        if (ImGui::MenuItem("About")) {showAbout = true; mouseDelay=MOUSEDELAY;}
        ImGui::EndMenu();
      }
#ifndef NOT_DOS
      ImGui::Text("\t\t\t\t\t\t%s  FPS/%d",currentTimeString().c_str(),(int)fps);
#else
      ImGui::Text("\t\t\t\t\t\t");
#endif
      ImGui::EndMainMenuBar();
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

void paintLine(int sx, int sy, int ex, int ey) {
  int w = getCurrentMap().width;
  int h = getCurrentMap().height;

  double dx = ex - sx;
  double dy = ey - sy;
  double d = fabs(dx);
  if (fabs(dy)>d) d = fabs(dy);
  int di = floor(d)+1;
  for (int k = 0; k <= di; k++) {
    int x = (int)floor((ex-sx)*k/di+sx);
    int y = (int)floor((ey-sy)*k/di+sy);
    if (x>=0&&x<w&&y>=0&&y<h) {
      updateMap();
      int adr = x + y * w;
      int spr = getCurrentTileSelect().spriteNr;
      if (abs(getCurrentMapTools().randomAdd)>0)
        spr += (rand() % abs(getCurrentMapTools().randomAdd))*sign(getCurrentMapTools().randomAdd);
      if (spr >= 0 && spr < (int)sprites.size()) {
        if (getCurrentMap().tiles[adr].tileIds.empty()) 
          getCurrentMap().tiles[adr].tileIds.push_back(0);
        const int tileId =  sprites[spr].getId();
        getCurrentMap().tiles[adr].tileIds[0] = tileId;
      } else {
        getCurrentMap().tiles[adr].tileIds.clear();
      }
    }
  }
}

void getAtSelected() {
  int x = getCurrentMap().selectedX;
  int y = getCurrentMap().selectedY;
  int w = getCurrentMap().width;
  int h = getCurrentMap().height;

  if (x>=0&&x<w&&y>=0&&x<h) {
    int adr = x + y * w;
    if (getCurrentMap().tiles[adr].tileIds.empty()) {
      getCurrentTileSelect().spriteNr = -1;
      return;
    }
    for (int i = 0; i < (int)sprites.size(); i++) {
      if (sprites[i].getId()==getCurrentMap().tiles[adr].tileIds[0]) {
        getCurrentTileSelect().spriteNr = i;
        return;
      }
    }
  }
}

void displayMapWindow() {
  static bool isInit = true; if (isInit) ImGui::SetNextWindowPos(ImVec2(_WIDTH(0),_HEIGHT(20))); isInit = false;
  ImGui::Begin("Map##Map123",NULL,ImVec2(_WIDTH(500),_HEIGHT(460)),-1,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove);

  ImGui::PushItemWidth(64);
  static float zoom = 1.0;
  ImGui::InputFloat("Zoom##Zoom123",&zoom,0.25);
  zoom = clamp(zoom,0.25f,16.0f);
  ImGui::SameLine();
  if (ImGui::InputInt("MapX##MapX123",&getCurrentMap().scrollX)) updateMap();
  ImGui::SameLine();
  if (ImGui::InputInt("MapY##MapY123",&getCurrentMap().scrollY)) updateMap();
  ImGui::SameLine();
  ImGui::Text("  Map[%d,%d] Pos[%d,%d]",getCurrentMap().width,getCurrentMap().height,getCurrentMap().lastMouseX,getCurrentMap().lastMouseY);
  ImGui::PopItemWidth();

  ImVec2 wp = ImGui::GetWindowPos();
  ImVec2 ws = ImGui::GetWindowSize();
  ImVec2 cp = ImGui::GetCursorScreenPos();
  ImVec2 siz = ImVec2(MAPTILESINX*MAPTILEWIDTH*getCurrentSpriteCanvas().aspect*zoom,MAPTILESINY*MAPTILEHEIGHT*zoom);

  ImGui::Image((ImTextureID)((intptr_t)getCurrentMap().texture()),siz);
 
  int mouseCellX = (int)floor((mouseX - cp.x)*MAPTILESINX/siz.x);
  int mouseCellY = (int)floor((mouseY - cp.y)*MAPTILESINY/siz.y);

  int msx = mouseCellX;
  int msy = mouseCellY;
  getCurrentMap().mouseX = getCurrentMap().scrollX + msx;
  getCurrentMap().mouseY = getCurrentMap().scrollY + msy;

  ImVec2 cp1 = ImVec2(cp.x+msx*siz.x/MAPTILESINX,cp.y+msy*siz.y/MAPTILESINY);
  ImVec2 cp2 = ImVec2(cp.x+(msx+1)*siz.x/MAPTILESINX,cp.y+(msy+1)*siz.y/MAPTILESINY);
  if (msx >= 0 && msy >= 0 && cp1.x < wp.x+ws.x && cp1.y < wp.y+ws.y && ImGui::IsMouseHoveringWindow()) {
    ImDrawList *l = ImGui::GetWindowDrawList();
    l->AddRect(cp1,cp2,0xff0000ff);
    cp1.x++;cp1.y++;cp2.x--;cp2.y--;
    l->AddRect(cp1,cp2,0xff000000);
    cp1.x--;cp1.y--;cp2.x++;cp2.y++;

    if (mouseButtons & 1) {
      getCurrentMap().selectedX = getCurrentMap().scrollX + msx;
      getCurrentMap().selectedY = getCurrentMap().scrollY + msy;

      if (getCurrentMapTools().currentTool == MAPTOOL_PENCIL) {
        int sx = getCurrentMap().lastMouseX;
        int sy = getCurrentMap().lastMouseY;
        int ex = getCurrentMap().mouseX;
        int ey = getCurrentMap().mouseY;
        if (sx>=0&&sy>=0&&ex>=0&&ey>=0) {
          paintLine(sx,sy,ex,ey);
        }
      }
    }
    if (mouseButtons & 2) {
      getCurrentMap().selectedX = getCurrentMap().scrollX + msx;
      getCurrentMap().selectedY = getCurrentMap().scrollY + msy;
      getAtSelected();
    }
  }
  {
    int ssx = getCurrentMap().selectedX-getCurrentMap().scrollX;
    int ssy = getCurrentMap().selectedY-getCurrentMap().scrollY;
    ImVec2 cp1 = ImVec2(cp.x+ssx*siz.x/MAPTILESINX,cp.y+ssy*siz.y/MAPTILESINY);
    ImVec2 cp2 = ImVec2(cp.x+(ssx+1)*siz.x/MAPTILESINX,cp.y+(ssy+1)*siz.y/MAPTILESINY);
    if (ssx >= 0 && ssy >= 0 && cp1.x < wp.x+ws.x && cp1.y < wp.y+ws.y) {
      ImDrawList *l = ImGui::GetWindowDrawList();
      l->AddRect(cp1,cp2,0xffffffff);
      cp1.x++;cp1.y++;cp2.x--;cp2.y--;
      l->AddRect(cp1,cp2,0xff000000);
      cp1.x--;cp1.y--;cp2.x++;cp2.y++;
    }
  }
  ImGui::End();
  getCurrentMap().lastMouseX = getCurrentMap().mouseX;
  getCurrentMap().lastMouseY = getCurrentMap().mouseY;
  getCurrentMap().mouseX = -1;
  getCurrentMap().mouseY = -1;
}

void displaySpriteSelectWindow() {
  static bool isInit = true; if (isInit) ImGui::SetNextWindowPos(ImVec2(_WIDTH(500),_HEIGHT(20))); isInit = false;
  ImGui::Begin("Tile Select##Tile Select123",NULL,ImVec2(_WIDTH(140),_HEIGHT(160)),-1,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove);

  ImGui::PushItemWidth(64);
  static float zoom = 1.0;
  ImGui::InputFloat("Zoom##Zoom123",&zoom,0.25);
  zoom = clamp(zoom,0.25f,16.0f);
  ImGui::InputInt("Scroll##FirstTile123",&getCurrentTileSelect().first);
  getCurrentTileSelect().first=clamp(getCurrentTileSelect().first,-1,(int)sprites.size()-1);
  ImGui::PopItemWidth();

  ImVec2 wp = ImGui::GetWindowPos();
  ImVec2 ws = ImGui::GetWindowSize();
  ImVec2 ts = ImVec2(16*getCurrentSpriteCanvas().aspect*zoom,16*zoom);
  int currentSprite = getCurrentTileSelect().first;
  currentSprite=clamp(currentSprite,-1,(int)sprites.size()-1);

  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2,2));
  while(currentSprite<(int)sprites.size()) {
    for (int x = 0; x < 32; x++) {
      if (x != 0) ImGui::SameLine();
      ImVec2 cp = ImGui::GetCursorScreenPos();
      ImVec2 cp2 = cp;
      cp2.x += ts.x;
      cp2.y += ts.y;
      if (cp.x > ws.x+wp.x-ts.x) {
        ImGui::Text(""); // sameLine
        break;
      }
      if (cp.y > ws.y+wp.y) {
        ImGui::Text(""); // sameLine
        currentSprite=(int)sprites.size();
        break;
      }
      if (currentSprite>=(int)sprites.size()) break;

      if (currentSprite>=0) {
        ImVec2 maxTex = ImVec2((float)sprites[currentSprite].width/MAXSPRITEWIDTH,(float)sprites[currentSprite].height/MAXSPRITEHEIGHT);
        ImGui::Image((ImTextureID)((intptr_t)sprites[currentSprite].texture()),ts,ImVec2(0,0),maxTex);
      } else {
        ImGui::Image((ImTextureID)0,ts);
        ImDrawList *l = ImGui::GetWindowDrawList();
        l->AddRect(cp,cp2,0xffff00ff);
      }
      
      if (ImGui::IsMouseHoveringRect(cp,cp2) && (mouseButtons & 1)) {
        getCurrentTileSelect().spriteNr = currentSprite;
        getCurrentMapTools().randomAdd = 0;
      }

      if (currentSprite==(int)sprites.size()-1) {
        ImDrawList *l = ImGui::GetWindowDrawList();
        l->AddLine(ImVec2(cp2.x,cp.y),cp2,0xffffffff);
        l->AddLine(ImVec2(cp.x,cp2.y),cp2,0xffffffff);
      }

      if (currentSprite==getCurrentTileSelect().spriteNr) {
        ImDrawList *l = ImGui::GetWindowDrawList();
        l->AddRect(cp,cp2,0xff0000ff);
      }

      currentSprite++;
    }
  }
  ImGui::PopStyleVar();

  ImGui::End();
}

void mapButtonActive(int toolType) {
  if (getCurrentMapTools().currentTool == toolType)
    ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(0.5f,0.6f,0.9f,1));
  else
    ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(0.2f,0.3f,0.4f,1));
}

void mapButtonActiveEnd() {
  ImGui::PopStyleColor();
}

void displayMapToolsWindow() {
  static bool isInit = true; if (isInit) ImGui::SetNextWindowPos(ImVec2(_WIDTH(500),_HEIGHT(180))); isInit = false;
  ImGui::Begin("Map Tools##MapTools123",NULL,ImVec2(_WIDTH(140),_HEIGHT(300)),-1,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove);
  ImGui::PushItemWidth(64);
  int currentSprite = getCurrentTileSelect().spriteNr;
  if (currentSprite>=0&&currentSprite<(int)sprites.size()) {
    ImVec2 maxTex = ImVec2((float)sprites[currentSprite].width/MAXSPRITEWIDTH,(float)sprites[currentSprite].height/MAXSPRITEHEIGHT);
    ImVec2 ts = ImVec2(_HEIGHT(40)*getCurrentSpriteCanvas().aspect,_HEIGHT(40));
    ImGui::Image((ImTextureID)((intptr_t)sprites[currentSprite].texture()),ts,ImVec2(0,0),maxTex);
    ImGui::SameLine();
    ImGui::BeginChild("Child##Child12345",ImVec2(100,ts.y+10));
    ImGui::InputText("Name##Name12345",sprites[currentSprite].name,8);
    ImGui::Text("#%04x",sprites[currentSprite].getId());
    ImGui::Text("Sprite:%d/%d",currentSprite, sprites.size());
    ImGui::EndChild();
  } else {
    ImGui::Text("Sprite:Eraser");
  }
  mapButtonActive(TOOL_PENCIL); if (ImGui::Button("Pencil")) getCurrentMapTools().currentTool = MAPTOOL_PENCIL; mapButtonActiveEnd();
  ImGui::InputInt("Random +", &getCurrentMapTools().randomAdd);
  ImGui::Text("Under Construction");


  ImGui::PopItemWidth();
  ImGui::End();
}

spArray<MapTile> oldArray;

void mapModal_Canvas() {
  bool finished = false;
  bool first = true;
  int borderLeft = 0;
  int borderTop = 0;
  int borderRight = getCurrentMap().width;
  int borderBottom = getCurrentMap().height;
  do
  {
    ImGuiNewFrame();
    if (first) ImGui::SetNextWindowPos(ImVec2(_WIDTH(0),_HEIGHT(0)));
    first = false;
    ImGui::Begin("Change Canvas Borders##NewSizeX11");
    ImGui::PushItemWidth(64);
    ImGui::InputInt("Left position", &borderLeft);
    ImGui::InputInt("Top position", &borderTop);
    ImGui::InputInt("Right position", &borderRight);
    ImGui::InputInt("Bottom position", &borderBottom);
    borderLeft = clamp(borderLeft,-MAXMAPWIDTH,MAXMAPWIDTH);
    borderRight = clamp(borderRight,borderLeft+1,MAXMAPWIDTH);
    borderTop = clamp(borderTop,-MAXMAPHEIGHT,MAXMAPHEIGHT);
    borderBottom = clamp(borderBottom,borderTop+1,MAXMAPHEIGHT);
    ImGui::PopItemWidth();
    if (ImGui::Button("Cancel##Cancel11")||ImGui::IsKeyDown(ImGui::GetIO().KeyMap[ImGuiKey_Escape])) {
      ImGui::GetIO().KeysDown[ImGui::GetIO().KeyMap[ImGuiKey_Escape]] = 0;
      finished = true;
    }             
    ImGui::SameLine();
    if (ImGui::Button("Proceed##Proceed11")) {
     int oldWidth = getCurrentMap().width;
     int oldHeight = getCurrentMap().height;
     oldArray = getCurrentMap().tiles;
     int newWidth = borderRight-borderLeft;
     int newHeight = borderBottom-borderTop;
     getCurrentMap().tiles.resize(newWidth*newHeight);
     getCurrentMap().width = newWidth;
     getCurrentMap().height = newHeight;
     for (int y = 0; y < newHeight; y++) {
       for (int x = 0; x < newWidth; x++) {
          int ox = x + borderLeft;
          int oy = y + borderTop;
          if (ox>=0&&ox<oldWidth&&oy>=0&&oy<oldHeight) {
            getCurrentMap().tiles[x+y*newWidth] = oldArray[ox+oy*oldWidth];
          } else {
            getCurrentMap().tiles[x+y*newWidth].clear();
          }
       }
     }
     getCurrentMap().update();
     finished = true;
    }             
    ImGui::End();
    render();
  
    refreshGraphics();
  } while(!finished);
}

bool saveMapSimpleLayer(const char *fname) {
  FILE *out = fopen(fname,"wb");
  if (out == NULL) {someError = true; return false;}
  unsigned short t;
  t = getCurrentMap().width;
  fwrite(&t,1,2,out);
  t = getCurrentMap().height;
  fwrite(&t,1,2,out);
  for (int y = 0; y < getCurrentMap().height; y++) {
    for (int x = 0; x < getCurrentMap().width; x++) {
      t = 0;
      if (!getCurrentMap().tiles[x+y*getCurrentMap().width].tileIds.empty())
        t = getCurrentMap().tiles[x+y*getCurrentMap().width].tileIds[0];
      fwrite(&t,1,2,out);
    }
  }  
   fclose(out);
  return true;
}

bool loadMapSimpleLayer(const char *fname) {
  FILE *in = fopen(fname,"rb");
  if (in == NULL) {someError = true; return false;}
  unsigned short t=0;
  fread(&t,1,2,in);
  getCurrentMap().width = t;
  fread(&t,1,2,in);
  getCurrentMap().height = t;
  getCurrentMap().tiles.resize(getCurrentMap().width*getCurrentMap().height);
  for (int y = 0; y < getCurrentMap().height; y++) {
    for (int x = 0; x < getCurrentMap().width; x++) {
      t = 0;                                                               
      fread(&t,1,2,in);
      getCurrentMap().tiles[x+y*getCurrentMap().width].clear();
      if (t != 0) 
        getCurrentMap().tiles[x+y*getCurrentMap().width].tileIds.push_back(t);
    }
  }  
  fclose(in);
  return true;
}

void displayMapModal() {
  switch(mapModal) {
  case MAPMODAL_CANVAS: mapModal_Canvas(); break;
  }
  mapModal = MAPMODAL_NONE;
  updateMap();
}

void display_mapPainter() {
  displayMenuBar_mapPainter();
  displayMapWindow();
  displaySpriteSelectWindow();
  displayMapToolsWindow();
  if (mapModal !=  MAPMODAL_NONE) {
    displayMapModal();
  }
}
