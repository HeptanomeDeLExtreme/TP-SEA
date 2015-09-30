void
dummy()
{
  return;
}
<<<<<<< HEAD
=======

>>>>>>> 58bc8308eb6cc50e66f61907744e12267e13e208
int
div(int dividend, int divisor)
{
  int result = 0;
  int remainder = dividend;
  while (remainder >= divisor) {
    result++;
    remainder -= divisor;
  }
  return result;
}
<<<<<<< HEAD
int
=======

int
__attribute__((naked))
>>>>>>> 58bc8308eb6cc50e66f61907744e12267e13e208
compute_volume(int rad)
{
  int rad3 = rad * rad * rad;
  return div(4*355*rad3, 3*113);
}
<<<<<<< HEAD
int
kmain( void )
{
  __asm("mrs r8, spsr");
  __asm("cps 0b10000");
  __asm("mrs r8, spsr");
  __asm("bl dummy");
  int radius = 5;
  __asm("mov r2, %0" : : "r"(radius));
  __asm("mov %0, r3" : "=r"(radius): :"r3");
  int volume;
=======

int
kmain( void )
{
  int radius = 5;
  int volume;
  dummy();
>>>>>>> 58bc8308eb6cc50e66f61907744e12267e13e208
  volume = compute_volume(radius);
  return volume;
}
