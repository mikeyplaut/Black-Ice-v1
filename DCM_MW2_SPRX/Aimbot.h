//Aimbot By NickBeHaxing & Milky4444
#define Angleoffset (0x3358)
#define BaseAngle (0xE0)
bool Aimbot = false;
bool Required = false;

struct vec3
{
	float x;
	float y;
	float z;
};
float angleseeeee[3];
vec3 vectoangles(vec3 Angles)
{
	float forward;
	float yaw, pitch;
	float PI = 3.1415926535897931;
	if (Angles.x == 0 && Angles.y == 0)
	{
		yaw = 0;
		if (Angles.z > 0) pitch = 90.00;
		else pitch = 270.00;
	}
	else
	{
		if (Angles.x != -1) yaw = (float)(atan2((double)Angles.y, (double)Angles.x) * 180.00 / PI);
		else if (Angles.y > 0) yaw = 90.00;
		else yaw = 270;
		if (yaw < 0) yaw += 360.00;

		forward = (float)sqrt((double)(Angles.x * Angles.x + Angles.y * Angles.y));
		pitch = (float)(atan2((double)Angles.z, (double)forward) * 180.00 / PI);
		if (pitch < 0) pitch += 360.00;
	}
	angleseeeee[0] = -pitch;
	angleseeeee[1] = yaw;
	angleseeeee[2] = 0;

	vec3 Angless = { angleseeeee[0], angleseeeee[1], angleseeeee[2] };
	return Angless;
}

float dx, dy, dz;
float GetDistance(vec3 c1, vec3 c2)
{
	float dx = c2.x - c1.x;
	float dy = c2.y - c1.y;
	float dz = c2.z - c1.z;

	return sqrt((float)((dx * dx) + (dy * dy) + (dz * dz)));
}
int GetIndex()
{
	int cg_s = *(int*)(0x915254);
	return *(int*)(cg_s + 0x3250); 
}
int GetHealth()
{
	int cg_s = *(int*)(0x915254);
	return *(char*)(cg_s + 0x153);
}
bool isFFA()
{
	int serverinfo = *(int*)(0x915258);
	char ffa = *(char*)(serverinfo + 0x21); 
	if(ffa == 0x6D)
	{
		return true;
	}
	return false;
}
long ClActive()
{
	return *(long*)0x9FD590;
}
int Centity(int ent)
{
	int cent = *(int*)0x91527C;
	return cent + (ent * 0x1F8);
};
//Thanks To Kurt & Milky4444 For Addresses In This Funcution
//Not Working, Call in Hook
opd_s SL_GSOS = {0x200E38,0x00724C38};
unsigned int(*SL_GetString)(const char *str) = (unsigned int(*)(const char*))&SL_GSOS;
opd_s GCDObJ = {0x1E59A0 ,0x00724C38};
int (*Com_GetClientDObj)(int handle, int localClientNum) = (int(*)(int,int))&GCDObJ;
opd_s DObjGWTP = {0x52738,0x00724C38};
bool (*CG_DObjGetWorldTagPos)(int targ_centity, int dobj, unsigned int tagIndex, float *pos) = (bool(*)(int,int,unsigned int,float*))&DObjGWTP;
bool AimTarget_GetTagPos(int targEnt, unsigned int targIndex,float *outPos)
{
	int DObj = Com_GetClientDObj(Centity(targEnt) + 0xD0 ,0);
	return CG_DObjGetWorldTagPos(Centity(targEnt), DObj, targIndex, outPos);
}
vec3 GetBonePos(int playerIndex, const char* tagName)
{
	vec3 ret;
	float out[3];
	if (AimTarget_GetTagPos(playerIndex, SL_GetString(tagName), out))
	{
		ret.x = out[0];
		ret.y = out[1];
		ret.z = out[2];
	}
	return ret;
}
int GetStance(int ent)
{
	char stance = *(char*)(Centity(ent) + 0x63,1);
	//0x00 = standing
	//0x04 = crouch
	//0x00 = prone
}
bool isSameTeam(int cl1,int cl2)
{
		//Online Address
        //ClientInfo_t = 0x367F4E85
	    //Offline Address
        //ClientInfo_t = 0x36BE4E85
	    int cg_s = *(int*)(0x915254);
		int ent2 = *(char*)(cg_s + 0xF6E80 + ((cl2) * 0x544) + 0x2F);
	    int ent1 = *(char*)(cg_s + 0xF6E80 + ((cl1) * 0x544) + 0x2F);
	    if((ent1) == (ent2))
	    {
	        return true;
	    }
	    return false;
}
bool isAlive(int cl)
{
	if(*(char*)(Centity(cl) + 0x02) == 0x01)
	{
		return true;
	}
	return false;
}
vec3 getOrigin(int ent)
{
	int cent = Centity(ent);
	return *(vec3*)(cent + 0x18);
}

