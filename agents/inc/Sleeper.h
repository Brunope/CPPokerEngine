#ifndef SLEEPER_H_
#define SLEEPER_H_

/**
 * Waits for some amount of time
 */
class Sleeper {
public:
  Sleeper(uint32_t millis = 200);
  
  void setSleepTime(uint32_t millis);
  uint32_t getSleepTime();
  void sleep();

protected:
  uint32_t sleep_millis_;
};

#endif  // SLEEPER_H_
