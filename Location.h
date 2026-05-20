#ifndef LOCATION_H_
#define LOCATION_H_

#include <string>

class Location {
 public:
  virtual ~Location() = default;
  virtual void Play() = 0;
  virtual bool IsCompleted() const = 0;
};

#endif
