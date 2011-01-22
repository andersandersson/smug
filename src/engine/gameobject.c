#include <smugstd.h>

#include <utils/linkedlist.h>
#include <utils/linkedlistiterator.h>

#include <engine/gameobject_protected.h>


/*********************/
/** Local functions **/
/*********************/

static BOOL _objectInvariant(GameObject* obj)
{
/*     BOOL ret = (obj != NULL);
    smug_assert(obj != NULL);
    InternalGameObject* tier = obj;
    while (tier->mParent != NULL)
    {
        tier = tier->mParent;
    }
    InternalGameObject* root = tier;
    ret = ret && root->isExactType(root, SMUG_TYPE_OBJECT);
    smug_assert(root->isExactType(root, SMUG_TYPE_OBJECT));
    tier = obj;
    while (tier->mChild != NULL)
    {
        tier = tier->mChild;
    }
    InternalGameObject* leaf = tier;
    while (tier->mParent != NULL)
    {
        ret = (ret && tier->mRoot == root && tier->mLeaf == leaf && tier->mParent->mChild == tier && tier->mData != NULL);
        smug_assert(tier->mRoot == root);
        smug_assert(tier->mLeaf == leaf);
        smug_assert(tier->mParent->mChild == tier);
        smug_assert(tier->mData != NULL);
        tier = tier->mParent;
    }
    return ret; */
    return TRUE;
}

static BOOL _hasAttribute(GameObject* self, SmugAttribute attr)
{
    smug_assert(_objectInvariant(self));
    // Base object has no attributes.
    return FALSE;
}

static BOOL _inheritAttribute(GameObject* self, SmugAttribute attr, SmugInheritType type)
{
    smug_assert(_objectInvariant(self));
    // Base object has no attributes, so cannot inherit anything.
    return FALSE;
}

static void _delete(void* self)
{
    GameObject_deInit((GameObject*)self);
    free((GameObject*)self);
}


/**********************/
/** public functions **/
/**********************/

GameObject* GameObject_newGeneric(void)
{
    GameObject* newObj = (GameObject*)malloc(sizeof(GameObject));
    GameObject_init(newObj);
    return newObj;
}

int GameObject_addObject(GameObject* self, GameObject* obj)
{
    smug_assert(_objectInvariant(self));
    smug_assert(_objectInvariant(obj));
#ifndef LINUSNDEBUG
    int old = GameObject_nrChildObjects(self);
#endif
    if (obj->mParent != NULL)
    {
        smug_assert(!"Tried to add a child object to another object!");
        // It is an error to add an object to more than one superobject.
        return 1;
    }
    else
    {
        LinkedList_addLast(self->mSubObjects, obj);
        obj->mParent = self;
#ifndef LINUSNDEBUG
        smug_assert(GameObject_nrChildObjects(self) == old + 1);
        smug_assert(GameObject_hasChildObjects(self));
#endif
        return 0;
    }
}

void GameObject_removeObject(GameObject* self, GameObject* child)
{
    smug_assert(_objectInvariant(self));
    // TODO: implement.
}

BOOL GameObject_hasChildObjects(GameObject* self)
{
    return !LinkedList_isEmpty(self->mSubObjects);
}

int GameObject_nrChildObjects(GameObject* self)
{
    return LinkedList_length(self->mSubObjects);
}

BOOL GameObject_isRootObject(GameObject* self)
{
    return self->mParent == NULL;
}

struct LinkedListIterator* GameObject_getChildIterator(GameObject* self)
{
    return LinkedList_getIterator(self->mSubObjects);
}

GameObject* GameObject_getParent(GameObject* self)
{
    smug_assert(_objectInvariant(self));
    smug_assert(!GameObject_isRootObject(self));
    return self->mParent;
}

void GameObject_delete(void* self)
{
    smug_assert(_objectInvariant((GameObject*)self));
    ((GameObject*)self)->deleteMe(self);
}

BOOL GameObject_isType(GameObject* self, SmugType type)
{
    smug_assert(_objectInvariant(self));
    return ((self->mTypes & type) != 0);
}

BOOL GameObject_isExactType(GameObject* self, SmugType type)
{
    // TODO: Make a proper implementation. This one is borked.
    // smug_assert(_objectInvariant(self));
    // return self->mTypes == type;
    return FALSE;
}

BOOL GameObject_hasAttribute(GameObject* self, SmugAttribute attr)
{
    smug_assert(_objectInvariant(self));
    return self->hasAttribute(self, attr);
}

BOOL GameObject_inheritAttribute(GameObject* self, SmugAttribute attr, SmugInheritType type)
{
    smug_assert(_objectInvariant(self));
    return self->inheritAttribute(self, attr, type);
}

void GameObject_doRecursive(GameObject* self, void(*function)(GameObject*))
{
    LinkedList_doList(self->mSubObjects, (void(*)(void*))function);
    function(self);
}


/*************************/
/** protected functions **/
/*************************/

void GameObject_deInit(GameObject* self)
{
    // LinkedList_deleteContents(data->mSubObjects, GameObject_delete);
    LinkedList_delete(self->mSubObjects);
    self->mSubObjects = NULL;
    self->mParent = NULL;
    self->mTypes = SMUG_TYPE_NONE;
    self->hasAttribute = NULL;
    self->inheritAttribute = NULL;
    self->deleteMe = NULL;
}

void GameObject_init(GameObject* self)
{
    self->mParent = NULL;
    self->mSubObjects = LinkedList_new();
    self->mTypes = SMUG_TYPE_OBJECT;
    self->hasAttribute = _hasAttribute;
    self->inheritAttribute = _inheritAttribute;
    self->deleteMe = _delete;
}

char* InternalGameObject_getTypeString(SmugType type)
{
    if (type == SMUG_TYPE_OBJECT)
        return "SMUG_TYPE_OBJECT";
    else if (type == SMUG_TYPE_POSITIONED)
        return "SMUG_TYPE_POSITION";
    else if (type == SMUG_TYPE_BODY)
        return "SMUG_TYPE_BODY";
    else if (type == SMUG_TYPE_DRAWABLE)
        return "SMUG_TYPE_DRAWABLE";
    else if (type == SMUG_TYPE_SHAPE)
        return "SMUG_TYPE_SHAPE";
    else if (type == SMUG_TYPE_IMAGE)
        return "SMUG_TYPE_IMAGE";
    else if (type == SMUG_TYPE_ANIMATED_SPRITE)
        return "SMUG_TYPE_ANIMATED_SPRITE";
    else if (type == SMUG_TYPE_TIMER)
        return "SMUG_TYPE_TIMER";
    else if (type == SMUG_TYPE_DATA)
        return "SMUG_TYPE_DATA";
    else if (type == SMUG_TYPE_ANY)
        return "SMUG_TYPE_ANY";
    else
        return "UNKNKOWN";
}
