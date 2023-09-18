
//======================================//
//                                      //
//                                      //
//=============HUD=ELEMENTS=============//
//                                      //
//                                      //
//======================================//

namespace huds
{
	union color_s 
	{ 
		struct
		{
			int8_t r;             
			int8_t g;             
			int8_t b;       
			int8_t a;   
		};
		int32_t rgba;
	};
	
	int G_LocalizedStringIndex(const char* Text)
	{
		int(*LocalizedStringIndex)(const char* Text) = (int(*)(const char*))0x00708080;
		return LocalizedStringIndex(Text);
	}
	
	struct hudelem_s
	{
		int type;                     // 0x00-0x04
		float y;                      // 0x04-0x08
		float x;                      // 0x08-0x0C
		float z;                      // 0x0C-0x10
		int targetEntNum;             // 0x10-0x14
		float fontScale;              // 0x14-0x18
		float fromFontScale;          // 0x18-0x1C
		int fontScaleStartTime;       // 0x1C-0x20
		int fontScaleTime;            // 0x20-0x24
		int label;                    // 0x24-0x28
		int font;                     // 0x28-0x2C
		int alignOrg;                 // 0x2C-0x30
		int alignScreen;              // 0x30-0x34
		color_s color;                  // 0x34-0x38
		color_s fromColor;              // 0x38-0x3C
		int fadeStartTime;            // 0x3C-0x40
		int fadeTime;                 // 0x40-0x44
		int height;                   // 0x44-0x48
		int width;                    // 0x48-0x4C
		int materialIndex;            // 0x4C-0x50
		int fromHeight;               // 0x50-0x54
		int fromWidth;                // 0x54-0x58
		int scaleStartTime;           // 0x58-0x5C
		int scaleTime;                // 0x5C-0x60
		float fromY;                  // 0x60-0x64
		float fromX;                  // 0x64-0x68
		int fromAlignOrg;             // 0x68-0x6C
		int fromAlignScreen;          // 0x6C-0x70
		int moveStartTime;            // 0x70-0x74
		int moveTime;                 // 0x74-0x78
		float value;                  // 0x78-0x7C
		int time;                     // 0x7C-0x80
		int duration;                 // 0x80-0x84
		int text;                     // 0x84-0x88
		float sort;                   // 0x88-0x8C
		color_s glowColor;              // 0x8C-0x90
		int fxBirthTime;              // 0x90-0x94
		int fxLetterTime;             // 0x94-0x98
		int fxDecayStartTime;         // 0x98-0x9C
		int fxDecayDuration;          // 0x9C-0xA0
		int soundID;                  // 0xA0-0xA4
		int flags;                    // 0xA4-0xA8
	};
	struct gameHudelem_s
	{
		hudelem_s elem;
		long clientNum;
		long team;
		long archived;
	};
	opd_s GMI = { 0x001BE758, 0x00724C38 };
	int(*GetMaterialIndex)(const char* Material) = (int(*)(const char*))&GMI;

	opd_s HEA = { 0x001806E0, 0x00724C38 };
	gameHudelem_s*(*HudElemAlloc)(int32_t client, int32_t teamNum) = (gameHudelem_s*(*)(int32_t, int32_t))&HEA;

	opd_s Destroy = { 0x00182660, 0x00724C38 };
	gameHudelem_s*(*HudElem_Free)(gameHudelem_s *hud) = (gameHudelem_s*(*)(gameHudelem_s*))&Destroy;

