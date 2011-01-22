#include <smugstd.h>

#include <utils/linkedlist.h>
#include <utils/linkedlistiterator.h>

#include <engine/gameobject_protected.h>

/* Function implementations for a generic game object */

static BOOL _dataInvariant(GenericGameObjectData* data)
{
    BOOL ret = (data != NULL &&
                data->mSubObjects != NULL);
    return ret;
}

static BOOL _objectInvariant(GameObject* obj)
{
    BOOL ret = (obj != NULL);
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
    return ret;
    // return TRUE;
}

static BOOL _isExactType(GameObject* self, SmugType type)
{
    if (type == SMUG_TYPE_OBJECT)
    {
        return TRUE;
    }
    else
    {
        // smug_printf("Type was not %s, but SMUG_TYPE_OBJECT", InternalGameObject_getTypeString(type));
        return FALSE;
    }
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

static void _deleteData(void* self)
{
    // LinkedList_deleteContents(data->mSubObjects, GameObject_delete);
    LinkedList_delete(((GenericGameObjectData*)self)->mSubObjects);
    free((GenericGameObjectData*)self);
}

/* Internal helper functions */

static GenericGameObjectData* GenericGameObjectData_new(void)
{
    GenericGameObjectData* data = (GenericGameObjectData*)malloc(sizeof(GenericGameObjectData));
    data->mSubObjects = LinkedList_new();
    data->mParent = NULL;
    smug_assert(_dataInvariant(data));
    return data;
}

static InternalGameObject* InternalGameObject_new(void* data)
{
    InternalGameObject* newObj = (InternalGameObject*)malloc(sizeof(InternalGameObject));
    newObj->mParent = NULL;
    newObj->mChild = NULL;
    newObj->mRoot = newObj;
    newObj->mLeaf = newObj;
    newObj->mData = data;
    newObj->hasAttribute = NULL;
    newObj->isExactType = NULL;
    newObj->inheritAttribute = NULL;
    newObj->deleteData = NULL;
    return newObj;
}

static GenericGameObjectData* _getGenericGameObjectData(GameObject* self)
{
    smug_assert(_objectInvariant(self));
    return (GenericGameObjectData*)(InternalGameObject_getAsGeneric(self)->mData);
}

/* Public functions for GameObject. */

GameObject* GameObject_newGeneric(void)
{
    InternalGameObject* newObj = InternalGameObject_new(GenericGameObjectData_new());

    newObj->hasAttribute = _hasAttribute;
    // newObj->isType = _isType;
    newObj->isExactType = _isExactType;
    newObj->inheritAttribute = _inheritAttribute;
    newObj->deleteData = _deleteData;
    smug_assert(_objectInvariant(newObj));
    return newObj;
}

int GameObject_addObject(GameObject* self, GameObject* obj)
{
    smug_assert(_objectInvariant(self));
    smug_assert(_objectInvariant(obj));
    smug_assert(_dataInvariant(_getGenericGameObjectData(self)));
    smug_assert(_dataInvariant(_getGenericGameObjectData(obj)));
#ifndef LINUSNDEBUG
    int old = GameObject_nrChildObjects(self);
#endif
    if (_getGenericGameObjectData(obj)->mParent != NULL)
    {
        smug_assert(!"Tried to add a child object to another object!");
        // It is an error to add an object to more than one superobject.
        return 1;
    }
    else
    {
        LinkedList_addLast(_getGenericGameObjectData(self)->mSubObjects, InternalGameObject_getAsGeneric(obj));
        _getGenericGameObjectData(obj)->mParent = self;
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
    return !LinkedList_isEmpty(_getGenericGameObjectData(self)->mSubObjects);
}

int GameObject_nrChildObjects(GameObject* self)
{
    return LinkedList_length(_getGenericGameObjectData(self)->mSubObjects);
}

BOOL GameObject_isRootObject(GameObject* self)
{
    return _getGenericGameObjectData(self)->mParent == NULL;
}

struct LinkedListIterator* GameObject_getChildIterator(GameObject* self)
{
    return LinkedList_getIterator(_getGenericGameObjectData(self)->mSubObjects);
}

GameObject* GameObject_getParent(GameObject* self)
{
    smug_assert(_objectInvariant((GameObject*)self));
    smug_assert(!GameObject_isRootObject(self));
    return _getGenericGameObjectData(self)->mParent;
}

void GameObject_delete(void* self)
{
    smug_assert(_objectInvariant((GameObject*)self));
    InternalGameObject* go = InternalGameObject_getMostSpecific((InternalGameObject*)self);
    while (go->mParent != NULL)
    {
        go = go->mParent;
        go->mChild->deleteData(go->mChild->mData);
        free(go->mChild);
    }
    go->deleteData(go->mData);
    free(go);
}

InternalGameObject* InternalGameObject_inherit(InternalGameObject* self, void* data)
{
    smug_assert(_objectInvariant(self));
    smug_assert(_dataInvariant(_getGenericGameObjectData(self)));
    smug_assert(self->mChild == NULL);
    InternalGameObject* newNode = InternalGameObject_new(data);
    newNode->mParent = self;
    self->mChild = newNode;
    newNode->mLeaf = newNode;
    self->mLeaf = newNode;
    while (self->mParent != NULL)
    {
        self = self->mParent;
        self->mLeaf = newNode;
    }
    newNode->mRoot = self;
    smug_assert(_objectInvariant(newNode));
    return newNode;
}

static BOOL InternalGameObject_isType(InternalGameObject* self, SmugType type)
{
    smug_assert(_objectInvariant(self));
    // return InternalGameObject_getMostSpecific(self)->isType(self, type);
    return InternalGameObject_isExactType(self, type) || (self->mParent != NULL && InternalGameObject_isType(self->mParent, type));
}

BOOL InternalGameObject_isExactType(InternalGameObject* self, SmugType type)
{
    smug_assert(_objectInvariant(self));
    return self->isExactType(self, type);
}

void* InternalGameObject_getData(InternalGameObject* self, SmugType type)
{
    smug_assert(_objectInvariant(self));
    smug_assert(_dataInvariant(_getGenericGameObjectData(self)));
    self = InternalGameObject_getAsGeneric(self);
    while (!InternalGameObject_isExactType(self, type))
    {
        if (self->mChild == NULL)
        {
            // Not an object of the specified type.
            return NULL;
        }
        self = self->mChild;
    }
    return self->mData;
}

BOOL GameObject_isType(GameObject* self, SmugType type)
{
    smug_assert(_objectInvariant(self));
    return InternalGameObject_isType(InternalGameObject_getMostSpecific(self), type);
}

BOOL GameObject_isExactType(GameObject* self, SmugType type)
{
    smug_assert(_objectInvariant(self));
    return InternalGameObject_getMostSpecific(self)->isExactType(self, type);
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
    LinkedList_doList(((GenericGameObjectData*)InternalGameObject_getData(self, SMUG_TYPE_OBJECT))->mSubObjects, (void(*)(void*))function);
    function(self);
}

/* Protected functions for (Internal)GameObject */

InternalGameObject* InternalGameObject_getParent(InternalGameObject* self)
{
    smug_assert(_objectInvariant(self));
    return self->mParent;
}

InternalGameObject* InternalGameObject_getChild(InternalGameObject* self)
{
    smug_assert(_objectInvariant(self));
    return self->mChild;
}

GameObject* InternalGameObject_getAsGeneric(InternalGameObject* self)
{
    smug_assert(_objectInvariant(self));
    // while (self->mParent != NULL)
    // {
        // self = self->mParent;
    // }
    // return self;
    return self->mRoot;
}

InternalGameObject* InternalGameObject_getMostSpecific(InternalGameObject* self)
{
    smug_assert(_objectInvariant(self));
    // while (self->mChild != NULL)
    // {
        // self = self->mChild;
    // }
    // return self;
    return self->mLeaf;
}

char* InternalGameObject_getTypeString(SmugType type)
{
    if (type == SMUG_TYPE_OBJECT)
        return "SMUG_TYPE_OBJECT";
    else if (type == SMUG_TYPE_POSITION)
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
