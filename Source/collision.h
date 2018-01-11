#ifndef collision
#define collision

int detectCollision(Object obj1, Object obj2);
Object reactCollision(Object obj1, Object obj2, bool bounce);
Object reactUpCollision(Object obj1, Object obj2);

#endif