	void setFlagForeground(gameHudelem_s * elem)
	{
		elem->elem.flags |= 0x01;
	}
	color_s White()
	{
		color_s color;
		color.r = 255; color.g = 255; color.b = 255; color.a = 255;
		return color;
	}
	gameHudelem_s* setShader(int clientIndex, const char* Shader, int Width, int Height, float X, float Y, int Allign = 5, unsigned char R = 0, unsigned char G = 0, unsigned char B = 0, unsigned char A = 0)
	{
		*(int*)0x131689C = 1; //Allows to use precached Shaders
		gameHudelem_s* elem = HudElemAlloc(clientIndex, 0);
		elem->clientNum = clientIndex;
		elem->elem.type = 6;
		elem->elem.materialIndex = GetMaterialIndex(Shader);
		elem->elem.width = Width;
		elem->elem.height = Height;
		elem->elem.x = X;
		elem->elem.y = Y;
		elem->elem.alignOrg = Allign;
		elem->elem.color.r = R;
		elem->elem.color.g = G;
		elem->elem.color.b = B;
		elem->elem.color.a = A;
		return elem;
	}
	gameHudelem_s* setText(int clientIndex, const char* Text, int Font, float FontScale, float X, float Y, int Allign = 5, unsigned char R = 0, unsigned char G = 0, unsigned char B = 0, unsigned char A = 0,unsigned char glowR = 0, unsigned char glowG = 0, unsigned char glowB = 0, unsigned char glowA = 0)
	{
		*(int*)0x131689C = 1;
		gameHudelem_s* elem = HudElemAlloc(clientIndex, 0);
		elem->clientNum = clientIndex;
		elem->elem.type = 1;
		elem->elem.text = G_LocalizedStringIndex(Text);
		elem->elem.font = Font;
		elem->elem.fontScale = FontScale;
		elem->elem.x = X;
		elem->elem.y = Y;
		elem->elem.alignOrg = Allign;
		elem->elem.alignScreen = 6;
		elem->elem.color.r = R;
		elem->elem.color.g = G;
		elem->elem.color.b = B;
		elem->elem.color.a = A;
		elem->elem.glowColor.r = glowR;
		elem->elem.glowColor.g = glowG;
		elem->elem.glowColor.b = glowB;
		elem->elem.glowColor.a = glowA;
		return elem;
	}
	void moveOverTime(gameHudelem_s * elem, float time, float X, float Y)
	{
		int * levelTime = (int*)0x0012E0304;
		elem->elem.moveStartTime = *levelTime;
		elem->elem.fromAlignOrg = elem->elem.alignOrg;
		elem->elem.fromAlignScreen = elem->elem.alignScreen;
		elem->elem.fromX = elem->elem.x;
		elem->elem.fromY = elem->elem.y;
		elem->elem.x = X;
		elem->elem.y = Y;
		elem->elem.moveTime = (int)floor(time * 1000 + 0.5);
	}
	void MoveShaderXY(gameHudelem_s* elem, float X, float Y)
	{
		elem->elem.x = X;
		elem->elem.y = Y;
	}
	void fadeOverTime(gameHudelem_s * elem, float time, unsigned char R = 0, unsigned char G = 0, unsigned char B = 0, unsigned char A = 0)
	{
		*(int*)0x131689C = 1;
		int * levelTime = (int*)0x0012E0304;
		elem->elem.fadeStartTime = *levelTime;
		elem->elem.fromColor = elem->elem.color;
		elem->elem.color.r = R;
		elem->elem.color.g = G;
		elem->elem.color.b = B;
		elem->elem.color.a = A;
		elem->elem.fadeTime = (int)floor(time * 1000 + 0.5);
	}
	void scaleOverTimeShader(gameHudelem_s * elem, float time, int width, int height)
	{
		int * levelTime = (int*)0x0012E0304;
		elem->elem.scaleStartTime = *levelTime;
		elem->elem.fromWidth = elem->elem.width;
		elem->elem.fromHeight = elem->elem.height;
		elem->elem.width = width;
		elem->elem.height = height;
		elem->elem.scaleTime = (int)floor(time * 1000 + 0.5);
	}
	void scaleOverTimeText(gameHudelem_s * elem, float time, float fontScale)
	{
		int * levelTime = (int*)0x0012E0304;
		elem->elem.fontScaleStartTime = *levelTime;
		elem->elem.fromFontScale = elem->elem.fontScale;
		elem->elem.fontScale = fontScale;
		elem->elem.fontScaleTime = (int)floor(time * 1000 + 0.5);
	}
	void setPulseFx(gameHudelem_s * elem, int speed, int decayStart, int decayDuration )
	{
		int * levelTime = (int*)0x0012E0304;
		elem->elem.fxBirthTime = *levelTime;
		elem->elem.fxLetterTime = speed;
		elem->elem.fxDecayStartTime = decayStart;
		elem->elem.fxDecayDuration = decayDuration;
	}
	void ChangeText(gameHudelem_s * elem, char* newText)
    {
	    elem->elem.text = huds::G_LocalizedStringIndex(newText);
    }

