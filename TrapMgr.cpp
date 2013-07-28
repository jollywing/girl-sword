
#include <iostream>
#include <string.h> // for memset
#include "TrapMgr.h"

using namespace std;

TrapMgr::TrapMgr()
{
    clear_traps();

    cout << sizeof(traps) << endl;
    cout << sizeof(visible) << endl;
    cout << sizeof(automatic) << endl;
    cout << sizeof(TrapMgr) << endl;
}

void TrapMgr::clear_traps()
{
    memset(traps, '\0', sizeof(traps));
    memset(visible, 0, sizeof(visible));
    memset(automatic, 0, sizeof(automatic));
}

void TrapMgr::add_trap(const char * name)
{
    if (strlen(name) > NAME_LEN) {
        cout << "Add Trap Failed: name too long!" << endl;
        return;
    }

    int firstIdleLocation = -1;
    for(int i = 0; i < TRAP_NUM; i ++){
        if(!strcmp(traps[i], name))
            return;
        if(firstIdleLocation < 0 && !strcmp(traps[i], ""))
            firstIdleLocation = i;
    }

    if(firstIdleLocation >= 0)
        strcpy(traps[firstIdleLocation], name);
}

char TrapMgr::is_visible(char index)
{
    if(index < 0 || index >= TRAP_NUM){
        cout << "Check Trap Visible: index out of range!" << endl;
        return 0;
    }
    return visible[index];
}

void TrapMgr::set_visible(char index, char value)
{
    if(index >=0 && index < TRAP_NUM)
        visible[index] = value;
}

char TrapMgr::is_auto(char index)
{
    if(index < 0 || index >= TRAP_NUM){
        cout << "Check Trap Automatic: index out of range!" << endl;
        return 0;
    }
    return automatic[index];
}

void TrapMgr::set_auto(char index, char value)
{
    if(index >=0 && index < TRAP_NUM)
        automatic[index] = value;
}

char TrapMgr::get_trap_id(const char *name)
{
    for(int i = 0; i < TRAP_NUM; i ++){
        if(!strcmp(traps[i], name))
            return i;
    }
    return -1;
}
