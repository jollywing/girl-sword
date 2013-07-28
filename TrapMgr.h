#ifndef __TRAP_MGR__
#define __TRAP_MGR__

class TrapMgr;
extern TrapMgr trapMgr;

/* in c++ class, property can not have the same name with some function. */
/* The size of a object is the size of its non-static data member.
   For example, a object of TrapMgr, it occupies 
   TRAP_NUM * NAME_LEN + TRAP_NUM + TRAP_NUM bytes.*/
class TrapMgr {
public:
    static const char TRAP_NUM = 50;
    static const char NAME_LEN = 32;
private:
    char traps[TRAP_NUM][NAME_LEN];  // The name of each trap
    char visible[TRAP_NUM];
    char automatic[TRAP_NUM];
public:
    TrapMgr();
    void clear_traps();
    void add_trap(const char * name);
    char get_trap_id(const char *name);
    char is_visible(char index);
    void set_visible(char index, char value);
    char is_auto(char index);
    void set_auto(char index, char value);
};

#endif
