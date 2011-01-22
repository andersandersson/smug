#ifndef SMUG_GRAPHICS_DRAWABLE_DRAWABLESHAPE_H
#define SMUG_GRAPHICS_DRAWABLE_DRAWABLESHAPE_H

// #include <utils/shapes.h>
// #include <engine/gameobject.h>

struct Shape;
struct BatchData;
struct GameObject;

// TEMP!
struct DrawableShapeData;

struct GameObject* DrawableShape_new();

BOOL DrawableShape_getShape(struct GameObject* self, struct Shape** shape);
BOOL DrawableShape_setShape(struct GameObject* self, struct Shape* shape);

/** Only for inheriting classes! ('protected') */
// void* DrawableShape_getChild(GameObject* self);
// void DrawableShape_setChild(GameObject* self, void* internal);
// void* DrawableShape_getInternals(struct GameObject* self);
// void DrawableShape_deleteInternals(GameObject* self);
// struct _DrawableShape* DrawableShape_getObjectAs(struct GameObject* self);
// struct GameObject* DrawableShape_getAsGameObject(struct _DrawableShape* obj);
// struct _DrawableShape* DrawableShape_newInherit(
    // void* inheriting,
    // void(*writeBatchDataFunc)(struct GameObject* d, struct BatchData* batch, unsigned int start),
    // int (*getDataSizeFunc)(struct GameObject* d));
struct GameObject* DrawableShape_newInherit(
    struct DrawableShapeData* data,
    void(*writeBatchDataFunc)(struct GameObject* d, struct BatchData* batch, unsigned int start),
    int (*getDataSizeFunc)(struct GameObject* d));

#endif // SMUG_GRAPHICS_DRAWABLE_DRAWABLESHAPE_H