	void glowText(gameHudelem_s * elem, float time, unsigned char GlowR = 0, unsigned char GlowG = 0, unsigned char GlowB = 0, unsigned char GlowA = 0)
	{
		int * levelTime = (int*)0x0012E0304;
		elem->elem.fadeStartTime = *levelTime;
		elem->elem.fromColor = elem->elem.glowColor;
		elem->elem.glowColor.r = GlowR;
		elem->elem.glowColor.g = GlowG;
		elem->elem.glowColor.b = GlowB;
		elem->elem.glowColor.a = GlowA;
		elem->elem.fadeTime = (int)floor(time * 1000 + 0.5);
	}

	enum TypewriterTextTypes
	{
		Normal = 0x800,
		Classic = 0x4000
	};

	gameHudelem_s* typewriter;
	bool typran;

	void SetTypewriter(int clientIndex, char* Text, int IconType, int Font, float FontSize, float X, float Y, char align, int WriteType = Normal, short Lettertime = 200, short fxDecayStartTime = 7000, short fxDecayDuration = 1000, char R = 255, char G = 255, char B = 255, char A = 255, char GlowR = 255, char GlowG = 255, char GlowB = 255, char GlowA = 0)
	{
		if(!typran)
		{
			gameHudelem_s* elem = setText(clientIndex, Text, Font, FontSize, X, Y, align, R, G, B, A, GlowR, GlowG, GlowB, GlowA);
			elem->elem.flags = WriteType;
			int * levelTime = (int*)0x0012E0304;
			elem->elem.fxBirthTime = *levelTime;
			elem->elem.fxLetterTime = Lettertime;
			elem->elem.fxDecayStartTime = fxDecayStartTime;
			elem->elem.fxDecayDuration = fxDecayDuration;
			typewriter = elem;
			typran = true;
		}
		else
		{
			gameHudelem_s* elem = typewriter;
			elem->clientNum = clientIndex;
			elem->elem.type = 1;
			elem->elem.text = G_LocalizedStringIndex(Text);
			elem->elem.font = Font;
			elem->elem.fontScale = FontSize;
			if (align != 0)
			{ elem->elem.alignOrg = 5; elem->elem.alignScreen = align; elem->elem.x = X; elem->elem.y = Y; }
			else
			{ elem->elem.x = X; elem->elem.y = Y; } 
			elem->elem.color.r = R;
			elem->elem.color.g = G;
			elem->elem.color.b = B;
			elem->elem.color.a = A;
			elem->elem.glowColor.r = GlowR;
			elem->elem.glowColor.g = GlowG;
			elem->elem.glowColor.b = GlowB;
			elem->elem.glowColor.a = GlowA;
			elem->elem.flags = WriteType;
			int * levelTime = (int*)0x0012E0304;
			elem->elem.fxBirthTime = *levelTime;
			elem->elem.fxLetterTime = Lettertime;
			elem->elem.fxDecayStartTime = fxDecayStartTime;
			elem->elem.fxDecayDuration = fxDecayDuration;
		}
	}
	
	gameHudelem_s* typewriter1;
	bool Message;

