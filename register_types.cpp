/* register_types.cpp */

#include "register_types.h"
#include "skin.h"
#include "skinserver.h"

void register_softskin_types() {
    ClassDB::register_class<SkinServer>();
    ClassDB::register_class<Skin>();
}

void unregister_softskin_types() {
    //nothing to do here
}
