//======================================//
//                                      //
//                                      //
//===============PS3=SHIT===============//
//                                      //
//                                      //
//======================================//

#define TOC							( 0x00724C38 )// 0x30F2310

void sleep(usecond_t time)
{
	sys_timer_usleep(time * 1000);
}

void SetMemory(int Address, unsigned char* Input)
{
		int i = 0;
		while(true)
		{
				if (*(unsigned char*)(Input + i) == 0x00)
						break;
				*(int*)(Address + i) = *(unsigned char*)Input;
		}
}
 
void WriteString(int Address, const char* Input)
{
		int i = 0;
		while(true)
		{
				if(*(char*)(Input+i) == 0x00)
				{break;}
 
				*(int*)(Address + i) = *(char*)(Input + i);
				i++;
		}
}
 
void WriteInt32(int Address, int Input)
{
		*(int*)Address = Input;
}
 
void WriteFloat(int Address, float Input)
{
		*(float*) Address = Input;
}
 
void WriteSingle(int Address, float Input, int Length)
{
		for (int i = 0; i < Length; i++)
				WriteFloat(Address + (i * 4), Input);
}
 
float ReadFloat(int Address)
{
		return *(float*)Address;
}
 
float* ReadSingle(int Address, int Length)
{
		float* Value;
		for (int i = 0; i < Length; i ++)
				Value[i] = ReadFloat(Address + (i * 4));
		return Value;
}
 
void WriteByte(int Address, unsigned char Input)
{
		*(unsigned char*)Address = Input;
}
 
unsigned char ReadByte(int Address)
{
		return *(unsigned char*)Address;
}
 
unsigned char* ReadBytes(int Address, int Length)
{
		unsigned char* Value;
		for (int i = 0; i < Length; i ++)
				Value[i] = ReadByte(Address + Length);
}
 
void WriteBytes(int Address, unsigned char Input, int Length)
{
		for (int i = 0; i < Length; i ++)
				WriteByte(Address + i, Input);
}
 
void WriteBool(int Address, bool Input)
{
		WriteByte(Address, (int)Input);
}
 
bool ReadBool(int Address)
{
		return ReadByte(Address) != 0;
}

int CALL(int numOfArgs, int addr, ...)
	{
	  int i,val;
	  va_list vl;
	  va_start(vl,numOfArgs);
  
		va_arg(vl,int);//skip one

	  for (i=1;i<(numOfArgs+1);i++)
	  {
		val=va_arg(vl,int);
		*(int*)(0x2100000+i*4) = val;
	  }
	  va_end(vl);
  
	  sleep(50);

	  *(int*)0x2100000 = addr;
	  return *(int*)(0x210004C);

	}



	void Calls_thread(std::uint64_t nothing)
	{
		for(;;)
		{
		
		__asm(
			"lis 3, 0x210;"
			"lwz 3, 0(3);"
			"cmpwi 3, 0x00;"
			"beq end;"


			/*save the general registers !*/
			"mflr 0;"
			"std 0, -0x520-0x10(1);"
			"std 31, -0x520-0x18(1);"
			"std 30, -0x520-0x20(1);"
			"std 29, -0x520-0x28(1);"
			"std 28, -0x520-0x30(1);"
			"std 27, -0x520-0x38(1);"
			"std 26, -0x520-0x40(1);"
			"std 2, -0x520-0x48(1);"
		
			/*emulate the Ghosts POC registers*/
			"lis 2, 0xA7;"
			"ori 2, 2, 0xF3F8;"   
			"lis 26, 0;"
			"lis 27, 0;"
			"li 28, 0x0C800;"
			"lis 29, 0;"
			"lis 30, 0xD00D;"
			"addic 30,30, 0x9DF0;"
			"lis 31, 0x157;"
			"addic 31,31, 0x059C;"
		
		/*Loading args*/
		"lis 12, 0x210;"
		"lwz 3, 0x4(12);"
		"lwz 4, 0x8(12);"
		"lwz 5, 0xC(12);"
		"lwz 6, 0x10(12);"
		"lwz 7, 0x14(12);"
		"lwz 8, 0x18(12);"
		"lwz 9, 0x1C(12);"
		"lwz 10, 0x20(12);"
		"lwz 11, 0x24(12);"

		"lfs 1, 0x28(12);"
		"lfs 2, 0x2C(12);"
		"lfs 3, 0x30(12);"
		"lfs 4, 0x34(12);"
		"lfs 5, 0x38(12);"
		"lfs 6, 0x3C(12);"	
		"lfs 7, 0x40(12);"
		"lfs 8, 0x44(12);"
		"lfs 9, 0x48(12);"

		/*Calling*/
		"lwz 12, 0(12);"
		"mtctr 12;"
		"bctrl;"
	
		/*returns*/
		"lis 4, 0x210;"
		"stw 3, 0x4C(4);"
		"stfs 1, 0x50(4);"
		"li 3, 0;"
		"stw 3, 0(4);"
		  

		"ld 0, -0x520-0x10(1);"
		"mtlr 0;"
		"ld 26, -0x520-0x40(1);"
		"ld 27, -0x520-0x38(1);"
		"ld 28, -0x520-0x30(1);"
		"ld 29, -0x520-0x28(1);"
		"ld 30, -0x520-0x20(1);"
		"ld 31, -0x520-0x18(1);"
		"ld 2, -0x520-0x48(1);"
			  );
			 __asm("end:;");
			sleep(5);
		}
	
	}