
//
// Lockable
//
// 31 January 2005 -- tds
//

#ifndef INDRI_LOCKABLE_HPP
#define INDRI_LOCKABLE_HPP

class Lockable {
public:
  virtual ~Lockable() {};

  virtual void lock() = 0;
  virtual void unlock() = 0;
};

#endif // INDRI_LOCKABLE_HPP