int GetNearestPlayer(int client)
{
    int nearestClient = -1;
    double nearestDistance = 1E+08f;
	for (int i = 0; i < 18; i++)
    {
        if (i != client)
        {
            if (isAlive(i))
            {
				if(!isFFA())
				{
					if (!isSameTeam(client, i))
					{
						float Distance = GetDistance(getOrigin(client), getOrigin(i));
						if (Distance < nearestDistance)
						{
							nearestDistance = Distance;
							nearestClient = i;
						}
					}
				}
				else
				{
						float Distance = GetDistance(getOrigin(client), getOrigin(i));
						if (Distance < nearestDistance)
						{
							nearestDistance = Distance;
							nearestClient = i;
						}
				}
			}
        }
    }
	return nearestClient;
}
void Aimthread(uint64_t)
{
	for(;;)
	{
		if(Aimbot && !Required)
		{
                int targindex = GetNearestPlayer(GetIndex());
				vec3 Aim = getOrigin(targindex);
				vec3 me = getOrigin(GetIndex());
				vec3 angles = {Aim.x - me.x,Aim.y - me.y,Aim.z - me.z - 12};
				float x = *(float*)read_process(ClActive() + BaseAngle,4);
				float y = *(float*)read_process(ClActive() + BaseAngle + 0x4,4);
				float z = *(float*)read_process(ClActive() + BaseAngle + 0x8,4);
			    vec3 base = {x,y,z};
				vec3 cangle = vectoangles(angles);
				vec3 fin = {cangle.x - base.x,cangle.y - base.y,0};
				*(float*)(ClActive() + Angleoffset) = fin.x;
				*(float*)(ClActive() + Angleoffset + 0x4) = fin.y;
				*(float*)(ClActive() + Angleoffset + 0x8) = fin.z;
			}
		
		else if(Aimbot && Required)
		{
			if (ButtonMonitoring::ButtonPressed(ButtonMonitoring::L1) && ButtonMonitoring::ButtonPressed(ButtonMonitoring::R1) || ButtonMonitoring::ButtonPressed(ButtonMonitoring::L1))
			{
                int targindex = GetNearestPlayer(GetIndex());
				vec3 Aim = getOrigin(targindex);
				vec3 me = getOrigin(GetIndex());
				vec3 angles = {Aim.x - me.x,Aim.y - me.y,Aim.z - me.z - 12};
				float x = *(float*)read_process(ClActive() + BaseAngle,4);
				float y = *(float*)read_process(ClActive() + BaseAngle + 0x4,4);
				float z = *(float*)read_process(ClActive() + BaseAngle + 0x8,4);
			    vec3 base = {x,y,z};
				vec3 cangle = vectoangles(angles);
				vec3 fin = {cangle.x - base.x,cangle.y - base.y,0};
				*(float*)(ClActive() + Angleoffset) = fin.x;
				*(float*)(ClActive() + Angleoffset + 0x4) = fin.y;
				*(float*)(ClActive() + Angleoffset + 0x8) = fin.z;
			}
		}
		else
		{
		sleep(200);
		}
	}
}