	void SetMessage(int clientIndex, char* Text, char* Text2, int WriteType = Normal, short Lettertime = 200, short fxDecayStartTime = 7000, short fxDecayDuration = 1000, char R = 255, char G = 255, char B = 255, char A = 255, char GlowR = 255, char GlowG = 255, char GlowB = 255, char GlowA = 0)
	{
		if(!Message)
		{
			gameHudelem_s* elem1 = setText(clientIndex, Text, 4, 1, 325, 130, 1, R, G, B, A, GlowR, GlowG, GlowB, GlowA);
			elem1->elem.flags = WriteType;
			int * levelTime = (int*)0x0012E0304;
			elem1->elem.fxBirthTime = *levelTime;
			elem1->elem.fxLetterTime = Lettertime;
			elem1->elem.fxDecayStartTime = fxDecayStartTime;
			elem1->elem.fxDecayDuration = fxDecayDuration;
			typewriter = elem1;
			gameHudelem_s* elem = setText(clientIndex, Text2, 4, 1, 325, 150, 1, R, G, B, A, GlowR, GlowG, GlowB, GlowA);
			elem->elem.flags = WriteType;
			elem->elem.fxBirthTime = *levelTime;
			elem->elem.fxLetterTime = Lettertime;
			elem->elem.fxDecayStartTime = fxDecayStartTime;
			elem->elem.fxDecayDuration = fxDecayDuration;
			typewriter1 = elem;
			Message = true;
			typran = true;
		}
		else
		{
			gameHudelem_s* elem1 = typewriter;
			elem1->clientNum = clientIndex;
			elem1->elem.type = 1;
			elem1->elem.text = G_LocalizedStringIndex(Text);
			elem1->elem.font = 4;
			elem1->elem.fontScale = 2;
			elem1->elem.alignOrg = 5; elem1->elem.alignScreen = 1; elem1->elem.x = 325; elem1->elem.y = 130; 
			elem1->elem.color.r = R;
			elem1->elem.color.g = G;
			elem1->elem.color.b = B;
			elem1->elem.color.a = A;
			elem1->elem.glowColor.r = GlowR;
			elem1->elem.glowColor.g = GlowG;
			elem1->elem.glowColor.b = GlowB;
			elem1->elem.glowColor.a = GlowA;
			elem1->elem.flags = WriteType;
			int * levelTime = (int*)0x0012E0304;
			elem1->elem.fxBirthTime = *levelTime;
			elem1->elem.fxLetterTime = Lettertime;
			elem1->elem.fxDecayStartTime = fxDecayStartTime;
			elem1->elem.fxDecayDuration = fxDecayDuration;
			gameHudelem_s* elem = typewriter1;
			elem->clientNum = clientIndex;
			elem->elem.type = 1;
			elem->elem.text = G_LocalizedStringIndex(Text2);
			elem->elem.font = 4;
			elem->elem.fontScale = 1.5;
			elem->elem.alignOrg = 5; elem->elem.alignScreen = 1; elem->elem.x = 325; elem->elem.y = 150; 
			elem->elem.color.r = R;
			elem->elem.color.g = G;
			elem->elem.color.b = B;
			elem->elem.color.a = A;
			elem->elem.glowColor.r = GlowR;
			elem->elem.glowColor.g = GlowG;
			elem->elem.glowColor.b = GlowB;
			elem->elem.glowColor.a = GlowA;
			elem->elem.flags = WriteType;
			elem->elem.fxBirthTime = *levelTime;
			elem->elem.fxLetterTime = Lettertime;
			elem->elem.fxDecayStartTime = fxDecayStartTime;
			elem->elem.fxDecayDuration = fxDecayDuration;
		}
	}

	gameHudelem_s* typewriter2;
	bool Title;

	void SetTitle(int clientIndex, char* Text, int WriteType = Normal, short Lettertime = 200, short fxDecayStartTime = 7000, short fxDecayDuration = 1000, char R = 255, char G = 255, char B = 255, char A = 255, char GlowR = 255, char GlowG = 255, char GlowB = 255, char GlowA = 0)
	{
		if(!Title)
		{
			gameHudelem_s* elem = setText(clientIndex, Text, 4, 2, 500, 20, 5, R, G, B, A, GlowR, GlowG, GlowB, GlowA);
			elem->elem.flags = WriteType;
			int * levelTime = (int*)0x0012E0304;
			elem->elem.fxBirthTime = *levelTime;
			elem->elem.fxLetterTime = Lettertime;
			elem->elem.fxDecayStartTime = fxDecayStartTime;
			elem->elem.fxDecayDuration = fxDecayDuration;
			typewriter2 = elem;
			Title = true;
			typran = true;
		}
		else
		{
			gameHudelem_s* elem = typewriter2;
			elem->clientNum = clientIndex;
			elem->elem.type = 1;
			elem->elem.text = G_LocalizedStringIndex(Text);
			elem->elem.font = 4;
			elem->elem.fontScale = 1.5;
			elem->elem.alignOrg = 5; elem->elem.alignScreen = 1; elem->elem.x = 500; elem->elem.y = 20; 
			elem->elem.color.r = R;
			elem->elem.color.g = G;
			elem->elem.color.b = B;
			elem->elem.color.a = A;
			elem->elem.glowColor.r = GlowR;
			elem->elem.glowColor.g = GlowG;
			elem->elem.glowColor.b = GlowB;
			elem->elem.glowColor.a = GlowA;
			elem->elem.flags = WriteType;
			int * levelTime = (int*)0x0012E0304;
			elem->elem.fxBirthTime = *levelTime;
			elem->elem.fxLetterTime = Lettertime;
			elem->elem.fxDecayStartTime = fxDecayStartTime;
			elem->elem.fxDecayDuration = fxDecayDuration;
		}
	}
};