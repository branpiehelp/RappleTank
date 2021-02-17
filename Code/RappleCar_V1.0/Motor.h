/********************************************//** 
BRANPIE All right reserved.
RAPPLE ROBOT
************************************************/
#ifndef Motor_h
#define Motor_h
#define FD 0
#define BD 1

class Motor{
private:
  int dir_p;
  int val_p;
  int dir;
  int val;
public:
  void attach(int dir_p, int val_p);
  void set_dir(int dir);
  void set_speed(int val);
  void update(void);
  void info(void);
};
#endif
