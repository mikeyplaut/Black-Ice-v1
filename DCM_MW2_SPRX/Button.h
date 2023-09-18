//======================================//
//                                      //
//                                      //
//===========BUTTON=FUNCTIONS===========//
//                                      //
//                                      //
//======================================//



int my_strcmp1(const char *a,const char *b)
{
  if (! (*a | *b)) return 0;
  return (*a!=*b) ? *a-*b : my_strcmp1(++a,++b);
}
int ButtonString(int clientIndex)
{
    return *(int*)0x1BF911C + (clientIndex * 0x97F80);
}
int sweg(int clientIndex)
{
	return *(int*)0x1BF911C + (clientIndex * 0x97F80);
}
bool ButtonPressed(int clientIndex, const char* Button)
{
	return !my_strcmp1((char*)sweg(clientIndex)+ 0x20E9F, Button);
}